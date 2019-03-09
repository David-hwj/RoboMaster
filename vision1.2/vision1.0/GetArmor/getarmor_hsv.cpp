#include "getarmor_hsv.h"

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
    if ((0>point.x) && (0<point.y))
    {
        angle = 360 - (180 - temp);
        return angle;
    }
    if ((0<point.x) && (0>point.y))
    {
        angle = temp;
        return angle;
    }
    if ((0>point.x) && (0>point.y))
    {
        angle = 180 - temp;
        return angle;
    }
    printf("sceneDrawing :: getAngle error!");
    return -1;
}

// 构造函数
Armor_Detector::Armor_Detector(){}
// 初始化函数
void Armor_Detector::init()
{
    contours_1.clear();
    contours_2.clear();
    light_rect.clear();
    armor_SECOND.clear();
}
//预处理蓝色
void Armor_Detector::filter_hsv_blue_preproc(Mat img, Mat& dst)
{
    Mat imgHSV = img.clone();
    cvtColor(imgHSV, imgHSV, CV_BGR2HSV);

    Mat HSV[3];
    split(imgHSV, HSV);
    /*
    opencv 的H范围是0~180，红色的H范围大概是(0~10)∪(156,180)
    S是饱和度，一般是大于一个值,S过低就是灰色（参考值S>80)，
    V是亮度，过低就是黑色，过高就是白色(参考值220>V>50)。
    */
    //imshow("vraw", HSV[2]);

    // 使用V通道二值化，阈值为200
    threshold(HSV[2], HSV[2], 200, 255, THRESH_BINARY);//最好设为动态阈值
    //imshow("vthresh", HSV[2]);
    morphologyEx(HSV[2], HSV[2], MORPH_DILATE, element_5);// 形态学变换函数，膨胀
    //imshow("vdilate", HSV[2]);

    // 将HSV图像透过一张蒙版，拷贝到b_filter_tmp1中
    Mat ttmp;
    //imshow("imghsv", imgHSV);
    imgHSV.copyTo(ttmp, HSV[2]);
    b_filter_tmp1 = ttmp;
   // imshow("B_filter_tmp1", b_filter_tmp1);

    namedWindow("b_filter_tmp2", 2);
    createTrackbar("BLOWH", "b_filter_tmp2", &BLowH, 255, bshow_hsv, this);
    createTrackbar("BHIGHH", "b_filter_tmp2", &BHighH, 255, bshow_hsv, this);
    createTrackbar("BLOWS", "b_filter_tmp2", &BLowS, 255, bshow_hsv, this);
    createTrackbar("BHIGHS", "b_filter_tmp2", &BHighS, 255, bshow_hsv, this);
    createTrackbar("BLOWV", "b_filter_tmp2", &BLowV, 255, bshow_hsv, this);
    createTrackbar("BHIGHV", "b_filter_tmp2", &BHighV, 255, bshow_hsv, this);

    //将b_filter_tmp1经过HSV过滤后保存在b_filter_tmp2中
    inRange(b_filter_tmp1, Scalar(BLowH, BLowS, BLowV), Scalar(BHighH, BHighS, BHighV), b_filter_tmp2);
   // imshow("B_filter_tmp2", b_filter_tmp2);
    // 对图像找轮廓
    find_contours(b_filter_tmp2, contours_result);
}


