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
