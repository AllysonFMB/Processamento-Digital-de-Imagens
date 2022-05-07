//#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  cv::Mat image;
  int p_1, p_2;

  std::cout << "Informe o P1:" <<std::endl;
  std:: cin >> p_1;
  std::cout << "Informe o P2:" <<std::endl;
  std:: cin >> p_2;

  image= cv::imread("D:/UFRN/22.1/PDI/img/biel.png",cv::IMREAD_GRAYSCALE);
  if(!image.data)
    std::cout << "nao abriu a img" << std::endl;

    for(int i=p_1;i<p_2;i++){
    for(int j=p_1;j<p_2;j++){
      image.at<uchar>(i,j) = 255 - image.at<uchar>(i,j);
    }
  }
  cv::imwrite("biel_negativo.png", image);
  cv::imshow("Janela", image);
  cv::waitKey();

  return 0;
}