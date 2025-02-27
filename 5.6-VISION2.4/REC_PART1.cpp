﻿#include "REC_PART1.h"

// 构造函数
Image_Threshold::Image_Threshold(VideoCapture cap, string color, int rather_show_img, int frame_width, int frame_height)
{	
	this->frame_height = frame_height;
	this->frame_width = frame_width;
	this->rather_show_img = rather_show_img;
	thres_color = color;
	int height = cap.get(3);
	int width = cap.get(4);
	cap_height = height;
	cap_width = width;
	// 构造查找表
	initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(), Size(frame_width, frame_height), 0, undistortion1, undistortion2);
}


int Image_Threshold::init(VideoCapture Cap)
{
	Mat IMG;
	cap = Cap;

	cap.set(CAP_PROP_AUTO_EXPOSURE, 0.25);
	cap.set(CAP_PROP_EXPOSURE, 0);
	// set_capinfo(cap);
	cout << "曝光值" << cap.get(CAP_PROP_EXPOSURE);
	// 读取图片
	cap >> IMG; 
	if (IMG.empty())
	{	
		cap.release();
		exit(0);
		return -1;
	}
	// 将图像拷贝一份到src_img
	src_img = IMG.clone();
	// 畸变矫正
	// remap(IMG, src_img, undistortion1, undistortion2, CV_INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
	// undistort(IMG, src_img, cameraMatrix, distCoeffs);
	
	if(thres_color == "red")
		// 识别红灯
		filter_hsv_red_preproc(IMG, r_filter_hsv);
	else if(thres_color == "blue")
		// 识别蓝灯
		filter_hsv_blue_preproc(IMG, b_filter_hsv);
	
	return 1;
}

// 蓝光
void Image_Threshold::filter_hsv_blue_preproc(Mat img, Mat& dst)
{
	Mat imgHSV = img.clone();				// 克隆原图
	cvtColor(imgHSV, imgHSV, CV_BGR2HSV);	// 转换色域 RGB->HSV
	if(rather_show_img >= 2)
		imshow("hsv", imgHSV);				// HSV显示
	Mat HSV[3];								// 三通道
	split(imgHSV, HSV);						// 分通道


	// 对V二值化 
	threshold(HSV[2], HSV[2], 200, 255, THRESH_BINARY);	
	morphologyEx(HSV[2], HSV[2], MORPH_DILATE, element_5);	// 形态学变化 膨胀
	//imshow("vdilate", HSV[2]);

	// HSV 图像通过蒙版
	Mat ttmp;
	//imshow("imghsv", imgHSV);
	imgHSV.copyTo(ttmp, HSV[2]);
	b_filter_tmp1 = ttmp; 

	namedWindow("b_filter_tmp2", 2);
	createTrackbar("BLOWH", "b_filter_tmp2", &BLowH, 255, bshow_hsv, this);
	createTrackbar("BHIGHH", "b_filter_tmp2", &BHighH, 255, bshow_hsv, this);
	createTrackbar("BLOWS", "b_filter_tmp2", &BLowS, 255, bshow_hsv, this);
	createTrackbar("BHIGHS", "b_filter_tmp2", &BHighS, 255, bshow_hsv, this);
	createTrackbar("BLOWV", "b_filter_tmp2", &BLowV, 255, bshow_hsv, this);
	createTrackbar("BHIGHV", "b_filter_tmp2", &BHighV, 255, bshow_hsv, this);
	createTrackbar("ELEMENT_LENGTH", "b_filter_tmp2", &element_length, 15, bshow_hsv, this);
	createTrackbar("ELEMENT_WIDTH", "b_filter_tmp2", &element_width, 15, bshow_hsv, this);

	// 将 b_filter_tmp1 经过HSV过滤后保存在 b_filter_tmp2中 
	// 检查输入图像是否在 Scalar 之间
	inRange(b_filter_tmp1, Scalar(BLowH, BLowS, BLowV), Scalar(BHighH, BHighS, BHighV), b_filter_tmp2);

	target_img = b_filter_tmp2;
	if(rather_show_img >= 1)
		imshow("B_filter_tmp2", b_filter_tmp2);
}


