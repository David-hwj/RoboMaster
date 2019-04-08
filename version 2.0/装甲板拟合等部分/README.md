# 文件结构 #



## REC_PART1.h ##
> 存放二值化过程的头文件

## REC_PART2.h ##
> 存放找轮廓、拟合椭圆、匹配装甲板等过程的头文件

## REC_PART1.cpp ##
> 存放二值化过程的源文件

## REC_PART2.cpp ##
> 存放找轮廓、拟合椭圆、匹配装甲板等过程的源文件

## 源.cpp ##
> 主函数所在

# 接口 #
## 如何调用REC_PART2 ##
声明一个类对象Armor_Detector A，通过A.init(Mat src_img, Mat thres_img)运行PART_2.其中src_img指的是未经处理的图像，thres_img指的是二值化图像
## 如何获得识别结果 ##
对于声明的类对象Armor_Detector A，通过A.shoot_object.armorS.center获得目标装甲板像素坐标