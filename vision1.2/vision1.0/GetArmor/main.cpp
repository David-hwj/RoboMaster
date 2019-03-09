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
    VideoCapture capture(1);
    //读取摄像头
    Mat frame;
    int count_time=0;
    //摄像头图
    while(1){
        count_time++;
        if(count_time==1000){
            count_time=0;
           cout<<"time--------------------------------------------------------"<<endl;
        }
        capture >> frame;
      //  Detector.hsv_rec(frame);
      //  imshow("frame", frame);
         combine(frame,open);
        //结合函数
        waitKey(1);
    }

    capture.release();

    return 0;
}
