/*
 *  V4L2 video capture based on gnome/gtk2.0+
 *  made by zhenguoyao 2011.11.18 at UESTC chengdu
 *  This program can be used and distributed without restrictions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <gtk/gtk.h>
#include <asm/types.h>          /* for videodev2.h */
#include "v4l2.h"
#include "yuv422_rgb.h"
#define dest_width 440
#define dest_height 240


static unsigned char image_ready = 0;
unsigned int quit_flag = 0;

static GtkWidget *window;
static GtkWidget *vbox;
static GtkWidget *drawingarea;

static gboolean on_darea_expose (GtkWidget *widget,
                          GdkEventExpose *event,
                          gpointer user_data);
static gboolean quit_capture (GtkWidget *widget,
                          GdkEventExpose *event,
                          gpointer user_data);
static void draw_thread(GtkWidget *widget);
static void capture_thread(struct camera *cam);

void process_image (guchar *p, struct camera *cam)
{
	image_ready = 1;
	yuv422_rgb24(p, cam->rgbbuf, cam->width, cam->height);
}
gboolean deal_mouse_press(GtkWidget *widget, GdkEventButton *event, gpointer data)  
{  
	struct camera *cam_g;
	cam_g = (struct camera *)(data);
	save_img_mpeg(cam_g);
GtkWidget *dialog;
dialog = gtk_message_dialog_new((gpointer)window,
GTK_DIALOG_DESTROY_WITH_PARENT,
GTK_MESSAGE_INFO,
GTK_BUTTONS_OK,
"send image done", "title");
gtk_window_set_title(GTK_WINDOW(dialog), "Information");
gtk_dialog_run(GTK_DIALOG(dialog));
gtk_widget_destroy(dialog);
	g_thread_create((GThreadFunc)draw_thread, drawingarea, FALSE, NULL);
	g_thread_create((GThreadFunc)capture_thread, cam_g, FALSE, NULL);

#if 0
    switch(event->button){    
        case 1:  
            printf("Left Button!!\n");  
            break;  
        case 2:  
            printf("Middle Button!!\n");  
            break;  
        case 3:  
            printf("Right Button!!\n");  
            break;  
        default:  
            printf("Unknown Button!!\n");  
    }  
  
    if(event->type == GDK_2BUTTON_PRESS){  
        printf("double click\n");  
    }  
#endif
  
    gint i = event->x;  
    gint j = event->y;  
    printf("press_x = %d, press_y = %d\n", i, j);  
  
    return TRUE;  
}  

static void draw_thread(GtkWidget *widget)
{
        for(;;) {
                g_usleep(10000);
		if (quit_flag == 1) break;
                gdk_threads_enter();
                if(image_ready) {
                        gtk_widget_queue_draw(GTK_WIDGET (widget));
                }
                else {
                        gdk_threads_leave();
                }
                gdk_threads_leave();
        }
}

static void capture_thread(struct camera *cam)
{

	for (;;) {
		g_usleep(10000);

		if (quit_flag == 1) break;

		gdk_threads_enter();
		fd_set fds;
		struct timeval tv;
		int r;

		FD_ZERO (&fds);
		FD_SET (cam->fd, &fds);

		/* Timeout. */
		tv.tv_sec = 20;
		tv.tv_usec = 0;

		r = select (cam->fd + 1, &fds, NULL, NULL, &tv);

		if (-1 == r) {
        		if (EINTR == errno)
                	continue;

        		errno_exit ("select");
		}

		if (0 == r) {
        		fprintf (stderr, "select timeout\n");
        		exit (EXIT_FAILURE);
		}

		if (read_frame (cam))
            		gdk_threads_leave();
		/* EAGAIN - continue select loop. */
	}	
}


