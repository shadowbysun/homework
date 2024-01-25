#include<iostream>
#include<opencv4/opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(){
    VideoCapture cap(0);
    Mat img;
    while (true){
        cap.read(img);
        imshow("Image",img);
        waitKey(1);
    }
}