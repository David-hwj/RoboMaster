#ifndef REC_PART1_H
#define REC_PART1_H
#include <opencv2/opencv.hpp>
#include <iostream>


using namespace std;
using namespace cv;

void bshow_hsv(int value, void* detector); // չʾ���˺��ͼ��
void rshow_hsv(int value, void* detector); // չʾ���˺��ͼ��


class Image_Threshold
{
public:
	//int BLowH = 80;
	int BLowH = 80;
	//int BHighH = 150;
	int BHighH = 166;

	//int BLowS = 80;
	int BLowS = 65;
	int BHighS = 255;

	int BLowV = 100;
	int BHighV = 255;

	int RLowH = 160;
	int RHighH = 195;
	int RLowH_2 = 0;
	int RHighH_2 = 30;

	int RLowS = 0;
	int RHighS = 90;

	int RLowV = 170;
	int RHighV = 255;

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

	Mat src_img;      // ԭʼͼ��
	Mat target_img;   // ���յõ��Ķ�ֵ��ͼ��

	Mat b_filter_hsv; // hsvͼ�񾭹���hsv����ͨ���Ĺ��˵õ�����ɫͼ��
	Mat r_filter_hsv; // hsvͼ�񾭹���hsv����ͨ���Ĺ��˵õ��ĺ�ɫͼ��

	// �м�ͼ��
	Mat b_filter_tmp1;
	Mat b_filter_tmp2;
	Mat r_filter_tmp1;
	Mat r_filter_tmp2_0;
	Mat r_filter_tmp2_1;
	Mat r_filter_tmp2_2;


	Image_Threshold(); // �๹�캯��

	void hsv_rec(Mat img);
	void filter_hsv_red_preproc(Mat img, Mat& dst);
	void filter_hsv_blue_preproc(Mat img, Mat& dst);

};


#endif
