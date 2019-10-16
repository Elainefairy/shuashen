
#ifndef _V4L2_H
#define _V4L2_H
#include <asm/types.h>          /* for videodev2.h */
#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libv4l1-videodev.h>
#include <libv4l1.h>
#include <libv4l2.h>
#include <linux/videodev2.h>
#include <libv4lconvert.h>
#define DEBUG_CAM	1	
struct buffer {
        void *  start;
        size_t  length;
};

extern unsigned int quit_flag;
struct camera {
	char *device_name;
	int fd;
	int width;
	int height;
	int display_depth;
	int image_size;
   	int frame_number;
	struct video_capability video_cap;
	struct v4l2_capability v4l2_cap;
	struct v4l2_cropcap v4l2_cropcap;
	struct v4l2_format v4l2_fmt;
	struct video_window video_win;
	struct video_picture video_pic;
	struct buffer *buffers;
	unsigned char *rgbbuf;
};
void process_image(unsigned char *buf, struct camera *cam);
void errno_exit(const char *s);
void open_camera(struct camera *cam);
void close_camera(struct camera *cam);
int read_frame(struct camera *cam);
void start_capturing(struct camera *cam);
void stop_capturing(struct camera *cam);
void uninit_camera(struct camera *cam);
void init_camera(struct camera *cam);
void get_cam_cap(struct camera *cam);
void get_cam_pic(struct camera *cam);
void set_cam_pic(struct camera *cam);
void get_cam_win(struct camera *cam);
void set_cam_win(struct camera *cam);
int xioctl(int fd, int request, void *arg);

int pixel_format; 
void save_img_mpeg(struct camera *cam);
#endif
