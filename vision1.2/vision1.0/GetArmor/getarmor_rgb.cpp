#include"getarmor_rgb.h"
#include<iostream>
/**
 * @brief brightAdjust 调节的阈值
 * @param src
 * @param dst
 * @param dContrast
 * @param dBright
 */
void brightAdjust(Mat src, Mat dst, double dContrast, double dBright) {//亮度调节函数
    int nVal;
    //omp_set_num_threads(8);
    for (int nI = 0; nI<src.rows; nI++) {
        Vec3b* p1 = src.ptr<Vec3b>(nI);
        Vec3b* p2 = dst.ptr<Vec3b>(nI);
        for (int nJ = 0; nJ <src.cols; nJ++) {
            for (int nK = 0; nK < 3; nK++) {
                //每个像素的每个通道的值都进行线性变换
                nVal = (int)(dContrast * p1[nJ][nK] + dBright);
                if (nVal < 0) nVal = 0;
                if (nVal > 255) nVal = 255;
                p2[nJ][nK] = nVal;
            }
        }
    }
}

/**
 * 提取目标颜色部分，手动二值化
 * @brief getDiffImage
 * @param src1 需要检测的颜色
 * @param src2 减去的颜色图
 * @param dst 输出图
 * @param nThre 判断值
 */
void getDiffImage(Mat src1, Mat src2, Mat dst, int nThre) {

    /* omp_set_num_threads(8); #pragma omp parallel for */
    for (int nI = 0; nI<src1.rows; nI++) {
        uchar* pchar1 = src1.ptr<uchar>(nI);
        uchar* pchar2 = src2.ptr<uchar>(nI);
        uchar* pchar3 = dst.ptr<uchar>(nI);
        for (int nJ = 0; nJ <src1.cols; nJ++) {
            if (pchar1[nJ] - pchar2[nJ]> nThre) //
            {
                pchar3[nJ] = 255;
            }
            else
            {
                pchar3[nJ] = 0;
            }
        }
    }
}
/**
 * 可能为装甲的矩阵（全部轮廓图）
 * @brief armorDetect
 * @param vEllipse
 * @return 筛选后装甲的寻转矩阵
 */
vector<RotatedRect> armorDetect(vector<RotatedRect> vEllipse) { //旋转矩阵的向量
    vector<RotatedRect> vRlt;
    RotatedRect armor; //定义装甲区域的旋转矩形
    double nL, nW;
    double dAngle;
    vRlt.clear();
    if (vEllipse.size() < 2) //如果检测到的旋转矩形个数小于2，则直接返回
        return vRlt;
    for (unsigned int nI = 0; nI < vEllipse.size() - 1; nI++) //求任意两个旋转矩形的夹角
    {
        for (unsigned int nJ = nI + 1; nJ < vEllipse.size(); nJ++) {
            dAngle = abs(vEllipse[nI].angle - vEllipse[nJ].angle); //角度偏差在10以内
            while (dAngle > 180) dAngle -= 180; //判断这两个旋转矩形是否是一个装甲的两个LED等条
            if ((dAngle < ANGLE_THRE || 180 - dAngle < ANGLE_THRE) &&
                    abs(vEllipse[nI].size.height - vEllipse[nJ].size.height) < (vEllipse[nI].size.height + vEllipse[nJ].size.height) / T_SIZE_THRE &&
                    abs(vEllipse[nI].size.width - vEllipse[nJ].size.width) < (vEllipse[nI].size.width + vEllipse[nJ].size.width) / T_SIZE_THRE) {
                armor.center.x = (vEllipse[nI].center.x + vEllipse[nJ].center.x) / 2;
                //装甲中心的x坐标
                armor.center.y = (vEllipse[nI].center.y + vEllipse[nJ].center.y) / 2;
                //装甲中心的y坐标
                armor.angle = (vEllipse[nI].angle + vEllipse[nJ].angle) / 2;
                //装甲所在旋转矩形的旋转角度
                if (180 - dAngle < ANGLE_THRE) armor.angle += 90;
                nL = (vEllipse[nI].size.height + vEllipse[nJ].size.height) / 2;
                //装甲的高度
                nW = sqrt((vEllipse[nI].center.x - vEllipse[nJ].center.x) *
                          (vEllipse[nI].center.x - vEllipse[nJ].center.x) +
                          (vEllipse[nI].center.y - vEllipse[nJ].center.y) *
                          (vEllipse[nI].center.y - vEllipse[nJ].center.y));
                //装甲的宽度等于两侧LED所在旋转矩形中心坐标的距离
                if (nL < nW)
                {
                    armor.size.height = nL;
                    armor.size.width = nW;
                }
                else
                {
                    armor.size.height = nW;
                    armor.size.width = nL;
                }
                if(3<=nW/nL&&nW/nL<=6&&(abs(armor.angle-180)<10||abs(armor.angle)<10)){ //长宽比
                    cout<<"angle:"<<armor.angle<<endl;
                    vRlt.push_back(armor);
                }

                //将找出的装甲的旋转矩形保存到vector
            }
        }
    }
    return vRlt;
}
/**
 * @brief drawBox
 * @param box
 * @param img
 */
