#include "GetBinary.h"
#include "REC_PART2.h"
const int BINARY_MAX = 255; //����ֵ����ֵ
const int EXPOSURE_MAX = 11;
const int BRIGHTNESS_MAX = 255;
const int CONTRAST_MAX = 94;
const int SATURATION_MAX = 99;
const int HUE_MAX = 256;
char c = 0;
VideoCapture  Capture(1);


int Index_Binary;	//��ɫ��ֵ�±�
int Index_Exposure; //�ع��
int Index_Brightness; //����
int Index_Contrast;		//�Ա�
int Index_Saturation;	//���Ͷ�
int Index_Hue;	//ɫ��
Mat Hsv_Frame, RED_result, BLUE_result;	//ת��HSV+���ֽ��
Mat RedChannel;		//��ɫͨ��
Mat BlueChannel;	//��ɫͨ��
Mat HueChannel;// = HSVChannels.at(0);		//ɫ��
Mat ValueChannel;

void openCapture() {
	
	printf("width=%.2f\n", Capture.get(CAP_PROP_FRAME_WIDTH)); //��
	printf("heigth=%.2f\n", Capture.get(CAP_PROP_FRAME_HEIGHT));	//��
	printf("fbs=%.2f\n", Capture.get(CAP_PROP_FPS)); //FPS ֡��
	printf("brightness=%.2f\n", Capture.get(CAP_PROP_BRIGHTNESS));//brightness ����
	printf("contrast=%.2f\n", Capture.get(CAP_PROP_CONTRAST)); //contrast �Աȶ�
	printf("saturation=%.2f\n", Capture.get(CAP_PROP_SATURATION)); //saturation ���Ͷ�
	printf("hue=%.2f\n", Capture.get(CAP_PROP_HUE));	//ɫ��
	printf("exposure=%.2f\n", Capture.get(CAP_PROP_EXPOSURE));	//�ع�
	namedWindow("����", 0);
	Capture.set(CAP_PROP_FRAME_WIDTH, 640);
	Capture.set(CAP_PROP_FRAME_HEIGHT, 480);
	Capture.set(CAP_PROP_FPS, 60);//֡��
	Capture.set(CAP_PROP_BRIGHTNESS, -131); //����
	Capture.set(CAP_PROP_CONTRAST, 26);//�Ա�
	Capture.set(CAP_PROP_SATURATION, 93);//����
	Capture.set(CAP_PROP_HUE, 0);
	Capture.set(CAP_PROP_EXPOSURE, 0);
	createTrackbar("��ֵ����ֵ", "����", &Index_Binary, BINARY_MAX, on_trackbar_binary);
	createTrackbar("�ع��", "����", &Index_Exposure, EXPOSURE_MAX, on_trackbar_exposure);
	createTrackbar("����", "����", &Index_Brightness, BRIGHTNESS_MAX, on_trackbar_brightness);
	createTrackbar("�Ա�", "����", &Index_Contrast, CONTRAST_MAX, on_trackbar_contrast);
	createTrackbar("����", "����", &Index_Saturation, SATURATION_MAX, on_trackbar_saturation);
	createTrackbar("ɫ��", "����", &Index_Hue, HUE_MAX, on_trackbar_hue);
	while (1) {
		//printf("exposure=%.2f\n", Capture.get(CAP_PROP_EXPOSURE));	//�ع�
		Mat frame;
		Capture >> frame;

		imshow("frame", frame); //��ʾԭͼ
		Mat result0;
		/*binaryImg(frame, result0);
		imshow("result", result0);*/
		cvtColor(frame, Hsv_Frame, COLOR_BGR2HSV); //ԭͼתΪhsv
		vector<Mat> BGRChannels;
		vector<Mat> HSVChannels;
		vector<Mat> RVC;	//red +value ͨ���ϲ�
		vector<Mat> BHC;	//bule +hue ͨ���ϲ�
		split(frame, BGRChannels);	//BGR��ͨ������
		split(frame, HSVChannels);	//HSV��ͨ������

		RedChannel = BGRChannels.at(2);     //��ɫͨ�� 
		BlueChannel = BGRChannels.at(0);	//��ɫͨ��
		HueChannel = HSVChannels.at(0);		//ɫ��
		ValueChannel = HSVChannels.at(2);			//����

		threshold(ValueChannel, RED_result, Index_Binary, 255, THRESH_BINARY); //��ֵ��ֵ�� ��ɫ
		threshold(BlueChannel, BLUE_result, Index_Binary, 255, THRESH_BINARY); //��ֵ��ֵ�� ��ɫ
		
		
		imshow("RED", RED_result);
		imshow("V", ValueChannel);
		/*imshow("Blue", BLUE_result);
		Armor_Detector tmp;
		tmp.init(frame, RED_result);*/

		if (waitKey(30) == 27) {
			break;
		}
	}
}

void on_trackbar_binary(int, void*) {
	threshold(RedChannel, RED_result, Index_Binary, 255, THRESH_BINARY_INV); //��ֵ��ֵ��
	threshold(BlueChannel, BLUE_result, Index_Binary, 255, THRESH_BINARY_INV); //��ֵ��ֵ��
	//threshold(HueChannel, RED_result, Index_Binary, 255, THRESH_BINARY); //��ֵ��ֵ�� ��ɫ
	//threshold(ValueChannel, BLUE_result, Index_Binary, 255, THRESH_BINARY); //��ֵ��ֵ�� ��ɫ
}
void on_trackbar_exposure(int, void*) {
	Capture.set(CAP_PROP_EXPOSURE, Index_Exposure - 11);
	
	printf("exposure=%.2f\n", Capture.get(CAP_PROP_EXPOSURE));	//�ع�
}
void on_trackbar_brightness(int, void*) {
	Capture.set(CAP_PROP_BRIGHTNESS, Index_Brightness); //����

	printf("brightness=%.2f\n", Capture.get(CAP_PROP_BRIGHTNESS));//brightness ����

}
void on_trackbar_contrast(int, void*) {
	Capture.set(CAP_PROP_CONTRAST, Index_Contrast);//�Ա�
	
	printf("contrast=%.2f\n", Capture.get(CAP_PROP_CONTRAST)); //contrast �Աȶ�

}
void on_trackbar_saturation(int, void*) {
	Capture.set(CAP_PROP_SATURATION, Index_Saturation);//����
	
	printf("saturation=%.2f\n", Capture.get(CAP_PROP_SATURATION)); //saturation ���Ͷ�

}
void on_trackbar_hue(int, void*) {
	Capture.set(CAP_PROP_HUE, Index_Hue);//����
	
	printf("hue=%.2f\n", Capture.get(CAP_PROP_HUE));	//ɫ��
	
}
