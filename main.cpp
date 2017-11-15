#include <QDebug>
#include <QVector>
#include <QRect>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

std::string image_window = "Source Image";
std::string result_window = "Result window";

class Match : public QRect{
    public:
        double matching_score;
};

QVector<Match> doMatching(cv::Mat img, cv::Mat templ, int max_matches)
{
    cv::namedWindow( image_window, CV_WINDOW_AUTOSIZE );
    cv::namedWindow( result_window, CV_WINDOW_AUTOSIZE );

    cv::Mat result;
    cv::Mat img_display;
    QVector<Match> matches;
    img.copyTo( img_display );

    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    result.create( result_rows, result_cols, CV_32FC1 );
    matchTemplate( img, templ, result, 5);
    while (true){

        double min_val; double max_val; cv::Point min_loc; cv::Point max_loc; cv::Point match_loc;
        minMaxLoc( result, &min_val, &max_val, &min_loc, &max_loc, cv::Mat() );
        match_loc = max_loc;

        if((max_val > 0.80) && (matches.size() != max_matches)){  //when the match has a value higher than 0.XX and matches to find hasnt been reached
            Match new_match;
            new_match.setLeft(match_loc.x);
            new_match.setTop(match_loc.y);
            new_match.setWidth(templ.cols);
            new_match.setHeight(templ.rows);
            new_match.matching_score = max_val;
            matches.append(new_match);

            qDebug() << "Matching number:" << matches.size() << new_match.left() << new_match.top() << new_match.matching_score;
            floodFill(result, cv::Point(match_loc.x,match_loc.y), cv::Scalar(0,0,0), 0, cv::Scalar(10,10,10));
            rectangle( img_display, match_loc, cv::Point( match_loc.x + templ.cols , match_loc.y + templ.rows ), cv::Scalar::all(0), 2, 8, 0 );
            imshow( image_window, img_display );
            imshow( result_window, result );
            cv::waitKey(0); //Check Match by Match by pressing a button
        }
        else
        {
            if (matches.size() == max_matches){
                qDebug() << matches.size() << "matches found, exit searching";
            }
            else {
            qDebug() << "no more matches";
             }
        break;
        }
    }
    return matches;
}

int main(int, char**)
{
    cv::Mat img = cv::imread("X://2.jpg", 1);
    cv::Mat templ = cv::imread("X://2template.jpg", 1);
    int max_matches = 10; //Anzahl der gesuchten Treffer
    QVector<Match> matches = doMatching(img, templ, max_matches);
    for(int i = 0; i <= (matches.length() - 1); ++i){
    qDebug() << "Matching number" << (i + 1) << matches[i].left() << matches[i].top() << matches[i].matching_score;
    }
    return 0;
}