//预处理红色
void Armor_Detector::filter_hsv_red_preproc(Mat img, Mat& dst)
{
    Mat imgHSV = img.clone();
    cvtColor(imgHSV, imgHSV, CV_BGR2HSV);

    Mat HSV[3];
    split(imgHSV, HSV);
    /*
    opencv 的H范围是0~180，红色的H范围大概是(0~10)∪(156,180)
    S是饱和度，一般是大于一个值,S过低就是灰色（参考值S>80)，
    V是亮度，过低就是黑色，过高就是白色(参考值220>V>50)。
    */
    // 获得一个形状
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
   // imshow("vraw", HSV[2]);

    // 使用V通道二值化，阈值为200
    threshold(HSV[2], HSV[2], 200, 255, THRESH_BINARY);//最好设为动态阈值
    //imshow("vthresh", HSV[2]);
    morphologyEx(HSV[2], HSV[2], MORPH_DILATE, element);// 形态学变换函数，膨胀
    //imshow("vdilate", HSV[2]);

    // 将HSV图像透过一张蒙版，拷贝到r_filter_tmp1中
    Mat ttmp;
    //imshow("imghsv", imgHSV);
    imgHSV.copyTo(ttmp, HSV[2]);
    r_filter_tmp1 = ttmp;
   // imshow("R_filter_tmp1", r_filter_tmp1);

    // 创建窗口
    namedWindow("r_filter_tmp2", CV_WINDOW_NORMAL);
    // 创建调整条
    createTrackbar("RLOWH", "r_filter_tmp2", &RLowH, 255, rshow_hsv, this);
    createTrackbar("RHIGHH", "r_filter_tmp2", &RHighH, 255, rshow_hsv, this);
    createTrackbar("RLOWH_2", "r_filter_tmp2", &RLowH_2, 255, rshow_hsv, this);
    createTrackbar("RHIGHH_2", "r_filter_tmp2", &RHighH_2, 255, rshow_hsv, this);
    createTrackbar("RLOWS", "r_filter_tmp2", &RLowS, 255, rshow_hsv, this);
    createTrackbar("RHIGHS", "r_filter_tmp2", &RHighS, 255, rshow_hsv, this);
    createTrackbar("RLOWV", "r_filter_tmp2", &RLowV, 255, rshow_hsv, this);
    createTrackbar("RHIGHV", "r_filter_tmp2", &RHighV, 255, rshow_hsv, this);
    // 过滤
    inRange(r_filter_tmp1, Scalar(RLowH, RLowS, RLowV), Scalar(RHighH, RHighS, RHighV), r_filter_tmp2_0);
    inRange(r_filter_tmp1, Scalar(RLowH_2, RLowS, RLowV), Scalar(RHighH_2, RHighS, RHighV), r_filter_tmp2_1);
    // 合成
    r_filter_tmp2_2 = r_filter_tmp2_0 | r_filter_tmp2_1;
    // 显示
   // imshow("R_filter_tmp2", r_filter_tmp2_2);
    // 对图像找轮廓
    find_contours(r_filter_tmp2_2, contours_result);
}

// 显示蓝色
void bshow_hsv(int value, void* detector)
{
    Armor_Detector detect = *(Armor_Detector*)detector;
    inRange(detect.b_filter_tmp1, Scalar(detect.BLowH, detect.BLowS, detect.BLowV), Scalar(detect.BHighH, detect.BHighS, detect.BHighV), detect.b_filter_tmp2);
    // 膨胀
    //morphologyEx(detect.b_filter_tmp2, detect.b_filter_tmp2, MORPH_DILATE, detect.element);// 形态学变换函数，膨胀
  //  imshow("B_filter_tmp2", detect.b_filter_tmp2);
    // 对图像找轮廓
    detect.find_contours(detect.b_filter_tmp2, detect.contours_result);
}
// 显示红色
void rshow_hsv(int value, void* detector)
{
    Armor_Detector detect = *(Armor_Detector*)detector;
    // 过滤
    inRange(detect.r_filter_tmp1, Scalar(detect.RLowH, detect.RLowS, detect.RLowV), Scalar(detect.RHighH, detect.RHighS, detect.RHighV), detect.r_filter_tmp2_0);
    inRange(detect.r_filter_tmp1, Scalar(detect.RLowH_2, detect.RLowS, detect.RLowV), Scalar(detect.RHighH_2, detect.RHighS, detect.RHighV), detect.r_filter_tmp2_1);
    detect.r_filter_tmp2_2 = detect.r_filter_tmp2_0 | detect.r_filter_tmp2_1;
    // 膨胀
    //morphologyEx(detect.r_filter_tmp2_2, detect.r_filter_tmp2_2, MORPH_DILATE, detect.element);// 形态学变换函数，膨胀
   // imshow("R_filter_tmp2", detect.r_filter_tmp2_2);
    // 对图像找轮廓
    detect.find_contours(detect.r_filter_tmp2_2, detect.contours_result);
}

