# Processamento Digital de Imagens com Opencv
## Tutoriais realizados durante a disciplina de processamento digital de imagens

### Primeiro Tutorial

O primeiro tutorial realizado se resume a escolher uma região delimitada por dois pontos e torna-la negativa. Isso significa inverter a tonalidade de cinza da imagem.
Para que isso seja feito, é necessário ler os pontos escolhidos, e através de um duplo-for percorrer os pixels da região, trocando a tonalidade dos mesmos por seu negativo. Lembrando que o negativo de um tom X é dado por 255 - X.


{% highlight c++ %}
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
{% endhighlight %}


Escolhendo a coordenada P1 como (70,70) e a coordenada P2 como (300,300), obtivemos:


![negativo](/img/regions.png)
###### Figura 1: Negativo da imagem.

### Segundo Tutorial

O segundo tutorial se trata da troca do primeio quadrante com o tercediro quadrante da imagem, e do segundo quadrante com o quarto quadrante.
Para que isso seja feito, de forma simples, basta varrer todos os pixels da imagem e verificar se ele esta no primeiro ou segundo quadrante da imagem. Possuindo essa informação, precisamos fazer a troca com o pixel que se encontra na exata posição do quadrante correspondente. Por exemplo: Se um pixel P com coordenadas (x,y) se encontra no primeiro quadrante, o pixel correspondente do terceiro quadrante será (x - (tamanhodaimagem/2) , y + (tamanhodaimagem/2)). Fazemos isso para todos os pixels da imagem utilizando o seguinte código:

{% highlight c++ %}
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
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
{% endhighlight %}

E conseguiremos como resultado: 


![troca de quadrantes](/img/trocaregioes.png)
###### Figura 2: Troca de quadrantes.

### Terceiro Tutorial

O terceiro tutorial tem como objetivo introduzir a contagem e a identificação de regiões na imagem. Foi feito um exemplo em sala de aula onde, por meio de um algoritmo, foram colocados labels na seguinte imagem:

![bolhas](/img/bolhas.png)
###### Figura 3: Bolhas.

O resultado obtido foi:

![labels](/img/labeling.png)
###### Figura 4: Labels.

Como podemos observar, cada região com cor branca foi pintada com um tom de cinza diferente, representando os rótulos. Porém existe um problema. Se a imagem possuir mais de 255 regiões, não existirão tons de cinzas suficientes para rotular cada uma dessas regiões, faz-se necessário o uso de uma tecnica diferente para de rotular as bolhas. Uma possivel solução seria usar tons RGB, assim poderemos ter 255x255x255 opções de cores distintas.

Um exercicio de aprimoramento foi proposto. O objetivo, agora, é que o algoritmo possa reconhecer regiões com buracos internos, não levando em conta as bolhas que tocam a borda da imagem. 

Para que esse objetivo fosse conquistado, foi necessário seguir os seguintes passos:
* Remover as bolhas que tocam as bordas da imagem.
* Fazer a busca das regiões (bolhas) e rotula-las com um tom de cinza especifico.
* Pintamos o fundo da imagem de branco para poder fazer a varredura novamente, mas agora buscando pixels pretos(0), pois esses pixels indicarão a presença de buracos nas bolhas.
* Fazer a varredura da imagem para encontrar as regiões com buracos e em seguida apagar essas regiões da imagem.

O algoritmo desenvolvido para a resolução desse problema e o resultado, respectivamente:

