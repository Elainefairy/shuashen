#include "v4l2.h"
#define CLEAR(x) memset (&(x), 0, sizeof (x))
static void write2file(const void *p, ssize_t size)   
 {  
   static int no_image = 0;
   char filename[1024];  
   int fd;  
   ssize_t written = 0;  
   
   snprintf(filename, sizeof(filename), "%d.%s", no_image++, "jpg");  
   
   fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);  
   printf("%s\n",filename);
   if (fd < 0)  
     {  
       fputc('*', stdout);  
       fflush(stdout);  
       return;  
     }  
   do  
     {  
       int ret;  
       ret = write(fd, p + written, size - written);  
       if (ret < 0)  
     {  
       fputc('+', stdout);  
       fflush(stdout);  
       return;  
     }  
       written += ret;  
     }  
   while (written < size);  
   close(fd);  
   
   fputc('.', stdout);  
   fflush(stdout);  

 } 

static void capture_img(struct camera *cam)
{

		g_usleep(10000);

		fd_set fds;
		struct timeval tv;
		int r;

		FD_ZERO (&fds);
		FD_SET (cam->fd, &fds);

		/* Timeout. */
		tv.tv_sec = 2;
		tv.tv_usec = 0;

		r = select (cam->fd + 1, &fds, NULL, NULL, &tv);

		if (-1 == r) {
        		if (EINTR == errno)
				printf("fd busy!\n");
        		errno_exit ("select");
		}

		if (0 == r) {
        		fprintf (stderr, "select timeout\n");
        		exit (EXIT_FAILURE);
		}

		struct v4l2_buffer buf;

       CLEAR (buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        if (-1 == xioctl (cam->fd, VIDIOC_DQBUF, &buf)) {
            switch (errno) {
            	case EAGAIN:
			return;			
	        case EIO:
		default:
			errno_exit ("VIDIOC_DQBUF");
			}
		}
assert (buf.index < 4);
write2file(cam->buffers[buf.index].start, cam->buffers[buf.index].length);
		if (-1 == xioctl (cam->fd, VIDIOC_QBUF, &buf))
			errno_exit ("VIDIOC_QBUF");	
		/* EAGAIN - continue select loop. */
		
}
void save_img_mpeg(struct camera *cam_g)
{
	//close global camera
	quit_flag = 1;
	stop_capturing (cam_g);
	uninit_camera(cam_g);
	close_camera(cam_g);

	//open local camera to capture image
	struct camera *cam;
	cam = malloc(sizeof(struct camera));
	if (!cam) { 
		printf("malloc camera failure!\n");
		exit(1);
	}
    cam->device_name = "/dev/video0";
	cam->buffers = NULL;
	cam->width = 1920;
	cam->height = 1080;
	cam->display_depth = 3;  /* RGB24 */
	cam->rgbbuf = malloc(cam->width * cam->height * cam->display_depth);

	if (!cam->rgbbuf) { 
		printf("malloc rgbbuf failure!\n");
		exit(1);
	}
	open_camera(cam);
	pixel_format = V4L2_PIX_FMT_YUV420;
	init_camera(cam);
        start_capturing (cam);

	capture_img(cam);

	//close local camera
	stop_capturing (cam);
	uninit_camera(cam);
	close_camera(cam);

//open global camera
	quit_flag = 0;
	open_camera(cam_g);
	pixel_format = V4L2_PIX_FMT_YUYV;
	init_camera(cam_g);
	start_capturing (cam_g);
}

