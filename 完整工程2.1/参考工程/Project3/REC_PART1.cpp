#include "REC_PART1.h"

// ���캯��
Image_Threshold::Image_Threshold(){}


// ���cap��ͼ�񣬷���1�����򷵻�0
int Image_Threshold::hsv_rec(VideoCapture Cap)
{
	Mat IMG;
	cap = Cap;

	// ����cap
	set_capinfo(cap);

	cap >> IMG;
	if (IMG.empty())
		// ûͼ�񣬷���0
		return 0;
	src_img = IMG.clone();
	imshow("ԭʼͼ��", src_img);
	filter_hsv_blue_preproc(IMG, r_filter_hsv);

	// �ɹ�, ����1
	return 1;
}

void Image_Threshold::set_capinfo(VideoCapture cap)
{
	// ��������ͷ����
	cap.set(CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CAP_PROP_FRAME_HEIGHT, 480);

	namedWindow("�������", 2);
	createTrackbar("�ع��", "�������", &Index_Exposure, EXPOSURE_MAX, on_trackbar_exposure, this);
	//createTrackbar("��ֵ����ֵ", "�������", &Index_Binary, BINARY_MAX, on_trackbar_binary, this);
	createTrackbar("����", "�������", &Index_Brightness, BRIGHTNESS_MAX, on_trackbar_brightness, this);
	createTrackbar("�Ա�", "�������", &Index_Contrast, CONTRAST_MAX, on_trackbar_contrast, this);
	createTrackbar("����", "�������", &Index_Saturation, SATURATION_MAX, on_trackbar_saturation, this);
	createTrackbar("ɫ��", "�������", &Index_Hue, HUE_MAX, on_trackbar_hue, this);
}
void Image_Threshold::print_capinfo(VideoCapture cap)
{
	printf("width=%.2f\n", cap.get(CAP_PROP_FRAME_WIDTH));      // ��
	printf("heigth=%.2f\n", cap.get(CAP_PROP_FRAME_HEIGHT));	// ��
	printf("fbs=%.2f\n", cap.get(CAP_PROP_FPS));                // FPS ֡��
	printf("brightness=%.2f\n", cap.get(CAP_PROP_BRIGHTNESS));  // brightness ����
	printf("contrast=%.2f\n", cap.get(CAP_PROP_CONTRAST));      // contrast �Աȶ�
	printf("saturation=%.2f\n", cap.get(CAP_PROP_SATURATION));  // saturation ���Ͷ�
	printf("hue=%.2f\n", cap.get(CAP_PROP_HUE));	            // ɫ��
	printf("exposure=%.2f\n", cap.get(CAP_PROP_EXPOSURE));	    // �ع�
}
//Ԥ������ɫ
void Image_Threshold::filter_hsv_blue_preproc(Mat img, Mat & dst)
{
	Mat imgHSV = img.clone();
	cvtColor(imgHSV, imgHSV, CV_BGR2HSV);
	imshow("hsv", imgHSV);
	Mat HSV[3];
	split(imgHSV, HSV);
	/*
	opencv ��H��Χ��0~180����ɫ��H��Χ�����(0~10)��(156,180)
	S�Ǳ��Ͷȣ�һ���Ǵ���һ��ֵ,S���;��ǻ�ɫ���ο�ֵS>80)��
	V�����ȣ����;��Ǻ�ɫ�����߾��ǰ�ɫ(�ο�ֵ220>V>50)��
	*/
	//imshow("vraw", HSV[2]);

	// ʹ��Vͨ����ֵ������ֵΪ200
	threshold(HSV[2], HSV[2], 200, 255, THRESH_BINARY);//�����Ϊ��̬��ֵ
	//imshow("vthresh", HSV[2]);
	morphologyEx(HSV[2], HSV[2], MORPH_DILATE, element_5);// ��̬ѧ�任����������
	//imshow("vdilate", HSV[2]);

	// ��HSVͼ��͸��һ���ɰ棬������b_filter_tmp1��
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

	//��b_filter_tmp1����HSV���˺󱣴���b_filter_tmp2��
	inRange(b_filter_tmp1, Scalar(BLowH, BLowS, BLowV), Scalar(BHighH, BHighS, BHighV), b_filter_tmp2);

	target_img = b_filter_tmp2;
	imshow("B_filter_tmp2", b_filter_tmp2);
}


