CC = gcc
OBJS := main.c yuv422_rgb.c v4l2.c save_img_mpeg.c
all: $(OBJS)
	$(CC) `pkg-config --cflags --libs gtk+-2.0`  -o camera_test $(OBJS)
 
clean:
	rm -rf *.o 