// 对img提取轮廓，将轮廓提取结果保存到dst中
void Armor_Detector::find_contours(Mat img, Mat& dst)
{
    // 拷贝img
    Mat src = img.clone();
    // 建立空图
    Mat drawout = Mat::zeros(src.size(), CV_8UC3);
    //// 膨胀
    //morphologyEx(src, src, MORPH_DILATE, element_6);// 形态学变换函数，腐蚀
    //imshow("src", src);

    // 对src找轮廓，结果保存在contours_1中
    findContours(src, contours_1, white_hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // 根据大小过滤轮廓，结果保存在contour_2中
    for (int i = 0; i < contours_1.size(); i++)
    {
        if (contours_1[i].size() >= contour_min && contours_1[i].size() <= contour_max)
        {
            contours_2.push_back(contours_1[i]);
        }
    }
    // 在空图中画出结果
    drawContours(drawout, contours_2, -1, Scalar(255, 255, 0));

    //把结果图保存到contours_result
    contours_result = drawout;

   // imshow("轮廓过滤结果", drawout);
  //  cout << "contours_2的数量 " << contours_2.size() << endl;

    // 将轮廓拟合，进行下一步操作
    fitting_contours(img, dst);
}

// 根据轮廓拟合成旋转矩形、旋转椭圆并过滤
void Armor_Detector::fitting_contours(Mat img, Mat& dst)
{
    // 定义三个旋转矩形
    RotatedRect s, s_fitEllipse, s_minAreaRect;
    // 建立空图
    Mat drawout = Mat::zeros(img.size(), CV_8UC3);
    Mat drawout_2 = Mat::zeros(img.size(), CV_8UC3);

   // cout << "contours_2的数量2 " << contours_2.size() << endl;
    // 拟合椭圆
    for (int i = 0; i < contours_2.size(); i++)
    {
        s_fitEllipse = fitEllipse(contours_2[i]);// 二维点集的椭圆拟合
        s_minAreaRect = minAreaRect(contours_2[i]);// 二维点集的旋转矩形拟合
        ellipse(drawout, s_fitEllipse, Scalar(0, 0, 255), 1, CV_AA);

    }
   // imshow("拟合椭圆", drawout);
    // 由椭圆矩及最小矩过滤
    int index = 0;
    for (int i = 0; i < contours_2.size(); i++)
    {
        s_fitEllipse = fitEllipse(contours_2[i]);// 二维点集的椭圆拟合
        s_minAreaRect = minAreaRect(contours_2[i]);// 二维点集的旋转矩形拟合
        s.angle = s_fitEllipse.angle; // 最小椭圆的角度赋给旋转矩阵
        s.center = s_fitEllipse.center; // 最小椭圆的中心点赋给旋转矩阵

         //统一长height 宽width
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

        // 根据大小过滤
        if ((s.size.height <= light_min_length)
            && (s.size.height >= light_max_length))
            continue;

        // 根据长宽比过滤
        if ((s.size.width / s.size.height) > light_wh_rate)
            continue;

        // 不知道根据什么过滤 ^_^
        int x = s.center.x - s.size.width;
        if (x < 0)
            continue;
        int y = s.center.y - s.size.height;
        if (y < 0)
            continue;

        // 根据角度过滤，角度为关于y轴的夹角
        if (s.angle >= light_lean_1 && s.angle <= light_lean_2)
            continue;

        // 过滤结束后将之压入s
        light_rect.push_back(s);
      //  cout << s.angle << endl;

        // 画出椭圆
        Scalar color = Scalar(255, 0, 0);
        ellipse(drawout_2, s.center, s.size, s.angle, 0, 360, color, 1, CV_AA);//画椭圆
        string Numbers1 = to_string(index);
        putText(drawout_2, Numbers1, s.center, FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255));
        index++;
    }
    // 画出过滤结果
  //  imshow("椭圆过滤结果", drawout_2);

    // 过滤结束后，将过滤结果进行装甲板拟合
    fitting_armor(img, dst);

}
void Armor_Detector::fitting_armor(Mat img, Mat& dst)
{
    // 建立空图
    Mat drawout = Mat::zeros(img.size(), CV_8UC3);

    // 根据椭圆，两两匹配成装甲
    Armor_builded armor_FIRST; //定义装甲区域的旋转矩形
    int armor_length; // 装甲板长度
    int armor_width; // 装甲板宽度

    // 若检测到的旋转矩形个数 < 2，直接返回
    if (light_rect.size() < 2) {}
    else
    {
        // 对任意两个旋转矩形
        for (int i = 0; i < light_rect.size(); i++)
        {
            for (int j = i + 1; j < light_rect.size(); j++)
            {
                double height_diff = abs(light_rect[i].size.height - light_rect[j].size.height);//高度差
                double height_sum = light_rect[i].size.height + light_rect[j].size.height;//高度和
                double width_diff = abs(light_rect[i].size.width - light_rect[j].size.width);//宽度差
                double width_sum = light_rect[i].size.width + light_rect[j].size.width;//宽度和
                double angle_middle = fabs(light_rect[i].angle + light_rect[j].angle) / 2; //角度均值
                double angle_diff = fabs(light_rect[i].angle - light_rect[j].angle);//角度差
                double center_angle = get_point_angle(light_rect[i].center, light_rect[j].center); // 两旋转矩形中心构成的线的角度
                if (center_angle > 90 && center_angle < 270) { center_angle -= 180; }
                if (center_angle >= 270) { center_angle -= 360; }
                double Y_diff = abs(light_rect[i].center.y - light_rect[j].center.y);//纵坐标差值
                double MH_diff = (min(light_rect[i].size.height, light_rect[j].size.height));//高度差限幅
                double height_max = (max(light_rect[i].size.height, light_rect[j].size.height));//最大高度
                double X_diff = abs(light_rect[i].center.x - light_rect[j].center.x);//横坐标差值

                //cout << "对于椭圆 " << i << " " << j << endl;
                //cout << "判断旋转矩形的条件中" << endl;
                //cout << "纵坐标插值满足：" << (Y_diff < MH_diff * 2) << endl;
                //cout << "横坐标差值满足：" << (X_diff < height_max * 7) << endl;
                // << "灯带的角度在规定范围内：" << (angle_diff < T_ANGLE_THRE || 180 - angle_diff < T_ANGLE_THRE180) << endl;
                //cout << "高度差小于高度和：" << (height_diff / height_sum < T_HIGH_RAT) << endl;
                //cout << "宽度差小于宽度和：" << (width_diff / width_sum < T_WHIDTH_RAT) << endl;
                //cout << "中心点构成的线与灯柱垂直：" << (angle_middle + center_angle < 195 && angle_middle + center_angle > 165) << endl;
                if (
                    Y_diff < MH_diff * 2 /*纵坐标差值小于高度差限幅*/
                    && X_diff < height_max * 7 /*横坐标差值小于最大高度*4*/
                    && (angle_diff < T_ANGLE_THRE || 180 - angle_diff < T_ANGLE_THRE180) /*灯带的角度在规定范围内*/
                    && height_diff / height_sum < T_HIGH_RAT /*高度差小于某一比例的高度和*/
                    && width_diff / width_sum < T_WHIDTH_RAT /*宽度差小于某一比例的宽度和*/
                    && (angle_middle + center_angle < 195 && angle_middle + center_angle > 165) /*中心点构成的线与灯柱垂直*/
                    )
                // 匹配成功
                {
                    //cout << "find_success i:" << i << " j: " << j << endl;
                    //circle(drawout, light_rect[i].center, 1, Scalar(255,0, 100), 5);
                    //circle(drawout, light_rect[j].center, 1, Scalar(255, 0, 100), 5);
                    //// 画出装甲板
                    //rectangle(drawout,
                    //	Point(light_rect[i].center.x, light_rect[i].center.y + light_rect[i].size.height * 0.5),
                    //	Point(light_rect[j].center.x, light_rect[j].center.y - light_rect[j].size.height * 0.5),
                    //	Scalar(0, 255, 255)
                    //);
                    //imshow("drawing", drawout_1);
                    armor_FIRST.armorS.center.x = ((light_rect[i].center.x + light_rect[j].center.x) / 2); //装甲中心的x坐标
                    armor_FIRST.armorS.center.y = ((light_rect[i].center.y + light_rect[j].center.y) / 2); //装甲中心的y坐标
                    armor_FIRST.armorS.angle = (light_rect[i].angle + light_rect[j].angle) / 2;   //装甲所在旋转矩形的旋转角度
                    if (180 - angle_diff < T_ANGLE_THRE180)
                        armor_FIRST.armorS.angle += 90;
                    armor_length = (light_rect[i].size.height + light_rect[j].size.height) / 2; //装甲的高度
                    //装甲的宽度等于两侧LED所在旋转矩形中心坐标的距离
                    armor_width = sqrt((light_rect[i].center.x - light_rect[j].center.x) * (light_rect[i].center.x - light_rect[j].center.x)
                                + (light_rect[i].center.y - light_rect[j].center.y) * (light_rect[i].center.y - light_rect[j].center.y));
                    // 统一长度为length，宽度为width
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
              //      cout << "纵坐标插值<一定比例装甲板的宽度：" << (Y_diff < armor_width / 3) << endl;
                    //// 纵坐标差值 < 三分之一装甲板的宽度
                    //if (Y_diff < armor_width / 3)
                    {
                        cout << "识别成功" << endl << endl;
                        armor_FIRST.build1_No = i; // 该装甲板由第i与第j根灯柱组成
                        armor_FIRST.build2_No = j;
                        armor_FIRST.build_features[0] = angle_diff; // 构成该装甲板的两灯柱的角度差
                        armor_FIRST.build_features[1] = Y_diff;     // 构成该装甲板的两灯柱的Y差
                        armor_FIRST.build_features[2] = height_diff;// 构成该装甲板的两灯柱的高度差
                        armor_FIRST.build_features[3] = width_diff; // 构成该装甲板的两灯柱的宽度差
                        armor_SECOND.push_back(armor_FIRST); //将找出的装甲的旋转矩形保存到vector
                        //cout << "两灯柱连线与灯柱的角度差差:" << fabs(center_angle - angle_middle) << " 两灯柱角度均值:" << angle_middle << " 两灯柱角度差：" << angle_diff << " 灯柱i的角度：" << light_rect[i].angle << " 灯柱j的角度：" << light_rect[j].angle << " 两灯柱连线的角度：" << center_angle << endl;
                        //cout << "i_center<x:" << light_rect[i].center.x << ", y:" << light_rect[i].center.y << "> j_center<x:" << light_rect[j].center.x << ", y:" << light_rect[j].center.y << ">" << endl;
                    }
                }
            }
        }
    }
    Mat tmp = src_img.clone();
    // 画出装甲板
    for (int i = 0; i < armor_SECOND.size(); i++)
    {
        ellipse(tmp, Point(armor_SECOND[i].armorS.center.x, armor_SECOND[i].armorS.center.y), Size(armor_SECOND[i].armorS.size.height * 0.5, armor_SECOND[i].armorS.size.width * 0.5), armor_SECOND[i].armorS.angle + 90, 0, 360, Scalar(0, 255, 255));
    }
   // imshow("装甲板识别结果", tmp);
    armor_detect_result = tmp;

    // 从匹配成功的装甲板你中寻找最佳射击目标
    find_shoot_object();
}

// 在armor_SECOND中寻找最佳射击目标
void Armor_Detector::find_shoot_object()
{
    int img_x = src_img.size().width;
    int img_y = src_img.size().height;
 //   cout << img_x << " " << img_y << endl;
    // 对于每一个装甲板
    for (int i = 0; i < armor_SECOND.size(); i++)
    {
        // 给出该装甲板与相机正中心的坐标差值
        armor_SECOND[i].x_diff = armor_SECOND[i].armorS.center.x - img_x;
        armor_SECOND[i].y_diff = armor_SECOND[i].armorS.center.y - img_y;
      //  cout << "diff: " << armor_SECOND[i].x_diff << " " << armor_SECOND[i].y_diff << endl;
    }
}

void Armor_Detector::hsv_rec(Mat img)
{
    Mat IMG = img.clone();
    src_img = img.clone();
    //counting += 1;
    //if (counting == 2)
    //{
    //	counting = 1;
    //	init();
    //}
    init();
    filter_hsv_blue_preproc(IMG, r_filter_hsv);
}
