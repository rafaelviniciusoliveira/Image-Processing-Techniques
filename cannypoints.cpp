#include <iostream>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
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
