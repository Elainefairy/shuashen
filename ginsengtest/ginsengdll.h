/*
author:tonglewang
date:2017/10/31
functionName:pictureDifferentDection
char* image1Path:char*ָ���ʾ�����һ��ͼƬ
char* image2Path:char*ָ���ʾ����ڶ���ͼƬ
xingtai initStruct[]:��ʾ���õ���̬�˲�˳����ֵĬ��Ϊ�Ѿ����úõ�7���������̬�˲����û��������裬��ֵӦ��ֻxingtai�ṹ������
int len_control:��ʾ��̬�˲�������Ĭ��Ϊ7
double threshold_control:��ֵ����ֵ��Ĭ��Ϊ0.1

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