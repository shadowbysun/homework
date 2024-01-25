#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
using namespace std;
using namespace cv;
Mat imgHSV, mask,imgColor;

int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;
VideoCapture cap("/home/root03/MyProject/resuorces/ood_blue.mp4");
Mat img;




int main()
{
    // string path = "Resources/1.png";
    // Mat img = imread(path);
    // cvtColor(img, imgHSV, COLOR_BGR2HSV);

    // 创建一个用于放置跟踪栏的窗口
    namedWindow("Trackbars", (640, 200)); //(640,200)是尺寸
    // 运行时，把3个min的都移到最小值，把3个max的都移到最大值，然后移动使其保持为白色
    // 添加Trackbar
    createTrackbar("Hue Min", "Trackbars", &hmin, 179);
    createTrackbar("Hue Max", "Trackbars", &hmax, 179);
    createTrackbar("Sat Min", "Trackbars", &smin, 255);
    createTrackbar("Sat Max", "Trackbars", &smax, 255);
    createTrackbar("Val Min", "Trackbars", &vmin, 255);
    createTrackbar("Val Max", "Trackbars", &vmax, 255);

    while (true)
    {
        cap.read(img);
        cvtColor(img, imgHSV, COLOR_BGR2HSV);
        // 检查数组元素是否位于其他两个数组的元素之间。
        // imgHSV为输入图像，mask为输出图像
        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);
        inRange(imgHSV, lower, upper, mask);
        imshow("Image", img);
        // imshow("Image HSV", imgHSV);
        imshow("Image mask", mask);
        waitKey(500); // 延时1ms
    }
    return 0;
}