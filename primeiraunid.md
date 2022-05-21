## Processamento Digital de Imagens (PDI)

Esta página se refere aos exercícios da 1º unidade da matéria de [PDI-DCA-UFRN](https://agostinhobritojr.github.io/tutorial/pdi/#_pref%C3%A1cio).

### 1. Exercício Regions

Utilizando o programa exemplos/pixels.cpp como referência, implemente um programa regions.cpp. Esse programa deverá solicitar ao usuário as coordenadas de dois pontos P1 e P2 localizados dentro dos limites do tamanho da imagem e exibir que lhe for fornecida. Entretanto, a região definida pelo retângulo de vértices opostos definidos pelos pontos P1 e P2 será exibida com o negativo da imagem na região correspondente.

#### [regions.cpp](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Regions/main.cpp)

```c++ 

  cv::Mat image;

  image = cv::imread("D:/UFRN/22.1/PDI/img/biel.png", cv::IMREAD_GRAYSCALE);
  if (!image.data)
    std::cout << "nao abriu a img" << std::endl;

  width = image.cols; //largura
  height = image.rows; //altura

  if (p_1 >= height) p_1 = height;
  if (p_2 >= width) p_2 = width;

  for (int i = p_1; i < p_2; i++)
  {
    for (int j = p_1; j < p_2; j++)
    {
      image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
    }
  }
 
  cv::imshow("Janela", image);
  cv::waitKey();
```
Pontos: p_1 = 20 e p_2 = 200

Saída:

![Saida do alogitmo regions, a imagem com uma área negativa informada pelo o usuário](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Regions/biel_negativo.png)

### 1.1 Exercício Troca de regiões

Utilizando o programa exemplos/pixels.cpp como referência, implemente um programa trocaregioes.cpp. Seu programa deverá trocar os quadrantes em diagonal na imagem. Explore o uso da classe Mat e seus construtores para criar as regiões que serão trocadas.

#### [trocaregioes.cpp](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Trocaregioes/main.cpp)
```c++
cv::Mat image, image_aux;
    int width, height;

    image = cv::imread("biel.png", cv::IMREAD_GRAYSCALE);
    if (!image.data)
        std::cout << "nao abriu a img" << std::endl;

    cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);
    
    width = image.cols; //largura
    height = image.rows; //altura

    //image_aux = rect1
    image(Rect(0, 0, width / 2, height / 2)).copyTo(image_aux);    
    
    //rect1 = rect4
    image(Rect(height / 2, width / 2, width / 2, height / 2)).copyTo(image(Rect(0, 0, width / 2, height / 2))); 
    
    //rect4 = image_aux
    image_aux.copyTo(image(Rect(height / 2, width / 2, width / 2, height / 2)));                                
    
    //image_aux = rect2
    image(Rect(0, width / 2, width / 2, height / 2)).copyTo(image_aux);    
    
    //rect2 = rect3                                    
    image(Rect(height / 2, 0, width / 2, height / 2)).copyTo(image(Rect(0, width / 2, width / 2, height / 2))); 
    
    //rect3 = image_aux
    image_aux.copyTo(image(Rect(height / 2, 0, width / 2, height / 2)));                                        
    
    cv::imshow("janela", image);
    cv::waitKey();
    return 0;

```
Etapas das trocas de regiões:

![Etapas das trocas de regiões da imagem](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Trocaregioes/biel_final.png)

Saída:

![Saída da troca de regioes](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Trocaregioes/biel_6.png)


### 2. Exercício contar bolhas e buracos

O programa labeling.cpp tem como limitação se existir mais de 255 objetos em cena, para solucionar esse possível problema pode se alterar o tipo da variável de rotulagem (newVal) para ponto flutuante.

```c++
int cv::floodFill(cv::InputOutputArray image, cv::Point seedPoint, cv::Scalar newVal);
```
Aprimore o algoritmo de contagem apresentado para identificar regiões com ou sem buracos internos que existam na cena. Assuma que objetos com mais de um buraco podem existir. Inclua suporte no seu algoritmo para não contar bolhas que tocam as bordas da imagem. Não se pode presumir, a priori, que elas tenham buracos ou não.

#### [labeling.cpp](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Labeling/main.cpp)
```c++
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
```
Etapas de contagem:

![Etapas do funcionamento do código](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Labeling/labeling_final.png)

Saída:

bolhas: 21 e bolhas com buracos: 7

### 3. Equalize

Utilizando o programa exemplos/histogram.cpp como referência, implemente um programa equalize.cpp. Este deverá, para cada imagem capturada, realizar a equalização do histogram antes de exibir a imagem. Teste sua implementação apontando a câmera para ambientes com iluminações variadas e observando o efeito gerado. Assuma que as imagens processadas serão em tons de cinza.

#### [equalize.cpp](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Equalize/main.cpp)
```c++
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
```

saída: 

![Imagem equalizada e normal](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Equalize/saidaequalize.png)

### 3.1 Motion detector

Utilizando o programa exemplos/histogram.cpp como referência, implemente um programa motiondetector.cpp. Este deverá continuamente calcular o histograma da imagem (apenas uma componente de cor é suficiente) e compará-lo com o último histograma calculado. Quando a diferença entre estes ultrapassar um limiar pré-estabelecido, ative um alarme. Utilize uma função de comparação que julgar conveniente.

O algoritmo abaixo é composto principalmente pela função de criar um histograma, utilizada para criar os histogramas que irão ser comparados, o método de comporação foi utilizando a função:

```c++
double cv::compareHist(InputArray H1, InputArray H2, int método)	
```
Onde retorna o valor da comparação, as variáveis H1 e H2 são os histogramas para serem comparados, e por fim, o método de comparação, que podem ser das seguintes formas:

  - cv::HISTCMP_CORREL = 0,
  - cv::HISTCMP_CHISQR = 1,
  - cv::HISTCMP_INTERSECT = 2,
  - cv::HISTCMP_BHATTACHARYYA = 3,
  - cv::HISTCMP_HELLINGER = HISTCMP_BHATTACHARYYA,
  - cv::HISTCMP_CHISQR_ALT = 4,
  - cv ::HISTCMP_CHISQR_ALT = 4: HITCMP_KL_DIV = 5


### [motiondetector.cpp](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Motiondetector/main.cpp)
```c++
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
```

Saída:

![frame do vídeo de saída para detecção de movimento](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Motiondetector/saida_videodetect.png)

### 4. Filtro espacial

Utilizando o programa exemplos/filtroespacial.cpp como referência, implemente um programa laplgauss.cpp. O programa deverá acrescentar mais uma funcionalidade ao exemplo fornecido, permitindo que seja calculado o laplaciano do gaussiano das imagens capturadas. Compare o resultado desse filtro com a simples aplicação do filtro laplaciano.

Foi utilizado como referência para essa atividade o arquivo http://www.ic.uff.br/~aconci/gabor.pdf para aplicação da máscara de Laplaciano de Gaussiano. Foi utilizada uma máscara 5 x 5 :

```c++
  float laplaciangauss[]={0, 0, -1, 0, 0,
                         0, -1, -2, -1, 0,
                         -1, -2, 16, -2, 1,
                         0, -1, -2, -1, 0,
                         0, 0, -1, 0, 0};
```
e adicionado ao exemplo do filtroesapcial.cpp original a matriz e mais um case para aplicar a nova máscara:
```c++
case 'k':
  mask = cv::Mat(5, 5, CV_32F, laplaciangauss);
  printmask(mask);   
  break;
```

### [Filtroespacial.cpp](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Filtroespacial/main.cpp)
```c++

void printmask(cv::Mat &m) {
  for (int i = 0; i < m.size().height; i++) {
    for (int j = 0; j < m.size().width; j++) {
      std::cout << m.at<float>(i, j) << ",";
    }
    std::cout << "\n";
  }
}

int main(int, char **) {
  cv::VideoCapture cap;  // open the default camera
  float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                   0.1111, 0.1111, 0.1111, 0.1111};
  float gauss[] = {0.0625, 0.125,  0.0625, 0.125, 0.25,
                   0.125,  0.0625, 0.125,  0.0625};
  float horizontal[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  float vertical[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
  float laplacian[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
  float boost[] = {0, -1, 0, -1, 5.2, -1, 0, -1, 0};
  float laplaciangauss[]={0, 0, -1, 0, 0,
                         0, -1, -2, -1, 0,
                         -1, -2, 16, -2, 1,
                         0, -1, -2, -1, 0,
                         0, 0, -1, 0, 0};

  cv::Mat frame, framegray, frame32f, frameFiltered;
  cv::Mat mask(3, 3, CV_32F);
  cv::Mat result;
  double width, height;
  int absolut;
  char key;

  cap.open(0);

  if (!cap.isOpened())  // check if we succeeded
    return -1;

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
  std::cout << "largura=" << width << "\n";
  ;
  std::cout << "altura =" << height << "\n";
  ;
  std::cout << "fps    =" << cap.get(cv::CAP_PROP_FPS) << "\n";
  std::cout << "format =" << cap.get(cv::CAP_PROP_FORMAT) << "\n";

  cv::namedWindow("filtroespacial", cv::WINDOW_NORMAL);
  cv::namedWindow("original", cv::WINDOW_NORMAL);

  mask = cv::Mat(3, 3, CV_32F, media);

  absolut = 1;  // calcs abs of the image

  for (;;) {
    cap >> frame;  // get a new frame from camera
    cv::cvtColor(frame, framegray, cv::COLOR_BGR2GRAY);
    cv::flip(framegray, framegray, 1);
    cv::imshow("original", framegray);
    framegray.convertTo(frame32f, CV_32F);
    cv::filter2D(frame32f, frameFiltered, frame32f.depth(), mask,
                 cv::Point(1, 1), 0);
    if (absolut) {
      frameFiltered = cv::abs(frameFiltered);
    }

    frameFiltered.convertTo(result, CV_8U);

    cv::imshow("filtroespacial", result);

    key = (char)cv::waitKey(10);
    if (key == 27) break;  // esc pressed!
    switch (key) {
      case 'a':
        absolut = !absolut;
        break;
      case 'm':
        mask = cv::Mat(3, 3, CV_32F, media);
        printmask(mask);
        break;
      case 'g':
        mask = cv::Mat(3, 3, CV_32F, gauss);
        printmask(mask);
        break;
      case 'h':
        mask = cv::Mat(3, 3, CV_32F, horizontal);
        printmask(mask);
        break;
      case 'v':
        mask = cv::Mat(3, 3, CV_32F, vertical);
        printmask(mask);
        break;
      case 'l':
        mask = cv::Mat(3, 3, CV_32F, laplacian);
        printmask(mask);
        break;
      case 'b':
        mask = cv::Mat(3, 3, CV_32F, boost);
        break;
      case 'k':
        mask = cv::Mat(5, 5, CV_32F, laplaciangauss);
        printmask(mask);   
        break;
        
      default:
        break;
    }
  }
  return 0;
}
```

Saída:

Máscara de Gauss:
![imagem com a mascara de Gauss](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Filtroespacial/gauss.png)

Máscara do Laplaciano da Gaussiano:
![imagem com a mascara de Laplaciana de Gauss](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Filtroespacial/laplaciangauss.png)

A diferença entre elas consiste em que a Laplaciano da Gaussiana calcula as derivadas segundas de uma imagem, áreas com intesidadde constantes terão valores igual a zero e a sua vizinhaça terá como mudança a sua intensidade, a resposta será positiva no lado mais escuro e negativa no lado mais claro, realizando assim um aumento de constraste.

### Tiltishift

Utilizando o programa exemplos/addweighted.cpp como referência, implemente um programa tiltshift.cpp. Três ajustes deverão ser providos na tela da interface:
- um ajuste para regular a altura da região central que entrará em foco;
- um ajuste para regular a força de decaimento da região borrada;
- um ajuste para regular a posição vertical do centro da região que entrará em foco. Finalizado o programa, a imagem produzida deverá ser salva em arquivo.

### [tiltishift.cpp](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/tree/gh-pages/Tiltshift/main.cpp)
```c++

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
  scaleAdd(mascara, 1/16.0, cv::Mat::zeros(5,5,CV_32F), mascara);

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

```

Imagem original:
![imagem da praia original](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Tiltshift/original.jpg)

Interface:
![interface dos trackbar](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Tiltshift/interface.png)

Saída:
![imagem da praia saida](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Tiltshift/tiltyshift.jpg)






