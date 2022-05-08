//#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char **argv)
{
  cv::Mat image, realce;
  int width, height;
  int nobjects, counter;

  cv::Point p;
  image = cv::imread("D:/UFRN/22.1/PDI/labeling/bolhas.png", cv::IMREAD_GRAYSCALE);

  if (!image.data)
  {
    std::cout << "imagem nao carregou corretamente\n";
    return (-1);
  }
  cv::imshow("imagem original", image);

  width = image.cols;
  height = image.rows;
  std::cout << width << "x" << height << std::endl;

  p.x = 0;
  p.y = 0;

  /*EXCLUIR BORDAS*/
  // Excluir elementos da primeira e última linha
  for (int i = 0; i < height; i++)
  {
    if (image.at<uchar>(0, i) == 255)
    {
      cv::floodFill(image, Point(i, 0), 0);
    }
    if (image.at<uchar>(width - 1, i) == 255)
    {
      cv::floodFill(image, Point(i, width - 1), 0);
    }
  }
  // Excluir elementos da primeira e última coluna
  for (int i = 0; i < width; i++)
  {
    if (image.at<uchar>(i, 0) == 255)
    {
      cv::floodFill(image, Point(0, i), 0);
    }
    if (image.at<uchar>(i, height - 1) == 255)
    {
      cv::floodFill(image, Point(height - 1, i), 0);
    }
  }
  /**/
  cv::imwrite("image_semborda.png", image);

  /*BUSCAR OBJETOS PRESENTES*/
  nobjects = 0;
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (image.at<uchar>(i, j) == 255)
      {
        // achou um objeto
        nobjects++;
        p.x = j;
        p.y = i;
        // preenche o objeto com o contador
        cv::floodFill(image, p, nobjects);
      }
    }
  }
  /**/
  cv::equalizeHist(image, realce);
  cv::imshow("imagem contada", image);
  cv::imshow("realce", realce);

  cv::imwrite("image_realce.png", realce);

  // PINTAR FUNDO DE BRANCO P/ CONTAGEM DE BURACOS--
  cv::floodFill(image, Point(0, 0), 255);

  /*PROCURANDO BURACOS */
  counter = 0;
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (image.at<uchar>(i, j) == 0 && (int)image.at<uchar>(i, j - 1) > counter)
      { // se encontrar um buraco e já não tiver contado a bolha
        // Found a bubble with a hole
        counter++;
        p.x = j - 1;
        p.y = i;
        cv::floodFill(image, p, counter);
      }
    }
  }
  /**/

  std::cout << "bolhas: " << nobjects << " e bolhas com buracos: " << counter << std::endl;
  cv::imshow("image final", image);
  cv::imwrite("labeling.png", image);
  cv::waitKey();
  return 0;
}