// 红色
void Image_Threshold::filter_hsv_red_preproc(Mat img, Mat& dst)
{
	Mat imgHSV = img.clone();				//克隆原图
	cvtColor(imgHSV, imgHSV, CV_BGR2HSV);	// 转化色域
	if(rather_show_img >= 2)
		imshow("hsv", imgHSV);				// HSV显示
	Mat HSV[3];								// 分三通道
	split(imgHSV, HSV);

	//  膨胀操作矩阵 （模式，大小）
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	//imshow("vraw", HSV[2]);

	// V 二值
	threshold(HSV[2], HSV[2], 200, 255, THRESH_BINARY); // 二值化
	morphologyEx(HSV[2], HSV[2], MORPH_DILATE, element);// 膨胀
	//imshow("vdilate", HSV[2]);


	Mat ttmp;
	//imshow("imghsv", imgHSV);
	imgHSV.copyTo(ttmp, HSV[2]);
	r_filter_tmp1 = ttmp;
	//imshow("R_filter_tmp1", r_filter_tmp1);


	namedWindow("r_filter_tmp2", CV_WINDOW_NORMAL);
	createTrackbar("RLOWH", "r_filter_tmp2", &RLowH, 255, rshow_hsv, this);
	createTrackbar("RHIGHH", "r_filter_tmp2", &RHighH, 255, rshow_hsv, this);
	createTrackbar("RLOWH_2", "r_filter_tmp2", &RLowH_2, 255, rshow_hsv, this);
	createTrackbar("RHIGHH_2", "r_filter_tmp2", &RHighH_2, 255, rshow_hsv, this);
	createTrackbar("RLOWS", "r_filter_tmp2", &RLowS, 255, rshow_hsv, this);
	createTrackbar("RHIGHS", "r_filter_tmp2", &RHighS, 255, rshow_hsv, this);
	createTrackbar("RLOWV", "r_filter_tmp2", &RLowV, 255, rshow_hsv, this);
	createTrackbar("RHIGHV", "r_filter_tmp2", &RHighV, 255, rshow_hsv, this);
	// 两个过滤，两个范围
	inRange(r_filter_tmp1, Scalar(RLowH, RLowS, RLowV), Scalar(RHighH, RHighS, RHighV), r_filter_tmp2_0);
	inRange(r_filter_tmp1, Scalar(RLowH_2, RLowS, RLowV), Scalar(RHighH_2, RHighS, RHighV), r_filter_tmp2_1);
	// 或运算
	r_filter_tmp2_2 = r_filter_tmp2_0 | r_filter_tmp2_1;

	target_img = r_filter_tmp2_2;
	// 显示过滤V后的红光
	if(rather_show_img >= 1)
		imshow("R_filter_tmp2", r_filter_tmp2_2);
}

// 更新蓝光 
void bshow_hsv(int value, void* detector)
{
	Image_Threshold detect = *(Image_Threshold*)detector;
	// 每次调参之后过滤一次
	inRange(detect.b_filter_tmp1, Scalar(detect.BLowH, detect.BLowS, detect.BLowV), Scalar(detect.BHighH, detect.BHighS, detect.BHighV), detect.b_filter_tmp2);
	// 膨胀
	Mat tmp_element = getStructuringElement(MORPH_RECT, Size(detect.element_length, detect.element_width));
	morphologyEx(detect.b_filter_tmp2, detect.b_filter_tmp2, MORPH_DILATE, tmp_element);
	// 显示
	detect.target_img = detect.b_filter_tmp2;
	if(detect.rather_show_img >= 1)
		imshow("B_filter_tmp2", detect.b_filter_tmp2);
}
// 更新红光
void rshow_hsv(int value, void* detector)
{
	Image_Threshold detect = *(Image_Threshold*)detector;
	// 每次更新
	inRange(detect.r_filter_tmp1, Scalar(detect.RLowH, detect.RLowS, detect.RLowV), Scalar(detect.RHighH, detect.RHighS, detect.RHighV), detect.r_filter_tmp2_0);
	inRange(detect.r_filter_tmp1, Scalar(detect.RLowH_2, detect.RLowS, detect.RLowV), Scalar(detect.RHighH_2, detect.RHighS, detect.RHighV), detect.r_filter_tmp2_1);
	detect.r_filter_tmp2_2 = detect.r_filter_tmp2_0 | detect.r_filter_tmp2_1;
	// 膨胀
	morphologyEx(detect.r_filter_tmp2_2, detect.r_filter_tmp2_2, MORPH_DILATE, detect.element_1);
	// 显示
	detect.target_img = detect.r_filter_tmp2_2;
	if(detect.rather_show_img >= 1)
		imshow("R_filter_tmp2", detect.r_filter_tmp2_2);
}


