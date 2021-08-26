# Processamento Digital de Imagens com Opencv
## Tutoriais realizados durante a disciplina de processamento digital de imagens

### Negativo da imagem

O primeiro tutorial realizado se resume a escolher uma região delimitada por dois pontos e torna-la negativa. Isso significa inverter a tonalidade de cinza da imagem.
Para que isso seja feito, é necessário ler os pontos escolhidos, e através de um duplo-for percorrer os pixels da região, trocando a tonalidade dos mesmos por seu negativo. Lembrando que o negativo de um tom X é dado por 255 - X.

~~~c++
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
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
~~~


Escolhendo a coordenada P1 como (70,70) e a coordenada P2 como (300,300), obtivemos:


![negativo](/img/regions.png)
###### Figura 1: Negativo da imagem.

### Troca de quadrantes

O segundo tutorial se trata da troca do primeio quadrante com o tercediro quadrante da imagem, e do segundo quadrante com o quarto quadrante.
Para que isso seja feito, de forma simples, basta varrer todos os pixels da imagem e verificar se ele esta no primeiro ou segundo quadrante da imagem. Possuindo essa informação, precisamos fazer a troca com o pixel que se encontra na exata posição do quadrante correspondente. Por exemplo: Se um pixel P com coordenadas (x,y) se encontra no primeiro quadrante, o pixel correspondente do terceiro quadrante será (x - (tamanhodaimagem/2) , y + (tamanhodaimagem/2)). Fazemos isso para todos os pixels da imagem utilizando o seguinte código:

~~~c++
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
~~~

E conseguiremos como resultado: 


![troca de quadrantes](/img/trocaregioes.png)
###### Figura 2: Troca de quadrantes.

### Labeling

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

~~~c++
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
~~~

![bolhas sem buracos](/img/bolhasburacos.png) 
###### Figura 5: Bolhas sem buracos.

### Equalização

No quarto tutorial foi pedido que utilizassemos a equalização para observar a difereça e o comportamento resultante nos histogramas da imagem.
O código utilizado para a criação do histograma equalizado é mostrado a seguir.
    
~~~c++
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
~~~

Em seguida, temos como resultado a imagem e seu histograma.
    
![imagem equalizada](/img/equalizada.png)
###### Figura 6: Histograma equalizado.

### Motion Detector

Agora, também utilizando o histograma, foi desenvolvido um motion detector. 
Para desenvolver esse sistema, foi necessário criar dois histogramas, um original e outro que armazena o histograma anterior na escala de tempo. E ao utilizarmos a função compareHist() do OpenCV, obtemos a diferença entre os dois histogramas. Caso a diferença seja maior que um limite pré-estabelecido, será um indicativo que houve um movimento na imagem. 
Para indicar o movimento, foi criado um retangulo que possui cor vermelha quando não existe movimento na imagem, e verde quando alguma diferença significativa nos histogramas é detectada.

O código em C++ é mostrado a seguir.

~~~c++
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
~~~

A seguir, esta a imagem referente a ausencia de movimento e a imagem no instante do movimento.
    
![imagem equalizada](/img/nomove.png)
###### Figura 7: Imagem quando não há movimento.
    
![imagem equalizada](/img/move.png)
###### Figura 8: Motion detector quando há movimento.
    
### Laplaciano do Gaussiano
O quinto tutorial consiste em implementar o filtro no dominio espacial utilizando o laplaciano do gaussiano.
O código em c++ e o resultado são mostrados abaixo.

~~~c++
#include <iostream>
#include <opencv2/opencv.hpp>

void printmask(cv::Mat& m) {
    for (int i = 0; i < m.size().height; i++) {
        for (int j = 0; j < m.size().width; j++) {
            std::cout << m.at<float>(i, j) << ",";
        }
        std::cout << "\n";
    }
}

int main() {
    cv::VideoCapture cap;  // open the default camera
    float media[] = { 0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                     0.1111, 0.1111, 0.1111, 0.1111 };
    float gauss[] = { 0.0625, 0.125,  0.0625, 0.125, 0.25,
                     0.125,  0.0625, 0.125,  0.0625 };
    float horizontal[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
    float vertical[] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
    float laplacian[] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 };
    float boost[] = { 0, -1, 0, -1, 5.2, -1, 0, -1, 0 };
    float lapofgauss[] = { 0, 0, 1, 0, 0, 0, 1, 2, 1, 0, 1, 2, -16, 2, 1, 0, 1, 2, 1, 0, 0, 0, 1, 0, 0 };

    cv::Mat frame, framegray, frame32f, frameFiltered;
    cv::Mat mask(3, 3, CV_32F);
    cv::Mat result;
    double width, height;
    int absolut;
    char key;

    cap.open(1);

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
        case 'n':
            mask = cv::Mat(5, 5, CV_32F, lapofgauss);
            break;
        default:
            break;
        }
    }
    return 0;
}
~~~   
    
