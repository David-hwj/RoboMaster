/*
    版权：湖南大学Robomaster视觉组
    功能：整合HSV+RGB模块 提取装甲板坐标
    版本：1.1
    时间：2019/3/5  - 00：14
*/
#include <iostream>
#include<rgb_and_hsv.h>
using namespace std;
int main()
{
    initsendate();
    int open= openusb();
    //打开串口通信
    VideoCapture capture(0);
    //读取摄像头
    Mat frame; 
    //摄像头图
    while(1){
		
        capture >> frame;
		if(frame.empty()) continue;
        //Detector.hsv_rec(frame);
         imshow("frame", frame);
         combine(frame,open);
			//imshow("camera",frame);
        //结合函数
        waitKey(1);
    }

    capture.release();

    return 0;
}
