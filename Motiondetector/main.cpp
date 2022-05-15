#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace std;
using namespace cv;

Mat histograma(Mat const imagem, int bins){
    int histSize[] = {bins}; 
    float lranges[] = {0, 256}; 
    const float* ranges[] = {lranges};
    Mat hist;
    int channels[]={0};
    calcHist(&imagem, 1, channels, Mat(), hist, 1, histSize, ranges, true, false);
    return hist;
}

int main(){
    Mat imagem, hist_novo, hist_anterior; 
    int temp=0;
    double compara;

    VideoCapture cap; 
    cap.open(0);
    if(!cap.isOpened()){
        cout << "cameras indisponiveis";
        return -1;
    }

    cap >> imagem;
    hist_novo = histograma(imagem, 256);
    while(1){
        hist_novo.copyTo(hist_anterior);
        cap >> imagem;
        cvtColor(imagem, imagem, COLOR_BGR2GRAY);
        hist_novo = histograma(imagem, 256);

        compara = compareHist(hist_novo, hist_anterior, 3);
        if(compara >= 0.07) cout<<"movimento detectado - num: "<<++temp<<endl;

        imshow("Detector de movimento", imagem);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}