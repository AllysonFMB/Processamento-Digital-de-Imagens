//#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char **)
{
    cv::Mat image, image_aux;
    int width, height;

    image = cv::imread("D:/UFRN/22.1/PDI/img/biel.png", cv::IMREAD_GRAYSCALE);
    if (!image.data)
        std::cout << "nao abriu a img" << std::endl;

    cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);
    
    width = image.cols; //largura
    height = image.rows; //altura

    //image_aux = rect1
    image(Rect(0, 0, width / 2, height / 2)).copyTo(image_aux);    
    cv::imwrite("biel_1.png", image_aux);                                            
    
    //rect1 = rect4
    image(Rect(height / 2, width / 2, width / 2, height / 2)).copyTo(image(Rect(0, 0, width / 2, height / 2))); 
    cv::imwrite("biel_2.png", image);  

    //rect4 = image_aux
    image_aux.copyTo(image(Rect(height / 2, width / 2, width / 2, height / 2)));                                
    cv::imwrite("biel_3.png", image); 
    //image_aux = rect2
    image(Rect(0, width / 2, width / 2, height / 2)).copyTo(image_aux);    
    cv::imwrite("biel_4.png", image_aux); 
    //rect2 = rect3                                    
    image(Rect(height / 2, 0, width / 2, height / 2)).copyTo(image(Rect(0, width / 2, width / 2, height / 2))); 
    cv::imwrite("biel_5.png", image); 
    //rect3 = image_aux
    image_aux.copyTo(image(Rect(height / 2, 0, width / 2, height / 2)));                                        
    cv::imwrite("biel_6.png", image); 
    
    cv::imshow("janela", image);
    cv::waitKey();
    return 0;
}