//Ԥ�����ɫ
void Image_Threshold::filter_hsv_red_preproc(Mat img, Mat & dst)
{
	Mat imgHSV = img.clone();
	cvtColor(imgHSV, imgHSV, CV_BGR2HSV);
	imshow("hsv", imgHSV);
	Mat HSV[3];
	split(imgHSV, HSV);
	/*
	opencv ��H��Χ��0~180����ɫ��H��Χ�����(0~10)��(156,180)
	S�Ǳ��Ͷȣ�һ���Ǵ���һ��ֵ,S���;��ǻ�ɫ���ο�ֵS>80)��
	V�����ȣ����;��Ǻ�ɫ�����߾��ǰ�ɫ(�ο�ֵ220>V>50)��
	*/
	// ���һ����״
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	imshow("vraw", HSV[2]);

	// ʹ��Vͨ����ֵ������ֵΪ200
	threshold(HSV[2], HSV[2], 200, 255, THRESH_BINARY);//�����Ϊ��̬��ֵ
	//imshow("vthresh", HSV[2]);
	morphologyEx(HSV[2], HSV[2], MORPH_DILATE, element);// ��̬ѧ�任����������
	//imshow("vdilate", HSV[2]);

	// ��HSVͼ��͸��һ���ɰ棬������r_filter_tmp1��
	Mat ttmp;
	//imshow("imghsv", imgHSV);
	imgHSV.copyTo(ttmp, HSV[2]);
	r_filter_tmp1 = ttmp;
	imshow("R_filter_tmp1", r_filter_tmp1);

	// ��������
	namedWindow("r_filter_tmp2", CV_WINDOW_NORMAL);
	// ����������
	createTrackbar("RLOWH", "r_filter_tmp2", &RLowH, 255, rshow_hsv, this);
	createTrackbar("RHIGHH", "r_filter_tmp2", &RHighH, 255, rshow_hsv, this);
	createTrackbar("RLOWH_2", "r_filter_tmp2", &RLowH_2, 255, rshow_hsv, this);
	createTrackbar("RHIGHH_2", "r_filter_tmp2", &RHighH_2, 255, rshow_hsv, this);
	createTrackbar("RLOWS", "r_filter_tmp2", &RLowS, 255, rshow_hsv, this);
	createTrackbar("RHIGHS", "r_filter_tmp2", &RHighS, 255, rshow_hsv, this);
	createTrackbar("RLOWV", "r_filter_tmp2", &RLowV, 255, rshow_hsv, this);
	createTrackbar("RHIGHV", "r_filter_tmp2", &RHighV, 255, rshow_hsv, this);
	// ����
	inRange(r_filter_tmp1, Scalar(RLowH, RLowS, RLowV), Scalar(RHighH, RHighS, RHighV), r_filter_tmp2_0);
	inRange(r_filter_tmp1, Scalar(RLowH_2, RLowS, RLowV), Scalar(RHighH_2, RHighS, RHighV), r_filter_tmp2_1);
	// �ϳ�
	r_filter_tmp2_2 = r_filter_tmp2_0 | r_filter_tmp2_1;
	// ����
	target_img = r_filter_tmp2_2;
	// ��ʾ
	imshow("R_filter_tmp2", r_filter_tmp2_2);
}

// ��ʾ��ɫ
void bshow_hsv(int value, void* detector)
{
	Image_Threshold detect = *(Image_Threshold*)detector;
	// ����
	inRange(detect.b_filter_tmp1, Scalar(detect.BLowH, detect.BLowS, detect.BLowV), Scalar(detect.BHighH, detect.BHighS, detect.BHighV), detect.b_filter_tmp2);
	// ����
	Mat tmp_element = getStructuringElement(MORPH_RECT, Size(detect.element_length, detect.element_width));
	morphologyEx(detect.b_filter_tmp2, detect.b_filter_tmp2, MORPH_DILATE, tmp_element);// ��̬ѧ�任����������
	// ����
	detect.target_img = detect.b_filter_tmp2;
	imshow("B_filter_tmp2", detect.b_filter_tmp2);
}
// ��ʾ��ɫ
void rshow_hsv(int value, void* detector)
{
	Image_Threshold detect = *(Image_Threshold*)detector;
	// ����
	inRange(detect.r_filter_tmp1, Scalar(detect.RLowH, detect.RLowS, detect.RLowV), Scalar(detect.RHighH, detect.RHighS, detect.RHighV), detect.r_filter_tmp2_0);
	inRange(detect.r_filter_tmp1, Scalar(detect.RLowH_2, detect.RLowS, detect.RLowV), Scalar(detect.RHighH_2, detect.RHighS, detect.RHighV), detect.r_filter_tmp2_1);
	detect.r_filter_tmp2_2 = detect.r_filter_tmp2_0 | detect.r_filter_tmp2_1;
	// ����
	//morphologyEx(detect.r_filter_tmp2_2, detect.r_filter_tmp2_2, MORPH_DILATE, detect.element);// ��̬ѧ�任����������
	// ����
	detect.target_img = detect.r_filter_tmp2_2;
	imshow("R_filter_tmp2", detect.r_filter_tmp2_2);
}


// ��������ͷ���ع��
void on_trackbar_exposure(int, void* detector) {
	Image_Threshold detect = *(Image_Threshold*)detector;
	detect.cap.set(CAP_PROP_EXPOSURE, detect.Index_Exposure - 11);

	printf("exposure=%.2f\n", detect.cap.get(CAP_PROP_EXPOSURE));	//�ع�
}

// ��������ͷ������
void on_trackbar_brightness(int, void* detector) {
	Image_Threshold detect = *(Image_Threshold*)detector;
	detect.cap.set(CAP_PROP_BRIGHTNESS, detect.Index_Brightness); //����

	printf("brightness=%.2f\n", detect.cap.get(CAP_PROP_BRIGHTNESS));//brightness ����

}
// ��������ͷ�ĶԱȶ�
void on_trackbar_contrast(int, void* detector) {
	Image_Threshold detect = *(Image_Threshold*)detector;
	detect.cap.set(CAP_PROP_CONTRAST, detect.Index_Contrast);//�Ա�

	printf("contrast=%.2f\n", detect.cap.get(CAP_PROP_CONTRAST)); //contrast �Աȶ�

}
// ��������ͷ�ı��Ͷ�
void on_trackbar_saturation(int, void*detector) {
	Image_Threshold detect = *(Image_Threshold*)detector;
	detect.cap.set(CAP_PROP_SATURATION, detect.Index_Saturation);//����

	printf("saturation=%.2f\n", detect.cap.get(CAP_PROP_SATURATION)); //saturation ���Ͷ�

}
// ��������ͷ��ɫ��
void on_trackbar_hue(int, void*detector) {
	Image_Threshold detect = *(Image_Threshold*)detector;
	detect.cap.set(CAP_PROP_HUE, detect.Index_Hue);//����

	printf("hue=%.2f\n", detect.cap.get(CAP_PROP_HUE));	//ɫ��

}