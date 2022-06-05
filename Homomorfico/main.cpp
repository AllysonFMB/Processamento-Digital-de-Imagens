#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#define RADIUS 20

using namespace cv;
using namespace std;

double gammaH, gammaL, c, d0;
int gammaH_slider = 0, gammaL_slider = 0, c_slider = 0, d0_slider = 0;
const int gammaH_max = 50, gammaL_max = 10, c_max = 100, d0_max = 200;

Mat padded, image, filtrada;

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image) {
  Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols / 2;
  int cy = image.rows / 2;

  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  // A <-> D
  A.copyTo(tmp);
  D.copyTo(A);
  tmp.copyTo(D);

  // C <-> B
  C.copyTo(tmp);
  B.copyTo(C);
  tmp.copyTo(B);
}

Mat filtroHomomorfico (double gammaL, double gammaH, double c, double d0){
    Mat filtro = Mat(padded.size(), CV_32FC2, Scalar(0)), tmp = Mat(padded.size(), CV_32F);
    int dft_M = padded.rows, dft_N = padded.cols;

    for(int i = 0; i < dft_M; i++){
        for(int j=0; j < dft_N; j++){
            tmp.at<float> (i,j) = (gammaH - gammaL)*(1 - exp(-c*(( (i-dft_M/2)*(i-dft_M/2) + (j-dft_N/2)*(j-dft_N/2) ) / (d0*d0) ))) + gammaL;
        }
    }

    Mat comps[]= {tmp,tmp};
    merge(comps, 2, filtro);
    normalize(tmp,tmp,0,1, NORM_MINMAX);
    imshow("filtro", tmp);
    return filtro;
}

Mat filtra(){
    Mat filter, complexImage;
    Mat_<float> realInput, zeros;
    vector<Mat> planos; 

    // identifica os tamanhos otimos para
    // calculo do FFT
    int dft_M = getOptimalDFTSize(image.rows);
    int dft_N = getOptimalDFTSize(image.cols);

    // realiza o padding da imagem
  copyMakeBorder(image, padded, 0, dft_M - image.rows, 0,
                     dft_N - image.cols, BORDER_CONSTANT,
                     Scalar::all(0));

  // parte imaginaria da matriz complexa (preenchida com zeros)
  zeros = Mat_<float>::zeros(padded.size());
  // prepara a matriz complexa para ser preenchida
  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  filter = complexImage.clone();

  Mat comps[]={Mat(dft_M, dft_N, CV_32F), Mat(dft_M, dft_N, CV_32F)};
  merge(comps,2, filter);

  // limpa o array de matrizes que vao compor a
    // imagem complexa
    planos.clear();
    // cria a compoente real
    realInput = Mat_<float>(padded);
    
    realInput += Scalar::all(1);
    log(realInput, realInput);

    // insere as duas componentes no array de matrizes
    planos.push_back(realInput);
    planos.push_back(zeros);

    // combina o array de matrizes em uma unica
    // componente complexa
    merge(planos, complexImage);

    // calcula o dft
    dft(complexImage, complexImage);
    // realiza a troca de quadrantes
    deslocaDFT(complexImage);

    resize(complexImage, complexImage, padded.size());
    normalize(complexImage, complexImage, 0, 1 , NORM_MINMAX);

    filter = filtroHomomorfico(gammaH, gammaL, c, d0);

    mulSpectrums(complexImage, filter, complexImage,0); // filtro frequencial
    deslocaDFT(complexImage);
    idft(complexImage, complexImage);

    planos.clear();
    //separa parte real e imaginaria
    split(complexImage, planos);
    exp(planos[0], planos[0]);
    planos[0] -= Scalar::all(1);

    //normalizar parte real para exibir
    normalize(planos[0], planos[0], 0, 1 , NORM_MINMAX);

    return planos[0];

}

void on_trackbar(int, void*) {
    gammaH = (double)gammaH_slider/10.0;
    gammaL = (double)gammaL_slider/10.0;
    c = (double)c_slider;
    d0 = (double)d0_slider;

    if(d0 < 1.0) d0 =1.0;
    if(c < 1.0) c = 1.0;

    filtrada = filtra();  
    imshow("UIFiltro", filtrada);
    normalize(filtrada, filtrada, 0, 255, NORM_MINMAX);
    imwrite("saida.png", filtrada);
}


int main(int, char**) {
  char TrackbarName[50];

  vector<String> end;
  glob("D:/UFRN/22.1/PDI/homomorfico/estacao.jpg", end, false);
  image = imread(end[0], IMREAD_GRAYSCALE);
  //resize(image, image, Size(516, 688));  // Redimensionamos a imagem por ser muito grande
  imshow("original", image);

  namedWindow("UIFiltro", WINDOW_FREERATIO);

  sprintf(TrackbarName, "gammaH - %d", gammaH_max);
  createTrackbar(TrackbarName, "UIFiltro", &gammaH_slider, gammaH_max, on_trackbar);

  sprintf(TrackbarName, "gammaL - %d", gammaL_max);
  createTrackbar(TrackbarName, "UIFiltro", &gammaL_slider, gammaL_max, on_trackbar);

  sprintf(TrackbarName, "c - %d", c_max);
  createTrackbar(TrackbarName, "UIFiltro", &c_slider, c_max, on_trackbar);

  sprintf(TrackbarName, "d0 - %d", d0_max);
  createTrackbar(TrackbarName, "UIFiltro", &d0_slider,d0_max, on_trackbar);
  waitKey();

  return 0;
}