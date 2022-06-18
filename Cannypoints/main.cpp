#include <iostream>
#include "opencv2/opencv.hpp"
#include <algorithm>
#include <vector>
#include <ctime>
#include <numeric>

using namespace std;
using namespace cv;

#define STEP 6
#define JITTER 4
#define RAIO 6
#define RAIO_PEQUENO 3

int main(int argc, char **argv)
{
  int width, height, limite_inferior = 80;
  int x, y;
  vector<int> xrange, yrange;
  vector<Vec6i> pontos;
  Mat points, image, border, image_bw;
  Vec3b gray;

  image = imread("D:/UFRN/22.1/PDI/cannypoints/lovedead.jpg",IMREAD_COLOR);

  cvtColor(image, image_bw, COLOR_BGR2GRAY);

  width = image.size().width;
  height = image.size().height;

  //Aplica o algoritmo de canny na imagem
  Canny(image_bw, border, limite_inferior, 3 * limite_inferior);
  imshow("bordas_canny", border);
    imwrite("canny.png", border);
  waitKey();

  //Obtem imagem pontilhista
  xrange.resize(height / STEP);
  yrange.resize(width / STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  //Realiza amostragem dos pontos
  for (uint i = 0; i < xrange.size(); i++)
  {
    xrange[i] = xrange[i] * STEP + STEP / 2;
  }

  for (uint i = 0; i < yrange.size(); i++)
  {
    yrange[i] = yrange[i] * STEP + STEP / 2;
  }

  points = Mat(height, width, CV_8UC3, Scalar(255, 255, 255));

  random_shuffle(xrange.begin(), xrange.end());

  for (auto i : xrange)
  {
    random_shuffle(yrange.begin(), yrange.end());
    for (auto j : yrange)
    {
      x = i + rand() % (2 * JITTER) - JITTER + 1;
      y = j + rand() % (2 * JITTER) - JITTER + 1;

      //Impede o algoritmo de pegar pontos além dos limites da imagem
      if (x >= height)
      {
        x = height - 1;
      }
      if (y >= width)
      {
        y = width - 1;
      }

      gray = image.at<Vec3b>(x, y);
      circle(points,
             cv::Point(y, x),
             RAIO,
             Scalar(gray[0], gray[1], gray[2]),
             -1,
             LINE_AA);
    }
  }

  imshow("imagem_pontilhista", points);
    imwrite("points.png", points);
  waitKey();

  //Percorre matriz em busca da borda gerada pelo algoritmo de canny para
  //desenhar pontos pequenos
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (border.at<uchar>(i, j) != 0)
      {
        //Armazena a cor origina do ponto, bem como sua posição na
        //estrutura Vec6i, que armazena 6 inteiros
        gray = image.at<Vec3b>(i, j);
        pontos.push_back(Vec6i(j, i, gray[0], gray[1], gray[2], 0));
      }
    }
  }

  random_shuffle(pontos.begin(), pontos.end());

  //Desenha pontos pequenos na imagem
  Scalar cor;
  for (int i = 0; i < pontos.size(); i++)
  {
    Point p(pontos.at(i)[0], pontos.at(i)[1]);
    cor = Scalar(pontos.at(i)[2], pontos.at(i)[3], pontos.at(i)[4]);
    circle(points,
           p,
           RAIO_PEQUENO,
           cor,
           -1,
           LINE_AA);
  }

  imshow("imagem_pontilhista_corrigida", points);
  waitKey();

  imwrite("cannypoints.png", points);
  return 0;
}