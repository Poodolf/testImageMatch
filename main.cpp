#include "mainwindow.h"
#include <QApplication>
#include "match.h"
#include <QDebug>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>


int main(int, char**)
{

    cv::Mat img = imread("X://2.jpg", 1);
    cv::Mat templ = imread("X://2template.jpg", 1);

    matching(img, templ);

    return 0;
}

