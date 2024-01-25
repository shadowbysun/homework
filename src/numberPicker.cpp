#include<iostream>
#include<opencv4/opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main()
{
    Mat temp = imread("/home/root03/MyProject/resuorces/屏幕截图 2024-01-23 212928.png");
    Mat src = imread("/home/root03/MyProject/resuorces/屏幕截图 2024-01-23 223004.png");
    Mat dst = src.clone();
    imshow("temp", temp);
	//1.构建结果图像resultImg(注意大小和类型)
    //如果原图(待搜索图像)尺寸为W x H, 而模版尺寸为 w x h, 则结果图像尺寸一定是(W-w+1)x(H-h+1)
    //结果图像必须为单通道32位浮点型图像
    int width = src.cols - temp.cols + 1;//result宽度
    int height = src.rows - temp.rows + 1;//result高度
    Mat result(height, width, CV_32FC1);//创建结果映射图像
    //2.模版匹配
    //这里我们使用的匹配算法是标准平方差匹配 method=TM_SQDIFF_NORMED，数值越小匹配度越好
    //matchTemplate(src, temp, result, TM_SQDIFF); //平方差匹配法(最好匹配0)
    matchTemplate(src, temp, result, TM_SQDIFF_NORMED); //归一化平方差匹配法(最好匹配0)
    //matchTemplate(src, temp, result, TM_CCORR); //相关匹配法(最坏匹配0)
    //matchTemplate(src, temp, result, TM_CCORR_NORMED); //归一化相关匹配法(最坏匹配0)
    //matchTemplate(src, temp, result, TM_CCOEFF); //系数匹配法(最好匹配1)
    //matchTemplate(src, temp, result, TM_CCOEFF_NORMED);//化相关系数匹配,最佳值1
    imshow("result", result);
    //3.正则化(归一化到0-1)
    normalize(result, result, 0, 1, NORM_MINMAX, -1);//归一化到0-1范围
    //4.找出result中的最大值及其位置
    double minValue, maxValue;
    Point minLoc, maxLoc;
    // 定位极值的函数
    minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);
    cout << "minValue=" << minValue << endl;
    cout << "maxValue=" << maxValue << endl;
	//5.根据result中的最大值位置在源图上画出矩形和中心点
    rectangle(dst, maxLoc, Point(maxLoc.x + temp.cols, maxLoc.y + temp.rows), Scalar(0, 255, 0), 2, 8);
    imshow("dst", dst);

    waitKey(0);
    return 0;
}
