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