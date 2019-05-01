#include "REC_PART1.h"

// 构造函数
Image_Threshold::Image_Threshold()
{

}

// 识别入口，输入相机
int Image_Threshold::hsv_rec(VideoCapture Cap)
{
	Mat IMG;
	cap = Cap;
	set_capinfo(cap);

	// 读取图片
	cap >> IMG;
	if (IMG.empty())
		return 0;
	src_img = IMG.clone();
	// 识别蓝灯
	filter_hsv_blue_preproc(IMG, r_filter_hsv);

	return 1;
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
	//printf("width=%.2f\n", cap.get(CAP_PROP_FRAME_WIDTH));      // width
	//printf("heigth=%.2f\n", cap.get(CAP_PROP_FRAME_HEIGHT));	// height
	//printf("fbs=%.2f\n", cap.get(CAP_PROP_FPS));                // FPS ֡帧率
	//printf("brightness=%.2f\n", cap.get(CAP_PROP_BRIGHTNESS));  // brightness 亮度
	//printf("contrast=%.2f\n", cap.get(CAP_PROP_CONTRAST));      // contrast 对比
	//printf("saturation=%.2f\n", cap.get(CAP_PROP_SATURATION));  // saturation 饱和
	//printf("hue=%.2f\n", cap.get(CAP_PROP_HUE));	            // 色度
	//printf("exposure=%.2f\n", cap.get(CAP_PROP_EXPOSURE));	    // 曝光
}

//	蓝光
void Image_Threshold::filter_hsv_blue_preproc(Mat img, Mat& dst)
{
	Mat imgHSV = img.clone();				// 克隆原图
	cvtColor(imgHSV, imgHSV, CV_BGR2HSV);	// 转换色域 RGB->HSV
	imshow("hsv", imgHSV);					// HSV显示
	Mat HSV[3];								// 三通道
	split(imgHSV, HSV);						// 分通道


	// 对V二值化 
	threshold(HSV[2], HSV[2], 200, 255, THRESH_BINARY);		// 
	morphologyEx(HSV[2], HSV[2], MORPH_DILATE, element_5);	// 形态学变化 膨胀
	//imshow("vdilate", HSV[2]);

	// HSV 图像通过蒙版
	Mat ttmp;
	//imshow("imghsv", imgHSV);
	imgHSV.copyTo(ttmp, HSV[2]);
	b_filter_tmp1 = ttmp;  			// 

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
	imshow("B_filter_tmp2", b_filter_tmp2);
}


// 红色
void Image_Threshold::filter_hsv_red_preproc(Mat img, Mat& dst)
{
	Mat imgHSV = img.clone();				//克隆原图
	cvtColor(imgHSV, imgHSV, CV_BGR2HSV);	// 转化色域
	imshow("hsv", imgHSV);					// HSV显示
	Mat HSV[3];								// 分三通道
	split(imgHSV, HSV);

	//  膨胀操作矩阵 （模式，大小）
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	imshow("vraw", HSV[2]);

	// V 二值
	threshold(HSV[2], HSV[2], 200, 255, THRESH_BINARY); // 二值化
	//imshow("vthresh", HSV[2]);
	morphologyEx(HSV[2], HSV[2], MORPH_DILATE, element);// 膨胀
	//imshow("vdilate", HSV[2]);


	Mat ttmp;
	//imshow("imghsv", imgHSV);
	imgHSV.copyTo(ttmp, HSV[2]);
	r_filter_tmp1 = ttmp;
	imshow("R_filter_tmp1", r_filter_tmp1);

	// 
	namedWindow("r_filter_tmp2", CV_WINDOW_NORMAL);
	// 
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
	// 显示过滤V后的 红光
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
	//morphologyEx(detect.r_filter_tmp2_2, detect.r_filter_tmp2_2, MORPH_DILATE, detect.element);
	// 显示
	detect.target_img = detect.r_filter_tmp2_2;
	imshow("R_filter_tmp2", detect.r_filter_tmp2_2);
}


void on_trackbar_exposure(int, void* detector) {
	Image_Threshold detect = *(Image_Threshold*)detector;
	detect.cap.set(CAP_PROP_EXPOSURE, detect.Index_Exposure - 11);

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
