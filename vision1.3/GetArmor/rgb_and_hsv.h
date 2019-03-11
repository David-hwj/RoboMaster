#ifndef RGB_AND_HSV_H
#define RGB_AND_HSV_H
#include "getarmor_hsv.h"
#include "getarmor_rgb.h"
#include "com.h"
#include<iostream>
#include<queue>

using namespace std;
//int openusb(Serial_port FCcom);
void combine(Mat frame0,int fd);
void Send(int fd, queue<Point> q);
#endif // RGB_AND_HSV_H
