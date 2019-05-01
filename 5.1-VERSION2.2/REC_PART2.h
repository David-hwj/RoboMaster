#ifndef REC_PART2
#define REC_PART2
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

double get_point_angle(CvPoint pointO, CvPoint pointA); // 获取角度 p0 p1 的夹角

class Armor_builded //  图像处理过程
{
public:
	RotatedRect armorS;
	int build1_No = 0; 		// 灯柱1
	int build2_No = 0;		// 灯柱2
	int build_features[4];	// 两个灯柱的角度差、高度坐标差、高度差、宽度差
	int distance; 			// 中心点与装甲板的距离
	int x_diff;				// 装甲板x坐标 与 相机中心点 差值
	int y_diff;				// 装甲板Y坐标 与 相机中心点 差值
};

class Armor_Detector
{
public:
	/////////////////////////////////////////////////
	////////////////////参数区域//////////////////////
	/******************1.contours*******************/
	int contour_min = 5; 		//点数 最大值
	int contour_max = 900;		//最小值

	/******************2.拟合椭圆*******************/
	double light_wh_rate = 0.8;		   // 灯条宽长比 ，保留小于0.8
	int light_max_length = 100;		   // 过滤灯条长度，最大值
	int light_min_length = 5;		   // 过滤灯条长度最小值
	int light_lean_1 = 45;		       // 灯条倾斜角度 最小值
	int light_lean_2 = 135;		  	   // 灯条倾斜角度 最大值
	/******************3.两两匹配 ？？？？？？*******************/
	int T_ANGLE_THRE = 20;			   // 灯条正向最大角度差
	int T_ANGLE_THRE180 = 20;		   // 灯条反向最大角度差
	int T_ANGLE_THREMIN = 3;		   // 灯条正向最小角度差
	int T_ANGLE_THRE180MIN = 2;		   // 灯条反向最大角度差
	double T_HIGH_RAT = 0.2;           // 灯条最大角度高差比 灯条高度差是最大灯柱的x倍
	double T_HIGH_RAT_ANGLE = 0.34;    // 灯条最小角度高差比 灯条高度差是最大灯柱的x倍
	double T_WHIDTH_RAT = 0.4;         // 灯条最小宽差比
	double T_WHIDTH_RAT_ANGLE = 0.55;  // 灯条最大宽差比
	/******************4.串口*******************/
	int fd;								//串口号


	/////////////////////////////////////////////////
	////////////////////变量区域//////////////////////
	/******************1.contours*******************/
	Mat src_img;                      	// 输入的二值图
	vector<vector<Point> > contours_1;  // 对灯柱提取轮廓
	vector<vector<Point> > contours_2;  // 经过大小过滤后的轮廓（5-900）
	vector<Vec4i> white_hierarchy;		// 发现轮廓的剩余量 contours_1 ？？有待研究
	Mat contours_result;				// 提取轮廓后的图像 
	/******************2.拟合椭圆*******************/
	vector<RotatedRect> light_rect;		// 存储过滤后的灯条 矩阵
	/******************3.两两匹配*******************/
	vector<RotatedRect> armors;			// 存储装甲板的矩阵（两两匹配后的）
	vector<Armor_builded> armor_SECOND;	// 存储过滤后的装甲板 
	Mat armor_detect_result;        	// 装甲板识别结果
	/******************4.寻找最佳*******************/
	Armor_builded shoot_object;        // 寻找最佳射击点
	int best_x_diff = 0;               // 最佳装甲板的横坐标
	int best_y_diff = 0;               // 最佳装甲板的纵坐标
	int count_no_object = 0;           // 连续出现x个未识别的帧
	Mat best_armor_detect;             // 最佳装甲板结果

	/////////////////////////////////////////////////
	////////////////////函数区域//////////////////////
	/******************0.构造函数*******************/
	Armor_Detector();                        // 类构造函数
	~Armor_Detector();
	void init(Mat src_img, Mat thres_img);   // 初始化函数
	/******************1.contours*******************/
	void find_contours(Mat img);
	/******************2.拟合椭圆*******************/
	void fitting_contours(Mat img);
	/******************3.两两匹配*******************/
	void fitting_armor(Mat img);
	/******************4.寻找最佳*******************/
	void find_shoot_object(Mat img);



	/////////////////////////////////////////////////
	////////////////////其他数据//////////////////////
	// 膨胀腐蚀的尺寸elemengt
	int element_length = 1;
	int element_width = 1;
	//  用于腐蚀膨胀
	Mat element_1 = getStructuringElement(MORPH_RECT, Size(1, 1));
	Mat element_2 = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat element_3 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_4 = getStructuringElement(MORPH_RECT, Size(4, 4));
	Mat element_5 = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat element_6 = getStructuringElement(MORPH_RECT, Size(6, 6));
};


#endif
