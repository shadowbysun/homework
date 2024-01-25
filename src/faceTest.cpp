#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp> //对象检测头文件
#include <iostream>
using namespace std;
using namespace cv;

int main()
{
    string path = "/home/root03/MyProject/resuorces/1_1_“桀桀桀，魂殿萧炎，七星斗宗，请赐教”_“桀桀桀，魂殿萧炎，七星斗宗，请赐教(2)”.mp4";
    // Mat img = imread(path);
    Mat img, imgGray;
    VideoCapture cap(path);
    //获取当前视频的大小
    int width = cap.get(3);
    int height = cap.get(4);
    // OpenCV提供的分类器
    CascadeClassifier faceCascade;
    // 从文件加载分类器(已经训练好的模型)
    faceCascade.load("/home/root03/MyProject/resuorces/lbpcascade_frontalface.xml");
    // 检测文件是否加载成功
    if (faceCascade.empty())
    {
        cout << "XML file not loaded" << endl;
    }
    vector<Rect> faces; // 定义用于接收检测结果的
    while (true)
    {
        cap.read(img);
        // 缩放来减小工作量
        resize(img,img,Size(width/3,height/3));
        // 转换为灰度图，减少数据量
        cvtColor(img, imgGray, COLOR_BGR2GRAY);
        // 在输入图像中检测不同大小的对象。检测到的对象将以矩形列表的形式返回。
        faceCascade.detectMultiScale(imgGray /*输入*/, faces /*输出*/, 1.1 /*比例因子*/, 5,0,Size(20,20) /*最近邻*/); // 通过增加最近邻的值可以消除误报，但是过大将会导致漏检测

        for (int i = 0; i < faces.size(); i++)
        {
            // int area = contourArea(faces[i]);
            rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 3); // 绘制矩形
        }

        namedWindow("Image", 0);
        imshow("Image", img);
        waitKey(30); // 增加延时，0表示无穷
    }

    return 0;
}