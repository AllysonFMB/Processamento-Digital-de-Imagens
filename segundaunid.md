## Processamento Digital de Imagens (PDI)

Esta página se refere aos exercícios da 2º unidade da matéria de [PDI-DCA-UFRN](https://agostinhobritojr.github.io/tutorial/pdi/#_pref%C3%A1cio).

### 1. Filtro homomórfico

Utilizando o programa exemplos/dft.cpp como referência, implemente o filtro homomórfico para melhorar imagens com iluminação irregular. Crie uma cena mal iluminada e ajuste os parâmetros do filtro homomórfico para corrigir a iluminação da melhor forma possível. Assuma que a imagem fornecida é em tons de cinza.

#### [homomorgico.cpp](https://github.com/AllysonFMB/Processamento-Digital-de-Imagens/blob/gh-pages/Regions/main.cpp)

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
