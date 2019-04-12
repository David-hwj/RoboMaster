#include "REC_PART2.h"

double get_point_angle(CvPoint pointO, CvPoint pointA)
{
	double angle = 0;
	CvPoint point;
	double temp;
	point = cvPoint((pointA.x - pointO.x), (pointA.y - pointO.y));
	if ((0 == point.x) && (0 == point.y))
	{
		return 0;
	}
	if (0 == point.x)
	{
		angle = 90;
		return angle;
	}
	if (0 == point.y)
	{
		angle = 0;
		return angle;
	}

	temp = fabsf(float(point.y) / float(point.x));
	temp = atan(temp);
	temp = temp * 180 / CV_PI;

	if ((0 < point.x) && (0 < point.y))
	{
		angle = 360 - temp;
		return angle;
	}
	if ((0 > point.x) && (0 < point.y))
	{
		angle = 360 - (180 - temp);
		return angle;
	}
	if ((0 < point.x) && (0 > point.y))
	{
		angle = temp;
		return angle;
	}
	if ((0 > point.x) && (0 > point.y))
	{
		angle = 180 - temp;
		return angle;
	}
	printf("sceneDrawing :: getAngle error!");
	return -1;
}

Armor_Detector::Armor_Detector(){}
// ��img��ȡ��������������ȡ������浽dst��

void Armor_Detector::init(Mat src_img, Mat thres_img)
{
	// ������ͼ�񱣴���src_img��
	this->src_img = src_img.clone();

	// ���contours_1 contours_2 white_hierarchy
	contours_1.clear();
	contours_2.clear();
	white_hierarchy.clear();

	// ���light_rec
	light_rect.clear();

	// ���armors armor_SECOND
	armors.clear();
	armor_SECOND.clear();
	
	cout << "Now:" << shoot_object.armorS.center << endl;
	// ����find_contours
	find_contours(thres_img);
}

