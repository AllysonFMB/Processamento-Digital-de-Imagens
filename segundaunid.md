## Processamento Digital de Imagens (PDI)

Esta página se refere aos exercícios da 2º unidade da matéria de [PDI-DCA-UFRN](https://agostinhobritojr.github.io/tutorial/pdi/#_pref%C3%A1cio).

### 1. Filtro homomórfico

Utilizando o programa exemplos/dft.cpp como referência, implemente o filtro homomórfico para melhorar imagens com iluminação irregular. Crie uma cena mal iluminada e ajuste os parâmetros do filtro homomórfico para corrigir a iluminação da melhor forma possível. Assuma que a imagem fornecida é em tons de cinza.

#### [homomorgico.cpp](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Homomorfico/main.cpp)

- Função filtroHomomorfico, ela recebe os dados de Gamma L, Gamma H, C e o D0:

```c++
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

```
- Função filtra, tem como função realizar a dft e a filtragem:

```c++
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
```
- Função on_trackbar, variação dos sliders:

```c++
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
```
Entrada:

![imagem original estação de trem](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Homomorfico/estacao.jpg)

Realizando o filtro:

![Aplicação do filtro, dela da interface do usuário modificando os sliders, tela da imagem original e a tela do filtro aplicado](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Homomorfico/filtrando.png)

Saída:

![imagem com aplicação do filtro](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Homomorfico/saida.png)

### 2. Cannypoints

Utilizando os programas exemplos/canny.cpp e exemplos/pontilhismo.cpp como referência, implemente um programa cannypoints.cpp. A idéia é usar as bordas produzidas pelo algoritmo de Canny para melhorar a qualidade da imagem pontilhista gerada. A forma como a informação de borda será usada é livre.

#### [cannypoints.cpp]
```c++
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

  Canny(image_bw, border, limite_inferior, 3 * limite_inferior);
  imshow("bordas_canny", border);
  imwrite("canny.png", border);
  waitKey();

  xrange.resize(height / STEP);
  yrange.resize(width / STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  //amostragem dos pontos
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

      //limite da imagem
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

  //Pontos pequenos por Canny
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (border.at<uchar>(i, j) != 0)
      {
        //Armazenar cor original e posição
        gray = image.at<Vec3b>(i, j);
        pontos.push_back(Vec6i(j, i, gray[0], gray[1], gray[2], 0));
      }
    }
  }

  random_shuffle(pontos.begin(), pontos.end());

  //pontos pequenos
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

  imshow("cannypoints", points);
  waitKey();

  imwrite("cannypoints.png", points);
  return 0;
}
```

Adaptação do algoritmo pontilhismo:

1º É aplicado o algoritmo de Canny na imagem
```c++
Canny(image_bw, border, limite_inferior, 3 * limite_inferior);
```

2º Armazena a cor original e a posição
```c++
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      if (border.at<uchar>(i, j) != 0)
      {
        //Armazenar cor original e posição
        gray = image.at<Vec3b>(i, j);
        pontos.push_back(Vec6i(j, i, gray[0], gray[1], gray[2], 0));
      }
    }
  }
```

3º Desenhar o círculos pequenos com os pontos obtidos das bordas de Canny
```c++
  //pontos pequenos
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
```

Entrada:

![Imagem da Série Love Death + Robots]

Saída:


### 3. Kmeans

Utilizando o programa kmeans.cpp como exemplo prepare um programa exemplo onde a execução do código se dê usando o parâmetro nRodadas=1 e inciar os centros de forma aleatória usando o parâmetro KMEANS_RANDOM_CENTERS ao invés de KMEANS_PP_CENTERS. Realize 10 rodadas diferentes do algoritmo e compare as imagens produzidas. Explique porque elas podem diferir tanto.

O kmeans é executado da seguinte forma:

A matriz com as amostras samples deve conter em cada linha uma das amostras a ser processada pela função disponível pelo opencv. nClusters informa a quantidade de aglomerados que se deseja obter, no nosso caso 8. A matriz rotulos é um objeto do tipo Mat preenchido com elementos do tipo int, onde cada elemento identifica a classe à qual pertence a amostra na matriz samples.

No exemplo, um máximo de até 10000 iterações ou tolerância de 0.0001 devem ser atingidos para finalizar o algoritmo. O algoritmo é repetido por uma quantidade de vezes definida por nRodadas (foi utilizada nRodadas = 1). A rodada que produz a menor soma de distâncias dos pontos para seus respectivos centros é escolhida como vencedora. Foi utilizada a inicialização dos centros de forma aleatória com KMEANS_RANDOM_CENTERS.

#### [kmeans.cpp](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/tree/gh-pages/kmeans/main.cpp)
```c++
for (int i = 0; i < 10; i++)
  {

    kmeans(samples,
           nClusters,
           rotulos,
           TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10000, 0.0001),
           nRodadas,
           KMEANS_RANDOM_CENTERS,
           centros);

    Mat rotulada(img.size(), img.type());
    for (int y = 0; y < img.rows; y++)
    {
      for (int x = 0; x < img.cols; x++)
      {
        int indice = rotulos.at<int>(y + x * img.rows, 0);
        rotulada.at<Vec3b>(y, x)[0] = (uchar)centros.at<float>(indice, 0);
        rotulada.at<Vec3b>(y, x)[1] = (uchar)centros.at<float>(indice, 1);
        rotulada.at<Vec3b>(y, x)[2] = (uchar)centros.at<float>(indice, 2);
      }
    }
    imshow("clustered image", rotulada);
    nome1[13] = i + '0';
    imwrite(nome1, rotulada);
  }
```
Entrada:

![imagem de Rick and Morty](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/kmeans/rickandmorty.png)

Saída: 

![kmeans interações](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/kmeans/rickandmorty_gif.gif)

A cada rodada a imagem apresentam cores diferentes, isso acontece por inicilaizar os pontos do centro de forma aleatória e o algoritmo não estabailiza por usar somente o número de rodadas igual a 1.
