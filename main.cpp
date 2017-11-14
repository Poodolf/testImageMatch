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

    Mat img = imread("X://2.jpg", 1);
    Mat templ = imread("X://2template.jpg", 1);
    int matchToFind = 10;
    QVector<Match> hits = matching(img, templ, matchToFind);
    for(unsigned i = 0; i <= (hits.length() - 1); ++i){
    qDebug() << "hit " << (i + 1) << hits[i].rect.left() << hits[i].rect.top() <<hits[i].dMatch;
    }
    return 0;
}

