/*
 * R = Y + 1.4075*(V-128)
 * G = Y - 0.3455*(U-128) - 0.7169*(V-128)
 * B = Y +1.779 *(U-128)
 * U' = U -128
 * V' = V - 128
 * R = Y + V' + ((V'*104) >> 8))
 * G = Y - ((U'*89) >> 8) - ((V' * 183) >> 8)
 * B = Y + U' + ((U'*199) >> 8)
 *  Made by zhenguoyao 2011.11.18 at UESTC chengdu
 *  This program can be used and distributed without restrictions.
 */ 
#define Y0	0
#define U	1
#define Y1	2
#define	V	3

#define R	0
#define G	1
#define	B	2

int yuv422_rgb24(unsigned char *yuv_buf, unsigned char *rgb_buf, unsigned int width, unsigned int height)
{
	int yuvdata[4];
	int rgbdata[3];
	unsigned char *rgb_temp;
 	unsigned int i, j;

	rgb_temp = rgb_buf;
	for (i = 0; i < height * 2; i++) {
		for (j = 0; j < width; j+= 4) {
			/* get Y0 U Y1 V */
			yuvdata[Y0] = *(yuv_buf + i * width + j + 0);
			yuvdata[U]  = *(yuv_buf + i * width + j + 1);
			yuvdata[Y1] = *(yuv_buf + i * width + j + 2);
			yuvdata[V]  = *(yuv_buf + i * width + j + 3);

			/* the first pixel */
			rgbdata[R] = yuvdata[Y0] + (yuvdata[V] - 128) + (((yuvdata[V] - 128) * 104 ) >> 8);
			rgbdata[G] = yuvdata[Y0] - (((yuvdata[U] - 128) * 89) >> 8) - (((yuvdata[V] - 128) * 183) >> 8);
			rgbdata[B] = yuvdata[Y0] + (yuvdata[U] - 128) + (((yuvdata[U] - 128) * 199) >> 8);


			if (rgbdata[R] > 255)  rgbdata[R] = 255;		 
			if (rgbdata[R] < 0) rgbdata[R] = 0;			
			if (rgbdata[G] > 255)  rgbdata[G] = 255;		 
			if (rgbdata[G] < 0) rgbdata[G] = 0;			
			if (rgbdata[B] > 255)  rgbdata[B] = 255;		 
			if (rgbdata[B] < 0) rgbdata[B] = 0;			
			
			*(rgb_temp++) = rgbdata[R] ;
			*(rgb_temp++) = rgbdata[G];
			*(rgb_temp++) = rgbdata[B];
			
			/* the second pix */ 
			rgbdata[R] = yuvdata[Y1] + (yuvdata[V] - 128) + (((yuvdata[V] - 128) * 104 ) >> 8);
			rgbdata[G] = yuvdata[Y1] - (((yuvdata[U] - 128) * 89) >> 8) - (((yuvdata[V] - 128) * 183) >> 8);
			rgbdata[B] = yuvdata[Y1] + (yuvdata[U] - 128) + (((yuvdata[U] - 128) * 199) >> 8);

			
			if (rgbdata[R] > 255)  rgbdata[R] = 255;		 
			if (rgbdata[R] < 0) rgbdata[R] = 0;			
			if (rgbdata[G] > 255)  rgbdata[G] = 255;		 
			if (rgbdata[G] < 0) rgbdata[G] = 0;			
			if (rgbdata[B] > 255)  rgbdata[B] = 255;		 
			if (rgbdata[B] < 0) rgbdata[B] = 0;		
	
			*(rgb_temp++) = rgbdata[R];
			*(rgb_temp++) = rgbdata[G];
			*(rgb_temp++) = rgbdata[B];
		
		}
	}
	return 0;
}
#if 0
static long int crv_tab[256];   
static long int cbu_tab[256];   
static long int cgu_tab[256];   
static long int cgv_tab[256];   
static long int tab_76309[256]; 
static unsigned char clp[1024];   //for clip in CCIR601   
void init_yuv420p_table() 
{   
    long int crv,cbu,cgu,cgv;   
    int i,ind;      
   
    crv = 104597; cbu = 132201;  /* fra matrise i global.h */   
    cgu = 25675;  cgv = 53279;   
   
    for (i = 0; i < 256; i++)    
    {   
        crv_tab[i] = (i-128) * crv;   
        cbu_tab[i] = (i-128) * cbu;   
        cgu_tab[i] = (i-128) * cgu;   
        cgv_tab[i] = (i-128) * cgv;   
        tab_76309[i] = 76309*(i-16);   
    }   
   
    for (i = 0; i < 384; i++)   
        clp[i] = 0;   
    ind = 384;   
    for (i = 0;i < 256; i++)   
        clp[ind++] = i;   
    ind = 640;   
    for (i = 0;i < 384; i++)   
        clp[ind++] = 255;   
}
void yuv420p_to_rgb24(unsigned char* yuvbuffer,unsigned char* rgbbuffer, int width,int height)   
{
    int y1, y2, u, v;    
    unsigned char *py1, *py2;   
    int i, j, c1, c2, c3, c4;   
    unsigned char *d1, *d2;   
    unsigned char *src_u, *src_v;
    static int init_yuv420p = 0;
    
    src_u = yuvbuffer + width * height;   // u
    src_v = src_u + width * height / 4;  // v

    py1 = yuvbuffer;   // y
    py2 = py1 + width;   
    d1 = rgbbuffer;   
    d2 = d1 + 3 * width;   

    if (init_yuv420p == 0)
    {
        init_yuv420p_table();
        init_yuv420p = 1;
    }

    for (j = 0; j < height; j += 2)    
    {    
        for (i = 0; i < width; i += 2)    
        {
            u = *src_u++;   
            v = *src_v++;   
   
            c1 = crv_tab[v];   
            c2 = cgu_tab[u];   
            c3 = cgv_tab[v];   
            c4 = cbu_tab[u];   
   
            //up-left   
            y1 = tab_76309[*py1++];    
            *d1++ = clp[384+((y1 + c1)>>16)];     
            *d1++ = clp[384+((y1 - c2 - c3)>>16)];   
            *d1++ = clp[384+((y1 + c4)>>16)];   
   
            //down-left   
            y2 = tab_76309[*py2++];   
            *d2++ = clp[384+((y2 + c1)>>16)];     
            *d2++ = clp[384+((y2 - c2 - c3)>>16)];   
            *d2++ = clp[384+((y2 + c4)>>16)];   
   
            //up-right   
            y1 = tab_76309[*py1++];   
            *d1++ = clp[384+((y1 + c1)>>16)];     
            *d1++ = clp[384+((y1 - c2 - c3)>>16)];   
            *d1++ = clp[384+((y1 + c4)>>16)];   
   
            //down-right   
            y2 = tab_76309[*py2++];   
            *d2++ = clp[384+((y2 + c1)>>16)];     
            *d2++ = clp[384+((y2 - c2 - c3)>>16)];   
            *d2++ = clp[384+((y2 + c4)>>16)];   
        }
        d1  += 3*width;
        d2  += 3*width;
        py1 += width;
        py2 += width;
    }
}
int YU12ToBGR24_Native(unsigned char* pYUV,unsigned char* pBGR24,int width,int height)  
{  
    if (width < 1 || height < 1)  
        return 0;  
    int len = width * height;  
    	unsigned char* yData;
	unsigned char* uData;
	unsigned char* vData;
	yData =(unsigned char*)malloc(len+2);
	uData =(unsigned char*)malloc(len/4+2);
	vData =(unsigned char *)malloc(len/4+2);
	yData=strncpy(yData,pYUV,len);
	yData[len-1]='\0';
	uData=strncpy(uData,pYUV+len,len/4);
	uData[len/4-1]='\0';
	vData=strncpy(vData,(pYUV+len+len/4),len/4);
	vData[len/4-1]='\0';
  
    int bgr[3];  
    int yIdx,uIdx,vIdx,idx,i,j,k;  
    for (i = 0;i < height;i++){  
        for (j = 0;j < width;j++){  
            yIdx = i * width + j;  
            vIdx = (i/2) * (width/2) + (j/2);  
            uIdx = vIdx;  
  
            bgr[2] = (int)(yData[yIdx] + 1.779 * (uData[vIdx] - 128));                                    // b分量  
            bgr[1] = (int)(yData[yIdx] - 0.3455 * (uData[uIdx] - 128) - 0.7169 * (vData[vIdx] - 128));    // g分量  
            bgr[0] = (int)(yData[yIdx] + 1.4075 * (vData[uIdx] - 128));                                    // r分量  
  
            for (k = 0;k < 3;k++){  
                idx = (i * width + j) * 3 + k;  
                if(bgr[k] >= 0 && bgr[k] <= 255)  
                    pBGR24[idx] = bgr[k];  
                else  
                    pBGR24[idx] = (bgr[k] < 0)?0:255;  
            }  
        }  
    }  
    return 1;  
}
#endif

