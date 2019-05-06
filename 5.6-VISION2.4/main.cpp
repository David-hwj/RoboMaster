#include "REC_PART1.h"
#include "REC_PART2.h"
#include "COM_SEND.h"


int main()
{
	// 打开usb串口
	int fd = openusb();
	Mat frame;

	// 打开摄像头
	VideoCapture cap(0);
	int frame_width = 640;
	int frame_height = 320;
	// 设置cap
	cap.set(CAP_PROP_FRAME_WIDTH, frame_width);
	cap.set(CAP_PROP_FRAME_HEIGHT, frame_height);
	
	// 选择颜色，red or blue
	string color = "blue";
	// 选择是否需要显示图像，0不显示，1显示少部分重要图像，2显示大部分重要图像，3显示全部图像
	int rather_show_img = 1;

	// 选择是否保存动画
	Size size = Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	
	// 初始化PART_1,PART_2
	Armor_Detector Detector(rather_show_img);
	Image_Threshold Thres(cap, color, rather_show_img, frame_width, frame_height);

	// Size size = Size(frame_width, frame_height);
	VideoWriter writer;
	writer.open("test4.avi", CV_FOURCC('M', 'J', 'P', 'G'), 60, size, true);
	// 是否需要测试原始帧率
	int this_fps=0;
	stringstream this_fpsString;
	double thist;
	bool test_fps = 0;
	while (1)
	{
		// 如果需要检测帧率
		if (test_fps)
		// calculate fps
		{
			this_fpsString.str("0");
			cap >> frame;
			// calculate fps
			this_fps = 1 / (((double)getTickCount() / getTickFrequency()) - thist);
			// cout << fps << endl;
			this_fpsString << this_fps;
			// cout << "fpsString " << fpsString.str() << endl;
			// draw fps
			putText(frame, this_fpsString.str(), Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255));

			// update t
			thist = getTickCount()/getTickFrequency();
			
			imshow("frame", frame);
		}
		// 正常运行程序
		else
		{
			// 对图像进行二值化
			Thres.init(cap);
			// 写入视频
			writer.write(Thres.src_img);
			// 使用二值化的结果匹配装甲板
			Detector.init(Thres.src_img, Thres.target_img);
			
			// 如果没有错误，发送正确坐标
			if(!Detector.e_error)
				SendDate(-Detector.best_x_diff-20, -Detector.best_y_diff+60 , fd);
			// 如果出现错误，发送错误信号
			else
			{
				cout<<"no target"<<endl;
				SendDate_error(fd);
			}
		}
		
		if (waitKey(1) == 27)
			break;
	}
	cap.release();
	return 0;
}
