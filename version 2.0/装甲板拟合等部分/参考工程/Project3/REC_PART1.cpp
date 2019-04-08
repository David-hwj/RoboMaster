#include "REC_PART1.h"

// 构造函数
Image_Threshold::Image_Threshold(){}


void Image_Threshold::hsv_rec(Mat img)
{
	Mat IMG = img.clone();
	src_img = img.clone();
	filter_hsv_blue_preproc(IMG, r_filter_hsv);
}

//预处理蓝色
void Image_Threshold::filter_hsv_blue_preproc(Mat img, Mat & dst)
{
	Mat imgHSV = img.clone();
	cvtColor(imgHSV, imgHSV, CV_BGR2HSV);
	imshow("hsv", imgHSV);
	Mat HSV[3];
	split(imgHSV, HSV);
	/*
	opencv 的H范围是0~180，红色的H范围大概是(0~10)∪(156,180)
	S是饱和度，一般是大于一个值,S过低就是灰色（参考值S>80)，
	V是亮度，过低就是黑色，过高就是白色(参考值220>V>50)。
	*/
	//imshow("vraw", HSV[2]);

	// 使用V通道二值化，阈值为200
	threshold(HSV[2], HSV[2], 200, 255, THRESH_BINARY);//最好设为动态阈值
	//imshow("vthresh", HSV[2]);
	morphologyEx(HSV[2], HSV[2], MORPH_DILATE, element_5);// 形态学变换函数，膨胀
	//imshow("vdilate", HSV[2]);

	// 将HSV图像透过一张蒙版，拷贝到b_filter_tmp1中
	Mat ttmp;
	//imshow("imghsv", imgHSV);
	imgHSV.copyTo(ttmp, HSV[2]);
	b_filter_tmp1 = ttmp;
	imshow("B_filter_tmp1", b_filter_tmp1);

	namedWindow("b_filter_tmp2", 2);
	createTrackbar("BLOWH", "b_filter_tmp2", &BLowH, 255, bshow_hsv, this);
	createTrackbar("BHIGHH", "b_filter_tmp2", &BHighH, 255, bshow_hsv, this);
	createTrackbar("BLOWS", "b_filter_tmp2", &BLowS, 255, bshow_hsv, this);
	createTrackbar("BHIGHS", "b_filter_tmp2", &BHighS, 255, bshow_hsv, this);
	createTrackbar("BLOWV", "b_filter_tmp2", &BLowV, 255, bshow_hsv, this);
	createTrackbar("BHIGHV", "b_filter_tmp2", &BHighV, 255, bshow_hsv, this);
	createTrackbar("ELEMENT_LENGTH", "b_filter_tmp2", &element_length, 15, bshow_hsv, this);
	createTrackbar("ELEMENT_WIDTH", "b_filter_tmp2", &element_width, 15, bshow_hsv, this);

	//将b_filter_tmp1经过HSV过滤后保存在b_filter_tmp2中
	inRange(b_filter_tmp1, Scalar(BLowH, BLowS, BLowV), Scalar(BHighH, BHighS, BHighV), b_filter_tmp2);

	target_img = b_filter_tmp2;
	imshow("B_filter_tmp2", b_filter_tmp2);
}