void on_trackbar_exposure(int, void* detector) {
	Image_Threshold detect = *(Image_Threshold*)detector;
	detect.cap.set(CAP_PROP_EXPOSURE, -detect.Index_Exposure );

	//printf("exposure=%.2f\n", detect.cap.get(CAP_PROP_EXPOSURE));	
}

void on_trackbar_brightness(int, void* detector) {
	Image_Threshold detect = *(Image_Threshold*)detector;
	detect.cap.set(CAP_PROP_BRIGHTNESS, detect.Index_Brightness); 

	//printf("brightness=%.2f\n", detect.cap.get(CAP_PROP_BRIGHTNESS));//brightness

}

void on_trackbar_contrast(int, void* detector) {
	Image_Threshold detect = *(Image_Threshold*)detector;
	detect.cap.set(CAP_PROP_CONTRAST, detect.Index_Contrast);

	//printf("contrast=%.2f\n", detect.cap.get(CAP_PROP_CONTRAST)); //contrast

}

void on_trackbar_saturation(int, void* detector) {
	Image_Threshold detect = *(Image_Threshold*)detector;
	detect.cap.set(CAP_PROP_SATURATION, detect.Index_Saturation);

	//printf("saturation=%.2f\n", detect.cap.get(CAP_PROP_SATURATION)); //saturation 

}

void on_trackbar_hue(int, void* detector) {
	Image_Threshold detect = *(Image_Threshold*)detector;
	detect.cap.set(CAP_PROP_HUE, detect.Index_Hue);

	//printf("hue=%.2f\n", detect.cap.get(CAP_PROP_HUE));	

}
// 直方图均衡化处理
Mat Image_Threshold::equalize(Mat img)
{
	vector<Mat> rgb_planes;           
	vector<Mat> result;
	split(img, rgb_planes);                         // 分割图像
	//imshow("raw", rgb_planes[1]);
	Mat output;
	Mat r_hist, g_hist, b_hist;                     // 存储直方图的矩阵
	equalizeHist(rgb_planes[0], rgb_planes[0]);
	equalizeHist(rgb_planes[1], rgb_planes[1]);
	equalizeHist(rgb_planes[2], rgb_planes[2]);

	//imshow("now", rgb_planes[1]);
	merge(rgb_planes, output);

	//imshow("输出", output);

	return output;
}
void Image_Threshold::set_capinfo(VideoCapture cap)
{
	// 设置参数
	//cap.set(CAP_PROP_FRAME_WIDTH, 640);
	//cap.set(CAP_PROP_FRAME_HEIGHT, 480);

	namedWindow("set_info", 2);
	createTrackbar("expose", "set_info", &Index_Exposure, EXPOSURE_MAX, on_trackbar_exposure, this);
	createTrackbar("Bright", "set_info", &Index_Brightness, BRIGHTNESS_MAX, on_trackbar_brightness, this);
	createTrackbar("Contrast", "set_info", &Index_Contrast, CONTRAST_MAX, on_trackbar_contrast, this);
	createTrackbar("Saturation", "set_info", &Index_Saturation, SATURATION_MAX, on_trackbar_saturation, this);
	createTrackbar("Hue", "set_info", &Index_Hue, HUE_MAX, on_trackbar_hue, this);
}
void Image_Threshold::print_capinfo(VideoCapture cap)
{
	printf("width=%.2f\n", cap.get(CAP_PROP_FRAME_WIDTH));      // width
	printf("heigth=%.2f\n", cap.get(CAP_PROP_FRAME_HEIGHT));	// height
	printf("fbs=%.2f\n", cap.get(CAP_PROP_FPS));                // FPS ֡帧率
	printf("brightness=%.2f\n", cap.get(CAP_PROP_BRIGHTNESS));  // brightness 亮度
	printf("contrast=%.2f\n", cap.get(CAP_PROP_CONTRAST));      // contrast 对比
	printf("saturation=%.2f\n", cap.get(CAP_PROP_SATURATION));  // saturation 饱和
	printf("hue=%.2f\n", cap.get(CAP_PROP_HUE));	            // 色度
	printf("exposure=%.2f\n", cap.get(CAP_PROP_EXPOSURE));	    // 曝光
}