{% highlight c++ %}
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    cv::Mat image, realce;
    int width, height;
    int nobjects;
    int nburacos;

    cv::Point p;
    image = cv::imread("C:\\opencv\\sources\\samples\\data\\bolhas.png", cv::IMREAD_GRAYSCALE);

    if (!image.data) {
        std::cout << "imagem nao carregou corretamente\n";
        return(-1);
    }

    width = image.cols;
    height = image.rows;
    std::cout << width << "x" << height << std::endl;

    p.x = 0;
    p.y = 0;

    // removendo bolhas que tocam a borda de cima
    for (int j = 0; j < width; j++) {
       if (image.at<uchar>(0, j) == 255) {
            p.x = j;
            p.y = 0;
            // remove o objeto
            cv::floodFill(image, p, 0);
       }
    }

    // removendo bolhas que tocam a borda de baixo
    for (int j = 0; j < width; j++) {
        if (image.at<uchar>(height-1, j) == 255) {
            p.x = j;
            p.y = height - 1;
            // remove o objeto
            cv::floodFill(image, p, 0);
        }
    }

    // removendo bolhas que tocam a borda da esquerda
    for (int i = 0; i < height; i++) {
        if (image.at<uchar>(i, 0) == 255) {
            p.x = 0;
            p.y = i;
            // remove o objeto
            cv::floodFill(image, p, 0);
        }
    }

    // removendo bolhas que tocam a borda da direita
    for (int i = 0; i < height; i++) {
        if (image.at<uchar>(i, width - 1) == 255) {
            p.x = width - 1;
            p.y = i;
            // remove o objeto
            cv::floodFill(image, p, 0);
        }
    }

    // busca objetos presentes
    nobjects = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 255) {
                // achou um objeto
                nobjects++;
                p.x = j;
                p.y = i;
                // preenche o objeto com o contador
                cv::floodFill(image, p, nobjects);
            }
        }
    }
    
    //Em seguida pintamos o fundo de branco
    p.x = 0;
    p.y = 0;
    cv::floodFill(image, p, 255);

   
    // busca objetos com buracos
    nburacos = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 0) {
                // achou um objeto com buraco
                nburacos++;

                //pintamos a regiao de branco
                p.x = j;
                p.y = i;
                cv::floodFill(image, p, 255);
                p.x = j - 1;
                p.y = i;
                cv::floodFill(image, p, 255);
            }
        }
    }
 
    std::cout << "A figura tem " << nobjects << " bolhas\n";
    std::cout << "Dessas bolhas " << nburacos << " possuiam buracos\n";
    cv::imshow("image", image);
    cv::imwrite("labeling.png", image);
    cv::waitKey();
    return 0;
}
{% endhighlight %}

![bolhas sem buracos](/img/bolhasburacos.png) 
###### Figura 5: Bolhas sem buracos.

### Quarto Tutorial

No quarto tutorial foi pedido que utilizassemos a equalização para observar a difereça e o comportamento resultante nos histogramas da imagem.
O código utilizado para a criação do histograma equalizado é mostrado a seguir.
    
{% highlight c++ %}
#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
    cv::Mat image;
    int width, height;
    cv::VideoCapture cap;
    std::vector<cv::Mat> planes;
    cv::Mat hist;
    int nbins = 64;
    float range[] = { 0, 255 };
    const float* histrange = { range };
    bool uniform = true;
    bool acummulate = true;
    int key;

    cap.open(1);

    if (!cap.isOpened()) {
        std::cout << "cameras indisponiveis";
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "largura = " << width << std::endl;
    std::cout << "altura  = " << height << std::endl;

    int histw = nbins, histh = nbins / 2;
    cv::Mat histImg(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));

    while (1) {
        cap >> image;
        cv::split(image, planes);
        cv::calcHist(&planes[0], 1, 0, cv::Mat(), hist, 1, &nbins, &histrange, uniform, acummulate);

        cv::normalize(hist, hist, 0, histImg.rows, cv::NORM_MINMAX, -1, cv::Mat());
    
        histImg.setTo(cv::Scalar(0));

        for (int i = 0; i < nbins; i++) {
            cv::line(histImg,
                cv::Point(i, histh),
                cv::Point(i, histh - cvRound(hist.at<float>(i))),
                cv::Scalar(0, 0, 255), 1, 8, 0);
        }
        histImg.copyTo(image(cv::Rect(0, 0, nbins, histh)));
        cv::imshow("image", image);
        key = cv::waitKey(30);
        if (key == 27) break;
    }
    return 0;
}
{% endhighlight %}

