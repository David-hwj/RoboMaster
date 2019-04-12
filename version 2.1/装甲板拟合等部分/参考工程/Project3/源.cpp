#include "REC_PART1.h"
#include "REC_PART2.h"
#include "GetBinary.h"

void video_show()
{
	Mat frame;
	VideoCapture cap;
	Image_Threshold Thres;
	Armor_Detector Detector;
	cap.open("C:\\Users\\Paul\\Desktop\\RM_VISION\\测试图片\\VID_20190331_213129.mp4");

	Size size = Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	VideoWriter writer;
	writer.open("C:\\Users\\Paul\\Desktop\\RM_VISION\\测试图片\\test2.avi", CV_FOURCC('M', 'J', 'P', 'G'), 60, size, true);

	while (1)
	{
		// 若某一帧没图像，则返回
		if (Thres.hsv_rec(cap) == 0)
			break;

		imshow("target", Thres.target_img);
		Detector.init(frame, Thres.target_img);
		writer.write(Detector.best_armor_detect);
		waitKey(10);//每帧延时10毫秒
	}
	//waitKey(0);
	cap.release();
}

int main()
{
	//读取本地的一张图片显示出来
	//读取图片的方式为BGR
	Mat img = imread("C:\\Users\\15437\\Desktop\\RM_VISION\\测试图片\\RM-PIC\\timg5.jpg");
	//imshow("原图", img);
	
	Armor_Detector Detector;
	Image_Threshold Thres;
	Mat frame;

	//openCapture();
	
	// 打开摄像头
	VideoCapture cap(0);

	bool stop = false;
	while (!stop)
	{
		Thres.hsv_rec(cap);
		
		if (waitKey(30) >= 0)
			stop = true;
	}
	
	//Detector.hsv_rec(img);
	//waitKey(0);
	//video_show();
	//RGB_REC(img);
	//waitKey(0);
	return 0;
}