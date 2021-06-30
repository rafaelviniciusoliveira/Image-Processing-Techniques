# Processamento Digital de Imagens com Opencv
## Tutoriais realizados durante a disciplina de processamento digital de imagens

### Primeiro Tutorial

O primeiro tutorial realizado se resume a escolher uma região delimitada por dois pontos e torna-la negativa. Isso significa inverter a tonalidade de cinza da imagem.
Para que isso seja feito, é necessário ler os pontos escolhidos, e através de um duplo-for percorrer os pixels da região, trocando a tonalidade dos mesmos por seu negativo. Lembrando que o negativo de um tom X é dado por 255 - X.

```C++
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**) {
    cv::Mat image;

    int X1, Y1, X2, Y2;

    image = cv::imread("C:\\opencv\\sources\\samples\\data\\lena.jpg", cv::IMREAD_GRAYSCALE);
    if (!image.data)
        cout << "nao abriu lena.png" << endl;

    cout << "Escolha a primeira coordenada:" << endl;
    cin >> X1;
    cin >> Y1;

    cout << "Escolha a segunda coordenada:" << endl;
    cin >> X2;
    cin >> Y2;

    cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);

    for (int i = X1; i < X2; i++) {
        for (int j = Y1; j < Y2; j++) {
            image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
        }
    }


    cv::imshow("janela", image);
    cv::waitKey();

    return 0;
}
```

Escolhendo a coordenada P1 como (70,70) e a coordenada P2 como (300,300), obtivemos:


![negativo](https://github.com/rafaelviniciusoliveira/tutoriais-pdi/blob/main/img/regions.png)

### Segundo Tutorial

O segundo tutorial se trata da troca do primeio quadrante com o tercediro quadrante da imagem, e do segundo quadrante com o quarto quadrante.
Para que isso seja feito, de forma simples, basta varrer todos os pixels da imagem e verificar se ele esta no primeiro ou segundo quadrante da imagem. Possuindo essa informação, precisamos fazer a troca com o pixel que se encontra na exata posição do quadrante correspondente. Por exemplo: Se um pixel P com coordenadas (x,y) se encontra no primeiro quadrante, o pixel correspondente do terceiro quadrante será (x - (tamanhodaimagem/2) , y + (tamanhodaimagem/2)). Fazemos isso para todos os pixels da imagem utilizando o seguinte código:

```C++
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**) {
    cv::Mat image;

    uchar aux1;
    uchar aux2;

    image = cv::imread("C:\\opencv\\sources\\samples\\data\\lena.jpg", cv::IMREAD_GRAYSCALE);
    if (!image.data)
        cout << "nao abriu lena.png" << endl;

    cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);

    for (int i = 0; i < image.size().width; i++) {
        for (int j = 0; j < image.size().height; j++) {

            if (i < (image.size().width) / 2 && j < (image.size().height) / 2) {
                aux1 = image.at<uchar>(i, j);
                image.at<uchar>(i, j) = image.at<uchar>(i + (image.size().width) / 2, j + (image.size().height) / 2);
                image.at<uchar>(i + (image.size().width) / 2, j + (image.size().height) / 2) = aux1;
            }
            else if (i > (image.size().width) / 2 && j < (image.size().height) / 2) {
                aux1 = image.at<uchar>(i, j);
                image.at<uchar>(i, j) = image.at<uchar>(i - (image.size().width) / 2, j + (image.size().height) / 2);
                image.at<uchar>(i - (image.size().width) / 2, j + (image.size().height) / 2) = aux1;
            }

        }
    }


    cv::imshow("janela", image);
    cv::waitKey();

    return 0;
}
```

E conseguiremos como resultado: 


![troca de quadrantes](https://github.com/rafaelviniciusoliveira/tutoriais-pdi/blob/main/img/trocaregioes.png) 

### Terceiro Tutorial

O terceiro tutorial tem como objetivo introduzir a contagem e a identificação de regiões na imagem. Foi feito um exemplo em sala de aula onde, por meio de um algoritmo, foram colocados labels na seguinte imagem:

![bolhas](https://github.com/rafaelviniciusoliveira/tutoriais-pdi/blob/main/img/bolhas.png) 

O resultado obtido foi:

![labels](https://github.com/rafaelviniciusoliveira/tutoriais-pdi/blob/main/img/labeling.png)

Como podemos observar, cada região com cor branca foi pintada com um tom de cinza diferente, representando os rótulos. Porém existe um problema. Se a imagem possuir mais de 255 regiões, não existirão tons de cinzas suficientes para rotular cada uma dessas regiões, faz-se necessário o uso de uma tecnica diferente para de rotular as bolhas. Uma possivel solução seria usar tons RGB, assim poderemos ter 255x255x255 opções de cores distintas.

Um exercicio de aprimoramento foi proposto. O objetivo, agora, é que o algoritmo possa reconhecer regiões com buracos internos, não levando em conta as bolhas que tocam a borda da imagem.




