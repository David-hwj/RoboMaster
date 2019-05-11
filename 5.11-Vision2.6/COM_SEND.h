#ifndef COM_SEND_H
#define COM_SEND_H
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> /*Unix 标准函数定义*/
#include <iostream>
#include <termios.h>
#include <fcntl.h>
#include "opencv2/video.hpp"
using namespace std;
#define BYTE unsigned char

class Serial_port{
public:
    Serial_port(int NSpeed, int NBits, char NEvent, int NStop):nSpeed(NSpeed),nBits(NBits),nEvent(NEvent),nStop(NStop){}//寻找设备
    int setOpt(int fd);     //串口属性设置
    ~Serial_port(){}
private:
    int  nSpeed;
    int  nBits;
    char nEvent;
    int  nStop;

};
void initsendate();
void double2byte(unsigned char *hexdata, int ddata);
short crc16_get(char dat);
bool SendDate(float x,float y,int fd);
bool SendDate_error(int fd);
int openusb();
#endif // COM_H