![Resultado do laplaciano do gaussiano](/img/lapgauss.png)
###### Figura 9: Resultado da filtragem utilizando o laplaciano do gaussiano.

### Tilt-Shift
O objetivo do sexto tutorial é simular o efeito do tilt-shift. Primeiramente foi desenvolvido o método para uma imagem.

~~~c++
#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>

double alfa;
int alfa_slider = 0;
int alfa_slider_max = 100;

int top_slider = 0;
int top_slider_max = 100;

int mid_slider = 0;
int mid_slider_max = 100;

cv::Mat image1, image2, blended;
cv::Mat imageTop;

char TrackbarName[50];

void on_trackbar_blend(int, void*) {
    alfa = (double)alfa_slider / alfa_slider_max;
    cv::addWeighted(image2, 1 - alfa, imageTop, alfa, 0.0, blended);
    cv::imshow("addweighted", blended);
}

void on_trackbar_line(int, void*) {
    image2.copyTo(imageTop);
    
    int width = image2.size().width;
    int height = image2.size().height;

    int limit = top_slider * height / 100;
    int centro = mid_slider * height / 100;


    if (limit > 0) {
        if (centro >= 0 && centro <= height - limit) {
            cv::Mat tmp = image1(cv::Rect(0, centro, width, limit));
            tmp.copyTo(imageTop(cv::Rect(0, centro, width, limit)));
        }
    }
    
    on_trackbar_blend(alfa_slider, 0);
}

int main() {
    image1 = cv::imread("C:\\opencv\\sources\\samples\\data\\butterfly.jpg");
    image1.copyTo(image2);
    cv::namedWindow("addweighted", 1);
    
    image2.convertTo(image2, CV_32F);
    float media[] = { 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111 };
    cv::Mat mask;
    mask = cv::Mat(3, 3, CV_32F, media);

    for (int i = 0; i < 15; i++) {
        filter2D(image2, image2, image2.depth(), mask, cv::Point(1, 1), 0);
    }

    image2.convertTo(image2, CV_8U);
    image2.copyTo(imageTop);

    std::sprintf(TrackbarName, "Alpha x %d", alfa_slider_max);
    cv::createTrackbar(TrackbarName, "addweighted",
        &alfa_slider,
        alfa_slider_max,
        on_trackbar_blend);
    on_trackbar_blend(alfa_slider, 0);

    std::sprintf(TrackbarName, "Altura x %d", top_slider_max);
    cv::createTrackbar(TrackbarName, "addweighted",
        &top_slider,
        top_slider_max,
        on_trackbar_line);
    on_trackbar_line(top_slider, 0);

    std::sprintf(TrackbarName, "Mid x %d", top_slider_max);
    cv::createTrackbar(TrackbarName, "addweighted",
        &mid_slider,
        mid_slider_max,
        on_trackbar_line);
    on_trackbar_line(mid_slider, 0);

    cv::waitKey(0);
    imwrite("desfocada.jpg", blended);
    imwrite("original.jpg", image1);
    return 0;
}
~~~

![Imagem original](/img/original.jpg)
###### Figura 10: Imagem original.

![Resultado do tilt-shift](/img/desfocada.jpg)
###### Figura 11: Resultado do tilt-shift.

Em seguida, foi implementado o método de tilt-shift para um vídeo. O código em c++ é mostrado abaixo.
    
~~~c++
#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>

double alfa;
int alfa_slider = 0;
int alfa_slider_max = 100;

int top_slider = 0;
int top_slider_max = 100;

int mid_slider = 0;
int mid_slider_max = 100;

cv::Mat image1, image2, blended;
cv::Mat imageTop;


char TrackbarName[50];

void on_trackbar_blend(int, void*) {
    alfa = (double)alfa_slider / alfa_slider_max;
    cv::addWeighted(image2, 1 - alfa, imageTop, alfa, 0.0, blended);
    cv::imshow("addweighted", blended);
}

