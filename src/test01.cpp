#include<iostream>
#include<opencv4/opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(){
    Mat mat(3000,4000,CV_8UC3);
    int es=mat.elemSize();
    int size=mat.rows*mat.cols*es;
    for(int i=0;i<size;i+=es){
        mat.data[i]=255;
        mat.data[i+1]=0;
        mat.data[i+2]=0;
    }
    namedWindow("mat");
    imshow("mat",mat);
    waitKey(0);
    return 0;
}