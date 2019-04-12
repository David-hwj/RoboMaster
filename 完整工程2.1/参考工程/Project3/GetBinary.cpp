#include "GetBinary.h"
#include "REC_PART2.h"
const int BINARY_MAX = 255; //最大二值化阈值
const int EXPOSURE_MAX = 11;
const int BRIGHTNESS_MAX = 255;
const int CONTRAST_MAX = 94;
const int SATURATION_MAX = 99;
const int HUE_MAX = 256;
char c = 0;
VideoCapture  Capture(1);


int Index_Binary;	//颜色阈值下标
int Index_Exposure; //曝光度
int Index_Brightness; //亮度
int Index_Contrast;		//对比
int Index_Saturation;	//饱和度
int Index_Hue;	//色相
Mat Hsv_Frame, RED_result, BLUE_result;	//转换HSV+两种结果
Mat RedChannel;		//红色通道
Mat BlueChannel;	//蓝色通道
Mat HueChannel;// = HSVChannels.at(0);		//色相
Mat ValueChannel;

void openCapture() {
	
	printf("width=%.2f\n", Capture.get(CAP_PROP_FRAME_WIDTH)); //宽
	printf("heigth=%.2f\n", Capture.get(CAP_PROP_FRAME_HEIGHT));	//高
	printf("fbs=%.2f\n", Capture.get(CAP_PROP_FPS)); //FPS 帧数
	printf("brightness=%.2f\n", Capture.get(CAP_PROP_BRIGHTNESS));//brightness 亮度
	printf("contrast=%.2f\n", Capture.get(CAP_PROP_CONTRAST)); //contrast 对比度
	printf("saturation=%.2f\n", Capture.get(CAP_PROP_SATURATION)); //saturation 饱和度
	printf("hue=%.2f\n", Capture.get(CAP_PROP_HUE));	//色调
	printf("exposure=%.2f\n", Capture.get(CAP_PROP_EXPOSURE));	//曝光
	namedWindow("参数", 0);
	Capture.set(CAP_PROP_FRAME_WIDTH, 640);
	Capture.set(CAP_PROP_FRAME_HEIGHT, 480);
	Capture.set(CAP_PROP_FPS, 60);//帧数
	Capture.set(CAP_PROP_BRIGHTNESS, -131); //亮度
	Capture.set(CAP_PROP_CONTRAST, 26);//对比
	Capture.set(CAP_PROP_SATURATION, 93);//饱和
	Capture.set(CAP_PROP_HUE, 0);
	Capture.set(CAP_PROP_EXPOSURE, 0);
	createTrackbar("二值化阈值", "参数", &Index_Binary, BINARY_MAX, on_trackbar_binary);
	createTrackbar("曝光度", "参数", &Index_Exposure, EXPOSURE_MAX, on_trackbar_exposure);
	createTrackbar("亮度", "参数", &Index_Brightness, BRIGHTNESS_MAX, on_trackbar_brightness);
	createTrackbar("对比", "参数", &Index_Contrast, CONTRAST_MAX, on_trackbar_contrast);
	createTrackbar("饱和", "参数", &Index_Saturation, SATURATION_MAX, on_trackbar_saturation);
	createTrackbar("色相", "参数", &Index_Hue, HUE_MAX, on_trackbar_hue);
	while (1) {
		//printf("exposure=%.2f\n", Capture.get(CAP_PROP_EXPOSURE));	//曝光
		Mat frame;
		Capture >> frame;

		imshow("frame", frame); //显示原图
		Mat result0;
		/*binaryImg(frame, result0);
		imshow("result", result0);*/
		cvtColor(frame, Hsv_Frame, COLOR_BGR2HSV); //原图转为hsv
		vector<Mat> BGRChannels;
		vector<Mat> HSVChannels;
		vector<Mat> RVC;	//red +value 通道合并
		vector<Mat> BHC;	//bule +hue 通道合并
		split(frame, BGRChannels);	//BGR三通道分离
		split(frame, HSVChannels);	//HSV三通道分离

		RedChannel = BGRChannels.at(2);     //红色通道 
		BlueChannel = BGRChannels.at(0);	//蓝色通道
		HueChannel = HSVChannels.at(0);		//色相
		ValueChannel = HSVChannels.at(2);			//亮度

		threshold(ValueChannel, RED_result, Index_Binary, 255, THRESH_BINARY); //二值阈值化 红色
		threshold(BlueChannel, BLUE_result, Index_Binary, 255, THRESH_BINARY); //二值阈值化 蓝色
		
		
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
	threshold(RedChannel, RED_result, Index_Binary, 255, THRESH_BINARY_INV); //二值阈值化
	threshold(BlueChannel, BLUE_result, Index_Binary, 255, THRESH_BINARY_INV); //二值阈值化
	//threshold(HueChannel, RED_result, Index_Binary, 255, THRESH_BINARY); //二值阈值化 红色
	//threshold(ValueChannel, BLUE_result, Index_Binary, 255, THRESH_BINARY); //二值阈值化 蓝色
}
void on_trackbar_exposure(int, void*) {
	Capture.set(CAP_PROP_EXPOSURE, Index_Exposure - 11);
	
	printf("exposure=%.2f\n", Capture.get(CAP_PROP_EXPOSURE));	//曝光
}
void on_trackbar_brightness(int, void*) {
	Capture.set(CAP_PROP_BRIGHTNESS, Index_Brightness); //亮度

	printf("brightness=%.2f\n", Capture.get(CAP_PROP_BRIGHTNESS));//brightness 亮度

}
void on_trackbar_contrast(int, void*) {
	Capture.set(CAP_PROP_CONTRAST, Index_Contrast);//对比
	
	printf("contrast=%.2f\n", Capture.get(CAP_PROP_CONTRAST)); //contrast 对比度

}
void on_trackbar_saturation(int, void*) {
	Capture.set(CAP_PROP_SATURATION, Index_Saturation);//饱和
	
	printf("saturation=%.2f\n", Capture.get(CAP_PROP_SATURATION)); //saturation 饱和度

}
void on_trackbar_hue(int, void*) {
	Capture.set(CAP_PROP_HUE, Index_Hue);//饱和
	
	printf("hue=%.2f\n", Capture.get(CAP_PROP_HUE));	//色调
	
}
