#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{
    cv::Mat image, equalizada;
    cv::VideoCapture cap;
    int key;

    cap.open(0);

    if (!cap.isOpened())
    {
        std::cout << "cameras indisponiveis";
        return -1;
    }

    while (1)
    {
        // captura imagem
        cap >> image;
        // Converter para grayscale
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
        // histograma equalizado
        cv::equalizeHist(image, equalizada);

        cv::namedWindow("normal", cv::WINDOW_NORMAL);
        cv::namedWindow("equalizada", cv::WINDOW_NORMAL);

        cv::imshow("normal", image);
        cv::imshow("equalizada", equalizada);

        cv::imwrite("frame_normal", image);
        cv::imwrite("frame_equalizada", equalizada);

        key = cv::waitKey(30);
        if (key == 30)
            break;
    }
    cv::destroyAllWindows();

    return 0;
}