void drawBox(RotatedRect box, Mat img) {
    Point2f pt[4];
    int i;
    for (i = 0; i<4; i++) {
        pt[i].x = 0;
        pt[i].y = 0;
    }
    box.points(pt); //计算二维盒子顶点
    line(img, pt[0], pt[1], CV_RGB(255, 200, 255), 2, 8, 0);
    line(img, pt[1], pt[2], CV_RGB(255, 200, 255), 2, 8, 0);
    line(img, pt[2], pt[3], CV_RGB(255, 200, 255), 2, 8, 0);
    line(img, pt[3], pt[0], CV_RGB(255, 200, 255), 2, 8, 0);
}
/**
 *  提取图片中的目标
 *
 * @brief getArmor
 * @param frame0
 * @return  偏差坐标值的vector
 */
vector<RotatedRect> getArmor( Mat frame0){
    Size imgSize;
    RotatedRect s;
    //定义旋转矩形
    vector<RotatedRect> vEllipse;
    //定以旋转矩阵的向量，用于存储发现的目标区域

    // vector<RotatedRect> vArmor; //装甲矩阵
    bool bFlag=false;
    vector<vector<Point>> contour;//点集
    //  capture >> frame0;//读取摄像头信息（获取帧的大小）
    //删除读取摄像头部分，以下处理阶段改成死循环，返回一个坐标

    imgSize =frame0.size();
//    int capture_width=imgSize.width; //摄像头的宽
//    int capture_height=imgSize.height;//摄像头的高
   // cout<<imgSize<<endl;
    //Point Cen(1260/2,714/2);//中心坐标
    Mat rawImg= Mat(imgSize,CV_8UC3);//RGB3通道
    Mat grayImage=Mat(imgSize,CV_8UC1);//RGB1通道
    Mat rImage = Mat(imgSize, CV_8UC1);
    Mat gImage = Mat(imgSize, CV_8UC1);
    Mat bImage = Mat(imgSize, CV_8UC1);
    Mat binary = Mat(imgSize, CV_8UC1);
    Mat rlt0 = Mat(imgSize, CV_8UC1);
    Mat rlt = Mat(imgSize, CV_8UC1);

    namedWindow("Raw");
  //  while(1){
        //if(capture.read(frame0)){
            // drawMarker(frame0,Cen,CV_RGB(200, 10, 10),1,8,0); 中心坐标
            //输出图片rawImage
    brightAdjust(frame0,rawImg,1,-120);//每个像素每个通道的值都减去120
    Mat bgr[3];
    split(rawImg,bgr);//将三个通道的像素值分离
    bImage =bgr[0]; //blue
    gImage =bgr[1]; //green
    rImage =bgr[2]; //red
    //如果像素R值-G值大于25,则返回的二值图像的值为255,否则为0

    getDiffImage(gImage,rImage,binary,50);//蓝光25-30,红光改成80-100
    dilate(binary,grayImage,Mat(),Point(1,1),3); //图像膨胀
    erode(grayImage,rlt0,Mat(),Point(1,1),1);//图像腐蚀，先膨胀在腐蚀 属于闭运算
    adaptiveThreshold(rlt0,rlt,0, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 7, 5);
  //  imshow("rlt0",rlt0);
    //自适应阈值分割
    findContours(rlt,contour,RETR_CCOMP,CHAIN_APPROX_SIMPLE);//在二值图像中寻找轮廓
    //RETR_CCOMP=2 ,CHAIN_APPROX_SIMPLE=2
    for(int i=0;i<contour.size();i++){
        if(contour[i].size()>5){//判断当前轮廓是否大于5个像素点&&contour[i].size()<30
            bFlag=true; //如果大于10个，则检测到目标区域
            //拟合目标区域为椭圆，返回一个旋转矩阵（中心，角度，尺寸）

            s=fitEllipse(contour[i]);

            for(int nI=0;nI<5;nI++){
                for (int nJ = 0; nJ < 5; nJ++) //遍历以旋转矩形中心点为中心的5*5的像素块
                {
                    if (s.center.y - 2 + nJ > 0 && s.center.y - 2 + nJ < 480 &&
                            s.center.x - 2 + nI > 0 && s.center.x - 2 + nI < 640)
                        //判断该像素是否在有效的位置
                    {     //长度为3的char数组
                        Vec3b v3b = frame0.at<Vec3b>((int)(s.center.y - 2 + nJ), (int)(s.center.x - 2 + nI));
                        //获取遍历点点像素值
                        //判断中心点是否接近白色
                        // cout<<v3b[0]<<v3b[1]<<v3b[2]<<endl;
                        if (v3b[0] < 200 || v3b[1] < 200 || v3b[2] < 200)
                            bFlag = false;
                        //如果中心不是白色，则不是目标区域
                    }
                }
            }

            if(bFlag){
                vEllipse.push_back(s);//将发现的目标保存

            }
        }

        imshow("rlt",rlt);
    }
    //调用子程序，在输入的LED所在旋转矩形的vector中找出装甲的位置，并包装成旋转矩形，存入vector并返回
   return  vEllipse;

}
