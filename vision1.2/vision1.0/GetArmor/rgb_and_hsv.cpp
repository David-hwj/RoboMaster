#include"rgb_and_hsv.h"
/**
 * @brief combine 结合HSV+RGB两个向量
 * @param frame0 图片
 * @param fd 串口信息
 */
void combine(Mat frame0,int fd){
    vector<RotatedRect> vRlt; //目标向量
     Armor_Detector Detector;
    vRlt = armorDetect(getArmor(frame0));//RGB目标向量
    queue<Point> result;
    for (unsigned int nI = 0; nI < vRlt.size(); nI++) //在当前图像中标出装甲的位置
    {
      drawBox(vRlt[nI], frame0);
      Point p;
      p.x=vRlt[nI].center.x-frame0.cols/2;
      p.y=vRlt[nI].center.y-frame0.rows/2;
      //计算偏差并保存
      result.push(p);//RGB数据
    // cout<<"目标   target:！！！！！！！！！！！！！！！！1！！！！！！！！1"<<p<<endl;
      //circle(frame0,vRlt[nI].center,10,Scalar(0,0,255),-1);
      //imshow("Version2.0", frame0);
     // rectangle(frame0, vRlt[nI], Scalar(0, 255, 255), 2, 8, 0);
      //cout<<vRlt[nI].center.x-capture_width/2<<"  "<<vRlt[nI].center.y-capture_height/2<<endl;
    }
   // imshow("Version2.0", frame0);
    Detector.hsv_rec(frame0);//HSV数据
    for(int i=0;i<Detector.armor_SECOND.size();i++){
       Point p;
       p.x= Detector.armor_SECOND[i].x_diff;
       p.y= Detector.armor_SECOND[i].y_diff;

       //获取偏差并保存
      // cout<<"目标   target:！！！！！！！！！！！！！！！！1！！！！！！！！1"<<p<<endl;
       drawBox(Detector.armor_SECOND[i].armorS, frame0);

       result.push(p);
    }
    Send(fd,result); //发送数据
    //circle(frame0,Point(200,200),50,Scalar(0,255,255),-1);
    imshow("Version1.0", frame0);
    //显示最后处理效果
    vRlt.clear();
    //数据待检查！！！！！！！！！！！！！！！！！！！！！！！
    for( int i=0;i<result.size();i++){
        result.pop();
    }
}
/**
 * @brief Send
 * @param fd
 * @param q 要发送的数据
 */
void Send(int fd,queue<Point> q){
    for(int i=0;i<q.size();i++){
        Point p;
        p=q.front();

        q.pop();
        SendDate(p.x,p.y/2,fd);
    }
}

