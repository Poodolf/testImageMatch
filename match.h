#ifndef MATCH_H
#define MATCH_H

#include <QVector>
#include <QDebug>
#include <QRect>
#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <QApplication>

using namespace std;
using namespace cv;

string image_window = "Source Image";
string result_window = "Result window";

class Match{
    public:
        QRect rect;
        double dMatch;
};

QVector<Match> matching(Mat img, Mat templ)
{
    namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    namedWindow( result_window, CV_WINDOW_AUTOSIZE );

    Mat result;
    Mat img_display;
    img.copyTo( img_display );

    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    result.create( result_rows, result_cols, CV_32FC1 );
    matchTemplate( img, templ, result, 5);
    double matchingGrade;
    int counter = 0;
    while (true){

        double minVal; double maxVal; Point minLoc; Point maxLoc;Point matchLoc;
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
        matchLoc = maxLoc;
        matchingGrade = maxVal;

        if(matchingGrade > 0.8){
            ++counter;
            Match newMatch;
            QRect temp = {matchLoc.x, matchLoc.y, templ.cols, templ.rows};
            newMatch.rect = temp;
            newMatch.dMatch = maxVal;

            qDebug() << "Matching Number:" << counter << "," << matchingGrade << "\n" << newMatch.rect.left() << newMatch.rect.top() << newMatch.dMatch;
            floodFill(result, Point(matchLoc.x,matchLoc.y), Scalar(0,0,0), 0, Scalar(10,10,10));
            rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
            imshow( image_window, img_display );
            imshow( result_window, result );
            //waitKey(0);
        }
        else
            qDebug() << "no more matches";
            break;
    }
    //return;
}



#endif // MATCH_H
