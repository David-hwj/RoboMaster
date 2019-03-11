#ifndef GETARMOR_HSV_H
#define GETARMOR_HSV_H
#include <opencv2/opencv.hpp>
#include <iostream>


using namespace std;
using namespace cv;

void bshow_hsv(int value, void* detector); // 展示过滤后的图像

void rshow_hsv(int value, void* detector); // 展示过滤后的图像

double get_point_angle(CvPoint pointO, CvPoint pointA); // 获取角度

class Armor_builded //装甲构成
{
public:
    RotatedRect armorS;
    int build1_No = 0; // 构成该装甲板的两灯柱的编号
    int build2_No = 0;
    int build_features[4]; // 两灯柱的角度差、高度坐标差、高度差、宽度差
    int distance; // 与装甲板的距离
    int x_diff; // 该装甲板x坐标与相机中心点作差
    int y_diff; // 该装甲板y坐标与相机中心点作差
};

class Armor_Detector
{
public:
    //int BLowH = 80;
    int BLowH = 100;
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

    // 定义elemengt的尺寸
    int element_length = 1;
    int element_width = 1;

    // 用于腐蚀膨胀的形状
    Mat element_1 = getStructuringElement(MORPH_RECT, Size(1, 1));
    Mat element_2 = getStructuringElement(MORPH_RECT, Size(2, 2));
    Mat element_3 = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat element_4 = getStructuringElement(MORPH_RECT, Size(4, 4));
    Mat element_5 = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat element_6 = getStructuringElement(MORPH_RECT, Size(6, 6));

    Mat src_img; // 原始图像

    Mat b_filter_hsv; // hsv图像经过对hsv三个通道的过滤得到的蓝色图像
    Mat r_filter_hsv; // hsv图像经过对hsv三个通道的过滤得到的红色图像

    // 中间图像
    Mat b_filter_tmp1;
    Mat b_filter_tmp2;
    Mat r_filter_tmp1;
    Mat r_filter_tmp2_0;
    Mat r_filter_tmp2_1;
    Mat r_filter_tmp2_2;

    vector< vector<Point> > contours_1; // 对灯柱提取轮廓
    vector< vector<Point> > contours_2; // 对轮廓轮廓按大小过滤后剩余的轮廓
    vector<Vec4i> white_hierarchy;

    Mat contours_result; // 提取轮廓后的图像
    Mat armor_detect_result; // 装甲板识别结果
    int contour_min = 40;
    int contour_max = 900;
    double light_wh_rate = 0.8; // 灯条宽高比
    int light_lean_1 = 45; // 灯条倾斜角度
    int light_lean_2 = 135;
    int light_max_length = 100; // 灯条最大长度
    int light_min_length = 5; // 灯条最小长度

    int T_ANGLE_THRE = 10;   //灯条正向最大角度差
    int T_ANGLE_THRE180 = 10;   //灯条反向最大角度差
    int T_ANGLE_THREMIN = 3;   //灯条正向最小角度差
    int T_ANGLE_THRE180MIN = 2;   //灯条反向最小角度差

    int best_x_diff; // 最佳装甲板的横坐标
    int best_y_diff; // 最佳装甲板的纵坐标
    int counting = 0;

    double T_HIGH_RAT = 0.2;   //灯带最大高差比 灯带高度差是最大灯柱的1/3
    double T_HIGH_RAT_ANGLE = 0.34;   //灯带角度符合最小角度高差比 灯带高度差是最大灯柱的1/2
    double T_WHIDTH_RAT = 0.4;  //灯带最大宽差比
    double T_WHIDTH_RAT_ANGLE = 0.55;  //灯带最大宽差比

    vector<RotatedRect> light_rect; // 存储过滤后的灯条代表的矩阵
    vector<RotatedRect> armors; // 存储能构成装甲板的矩阵
    vector<Armor_builded> armor_SECOND;
    Armor_builded shoot_object; // 存储最佳射击点

    Armor_Detector(); // 类构造函数


    void init();
    void hsv_rec(Mat img);
    void filter_hsv_red_preproc(Mat img, Mat& dst);
    void filter_hsv_blue_preproc(Mat img, Mat& dst);

    void find_contours(Mat img, Mat& dst);
    void fitting_contours(Mat img, Mat& dst);
    void fitting_armor(Mat img, Mat& dst);

    void find_shoot_object();
};



#endif // GETARMOR_HSV_H