static void gtk_window_init(int argc, char **argv, struct camera *cam)
{
	if(!g_thread_supported()) {
		g_thread_init(NULL);
	}

	gdk_threads_init();

	gtk_init (&argc, &argv);

	/* window init */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "camera data");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), dest_width, dest_height);
	g_signal_connect(G_OBJECT(window),"delete_event", GTK_SIGNAL_FUNC(quit_capture), cam);

	/* vbox init */
	vbox = gtk_vbox_new(FALSE,0);
	gtk_container_add(GTK_CONTAINER(window),vbox);

	/* drawingarea init */
	drawingarea = gtk_drawing_area_new();
	gtk_box_pack_start(GTK_BOX(vbox),drawingarea,TRUE,TRUE,0);
   	gtk_drawing_area_size (GTK_DRAWING_AREA (drawingarea), dest_width,dest_height);
	g_signal_connect (GTK_OBJECT (drawingarea), "expose-event",                         // Put RGB image in darea.
                        GTK_SIGNAL_FUNC (on_darea_expose), cam);

	gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
	g_signal_connect(G_OBJECT(window), "button-press-event", G_CALLBACK(deal_mouse_press), cam);

	gtk_widget_show(vbox);							
	gtk_widget_show(drawingarea);
	gtk_widget_show(window);

}


int main(int argc, char **argv)
{
	/*
	 * init struct camera 
	 */
	struct camera *cam;
	
	cam = malloc(sizeof(struct camera));
	if (!cam) { 
		printf("malloc camera failure!\n");
		exit(1);
	}
        cam->device_name = "/dev/video0";
	cam->buffers = NULL;
	cam->width = 320; 
	cam->height = 240;
	cam->display_depth = 3;  /* RGB24 */
	cam->rgbbuf = malloc(cam->width * cam->height * cam->display_depth);

	if (!cam->rgbbuf) { 
		printf("malloc rgbbuf failure!\n");
		exit(1);
	}
	open_camera(cam);
	/*
	get_cam_cap(cam);
	get_cam_pic(cam);
	get_cam_win(cam);
	*/
	cam->video_win.width = cam->width;
	cam->video_win.height = cam->height;
	/*
	set_cam_win(cam);
	get_cam_win(cam);	
	*/
	pixel_format = V4L2_PIX_FMT_YUYV;
        init_camera(cam);
        start_capturing (cam);

	gtk_window_init(argc,argv,cam);

	g_thread_create((GThreadFunc)draw_thread, drawingarea, FALSE, NULL);
	g_thread_create((GThreadFunc)capture_thread, cam, FALSE, NULL);

	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();
  	return 0;
}

static gboolean on_darea_expose (GtkWidget *widget,
                 GdkEventExpose *event,
                 gpointer user_data)
{      
	struct camera *cam;
	unsigned char *dest_pixbuf;

	cam = (struct camera *)(user_data); 
	dest_pixbuf = malloc(dest_width * dest_height * cam->display_depth);
	RGB_image_scale_nearest(dest_pixbuf, dest_width, dest_height, cam->rgbbuf,cam->width,cam->height);

        gdk_draw_rgb_image (widget->window, widget->style->fg_gc[GTK_STATE_NORMAL],
                	0, 0, dest_width, dest_height,
                     	GDK_RGB_DITHER_MAX, dest_pixbuf, dest_width * cam->display_depth);
        /*gdk_draw_rgb_image (widget->window, widget->style->fg_gc[GTK_STATE_NORMAL],
                	0, 0, cam->width, cam->height,
                     	GDK_RGB_DITHER_MAX, cam->rgbbuf, cam->width * cam->display_depth);*/
	image_ready = 0;
        return 1;
}

static gboolean quit_capture(GtkWidget *widget,
                 GdkEventExpose *event,
                 gpointer user_data)
{
	struct camera *cam;
	
	cam = (struct camera *)user_data;
	
	quit_flag = 1;

        stop_capturing (cam);
	  printf("stop capturing\n");
        uninit_camera(cam);
	  printf("uninit camera\n");

        close_camera(cam);
	  printf("close camera.\n");
	free(cam->rgbbuf);
	free(cam);

	gtk_main_quit();

	return 1;

}