void RGB_scanline_resample_nearest (unsigned char * dest, unsigned char * src, int n, int *accumulator, int increment)  
{  
  int acc = *accumulator;  
  int i;  
  int j;  
  int x;  
  for (i = 0; i < n; i++) {  
    j = acc >> 16;  
    x = acc & 0xffff;  
    dest[i * 3 + 0] = (x < 32768) ? src[j * 3 + 0] : src[j * 3 + 3];  
    dest[i * 3 + 1] = (x < 32768) ? src[j * 3 + 1] : src[j * 3 + 4];  
    dest[i * 3 + 2] = (x < 32768) ? src[j * 3 + 2] : src[j * 3 + 5];  
    acc += increment;  
  }  
  *accumulator = acc;  
}  

void RGB_image_scale_nearest (unsigned char * dest, int dest_width, int dest_height,  
                         unsigned char * src,  int src_width, int src_height)  
{  
  int acc;  
  int y_increment;  
  int x_increment;  
  int i;  
  int j;  
  int x;  
  int xacc;  
  int src_stride = src_width*3;  
  int dest_stride = dest_width*3;  
  if (dest_height == 1)  
    y_increment = 0;  
  else  
    y_increment = ((src_height - 1) << 16) / (dest_height - 1);  
  if (dest_width == 1)  
    x_increment = 0;  
  else  
    x_increment = ((src_width - 1) << 16) / (dest_width - 1);  
  acc = 0;  
  for (i = 0; i < dest_height; i++) {  
    j = acc >> 16;  
    x = acc & 0xffff;  
    xacc = 0;  
    RGB_scanline_resample_nearest(dest + i * dest_stride,  
                           src + j * src_stride, dest_width, &xacc, x_increment);  
    acc += y_increment;  
  }  
}

