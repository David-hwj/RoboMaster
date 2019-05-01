#include "REC_PART1.h"
#include "REC_PART2.h"
#include "COM_SEND.h"

int main()
{
	// 打开usb串口
	int fd = openusb();
	Armor_Detector Detector;
	Image_Threshold Thres;
	Mat frame;

	// 打开摄像头
	VideoCapture cap(0);
	
	bool stop = false;
	while (!stop)
	{
		// 对图像进行二值化
		Thres.hsv_rec(cap);
		// 对二值化的结果匹配装甲板
		Detector.init(Thres.src_img, Thres.target_img);
		
		// 发送坐标
		SendDate(-Detector.best_x_diff, -Detector.best_y_diff, fd);
		
		if (waitKey(30) >= 0)
			stop = true;
	}
	
	return 0;
}
