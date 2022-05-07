## Processamento Digital de Imagens (PDI)

Esta página se refere aos exercícios da 1º unidade da matéria de [PDI-DCA-UFRN](https://agostinhobritojr.github.io/tutorial/pdi/#_pref%C3%A1cio).

### Exercício Regions

Utilizando o programa exemplos/pixels.cpp como referência, implemente um programa regions.cpp. Esse programa deverá solicitar ao usuário as coordenadas de dois pontos P1 e P2 localizados dentro dos limites do tamanho da imagem e exibir que lhe for fornecida. Entretanto, a região definida pelo retângulo de vértices opostos definidos pelos pontos P1 e P2 será exibida com o negativo da imagem na região correspondente.


```c++

  cv::Mat image;
  image= cv::imread("biel.png",cv::IMREAD_GRAYSCALE);
  if(!image.data)
    std::cout << "nao abriu a img" << std::endl;

    for(int i=p_1;i<p_2;i++){
    for(int j=p_1;j<p_2;j++){
      image.at<uchar>(i,j) = 255 - image.at<uchar>(i,j);
    }
  }
  
  cv::imshow("Janela", image);
  cv::waitKey();
```
Pontos: p_1 = 50 e p_2 = 200

Saída:
![Saida regions](/Processamento-Digital-de-Imagens/Regions/biel_negativo.png)



Syntax highlighted code block

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
