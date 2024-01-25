#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>
using namespace std;
using namespace cv;

const int kThreashold = 220;
const int kMaxVal = 255;
const Size kGaussianBlueSize = Size(5, 5);

// 获取符合条件的灯条的相应的矩形集合
vector<RotatedRect> getContours(Mat imgDil, Mat img)
{
    // imgDil是传入的扩张边缘的图像用来查找轮廓，img是要在其上绘制轮廓的图像
    vector<vector<Point>> contours; // 轮廓检测到的轮廓。每个轮廓线存储为一个点的向量
    // 包含关于映像拓扑的信息  typedef Vec<int, 4> Vec4i;具有4个整数值
    vector<Vec4i> hierarchy;
    // 在二值图像中查找轮廓。该函数利用该算法从二值图像中提取轮廓
    findContours(imgDil, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);
    vector<vector<Point>> conPoly(contours.size()); // 定义approxCurve
    vector<RotatedRect> boundRect(contours.size()); // 定义存储边界框的变量
    vector<Point> result;

    for (int i = 0; i < contours.size(); i++)
    { // 遍历检测到的轮廓
        int area = contourArea(contours[i]);
        if (area > 500)
        { // 轮廓面积＞1000才绘制
            // 定义 0.02*轮廓周长为给定的界限（精度）
            float peri = arcLength(contours[i], true);

            // 筛选
            if (fitEllipse(contours[i]).size.width / fitEllipse(contours[i]).size.height > 4)
            {
                continue;
            }
            // 用椭圆进行拟合
            RotatedRect result = fitEllipse(contours[i]);
            boundRect.push_back(result);
        }
    }
    // 最后将边界矩形返回
    return boundRect;
}

Mat findColor(Mat img)
{
    Mat imgHSV, mask;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);
    imshow("2", imgHSV);
    Scalar lower(135, 0, 130);
    Scalar upper(179, 32, 225);
    inRange(img, lower, upper, mask);
    imshow("1", mask);
    return mask;
}

