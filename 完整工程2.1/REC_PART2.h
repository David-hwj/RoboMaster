#ifndef REC_PART2
#define REC_PART2
#include <opencv2/opencv.hpp>
#include <iostream>


using namespace std;
using namespace cv;

double get_point_angle(CvPoint pointO, CvPoint pointA); // ��ȡ�Ƕ�

class Armor_builded // װ�׹���
{
public:
	RotatedRect armorS;
	int build1_No = 0; // ���ɸ�װ�װ���������ı��
	int build2_No = 0;
	int build_features[4]; // �������ĽǶȲ�߶������߶Ȳ��Ȳ�
	int distance; // ��װ�װ�ľ���
	int x_diff; // ��װ�װ�x������������ĵ�����
	int y_diff; // ��װ�װ�y������������ĵ�����
};

class Armor_Detector
{
public:
	/////////////////////////////////////////////////
	////////////////////��������/////////////////////
	/******************1.contours*******************/
	int contour_min = 30;
	int contour_max = 900;
	/******************2.�����Բ*******************/
	double light_wh_rate = 0.8;		   // �������ȣ����˵����ʵ���Բ
	int light_max_length = 100;		   // ������󳤶�
	int light_min_length = 5;		   // ������С����
	int light_lean_1 = 45;		       // ������б�Ƕ�
	int light_lean_2 = 135;		  	   // ������б�Ƕ�
	/******************3.����ƥ��*******************/
	int T_ANGLE_THRE = 20;			   // �����������ǶȲ�
	int T_ANGLE_THRE180 = 20;		   // �����������ǶȲ�
	int T_ANGLE_THREMIN = 3;		   // ����������С�ǶȲ�
	int T_ANGLE_THRE180MIN = 2;		   // ����������С�ǶȲ�
	double T_HIGH_RAT = 0.2;           // �������߲�� �����߶Ȳ�����������x
	double T_HIGH_RAT_ANGLE = 0.34;    // �����Ƕȷ�����С�Ƕȸ߲�� �����߶Ȳ�����������x
	double T_WHIDTH_RAT = 0.4;         // ����������
	double T_WHIDTH_RAT_ANGLE = 0.55;  // ����������
	/******************4.Ѱ�����*******************/



	/////////////////////////////////////////////////
	////////////////////��������/////////////////////
	/******************1.contours*******************/
	Mat src_img;                       // ��ֵ��֮���ͼ��
	vector<vector<Point>> contours_1;  // �Ե�����ȡ����
	vector<vector<Point>> contours_2;  // ��������������С���˺�ʣ�������
	vector<Vec4i> white_hierarchy;     // ����contours_1���ɵĶ���
	Mat contours_result;               // ��ȡ�������ͼ��
	/******************2.�����Բ*******************/
	vector<RotatedRect> light_rect;    // �洢���˺�ĵ�������ľ���
	/******************3.����ƥ��*******************/
	vector<RotatedRect> armors;        // �洢�ܹ���װ�װ�ľ���
	vector<Armor_builded> armor_SECOND;// �洢����֮���װ�װ�
	Mat armor_detect_result;           // װ�װ�ʶ����
	/******************4.Ѱ�����*******************/
	Armor_builded shoot_object;        // �洢��������
	int best_x_diff = 0;               // ���װ�װ�ĺ�����
	int best_y_diff = 0;               // ���װ�װ��������
	int count_no_object = 0;           // ��������x��δʶ���֡
	Mat best_armor_detect;             // ���װ�װ���

	/////////////////////////////////////////////////
	////////////////////��������/////////////////////
	/******************0.���캯��*******************/
	Armor_Detector();                  // �๹�캯��
	void init(Mat src_img, Mat thres_img);                       // ��ʼ������
	/******************1.contours*******************/
	void find_contours(Mat img);
	/******************2.�����Բ*******************/
	void fitting_contours(Mat img);
	/******************3.����ƥ��*******************/
	void fitting_armor(Mat img);
	/******************4.Ѱ�����*******************/
	void find_shoot_object(Mat img);



	/////////////////////////////////////////////////
	////////////////////��������/////////////////////
	// ����elemengt�ĳߴ�
	int element_length = 1;
	int element_width = 1;
	// ���ڸ�ʴ���͵���״
	Mat element_1 = getStructuringElement(MORPH_RECT, Size(1, 1));
	Mat element_2 = getStructuringElement(MORPH_RECT, Size(2, 2));
	Mat element_3 = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat element_4 = getStructuringElement(MORPH_RECT, Size(4, 4));
	Mat element_5 = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat element_6 = getStructuringElement(MORPH_RECT, Size(6, 6));
};


#endif#
