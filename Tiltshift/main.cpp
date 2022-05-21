#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

double alfa;
int center_slider=0;
int center_slider_max=100;

int alfa_slider = 0;
int alfa_slider_max = 100;

int top_slider = 0;
int top_slider_max = 100;

cv::Mat image1, image2, blended;
cv::Mat imageTop;

char TrackbarName[50];

void on_trackbar_blend(int, void*){
 alfa = (double) alfa_slider/alfa_slider_max ;
 cv::addWeighted( image2, alfa, imageTop, 1-alfa, 0.0, blended);
 cv::imshow("addweighted", blended);
}

void on_trackbar_line(int, void*){
  image2.copyTo(imageTop);

  int width = image2.size().width;
  int height = image2.size().height;

  int limit = top_slider*height/100;
  int base = center_slider*height/100;


  if(limit > 0){
    if(base >= 0 && base <= height-limit){
      cv::Mat tmp = image1(cv::Rect(0, base, width,limit));
      tmp.copyTo(imageTop(cv::Rect(0, base, width,limit)));
    }

    else{
      cv::Mat tmp = image1(cv::Rect(0, 0, width,limit));
      tmp.copyTo(imageTop(cv::Rect(0, 0, width,limit)));
    }
  }

  on_trackbar_blend(alfa_slider,0);
}

int main(int argvc, char** argv){
  image1 = cv::imread("D:/UFRN/22.1/PDI/addweighted/original.jpg");

  resize(image1,image1,Size(800,800));
  image1.copyTo(image2);
  cv::namedWindow("addweighted", 1);
  image2.convertTo(image2,CV_32F);

  /*
  float media3x3[]={1,1,1,
                 1,1,1,
                 1,1,1};

  float gauss7x7[] = {1, 4, 7, 4, 1,
                  4, 16, 26, 16, 4,
                  7, 26, 41, 26, 7,
                  4, 16, 26, 16, 4,
                  1, 4, 7, 4, 1};
  */
    float gauss3x3[]={1,2,1,
                 2,4,2,
                 1,2,1};
  
  cv::Mat mascara;
  mascara = cv::Mat(3,3,CV_32F,gauss3x3);
  scaleAdd(mascara, 1/16.0, cv::Mat::zeros(3,3,CV_32F), mascara);

  for (int i = 0; i < 7; i++) {
      cv::filter2D(image2, image2, image2.depth(), mascara, cv::Point(1,1), 0);
  }
  image2.convertTo(image2, CV_8U);
  image2.copyTo(imageTop);
  
  sprintf( TrackbarName, "Decaimento x %d", alfa_slider_max );
  createTrackbar( TrackbarName, "addweighted",
          &alfa_slider,
          alfa_slider_max,
          on_trackbar_blend );
  on_trackbar_blend(alfa_slider, 0 );

  sprintf( TrackbarName, "altura x %d", top_slider_max );
  createTrackbar( TrackbarName, "addweighted",
          &top_slider,
          top_slider_max,
          on_trackbar_line );
  on_trackbar_line(top_slider, 0 );

  sprintf( TrackbarName, "Centrox %d", top_slider_max );
  createTrackbar( TrackbarName, "addweighted",
          &center_slider,
          center_slider_max,
          on_trackbar_line );
  on_trackbar_line(center_slider, 0 );

  waitKey(0);
  cv::imwrite("tiltyshift.jpg", blended);
  cv::imshow("window",blended);
  
  cv::imwrite("original.jpg", image1);
  cv::imshow("window2",image1);
  return 0;
}