void on_trackbar_line(int, void*) {
    image2.copyTo(imageTop);

    int width = image2.size().width;
    int height = image2.size().height;

    int limit = top_slider * height / 100;
    int centro = mid_slider * height / 100;


    if (limit > 0) {
        if (centro >= 0 && centro <= height - limit) {
            cv::Mat tmp = image1(cv::Rect(0, centro, width, limit));
            tmp.copyTo(imageTop(cv::Rect(0, centro, width, limit)));
        }
    }

    on_trackbar_blend(alfa_slider, 0);
}

int main() {
    cv:: VideoCapture cap("C:\\opencv\\sources\\samples\\data\\videotest.mp4");

    if (!cap.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        return 1;
    }

    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    cv::VideoWriter video("outcpp.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, cv::Size(frame_width, frame_height));

    while (1) {
        
        cap >> image1;
        if (image1.empty())
            break;

        image1.copyTo(image2);
        cv::namedWindow("addweighted", 1);
        image2.convertTo(image2, CV_32F);
        float media[] = { 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111 };
        cv::Mat mask;
        mask = cv::Mat(3, 3, CV_32F, media);

        for (int i = 0; i < 30; i++) {
            filter2D(image2, image2, image2.depth(), mask, cv::Point(1, 1), 0);
        }

        image2.convertTo(image2, CV_8U);
        image2.copyTo(imageTop);

        std::sprintf(TrackbarName, "Alpha x %d", alfa_slider_max);
        cv::createTrackbar(TrackbarName, "addweighted",
            &alfa_slider,
            alfa_slider_max,
            on_trackbar_blend);
        on_trackbar_blend(alfa_slider, 0);

        std::sprintf(TrackbarName, "Altura x %d", top_slider_max);
        cv::createTrackbar(TrackbarName, "addweighted",
            &top_slider,
            top_slider_max,
            on_trackbar_line);
        on_trackbar_line(top_slider, 0);

        std::sprintf(TrackbarName, "Mid x %d", top_slider_max);
        cv::createTrackbar(TrackbarName, "addweighted",
            &mid_slider,
            mid_slider_max,
            on_trackbar_line);
        on_trackbar_line(mid_slider, 0);

        video.write(imageTop);

        char c = (char)cv::waitKey(25);
        if (c == 27)
            break;
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
~~~

### Filtro Homomórfico
O objetivo do sétimo tutorial é utilizar o filtro homomórfico para melhorar a iluminação de uma cena. Esse processo é feito atenuando a iluminância.
A equação do filtro é mostrada a seguir:

![Equação do filtro homomórfico](/img/filter.png)

###### Figura 12: Equação do filtro homomórfico

Os parametros utilizados para ajustar a cen são:
- componente de baixa frequencia gl
- componente de alta frequencia gh
- c
- d0

~~~c++
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


//definindo variavais do filtro
double    gh, gl, c, d0;
int       gh_slider = 2, gl_slider = 2, c_slider = 1, d0_slider = 3;
const int gh_max = 10, gl_max = 10, c_max = 100, d0_max = 200;

//imagem original, imagem para o filtro e imagem filtrada, respectivamente
Mat img_original, padded, img_final;


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

    A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);// A <-> D
    C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B); // C <-> B
}

//funcao que cria o filtro
Mat filtro(double gl, double gh, double c, double d0) {

    int dft_M = padded.rows, dft_N = padded.cols;
    Mat filter = Mat(padded.size(), CV_32FC2, Scalar(0)), tmp = Mat(padded.size(), CV_32F);

    for (int i = 0; i < dft_M; i++) {
        for (int j = 0; j < dft_N; j++) {
            tmp.at<float>(i, j) = (gh - gl) * (1 - exp(-c * (((i - dft_M / 2) * (i - dft_M / 2) + (j - dft_N / 2) * (j - dft_N / 2)) / (d0 * d0)))) + gl;
        }
    }

    Mat comps[] = { tmp,tmp };
    merge(comps, 2, filter);
    normalize(tmp, tmp, 0, 1, NORM_MINMAX);
    imshow("filtro", tmp);
    return filter;
}

