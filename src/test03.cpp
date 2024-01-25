#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
using namespace std;
using namespace cv;
float w = 450, h = 650;//图片大小
Mat matrix, imgWarp;//变化矩阵和变化后的图像
int main()
{
    string path="/home/root03/MyProject/Resources/cards.jpg";
    Mat img=imread(path);
    //我们图片中k的四个顶点的位置
    Point2f src[4] = { {529,142},{771,190},{405,395},{674,457} };
    //变化之后的四个顶点的位置
	Point2f dst[4] = { {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };
    //我们要根据这两个位置矩阵通过函数来计算出变换矩阵
    matrix=getPerspectiveTransform(src,dst);
    //然后根据变化矩阵调用函数来得出变化后的图片
    warpPerspective(img,imgWarp,matrix,Size(w,h));
    imshow("image",img);
    imshow("image warp",imgWarp);
    
    waitKey(0);
    return 0;
}