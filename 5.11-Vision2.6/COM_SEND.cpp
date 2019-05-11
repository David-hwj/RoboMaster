#include"COM_SEND.h"
unsigned char sendata[30];
Serial_port FCcom(115200, 8, 'N', 1);//实例化串口
int Serial_port::setOpt(int fd)
{
    struct termios newtio, oldtio;
    if (tcgetattr(fd, &oldtio) != 0)
    { return -1; }
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag |= CLOCAL | CREAD;//终端读使能
    newtio.c_cflag &= ~CSIZE;

    switch (nBits)//设置数据位数
    {
    case 7:
        newtio.c_cflag |= CS7;//7位
        break;
    case 8:
        newtio.c_cflag |= CS8;//8位
        break;
    }

    switch (nEvent)//设置校验位
    {
    case 'O':                     //奇校验
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':                     //偶校验
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':                    //无校验
        newtio.c_cflag &= ~PARENB;
        break;
    }

    switch (nSpeed)//设置波特率
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 57600:
        cfsetispeed(&newtio, B57600);
        cfsetospeed(&newtio, B57600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
    if (nStop == 1)//设置停止位
    {                newtio.c_cflag &= ~CSTOPB;        }
    else if (nStop == 2)
    {                newtio.c_cflag |= CSTOPB;        }
    newtio.c_cc[VTIME] = 4;//设置缓冲区字符读取等待时间
    newtio.c_cc[VMIN] = 0;//设置读取缓冲区字节数限制
    tcflush(fd, TCIFLUSH);
    if ((tcsetattr(fd, TCSANOW, &newtio)) != 0)
        return -1;
    else
        return 0;
}
/**
 * 通信协议
 */
void initsendate(){
    sendata[0]=0x3A;//数据头1
    sendata[1]=0xA3;//数据头2
    sendata[2]=0x13;//数据长度
    sendata[3]=0x1A;//命令ID
}
//******将int转成BYTE类型,四个字节读取**************
void double2byte(unsigned char *hexdata, int ddata)//输出整形
{   //十六进制写入内存（8位存储），再取出
    unsigned char str[128];
    sprintf((char*)str, "%d", ddata);
    hexdata[0] = (str[0]-'0')&0xff;
    hexdata[1] = (str[1]-'0')&0xff;
    hexdata[2] = (str[2]-'0')&0xff;
    hexdata[3] = (str[3]-'0')&0xff;
}
/*前二标识符 */
short crc16_get(char dat){
    char i;
    unsigned short crc16;
    int crc_Data;
    crc_Data=dat&0xffff;
    for(i=0;i<16;i++){
        if(crc_Data&0x4000){
            crc_Data=(crc_Data<<1)^0xa081;
        }
        else crc_Data=crc_Data<<1;
    }
    crc16=crc_Data;
    return crc16;
}

bool SendDate(float x,float y,int fd){
    
    int sendx=(int) x;
    int sendy=(int) y;
    int count,cx=sendx,cy=sendy;
    unsigned short xcrc=crc16_get(0xa1)&0xffff;//x坐标发射指令
    unsigned short ycrc=crc16_get(0xa2)&0xffff;//y坐标发射指令
    cout<<" zhuanma "<<xcrc<<" "<<(xcrc<<8&0xff)<<" "<<ycrc<<endl;
    for(count=0;abs(cx)>0;count++){
        cx/=10;
    }
    cx=count;

    for(count=0;abs(cy)>0;count++){
        cy/=10;
    }
    cy=count;
    cout<<"X:	"<<sendx<<" 	Y:	"<<sendy<<"	WEI:"<<cx<<"  "<<cy<<endl;

    // memset(&sendata,'0',10);
    for(int i=0;i<8;i++){
        sendata[i]=0x00;
    }
    sendata[0]=0xa1;
    sendata[1]=0x60;//(xcrc>>8)&0xff;
    sendata[2]=0x83;//xcrc&0x00ff;//低八位
    if(sendx>0) sendata[3]=0x80;
    else sendata[3]=0x00;
    if(sendy>0) sendata[5]=0x80;
    else sendata[5]=0x00;
//    double2byte(&sendata[7-cx],abs(sendx));//发送x
    // 3 4 ->x  5 6-> y
    sendata[3]=sendata[3]|(((unsigned int)abs(sendx)>>8)&0xff);
    sendata[4]=((unsigned int)abs(sendx))&0xff;
    sendata[5]=sendata[5]|(((unsigned int)abs(sendy)>>8)&0xff);

    sendata[6]=((unsigned int)abs(sendy))&0xff;
    sendata[7]=0xff;
    //检查发送结果
     for(int i=0;i<8;i++)
         cout<<i<<" : "<<(0xff&sendata[i])<<"| ";
     cout<<endl;
    int nr=write(fd,sendata,8); //发送x
    if(nr>0) printf("send data to uart success! nr=%d\n",nr);
    else  {
        printf("\n send data to uart fail!\n");
        //exit(0);
        return false;
    } 
    return true;
}
/* 无数据用此发送 0x0a+crc16_get(0x0a)*2+0x00*4+0xff*/
bool SendDate_error(int fd){
	if(SendDate(0,0,fd)){
        return true;
    }
	return false;
}
/**
 * @brief openusb
 * @return 串口信息
 */
int openusb(){
    int fd,fcom;
    char port[]="/dev/ttyUSB0"; //打开usb通信串口
    fd=open(port,O_WRONLY); //串口+读写模式
    if(fd==-1)
    {
        cout<<"can't find ttyusb0"<<endl; //打开失败
        return -1;
    }    
    fcom=FCcom.setOpt(fd);//实例化串口名 FCcom
    if(fcom==-1)
        cout<<"can't set option"<<endl;

    cout<<"success open USB"<<endl;
    return fd;
}
