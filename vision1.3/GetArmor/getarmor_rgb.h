#ifndef GETARMOR_RGB_H
#define GETARMOR_RGB_H
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"
#include "opencv2/imgproc.hpp"
#include <unistd.h> /*Unix 标准函数定义*/


using namespace  cv ;
using namespace std ;
#define ANGLE_THRE 10
#define T_SIZE_THRE 5
struct point
{
    point() {}
    int x;
    int y;
};
void brightAdjust(Mat src, Mat dst, double dContrast, double dBright); //亮度调节函数
void getDiffImage(Mat src1, Mat src2, Mat dst, int nThre); //二值化
vector<RotatedRect> armorDetect(vector<RotatedRect> vEllipse); //检测装甲
void drawBox(RotatedRect box, Mat img); //标记装甲
void SendDate(float x,float y,int fd);//发送数据
void getArmor();
vector<RotatedRect> getArmor(Mat frame0);
#endif // GETARMOR_RGB_H
