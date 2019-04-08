#include "REC_PART1.h"
#include "REC_PART2.h"

void video_show()
{
	Mat frame;
	VideoCapture cap;
	Image_Threshold Thres;
	Armor_Detector Detector;
	cap.open("C:\\Users\\Paul\\Desktop\\RM_VISION\\����ͼƬ\\VID_20190331_213129.mp4");

	Size size = Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	VideoWriter writer;
	writer.open("C:\\Users\\Paul\\Desktop\\RM_VISION\\����ͼƬ\\test2.avi", CV_FOURCC('M', 'J', 'P', 'G'), 60, size, true);

	while (1)
	{
		cap >> frame;
		if (frame.empty())//���ĳ֡Ϊ�����˳�ѭ��
			break;
		Thres.hsv_rec(frame);
		imshow("target", Thres.target_img);
		Detector.init(frame, Thres.target_img);
		writer.write(Detector.best_armor_detect);
		waitKey(10);//ÿ֡��ʱ10����
	}
	//waitKey(0);
	cap.release();
}

int main()
{
	//��ȡ���ص�һ��ͼƬ��ʾ����
	//��ȡͼƬ�ķ�ʽΪBGR
	Mat img = imread("C:\\Users\\15437\\Desktop\\RM_VISION\\����ͼƬ\\RM-PIC\\timg5.jpg");
	//imshow("ԭͼ", img);
	
	Armor_Detector Detector;
	Mat frame;

	//// //������ͷ
	//VideoCapture cap(0);
	//bool stop = false;
	//while (!stop)
	//{
	//	cap >> frame;
	//	Detector.hsv_rec(frame);
	//	//imshow("result", )
	//	//imshow("frame", frame);
	//	if (waitKey(30) >= 0)
	//		stop = true;
	//}
	//Detector.hsv_rec(img);
	//waitKey(0);
	video_show();
	//RGB_REC(img);
	//waitKey(0);
	return 0;
}