int main()
{

    string path = "/home/root03/MyProject/resuorces/ood_blue.mp4";
    VideoCapture cap(path);
    Mat img, channels[3];
    Mat imgGray, imgBlur, imgCanny, imgDil, imgBina;
    vector<Mat> mat;
    while (true)
    {
        // 读取cap里面的每一张图片放到image里面
        cap.read(img);
        // img=findColor(img);
        // 对读取到的每一张图片进行预处理

        // 将三个颜色通道分离出来
        split(img, channels);

        // 对蓝色的部分进行二值化处理
        threshold(channels[0], imgBina, 220, 255, 0);

        // 再进行高斯模糊
        GaussianBlur(imgBina, imgBlur, Size(5, 5), 0);

        // 使用高斯滤波器模糊图像。该函数将源图像与指定的高斯核进行卷积,Size(7,7)是核大小,数字越大越模糊
        // Canny(imgBlur, imgCanny, 25, 75);
        // 边缘检测，阈值1，2可调，目的：显示更多的边缘
        Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
        // 创建一个核，增加Size（只能是奇数）会扩张/侵蚀更多
        dilate(imgBlur, imgDil, kernel); // 扩张边缘（增加边缘厚度
        dilate(imgBlur, imgDil, kernel, Point(-1, -1));
        vector<RotatedRect> boundRect = getContours(imgDil, img);
        for (int i = 0; i < boundRect.size(); i++)
        {

            for (int j = i + 1; j < boundRect.size(); j++)
            {
                if (boundRect[i].size.width == 0 || boundRect[j].size.width == 0)
                {
                    continue;
                }
                RotatedRect leftLight = boundRect[i];
                RotatedRect rightLight = boundRect[j];
                float angleDiff = abs(leftLight.angle - rightLight.angle);
                float LenDiff_ratio = abs(leftLight.size.height - rightLight.size.height) / max(leftLight.size.height, rightLight.size.height);

                // 左右灯条相距距离
                float dis = pow(pow((leftLight.center.x - rightLight.center.x), 2) + pow((leftLight.center.y - rightLight.center.y), 2), 0.5);

                // 左右灯条长度的平均值
                float meanLen = (leftLight.size.height + rightLight.size.height) / 2;
                // 左右灯条长度差比值
                float lendiff = abs(leftLight.size.height - rightLight.size.height) / meanLen;
                // 左右灯条中心点y的差值
                float yDiff = abs(leftLight.center.y - rightLight.center.y);
                // y差比率
                float yDiff_ratio = yDiff / meanLen;
                // 左右灯条中心点x的差值
                float xDiff = abs(leftLight.center.x - rightLight.center.x);
                // x差比率
                float xDiff_ratio = xDiff / meanLen;
                // 相距距离与灯条长度比值
                float ratio = dis / meanLen;
                // 筛选

                if (angleDiff > 15)
                {
                    // cout << "1" << endl;
                    //  waitKey(30);
                    continue;
                }
                if (LenDiff_ratio > 1)
                {
                    // cout << "2" << endl;
                    //  waitKey(30);
                }
                // 左右灯条长度差比值
                if (lendiff > 0.8)
                {
                    // cout << "3" << endl;
                    //  waitKey(30);
                    continue;

                } // y差比率
                if (yDiff_ratio > 1.5)
                {
                    // cout << "4" << endl;
                    //  waitKey(30);
                    continue;

                } // x差比率
                if (xDiff_ratio > 2)
                {
                    // cout << "5" << endl;
                    //  waitKey(30);
                    continue;
                }
                if (xDiff_ratio < 0.8)
                {
                    continue;
                }
                // 相距距离与灯条长度比值
                if (ratio > 3)
                {
                    // cout << "6" << endl;
                    //  waitKey(30);
                    continue;
                }

                if (ratio < 0.8)
                {

                    // cout << "2" << endl;
                    // waitKey(10);
                    continue;
                }
                // 如果筛选条件过了，就开始绘制图像
                // 计算中心点坐标
                Point center = Point((leftLight.center.x + rightLight.center.x) / 2, (leftLight.center.y + rightLight.center.y) / 2);
                // 得到旋转矩形
                RotatedRect rect = RotatedRect(center, Size(dis, meanLen), (leftLight.angle + rightLight.angle) / 2);
                Point2f vertices[4];
                rect.points(vertices);

                // 便利旋转矩形的四个顶点，两两连线，画出矩形
                for (int i = 0; i < 4; i++)
                {
                    line(img, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0), 2);
                    boundRect[i].size.width = 0;
                    boundRect[j].size.width = 0;
                    continue;
                }
                // 计算ROI的旋转矩阵
                float angle = rect.angle;
                //这里宽度减40是为了去除两边灯柱的干扰
                Size rect_size = Size(dis - 40, meanLen);
                // // 确保angle在-45到45度之间
                // if (angle < -45.)
                // {
                //     angle += 90.0;
                //     swap(rect_size.width, rect_size.height);
                // }
                // else if (angle > 45.)
                // {
                //     angle -= 90.0; // 如果角度大于45度，逆时针旋转使其小于45度
                //     swap(rect_size.width, rect_size.height);
                // }

                // 创建旋转矩阵
                Mat rotation_matrix = getRotationMatrix2D(rect.center, angle, 1);

                // 创建ROI图像
                Mat img_rotated;
                warpAffine(img, img_rotated, rotation_matrix, img.size(), INTER_CUBIC);
                // 查看矩形尺寸，如果宽度大于高度，考虑再次旋转180度来矫正
                // if (rect_size.height > rect_size.width)
                // {
                //     // 计算并应用额外的180度旋转
                //     Mat rotation_matrix_180 = getRotationMatrix2D(rect.center, 180, 1.0);
                //     warpAffine(img_rotated, img_rotated, rotation_matrix_180, img.size(), INTER_CUBIC);
                // }

                // 再次创建ROI，这次是截取数字区域
                Mat img_cropped;
                getRectSubPix(img_rotated, rect_size, rect.center, img_cropped);

                cv::Mat gray;
                cv::cvtColor(img_cropped, gray, cv::COLOR_BGR2GRAY);

                // Enhance brightness if necessary
                // 对亮度非常低的图像进行调整，可以灵活设置参数
                double alpha = 3.0; // 控制对比度
                int beta = 100;      // 控制亮度
                cv::Mat brightened;
                gray.convertTo(brightened, -1, alpha, beta); // 提高对比度和亮度

                // Threshold the image to isolate white pixels
                // 阈值设置也可以根据图片亮度微调
                cv::Mat thresholded;
                cv::threshold(brightened, thresholded, 200, 255, cv::THRESH_BINARY);

                // Calculate the white pixel count
                int white_pixels = cv::countNonZero(thresholded);
                imshow("1",thresholded);
                // 计算二值化图像中的白色像素数量
                // double white_pixels = countNonZero(imgBina);

                // 如果白色像素数量高于某个阈值，则认为存在数字，否则不显示
                double area = img_cropped.total(); // 区域总像素数
                double ratio1 = white_pixels / area;

                // 白色像素所占比例阈值
                const double ratioThreshold = 0.25; // 比如白色像素要占总数的10%

                if (ratio1 > ratioThreshold)
                {
                    // 如果点的数量超过了阈值百分比，我们认为图像中有数字
                    // 显示数字区域
                    imshow("Digit Area", img_cropped); // 或者为了显示原图而不是二进制图，可以返回img_cropped
                }
                else
                {
                    // 没有足够的白色像素，我们认为图像中没有数字
                }
            }
        }
        // imshow("imgGray", imgBina);
        // imshow("imgBlur", imgBlur);
        // imshow("imgCanny", imgCanny);
        imshow("imgDil", imgDil);
        namedWindow("Image", 0);
        imshow("Image", img);
        waitKey(100);
    }
    return 0;
}