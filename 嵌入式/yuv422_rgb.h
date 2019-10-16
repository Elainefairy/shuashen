#include <stdio.h>
#include <string.h>
int  yuv422_rgb24(unsigned char *yuv_buf, unsigned char *rgb_buf, unsigned int width, unsigned int height);
//int YU12ToBGR24_Native(unsigned char* pYUV,unsigned char* pBGR24,int width,int height);
//void yuv420p_to_rgb24(unsigned char* yuvbuffer,unsigned char* rgbbuffer, int width,int height);


void RGB_scanline_resample_nearest (unsigned char * dest, unsigned char * src, int n, int *accumulator, int increment);
void RGB_image_scale_nearest (unsigned char * dest, int dest_width, int dest_height, unsigned char * src,  int src_width, int src_height);


