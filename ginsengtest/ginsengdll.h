/*
author:tonglewang
date:2017/10/31
functionName:pictureDifferentDection
char* image1Path:char*指针表示输入第一张图片
char* image2Path:char*指针表示输入第二张图片
xingtai initStruct[]:表示采用的形态滤波顺序，其值默认为已经设置好的7个步骤的形态滤波，用户可以自设，赋值应该只xingtai结构体数组
int len_control:表示形态滤波个数，默认为7
double threshold_control:二值化阈值，默认为0.1

*/
#include<opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\nonfree.hpp>
#include <opencv2\legacy\legacy.hpp>
#include <iostream>
#include<cv.h>
using namespace cv;
using namespace std;

int pictureDifferentDection(char* image1Path, char*image2Path);


vector<DMatch> myBFmatch(Mat descriptors_object, Mat descriptors_scene, vector<KeyPoint> keypoints_object, vector<KeyPoint> keypoints_scene);

double GetEmd(const char*path1, const char*path2);