void Armor_Detector::find_contours(Mat img)
{
	// ����img
	Mat src = img.clone();
	
	// ������ͼ
	Mat drawout = Mat::zeros(src.size(), CV_8UC3);

	// ��src�����������������contours_1��
	findContours(src, contours_1, white_hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	// ���ݴ�С���㼶�������������������contours_2��
	for (int i = 0; i < contours_1.size(); i++)
	{
		//cout << "������" << white_hierarchy[i][3] << " " << endl;
		if (contours_1[i].size() >= contour_min && contours_1[i].size() <= contour_max)
		{
			// û����Ƕ�������������������ڲ�����
			//if(white_hierarchy[i][3] != -1)
			contours_2.push_back(contours_1[i]);
		}
	}
	// �ڿ�ͼ�л������
	drawContours(drawout, contours_2, -1, Scalar(255, 255, 0));

	//�ѽ��ͼ���浽contours_result
	contours_result = drawout;

	imshow("�������˽��", drawout);
	//cout << "contours_2������ " << contours_2.size() << endl;

	// ��������ϣ�������һ������
	fitting_contours(img);
}

// ����������ϳ���ת���Ρ���ת��Բ������
void Armor_Detector::fitting_contours(Mat img)
{
	// ����������ת����
	RotatedRect s, s_fitEllipse, s_minAreaRect;
	// ������ͼ
	Mat drawout = Mat::zeros(img.size(), CV_8UC3);
	Mat drawout_2 = Mat::zeros(img.size(), CV_8UC3);

	//cout << "contours_2������ " << contours_2.size() << endl;
	// �����Բ
	for (int i = 0; i < contours_2.size(); i++)
	{
		s_fitEllipse = fitEllipse(contours_2[i]);  // ��ά�㼯����Բ���
		s_minAreaRect = minAreaRect(contours_2[i]);// ��ά�㼯����ת�������
		ellipse(drawout, s_fitEllipse, Scalar(0, 0, 255), 1, CV_AA);
	}
	imshow("�����Բ", drawout);

	// ����Բ�ؼ���С�ع���
	int index = 0;
	for (int i = 0; i < contours_2.size(); i++)
	{
		s_fitEllipse = fitEllipse(contours_2[i]);  // ��ά�㼯����Բ���
		s_minAreaRect = minAreaRect(contours_2[i]);// ��ά�㼯����ת�������
		s.angle = s_fitEllipse.angle;              // ��С��Բ�ĽǶȸ�����ת����
		s.center = s_fitEllipse.center;            // ��С��Բ�����ĵ㸳����ת����

		 //ͳһ��height ��width
		if (s_minAreaRect.size.width > s_minAreaRect.size.height)
		{
			s.size.height = s_minAreaRect.size.width;
			s.size.width = s_minAreaRect.size.height;
		}
		else
		{
			s.size.height = s_minAreaRect.size.height;
			s.size.width = s_minAreaRect.size.width;
		}

		// ���ݴ�С����
		if ((s.size.height <= light_min_length)
			&& (s.size.height >= light_max_length))
			continue;

		// ���ݳ���ȹ���
		if ((s.size.width / s.size.height) > light_wh_rate)
			continue;

		// ���ݽǶȹ��ˣ��Ƕ�Ϊ����y��ļн�
//		if (s.angle >= light_lean_1 && s.angle <= light_lean_2)
//			continue;

		// ���˽�����֮ѹ��light_rect
		light_rect.push_back(s);
		cout << s.angle << endl;

		// ������Բ
		Scalar color = Scalar(255, 0, 0);
		ellipse(drawout_2, s.center, s.size, s.angle, 0, 360, color, 1, CV_AA);//����Բ
		//string Numbers1 = to_string(index);
		//putText(drawout_2, Numbers1, s.center, FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255));
		index++;
	}
	// �������˽��
	imshow("��Բ���˽��", drawout_2);

	// ���˽����󣬽����˽������װ�װ����
	fitting_armor(img);

}
void Armor_Detector::fitting_armor(Mat img)
{
	// ������ͼ
	Mat drawout = Mat::zeros(img.size(), CV_8UC3);

	// ������Բ������ƥ���װ��
	Armor_builded armor_FIRST; // ����װ���������ת����
	int armor_length;          // װ�װ峤��
	int armor_width;           // װ�װ���

	// ����⵽����ת���θ��� < 2��ֱ�ӷ���
	if (light_rect.size() < 2) {}
	else
	{
		// ������������ת����
		for (int i = 0; i < light_rect.size(); i++)
		{
			for (int j = i + 1; j < light_rect.size(); j++)
			{
				double height_diff = abs(light_rect[i].size.height - light_rect[j].size.height);     // �߶Ȳ�
				double height_sum = light_rect[i].size.height + light_rect[j].size.height;           // �߶Ⱥ�
				double width_diff = abs(light_rect[i].size.width - light_rect[j].size.width);        // ��Ȳ�
				double width_sum = light_rect[i].size.width + light_rect[j].size.width;              // ��Ⱥ�
				double angle_middle = fabs(light_rect[i].angle + light_rect[j].angle) / 2;           // �ǶȾ�ֵ
				double angle_diff = fabs(light_rect[i].angle - light_rect[j].angle);                 // �ǶȲ�
				double center_angle = get_point_angle(light_rect[i].center, light_rect[j].center);   // ����ת�������Ĺ��ɵ��ߵĽǶ�
				if (center_angle > 90 && center_angle < 270) { center_angle -= 180; }
				if (center_angle >= 270) { center_angle -= 360; }
				double Y_diff = abs(light_rect[i].center.y - light_rect[j].center.y);                 // �������ֵ
				double MH_diff = (min(light_rect[i].size.height, light_rect[j].size.height));         // �߶Ȳ��޷�
				double height_max = (max(light_rect[i].size.height, light_rect[j].size.height));      // ���߶�
				double X_diff = abs(light_rect[i].center.x - light_rect[j].center.x);                 // �������ֵ

				//cout << "������Բ " << i << " " << j << endl;
				//cout << "�ж���ת���ε�������" << endl;
				//cout << "�������ֵ���㣺" << (Y_diff < MH_diff * 2) << " Y_diff: " << Y_diff << " MH_diff" << MH_diff << endl;
				//cout << "�������ֵ���㣺" << (X_diff < height_max * 7) << endl;
				//cout << "�ƴ��ĽǶ��ڹ涨��Χ�ڣ�" << (angle_diff < T_ANGLE_THRE || 180 - angle_diff < T_ANGLE_THRE180) << endl;//��Ϊ���ƴ��нǵĲ�ֵֵ������ֵ��Ϊʲô��Ҫһ����||���������ɡ������أ���Ϊ����һ��angle��45�ȣ�һ����225�ȣ���ôfabs(light_rect[i].angle - light_rect[j].angle)�Ľ������180�������ϡ�angle_diff < T_ANGLE_THRE������������������Ҫ�ټ�ȥ180
				//cout << "�߶Ȳ�С�ڸ߶Ⱥͣ�" << (height_diff / height_sum < T_HIGH_RAT) << endl;
				//cout << "��Ȳ�С�ڿ�Ⱥͣ�" << (width_diff / width_sum < T_WHIDTH_RAT) << endl;
				//cout << "���ĵ㹹�ɵ����������ֱ��" << ((angle_middle + center_angle > 165 && angle_middle + center_angle < 195) || (angle_middle + center_angle > -15 && angle_middle + center_angle < 15) || (angle_middle + center_angle > 80 && angle_middle < 100)) << endl;
				//cout << "����" << i << "�Ƕȣ�" << light_rect[i].angle << " " << "����" << j << "�Ƕȣ�" << light_rect[j].angle << " �ǶȾ�ֵ��" << angle_middle << " ���ĵ㹹�ɵ��ߵĽǶȣ�" << center_angle << endl;
				if (
					Y_diff < MH_diff * 2                                                 /*�������ֵС�ڸ߶Ȳ��޷�*/
					&& X_diff < height_max * 7                                           /*�������ֵС�����߶�*4*/
					&& (angle_diff < T_ANGLE_THRE || 180 - angle_diff < T_ANGLE_THRE180) /*�ƴ��ĽǶ��ڹ涨��Χ��*/
					&& height_diff / height_sum < T_HIGH_RAT                             /*�߶Ȳ�С��ĳһ�����ĸ߶Ⱥ�*/
					&& width_diff / width_sum < T_WHIDTH_RAT                             /*��Ȳ�С��ĳһ�����Ŀ�Ⱥ�*/
					//&& ((angle_middle + center_angle > 165 && angle_middle + center_angle < 195) || (angle_middle + center_angle > -15 && angle_middle + center_angle < 15) || (angle_middle + center_angle > 80 && angle_middle < 100))  /*���ĵ㹹�ɵ����������ֱ*/
					)
					// ƥ��ɹ�
				{
					//cout << "find_success i:" << i << " j: " << j << endl;
					//circle(drawout, light_rect[i].center, 1, Scalar(255,0, 100), 5);
					//circle(drawout, light_rect[j].center, 1, Scalar(255, 0, 100), 5);
					//// ����װ�װ�
					//rectangle(drawout,
					//	Point(light_rect[i].center.x, light_rect[i].center.y + light_rect[i].size.height * 0.5),
					//	Point(light_rect[j].center.x, light_rect[j].center.y - light_rect[j].size.height * 0.5),
					//	Scalar(0, 255, 255)
					//);
					//imshow("drawing", drawout_1);
					armor_FIRST.armorS.center.x = ((light_rect[i].center.x + light_rect[j].center.x) / 2); // װ�����ĵ�x���� 
					armor_FIRST.armorS.center.y = ((light_rect[i].center.y + light_rect[j].center.y) / 2); // װ�����ĵ�y����
					armor_FIRST.armorS.angle = (light_rect[i].angle + light_rect[j].angle) / 2;            // װ��������ת���ε���ת�Ƕ�
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					armor_length = (light_rect[i].size.height + light_rect[j].size.height) / 2;            // װ�׵ĸ߶�
					// װ�׵Ŀ�ȵ�������LED������ת������������ľ���
					armor_width = sqrt((light_rect[i].center.x - light_rect[j].center.x) * (light_rect[i].center.x - light_rect[j].center.x)
						+ (light_rect[i].center.y - light_rect[j].center.y) * (light_rect[i].center.y - light_rect[j].center.y));
					// ͳһ����Ϊlength�����Ϊwidth
					if (armor_length < armor_width)
					{
						armor_FIRST.armorS.size.height = armor_length;
						armor_FIRST.armorS.size.width = armor_width;
					}
					else
					{
						armor_FIRST.armorS.size.height = armor_width;
						armor_FIRST.armorS.size.width = armor_length;
					}
					//cout << "�������ֵ<һ������װ�װ�Ŀ�ȣ�" << (Y_diff < armor_width / 3) << endl;
					//// �������ֵ < ����֮һװ�װ�Ŀ��
					//if (Y_diff < armor_width / 3)
					{
						cout << "ʶ��ɹ�" << endl << endl;
						armor_FIRST.build1_No = i; // ��װ�װ��ɵ�i���j���������
						armor_FIRST.build2_No = j;
						armor_FIRST.build_features[0] = angle_diff; // ���ɸ�װ�װ���������ĽǶȲ�
						armor_FIRST.build_features[1] = Y_diff;     // ���ɸ�װ�װ����������Y��
						armor_FIRST.build_features[2] = height_diff;// ���ɸ�װ�װ���������ĸ߶Ȳ�
						armor_FIRST.build_features[3] = width_diff; // ���ɸ�װ�װ���������Ŀ�Ȳ�
						armor_SECOND.push_back(armor_FIRST); //���ҳ���װ�׵���ת���α��浽vector
						//cout << "����������������ĽǶȲ��:" << fabs(center_angle - angle_middle) << " �������ǶȾ�ֵ:" << angle_middle << " �������ǶȲ" << angle_diff << " ����i�ĽǶȣ�" << light_rect[i].angle << " ����j�ĽǶȣ�" << light_rect[j].angle << " ���������ߵĽǶȣ�" << center_angle << endl;
						//cout << "i_center<x:" << light_rect[i].center.x << ", y:" << light_rect[i].center.y << "> j_center<x:" << light_rect[j].center.x << ", y:" << light_rect[j].center.y << ">" << endl;
					}
				}
			}
		}
	}
	Mat tmp = src_img.clone();
	// ����װ�װ�
	//cout << "size" << armor_SECOND.size() << endl;
	for (int i = 0; i < armor_SECOND.size(); i++)
	{
		ellipse(tmp, Point(armor_SECOND[i].armorS.center.x, armor_SECOND[i].armorS.center.y), Size(armor_SECOND[i].armorS.size.height * 0.5, armor_SECOND[i].armorS.size.width * 0.5), armor_SECOND[i].armorS.angle + 90, 0, 360, Scalar(0, 255, 255));
	}
	imshow("װ�װ�ʶ����", tmp);
	armor_detect_result = tmp;

	Mat ttmp = tmp.clone();
	// ��ƥ��ɹ���װ�װ���Ѱ��������Ŀ��
	find_shoot_object(ttmp);
}

// ��armor_SECOND��Ѱ��������Ŀ��
// ��������һ�����Ը����㷨
void Armor_Detector::find_shoot_object(Mat img)
{
	int img_x = src_img.size().width;
	int img_y = src_img.size().height;
	int min_x_diff = 0, min_y_diff = 0;
	long min_pow = 999999;
	int final_num = 0;
	
	// ���ʶ�𵽵�armor��Ϊ0
	if (armor_SECOND.size() == 0)
		count_no_object++;
	else
	{
		// ��ո�ֵ
		count_no_object = 0;
		// ����ÿһ��װ�װ�
		for (int i = 0; i < armor_SECOND.size(); i++)
		{
			// ������װ�װ�����������ĵ������ֵ
			cout << "armor_x: " << armor_SECOND[i].armorS.center.x << "  " << " armor_y: " << armor_SECOND[i].armorS.center.y << endl;
			cout << "img_X: " << img_x << " " << " img_Y: " << img_y << endl;
			armor_SECOND[i].x_diff = armor_SECOND[i].armorS.center.x - 0.5 * img_x;
			armor_SECOND[i].y_diff = armor_SECOND[i].armorS.center.y - 0.5 * img_y;
			cout << "diff: " << armor_SECOND[i].x_diff << " " << armor_SECOND[i].y_diff << endl;

			//����x_diff��y_diff��ƽ����
			int count = pow(armor_SECOND[i].x_diff, 2) + pow(armor_SECOND[i].y_diff, 2);
			if (count < min_pow)
			{
				min_pow = count;
				min_x_diff = armor_SECOND[i].x_diff;
				min_y_diff = armor_SECOND[i].y_diff;
				final_num = i;
			}
		}
		/*best_x_diff = min_x_diff;
		best_y_diff = min_y_diff;*/
		shoot_object = armor_SECOND[final_num];

		//cout << "current_best is: " << final_num << " " << best_x_diff << " " << best_y_diff << endl;
	}
	// �������װ�װ�
	Mat tmp = src_img.clone();
	ellipse(tmp, Point(shoot_object.armorS.center.x, shoot_object.armorS.center.y), Size(shoot_object.armorS.size.height * 0.5, shoot_object.armorS.size.width * 0.5), shoot_object.armorS.angle + 90, 0, 360, Scalar(255, 0, 255));
	imshow("���װ�װ�", tmp);

	// ��������浽best_armor_detect��
	best_armor_detect = tmp;
}

