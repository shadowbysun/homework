#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
using namespace std;
using namespace cv;

list<int> myColor{121, 179, 0, 32, 130, 255};

void getContours(Mat imgDil, Mat img)
{
	// imgDil是传入的扩张边缘的图像用来查找轮廓，img是要在其上绘制轮廓的图像
	vector<vector<Point>> contours; // 轮廓检测到的轮廓。每个轮廓线存储为一个点的向量
	// 包含关于映像拓扑的信息  typedef Vec<int, 4> Vec4i;具有4个整数值
	vector<Vec4i> hierarchy;
	// 在二值图像中查找轮廓。该函数利用该算法从二值图像中提取轮廓
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conPoly(contours.size()); // 定义approxCurve
	vector<Rect> boundRect(contours.size());		// 定义存储边界框的变量
	for (int i = 0; i < contours.size(); i++)
	{ // 遍历检测到的轮廓
		int area = contourArea(contours[i]);
		// cout << area << endl;
		string objectType; // 定义轮廓类型，便于添加文字到边界框
		if (area > 1000)
		{ // 轮廓面积＞1000才绘制
			// 定义 0.02*轮廓周长为给定的界限（精度）
			float peri = arcLength(contours[i], true);
			// 以指定的精度近似多边形曲线。第二个参数conPloy[i]存储近似的结果，是输出。
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			boundRect[i] = boundingRect(conPoly[i]); // 计算边界矩形
			// // 找近似多边形的角点,三角形有3个角点，矩形/正方形有4个角点，圆形>4个角点
			// int objCor = (int)conPoly[i].size();

			//绘制边界矩形
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
			//画中心点
			circle(img, (boundRect[i].tl() + boundRect[i].br()) / 2, 2, Scalar(0, 255, 0), FILLED);

			// // 这里 objCor 大于3 就判断为圆形
			// if (objCor > 3)
			// {
			// 	objectType = "Circle";
			// 	// 检测到圆形，就绘制边界矩形
			// 	rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
			// 	// 然后画点
			// 	circle(img, (boundRect[i].tl() + boundRect[i].br()) / 2, 7, Scalar(0, 255, 0), FILLED);
			// }
			// drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			// 绘制边界矩形,如果是圆形再绘制边界矩形
			// rectangle(img, boundRect[0].tl(), boundRect[0].br(), Scalar(0, 255, 0), 5);
			// 添加标注，boundRect[i].y-5 是为了将文字房子框的上方
			// putText(img, objectType, {boundRect[i].x, boundRect[i].y - 5} /*文字坐标*/, FONT_HERSHEY_PLAIN, 1, Scalar(0, 69, 255), 1);
		}
	}
}

Mat findColor(Mat img)
{
	Mat imgHSV, mask;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	imshow("2",imgHSV);
	Scalar lower(0, 89, 157);
	Scalar upper(58, 146, 225);
	inRange(imgHSV, lower, upper, mask);
	imshow("1",mask);
	return mask;
}

int main()
{

	string path = "/home/root03/MyProject/resuorces/kunkunplayball.mp4";
	VideoCapture cap(path);
	Mat img, newimg;

	while (true)
	{
		// 读取cap里面的每一张图片放到image里面
		cap.read(img);
		// img=findColor(img);
		// 对读取到的每一张图片进行预处理
		Mat imgGray, imgBlur, imgCanny, imgDil;
		// cvtColor(img, imgGray, COLOR_BGR2GRAY);
		imgGray = findColor(img);
		// cvt是convert的缩写，将图像从一种颜色空间转换为另一种颜色空间。
		GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
		// 使用高斯滤波器模糊图像。该函数将源图像与指定的高斯核进行卷积,Size(7,7)是核大小,数字越大越模糊
		Canny(imgBlur, imgCanny, 25, 75);
		// 边缘检测，阈值1，2可调，目的：显示更多的边缘
		Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
		// 创建一个核，增加Size（只能是奇数）会扩张/侵蚀更多
		dilate(imgCanny, imgDil, kernel); // 扩张边缘（增加边缘厚度
		getContours(imgDil, img);		  // img是在其上绘轮廓的图片

		// imshow("imgGray", imgGray);
		// imshow("imgBlur", imgBlur);
		// imshow("imgCanny", imgCanny);
		// imshow("imgDil", imgDil);
		// namedWindow("Image",0);
		imshow("Image", img);
		waitKey(50);
	}
	return 0;
}