//funcao para aplicar o filtro
Mat filtragem() {
    //filtro e imagem complexa  
    Mat filter, cplx_img;
    //parte real da imagem e imagem totalmente preta
    Mat_<float> real_img, zeros;
    //plano real e imaginario da imagem
    vector<Mat> planos;

    //dimensoes para a dft
    int dft_M = getOptimalDFTSize(img_original.rows);
    int dft_N = getOptimalDFTSize(img_original.cols);

    //realiza o padding da imagem
    copyMakeBorder(img_original, padded, 0,
        dft_M - img_original.rows, 0,
        dft_N - img_original.cols,
        BORDER_CONSTANT, Scalar::all(0));

    //parte imaginaria da matriz complexa (preenchida com zeros)
    zeros = Mat_<float>::zeros(padded.size());
    //prepara a matriz complexa para ser preenchida
    cplx_img = Mat(padded.size(), CV_32FC2, Scalar(0));
    //o filtro deve ter mesma dimensao e tipo que a matriz complexa
    filter = cplx_img.clone();
    //cria a matriz com as componentes do filtro e junta ambas em uma matriz multicanal complexa
    Mat comps[] = { Mat(dft_M, dft_N, CV_32F), Mat(dft_M, dft_N, CV_32F) };
    merge(comps, 2, filter);
    //limpa o array de matrizes que vao compor a imagem complexa
    planos.clear();
    //cria a parte real
    real_img = Mat_<float>(padded);
    real_img += Scalar::all(1);
    log(real_img, real_img);
    //insere as duas partes (real e complexa) no array de matrizes
    planos.push_back(real_img);
    planos.push_back(zeros);
    //transforma as duas partes em uma unica componente complexa
    merge(planos, cplx_img);
    //dft
    dft(cplx_img, cplx_img);
    //troca os quadrantes
    deslocaDFT(cplx_img);
    resize(cplx_img, cplx_img, padded.size());
    normalize(cplx_img, cplx_img, 0, 1, NORM_MINMAX);

    //criamos o filtro
    filter = filtro(gh, gl, c, d0);
    //filtragem
    mulSpectrums(cplx_img, filter, cplx_img, 0);
    //reorganiza os quadrantes
    deslocaDFT(cplx_img);
    //transformada inversa
    idft(cplx_img, cplx_img);
    //clear no array de planos
    planos.clear();
    //separa as partes real e imaginaria da imagem filtrada
    split(cplx_img, planos);
    exp(planos[0], planos[0]);
    planos[0] -= Scalar::all(1);
    //normaliza a parte real 
    normalize(planos[0], planos[0], 0, 1, NORM_MINMAX);
    return planos[0];
}


//controla os parametros do filtro
void on_trackbar(int, void*) {
    gh = (double)gh_slider / 100.0;
    gl = (double)gl_slider / 20.0;
    c = (double)c_slider;
    d0 = (double)d0_slider;
    img_final = filtragem();
    imshow("img_final", img_final);
    normalize(img_final, img_final, 0, 255, NORM_MINMAX);
    imwrite("saida.png", img_final);
}


int main() {
    char TrackbarName[50];
    img_original = imread("C:\\opencv\\sources\\samples\\data\\ruaescura.jpg", IMREAD_GRAYSCALE);
    resize(img_original, img_original, cv::Size(600, 480));
    imshow("original", img_original);

    namedWindow("img_final", WINDOW_FREERATIO);

    sprintf(TrackbarName, "gh - %d", gh_max);
    createTrackbar(TrackbarName, "img_final", &gh_slider, gh_max, on_trackbar);

    sprintf(TrackbarName, "gl - %d", gl_max);
    createTrackbar(TrackbarName, "img_final", &gl_slider, gl_max, on_trackbar);

    sprintf(TrackbarName, "c - %d", c_max);
    createTrackbar(TrackbarName, "img_final", &c_slider, c_max, on_trackbar);

    sprintf(TrackbarName, "d0 - %d", d0_max);
    createTrackbar(TrackbarName, "img_final", &d0_slider, d0_max, on_trackbar);
    waitKey();

    return 0;
}
~~~

Resultados:

![Rua escura](/img/ruaescura.png)
###### Figura 13: Rua com péssima iluminação

![Rua escura após filtragem](/img/paramhomo.png)
###### Figura 14: Rua com péssima iluminação após aplicarmos o filtro

![Filtro homomórfico](/img/filtrohomomorfico.png)
###### Figura 15: Filtro homomórfico

### Filtro de Canny e Pontilhismo
No oitavo tutorial foi utilizado o filtro de Canny para identificar as bordas e em seguida foi aplicado o pontilhismo.
Nesse projeto, o objetivo foi criar pontos apenas nas regiões onde estão presentes as bordas.
- O STEP utilizado foi 2
- O raio utilizado foi 1
- O JITTER utilizado foi 1

~~~c++
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;


#define STEP 2
#define JITTER 1

int slider = 1;
int slider_max = 200;

char TrackbarName[50];

