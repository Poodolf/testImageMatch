#include <QDebug>
#include <QVector>
#include <QRect>
//#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

std::string image_window = "Source Image";
std::string result_window = "Result window";

class Match{
    public:
        QRect rect;
        double dMatch;
};

QVector<Match> matching(cv::Mat img, cv::Mat templ, int matchToFind)
{
    cv::namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    cv::namedWindow( result_window, CV_WINDOW_AUTOSIZE );

    cv::Mat result;
    cv::Mat img_display;
    QVector<Match> hits;
    img.copyTo( img_display );

    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    result.create( result_rows, result_cols, CV_32FC1 );
    matchTemplate( img, templ, result, 5);
    double matchingGrade;
    int counter = 0;
    while (true){

        double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc; cv::Point matchLoc;
        minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
        matchLoc = maxLoc;
        matchingGrade = maxVal;

        if((maxVal > 0.80) && !(counter == matchToFind)){
            ++counter;
            Match newMatch;
            newMatch.rect = {matchLoc.x, matchLoc.y, templ.cols, templ.rows};
            newMatch.dMatch = maxVal;
            hits.append(newMatch);

            qDebug() << "Matching Number:" << counter << "," << matchingGrade << "\n" << newMatch.rect.left() << newMatch.rect.top() << newMatch.dMatch;
            floodFill(result, cv::Point(matchLoc.x,matchLoc.y), cv::Scalar(0,0,0), 0, cv::Scalar(10,10,10));
            rectangle( img_display, matchLoc, cv::Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), cv::Scalar::all(0), 2, 8, 0 );
            imshow( image_window, img_display );
            imshow( result_window, result );
            cv::waitKey(0); //Check Match by Match
        }
        else
        {
            if (counter == matchToFind){
                qDebug() << counter << "matches found, exit searching";
                break;
            }
            else {
            qDebug() << "no more matches";
            break;
            }
        }
    }
    return hits;
}

int main(int, char**)
{
    cv::Mat img = cv::imread("X://2.jpg", 1); //Source image
    cv::Mat templ = cv::imread("X://2template.jpg", 1); //Reference
    int matchToFind = 100; //Anzahl der gesuchten Treffer
    QVector<Match> hits = matching(img, templ, matchToFind);
    for(int i = 0; i <= (hits.length() - 1); ++i){
    qDebug() << "hit " << (i + 1) << hits[i].rect.left() << hits[i].rect.top() <<hits[i].dMatch;
    }
    return 0;
}

