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
    cv::waitKey();
    return 0;
}
