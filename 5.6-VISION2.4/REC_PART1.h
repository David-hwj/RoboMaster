#ifndef REC_PART1_H
#define REC_PART1_H
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void bshow_hsv(int value, void* detector);			// 展示过滤后的图像，HSV分通道后再取交集、蓝光HSV对应范围
void rshow_hsv(int value, void* detector);			// 红光对应HSV
void on_trackbar_exposure(int, void* detector);     // 摄像头曝光值
//void on_trackbar_binary(int, void* detector);       // 二值图
void on_trackbar_brightness(int, void* detector);   // 摄像头亮度
void on_trackbar_contrast(int, void* detector);     // 对比度
void on_trackbar_saturation(int, void* detector);   // 饱和度
void on_trackbar_hue(int, void* detector);          // 色调

class Image_Threshold
{
public:
	string thres_color;     // 待识别颜色
	int frame_width;		// 图像宽度
	int frame_height;		// 图像高度
	int rather_show_img;    // 是否显示图像

	int cap_width;          // 视频流图像的宽度
	int cap_height;         // 视频流图像的高度

	// 蓝色灯条
	int BLowH = 60;			// 保留色调在 blowH-BhighH 范围
	int BHighH = 166;

	int BLowS = 105;		// 饱和度在 BlowS-BhighS
	int BHighS = 255;

	int BLowV = 160;		// 亮度在 BlowV-BhighV
	int BHighV = 255;

	// 红色灯条，同上
	int RLowH = 160;		// 红色右部分
	int RHighH = 195;
	int RLowH_2 = 0;		// 红色左半部分
	int RHighH_2 = 30;
	int RLowS = 150;
	int RHighS = 255;		// 红饱和

	int RLowV = 100;		// 亮度
	int RHighV = 255;

	const int BINARY_MAX = 255;
	const int EXPOSURE_MAX = 11;
	const int BRIGHTNESS_MAX = 255;
	const int CONTRAST_MAX = 94;
	const int SATURATION_MAX = 99;
	const int HUE_MAX = 256;

	int Index_Exposure;   	// 曝光
	int Index_Binary;     	// 二值
	int Index_Brightness; 	// 亮度
	int Index_Contrast;   	// 对比
	int Index_Saturation; 	// 饱和
	int Index_Hue;	      	// 色相


	// 膨胀腐蚀的尺寸 elemengt
	int element_length = 1;
	int element_width = 1;

	// 膨胀腐蚀的矩阵
	Mat element_1 = getStructuringElement(MORPH_RECT, Size(1, 1));
	Mat element_2 = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat element_3 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_4 = getStructuringElement(MORPH_RECT, Size(4, 4));
	Mat element_5 = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat element_6 = getStructuringElement(MORPH_RECT, Size(6, 6));

	Mat src_img;      		// 原图
	Mat target_img;   		// 二值化图像

	Mat b_filter_hsv; 		// hsv 过滤后得到的 蓝色图
	Mat r_filter_hsv; 		// hsv 过滤后的红色图像

	// 中间图像
	Mat b_filter_tmp1;
	Mat b_filter_tmp2;
	Mat r_filter_tmp1;
	Mat r_filter_tmp2_0;
	Mat r_filter_tmp2_1;
	Mat r_filter_tmp2_2;

	// 畸变矫正
	Mat cameraMatrix = (Mat_<double>(3, 3) << 3.4754484856900660e+02, 0, 320, 0, 3.4754484856900660e+02, 240, 0, 0, 1);
	Mat distCoeffs = (Mat_<double>(5, 1) << -3.4474346956242113e-01, 1.4289845255304084e-01, 0, 0, -2.2170221052742516e-02);
	Mat undistortion1;
	Mat undistortion2;
	VideoCapture cap;

	Image_Threshold() {} 								                      	// 类构造函数
	Image_Threshold(VideoCapture cap, string color, int rather_show_img, int frame_width, int frame_height);     	// 类构造函数

	int init(VideoCapture Cap);										   	  		// 输入摄像头，鲁棒性
	Mat equalize(Mat img);                                  					// 直方图均衡化
	void filter_hsv_red_preproc(Mat img, Mat& dst);								// 红灯处理
	void filter_hsv_blue_preproc(Mat img, Mat& dst);							// 蓝灯处理
	void set_capinfo(VideoCapture cap);											// 摄像头参数、不使用
	void print_capinfo(VideoCapture cap);										// 打印参数 不使用
};


#endif
