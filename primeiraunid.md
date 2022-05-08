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






# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [Basic writing and formatting syntax](https://docs.github.com/en/github/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/settings/pages). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://support.github.com/contact) and we’ll help you sort it out.
