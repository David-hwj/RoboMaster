#pragma once
//#ifndef GETBINARY_H
//#define GETBINARY_H

#include<iostream>
#include<vector>

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

//Mat RVChannels, BHChannels; //通道合并之后的两种颜色

void on_trackbar_exposure(int, void*);
void on_trackbar_binary(int, void*);
void on_trackbar_brightness(int, void*);
void on_trackbar_contrast(int, void*);
void on_trackbar_saturation(int, void*);
void on_trackbar_hue(int, void*);
//void binaryImg(Mat src_colorImg, Mat &dst_binaryImg);
void openCapture();

//#endif 