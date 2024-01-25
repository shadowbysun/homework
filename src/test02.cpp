#include<iostream>
#include<opencv4/opencv2/opencv.hpp>
using namespace std;
using namespace cv;
// int main(){
//     String path="/home/root03/MyProject/resuorces/bnana.jpg";
//     Mat img=imread(path);
//     Mat imgGray;
//     cvtColor(img,imgGray,COLOR_BGR2GRAY);
//     imshow("image",img);
//     imshow("image Gray",imgGray);

//     waitKey(0);
//     exit(0);
//     return 0;
// }

int main(){
    String path="/home/root03/MyProject/resuorces/kunkunplayball.mp4";
    VideoCapture cap(path);
    Mat image;
    while(true){
        //读取cap里面的每一张图片放到image里面
        cap.read(image);
        imshow("Image",image);
        waitKey(100);
    }
    
    return 0;
}