Mat image,cinza, border,img_final;
  int width, height;
  Vec3b colors;
  int x, y;
  vector<int> yrange;
  vector<int> xrange;

void on_trackbar_canny(int, void*){

  Canny(cinza, border, slider, 3*slider);
  imshow("bodas", border);
  
  img_final = Mat(height, width, CV_8UC3, Scalar(255,255,255));
  for(int i =0; i<width; i++){
    for(int j =0; j<height; j++){
      if(border.at<uchar>(j,i) == 255)
      {
          x = i+rand()%(2*JITTER)-JITTER+1;
          y = j+rand()%(2*JITTER)-JITTER+1;
          colors = image.at<Vec3b>(y,x);
          circle(img_final,
                 cv::Point(x,y),
                 1,
                 CV_RGB(colors[2],colors[1],colors[0]),
                 -1,
                 LINE_AA);
      }
    }
  }
  imshow("canny",img_final);

}

int main(int argc, char**argv){

  image= imread(argv[1],IMREAD_COLOR);
  cvtColor(image,cinza, COLOR_BGR2GRAY);

  srand(time(0));

  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  
  sprintf(TrackbarName,"Threshold_inferior-%d",slider_max);
  namedWindow("canny",1);
  createTrackbar( TrackbarName, "canny",
                &slider,
                slider_max,
                on_trackbar_canny );

  on_trackbar_canny(slider, 0 );

  waitKey();
  imwrite("bordas.png",border);
  imwrite("cannypontos.png",img_final);
  return 0;
}
~~~

Resultados:

![Imagem original](/img/predios.jpg)
###### Figura 15: Imagem original

![Bordas](/img/bordas.png)
###### Figura 16: Bordas encontradas com o filtro de Canny

![Pontilhismo aplicado às bordas](/img/cannypontos.png)
###### Figura 17: Pontilhismo aplicado às bordas

### K-Means
O último tutorial se trata de utilizar o kmeans para mapear os dados(cores) em um conjunto menor de classes. O objetivo foi verificar o resultado de dez classificações rodando o algoritmo apenas uma única vez e alterando o parametro que define os centros iniciais (foi modificado de MEANS_PP_CENTERS para KMEANS_RANDOM_CENTERS).

~~~c++
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <opencv2/core/types_c.h>

using namespace cv;

int main( int argc, char** argv ){
    for(int i = 0; i< 10; i++){
        int nClusters = 12;
        Mat rotulos;
        int nRodadas = 1;
        Mat centros;
        char name[15];

        if(argc!=3){
            exit(0);
        }

        Mat img = imread( argv[1], IMREAD_COLOR);
        Mat samples(img.rows * img.cols, 3, CV_32F);

        for( int y = 0; y < img.rows; y++ ){
            for( int x = 0; x < img.cols; x++ ){
            for( int z = 0; z < 3; z++){
                samples.at<float>(y + x*img.rows, z) = img.at<Vec3b>(y,x)[z];
            }
            }
        }

        kmeans(samples,
                nClusters,
                rotulos,
                TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001),
                nRodadas,
                KMEANS_RANDOM_CENTERS,
                centros );


        Mat rotulada( img.size(), img.type() );
        for( int y = 0; y < img.rows; y++ ){
            for( int x = 0; x < img.cols; x++ ){
            int indice = rotulos.at<int>(y + x*img.rows,0);
            rotulada.at<Vec3b>(y,x)[0] = (uchar) centros.at<float>(indice, 0);
            rotulada.at<Vec3b>(y,x)[1] = (uchar) centros.at<float>(indice, 1);
            rotulada.at<Vec3b>(y,x)[2] = (uchar) centros.at<float>(indice, 2);
            }
        }
        sprintf(name,  "kmeans%d.jpg", i);
        imwrite(name, rotulada);
    }
}
~~~

O resultado pode ser observado no GIF gerado a partir das 10 imagens de cada classificação.

![10 classificações](/img/gifkmeans.gif)

###### Figura 18: Diferença entre as 10 classificações

As classificações tiveram resultados diferentes porque houve apenas uma repetição do algoritmo, o que faz com que os centros não sejam recalculados e não exista uma aproximação das classificações. Além disso, o parâmetro KMEANS_RANDOM_CENTERS resulta em centros iniciais aleatórios, agravando a discrepância entre as dez classificações, diferentemente da flag KMEANS_PP_CENTERS, que é um método que interage com toda imagem para determinar aonde serão os centros iniciais e em seguida iniciar a convergência.






