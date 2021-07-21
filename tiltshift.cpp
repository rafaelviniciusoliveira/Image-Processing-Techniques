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

int main9() {
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