Em seguida, temos como resultado a imagem e seu histograma.
    
![imagem equalizada](/img/equalizada.png)
###### Figura 6: Histograma equalizado.

Agora, também utilizando o histograma, foi desenvolvido um motion detector. 
Para desenvolver esse sistema, foi necessário criar dois histogramas, um original e outro que armazena o histograma anterior na escala de tempo. E ao utilizarmos a função compareHist() do OpenCV, obtemos a diferença entre os dois histogramas. Caso a diferença seja maior que um limite pré-estabelecido, será um indicativo que houve um movimento na imagem. 
Para indicar o movimento, foi criado um retangulo que possui cor vermelha quando não existe movimento na imagem, e verde quando alguma diferença significativa nos histogramas é detectada.

O código em C++ é mostrado a seguir.

{% highlight c++ %}
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdlib>
#include <cmath>

int main() {
    cv::Mat image;
    int width, height;
    cv::VideoCapture cap;
    std::vector<cv::Mat> planes;
    cv::Mat hist;
    int nbins = 64;
    float range[] = { 0, 255 };
    const float* histrange = { range };
    bool uniform = true;
    bool acummulate = false;
    int key;
    cap.open(1);

    if (!cap.isOpened()) {
        std::cout << "cameras indisponiveis";
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "largura = " << width << std::endl;
    std::cout << "altura  = " << height << std::endl;

    int histw = nbins, histh = nbins / 2;
    cv::Mat histImg(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat led(histh, histw, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat hist2;
    cap >> image;

    cv::split(image, planes);
    cv::calcHist(&planes[0], 1, 0, cv::Mat(), hist2, 1, &nbins, &histrange, uniform, acummulate);
    cv::normalize(hist2, hist2, 0, histImg.rows, cv::NORM_MINMAX, -1, cv::Mat());
    

    while (1) {
        cap >> image;
        cv::split(image, planes);
        cv::calcHist(&planes[0], 1, 0, cv::Mat(), hist, 1, &nbins, &histrange, uniform, acummulate);
        cv::normalize(hist, hist, 0, histImg.rows, cv::NORM_MINMAX, -1, cv::Mat());
        histImg.setTo(cv::Scalar(0));

        led.setTo(cv::Scalar(0, 0, 255));
        led.copyTo(image(cv::Rect(0, histh, nbins, histh)));

        double dif = cv::compareHist(hist, hist2, 0);
        std::cout << dif << std::endl;

        if (dif < 0.995) {
            led.setTo(cv::Scalar(0, 255, 0));
            led.copyTo(image(cv::Rect(0, histh, nbins, histh)));
            std::cout << abs(dif) << std::endl;
        }

        cv::calcHist(&planes[0], 1, 0, cv::Mat(), hist2, 1, &nbins, &histrange, uniform, acummulate);
        cv::normalize(hist2, hist2, 0, histImg.rows, cv::NORM_MINMAX, -1, cv::Mat());

        for (int i = 0; i < nbins; i++) {
            cv::line(histImg,
                cv::Point(i, histh),
                cv::Point(i, histh - cvRound(hist.at<float>(i))),
                cv::Scalar(255, 0, 0), 1, 8, 0);
        }
        histImg.copyTo(image(cv::Rect(0, 0, nbins, histh)));
        cv::imshow("image", image);
        key = cv::waitKey(30);
        if (key == 27) break;
    }
    return 0;
}
{% endhighlight %}

A seguir, esta a imagem referente a ausencia de movimento e a imagem no instante do movimento.
    
![imagem equalizada](/img/nomove.png)
###### Figura 7: Imagem quando não há movimento.
    
![imagem equalizada](/img/move.png)
###### Figura 8: Motion detector quando há movimento.
    
### Quinto Tutorial

    