//预处理红色
void Image_Threshold::filter_hsv_red_preproc(Mat img, Mat & dst)
{
	Mat imgHSV = img.clone();
	cvtColor(imgHSV, imgHSV, CV_BGR2HSV);
	imshow("hsv", imgHSV);
	Mat HSV[3];
	split(imgHSV, HSV);
	/*
	opencv 的H范围是0~180，红色的H范围大概是(0~10)∪(156,180)
	S是饱和度，一般是大于一个值,S过低就是灰色（参考值S>80)，
	V是亮度，过低就是黑色，过高就是白色(参考值220>V>50)。
	*/
	// 获得一个形状
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	imshow("vraw", HSV[2]);

	// 使用V通道二值化，阈值为200
	threshold(HSV[2], HSV[2], 200, 255, THRESH_BINARY);//最好设为动态阈值
	//imshow("vthresh", HSV[2]);
	morphologyEx(HSV[2], HSV[2], MORPH_DILATE, element);// 形态学变换函数，膨胀
	//imshow("vdilate", HSV[2]);

	// 将HSV图像透过一张蒙版，拷贝到r_filter_tmp1中
	Mat ttmp;
	//imshow("imghsv", imgHSV);
	imgHSV.copyTo(ttmp, HSV[2]);
	r_filter_tmp1 = ttmp;
	imshow("R_filter_tmp1", r_filter_tmp1);

	// 创建窗口
	namedWindow("r_filter_tmp2", CV_WINDOW_NORMAL);
	// 创建调整条
	createTrackbar("RLOWH", "r_filter_tmp2", &RLowH, 255, rshow_hsv, this);
	createTrackbar("RHIGHH", "r_filter_tmp2", &RHighH, 255, rshow_hsv, this);
	createTrackbar("RLOWH_2", "r_filter_tmp2", &RLowH_2, 255, rshow_hsv, this);
	createTrackbar("RHIGHH_2", "r_filter_tmp2", &RHighH_2, 255, rshow_hsv, this);
	createTrackbar("RLOWS", "r_filter_tmp2", &RLowS, 255, rshow_hsv, this);
	createTrackbar("RHIGHS", "r_filter_tmp2", &RHighS, 255, rshow_hsv, this);
	createTrackbar("RLOWV", "r_filter_tmp2", &RLowV, 255, rshow_hsv, this);
	createTrackbar("RHIGHV", "r_filter_tmp2", &RHighV, 255, rshow_hsv, this);
	// 过滤
	inRange(r_filter_tmp1, Scalar(RLowH, RLowS, RLowV), Scalar(RHighH, RHighS, RHighV), r_filter_tmp2_0);
	inRange(r_filter_tmp1, Scalar(RLowH_2, RLowS, RLowV), Scalar(RHighH_2, RHighS, RHighV), r_filter_tmp2_1);
	// 合成
	r_filter_tmp2_2 = r_filter_tmp2_0 | r_filter_tmp2_1;
	// 保存
	target_img = r_filter_tmp2_2;
	// 显示
	imshow("R_filter_tmp2", r_filter_tmp2_2);
}

// 显示蓝色
void bshow_hsv(int value, void* detector)
{
	Image_Threshold detect = *(Image_Threshold*)detector;
	// 过滤
	inRange(detect.b_filter_tmp1, Scalar(detect.BLowH, detect.BLowS, detect.BLowV), Scalar(detect.BHighH, detect.BHighS, detect.BHighV), detect.b_filter_tmp2);
	// 膨胀
	Mat tmp_element = getStructuringElement(MORPH_RECT, Size(detect.element_length, detect.element_width));
	morphologyEx(detect.b_filter_tmp2, detect.b_filter_tmp2, MORPH_DILATE, tmp_element);// 形态学变换函数，膨胀
	// 保存
	detect.target_img = detect.b_filter_tmp2;
	imshow("B_filter_tmp2", detect.b_filter_tmp2);
}
// 显示红色
void rshow_hsv(int value, void* detector)
{
	Image_Threshold detect = *(Image_Threshold*)detector;
	// 过滤
	inRange(detect.r_filter_tmp1, Scalar(detect.RLowH, detect.RLowS, detect.RLowV), Scalar(detect.RHighH, detect.RHighS, detect.RHighV), detect.r_filter_tmp2_0);
	inRange(detect.r_filter_tmp1, Scalar(detect.RLowH_2, detect.RLowS, detect.RLowV), Scalar(detect.RHighH_2, detect.RHighS, detect.RHighV), detect.r_filter_tmp2_1);
	detect.r_filter_tmp2_2 = detect.r_filter_tmp2_0 | detect.r_filter_tmp2_1;
	// 膨胀
	//morphologyEx(detect.r_filter_tmp2_2, detect.r_filter_tmp2_2, MORPH_DILATE, detect.element);// 形态学变换函数，膨胀
	// 保存
	detect.target_img = detect.r_filter_tmp2_2;
	imshow("R_filter_tmp2", detect.r_filter_tmp2_2);
}