//
//  findingOutChemComp.cpp
//  CV_LAB_6
//
//  Created by Elijah Ozhmegov on 04/02/2018.
//  Copyright © 2018 Elijah Ozhmegov. All rights reserved.
//

#include "findingOutChemComp.hpp"
#include "comparingCoins.hpp"

cv::Mat gray;
std::vector<Circle> actions;

std::string windowName = "Calibration of thresholds";

int upThreshold  = 200;
int lowThreshold = 50;


void on_trackbar ( int, void* )
{
    cv::Mat copy;
    cv::Canny ( gray, copy, lowThreshold, upThreshold );
    imshow ( windowName, copy );
    cv::moveWindow ( windowName, 720-copy.cols/2, 0 );
}


void calibration ()
{
    cv::VideoCapture cap(0); // открываем камеру
    if ( cap.isOpened() )  // проверяем успешно ли открыли источник видео
    {
        
        cv::namedWindow (windowName);
        
        cv::Mat frame;
        cap >> frame; // получаем новый кадр с камеры
        
        GaussianBlur( frame, frame, Size(5,5), 1, 1);
        
        cvtColor (frame, gray, CV_BGR2GRAY);
        //        GaussianBlur ( gray, gray, cvSize(9, 9), 2, 2 );
        
        cv::createTrackbar ("Low",   windowName, &lowThreshold, 255, on_trackbar);
        cv::createTrackbar ("Upper", windowName, &upThreshold, 255, on_trackbar);
        
        // Show some stuff, there are some random numbers
        on_trackbar ( 9, 0);

        cv::waitKey ();
        
        std::cout << "low thresh" <<lowThreshold << std::endl;
        std::cout << "up thresh" <<upThreshold << std::endl;
    }
}


void showingQuestion()
{
    int baseline = 0;
    int fontFace = cv::FONT_HERSHEY_PLAIN;
    double fontScale = 2;
    int thickness = 2;
    
    cv::Mat background (300, 800, CV_8UC3,  cv::Scalar::all(200));
    
    std::string text = "Would you like calibrate thresholds? (type Y)";
    cv::Size textSize = cv::getTextSize ( text, fontFace, fontScale, thickness, &baseline);
    
    // center the text
    cv::Point textOrg ( (background.cols - textSize.width)/2, background.rows/2 );
    
    // on right side
    putText ( background, text, textOrg, fontFace, fontScale , cv::Scalar::all(0), thickness );
    
    imshow ( windowName, background );
    cv::moveWindow ( windowName, 720 - background.cols/2, 300 - background.rows/2 );
    
    int keyboard = 0;
    keyboard = cvWaitKey ();
    
    if ( ((char) keyboard == 'Y') ||  ((char) keyboard == 'y') ) calibration ();
    
}

void buildingSelectionVector ( cv::Mat &selectedGray )
{
    std::vector<Point> commonP;
    
    cv::Mat temp (gray.rows, gray.cols, CV_8UC1,  cv::Scalar::all(0));
    
    for (int i = 0; i<gray.rows; i++)
    {
        for ( int j = 0; j<gray.cols; j++)
        {
            if (gray.at<uchar> (i,j))
            {
                Point tempPoint;
                
                tempPoint.x = j;
                tempPoint.y = i;
                
                commonP.push_back (tempPoint);
            }
            
        }
    }
    
    
    int max = commonP.size() * 0.01;
    
//    std::cout << "max:" << max << std::endl;
    
    std::vector<Point> selectedP;
    actions.clear();
    
    
    for (int i=0; i<max; i++)
    {
        auto index = rand() % commonP.size();
        selectedP.push_back ( commonP[index] );
    
        
        temp.at<uchar>( commonP[index].y, commonP[index].x ) = 255;
        
        std::vector<Point>::iterator itr = commonP.begin() + index;
        commonP.erase ( itr );
        
        if (i>=2)
        {
            for (int j=0; j<i-2; j++)
            {
                for (int k=j+1; k<i-1; k++)
                {
                    Circle tempCircle ( &selectedP[i],
                                       &selectedP[j],
                                       &selectedP[k]);
                    
                    if ( checkNewCircle (tempCircle) )
                    {
                        if (actions.size()){
                            int t = 0;
                            
                            for ( ; t<actions.size(); t++)
                            {
                                if (actions[t] == tempCircle) break;
                            }
                            
                            if ( t == actions.size() ) actions.push_back (tempCircle);
                            
                        }else{
                            actions.push_back (tempCircle);
                        }
                    }
                }
            }
        }
    }
    
    commonP.clear();
    
    temp.copyTo (selectedGray);
    
}

void checkMemAccess ( Point centre, double radius,
                      Point &beginEnvironment, int &width, int &height,
                      Point &beginAction )
{
    beginEnvironment.x = centre.x - radius;
    beginEnvironment.y = centre.y - radius;
    
    width  = 2*radius;
    height = 2*radius;
    
    beginAction.x = 0;
    beginAction.y = 0;
    
    
    if ( beginEnvironment.x < 0)
    {
        beginEnvironment.x = 0;
        width = radius + centre.x;
        
        
        beginAction.x = 2*radius - width;
        
    }
    
    if ( beginEnvironment.y < 0)
    {
        beginEnvironment.y = 0;
        height = radius + centre.y;
        
        beginAction.y = 2*radius - height;
        
    }
    
    if (beginEnvironment.x + width > gray.cols)
    {
        width = gray.cols - beginEnvironment.x;
        
        
    }

    if (beginEnvironment.y + height > gray.rows)
    {
        height = gray.rows - beginEnvironment.y;
        
    }
}

int max( std::vector<float> &distribution )
{
    int imax = 0;
    float max = -1;
    for (int i=0; i<distribution.size(); i++)
    {
        if (distribution[i] > max)
        {
            max = distribution[i];
            imax = i;
        }
    }
    
    return imax;
}

void updateDistribution (float Betha, std::vector<float> &distribution, int imax)
{
    float thetha = 1; // the learning rate
    
    
    if ( Betha > 0.23 )// reward
    {
        for (int i=0; i<distribution.size(); i++)
        {
            if (i==imax){
                
                distribution[i] = distribution[i] + thetha * Betha * (1 - distribution[i] );
                i++;
            }
            
            distribution[i] = distribution[i] * (1 - thetha*Betha);
        }
    }
}

void findingSolution (std::vector<Circle> &candidates)
{
    auto numberCircles = actions.size();
    
    std::vector<float> distribution;
    
    distribution.assign (numberCircles, 1.0/numberCircles);
    
    //    Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
    Point beginEnvironment;
    Point beginAction;
    
    int width  = 0;
    int height = 0;
    
    
    
    int imax = 0;
    
    for (auto i=0; i<numberCircles; i++)
    {
        
        Circle bubble = actions[i];
        
        double Rad = bubble.GetRadius();
        Point  Cen = *bubble.GetCenter();
        
        checkMemAccess ( Cen, Rad,
                        beginEnvironment, width, height,
                        beginAction );
        
        cv::Mat testField ( 2*Rad, 2*Rad, CV_8UC1,  cv::Scalar::all(0) );
        
        cv::Mat roi;
        gray( Rect(beginEnvironment.x, beginEnvironment.y, width, height) ).copyTo(roi);
        
        circle ( testField, Point(Rad, Rad), Rad, cv::Scalar::all(255) );
        
        cv::Mat roiCircle = testField ( Rect(beginAction.x, beginAction.y, width, height) );
        
        bitwise_and (roiCircle, roi, roi);
        
        
        float sample   = countNonZero (roiCircle);
        float response = countNonZero (roi);
        
        float Betha = response / sample; //The reinforcement signal
        

        updateDistribution (Betha, distribution, i);
        
    }
    
    imax = max(distribution);
    
    candidates.clear();
    
    
    
    while (actions.size() != 0 && (distribution[imax] > 0.23) ){
        
        std::cout << distribution[imax] << std::endl;
        candidates.push_back(actions[imax]);
        
        std::vector<float>::iterator itr = distribution.begin() + imax;
        distribution.erase ( itr );
        
        std::vector<Circle>::iterator itrC = actions.begin() + imax;
        actions.erase ( itrC );
        
        imax = max(distribution);
        
    }
    
    
}

void findingOut ()
{
//    showingQuestion();
    
    cv::VideoCapture cap(0);
    if ( cap.isOpened() )
    {
        std::string framName = "Streaming";
        
        cv::Mat frame;
        cv::Mat selectedGray;
        
        std::vector<Circle> candidates;
        
        int keyboard = 0;
        int pace_time = 30;
        
        while ( cap.grab() && (char)keyboard != 'q' && (char)keyboard != 27  )
        {
            cap.retrieve(frame);
            
            pace_time = 30;
                
            gray = frame( Rect ( frame.cols/4,
                                 frame.rows/4,
                                 frame.cols/2,
                                 frame.rows/2) );
        
            extractChannel (gray, gray, 0);
            
            GaussianBlur ( gray, gray, Size(5,5), 1, 1);
        
            cv::Canny ( gray, gray, lowThreshold, upThreshold );
            
//            imshow ( "gray" , gray );
            
            buildingSelectionVector ( selectedGray );
            
            findingSolution ( candidates );
            
            
            for( size_t i = 0; i < candidates.size(); i++ )
            {

                // draw the circle center
                circle( frame( Rect ( frame.cols/4,
                                      frame.rows/4,
                                      frame.cols/2,
                                      frame.rows/2) ),
                        *candidates[i].GetCenter(), 3, cvScalar(255,0,0), -1, 8, 0 );
    
                // draw the circle outline
                circle( frame( Rect ( frame.cols/4,
                                      frame.rows/4,
                                      frame.cols/2,
                                      frame.rows/2) ),
                       *candidates[i].GetCenter(), candidates[i].GetRadius(),
                       cvScalar(0,0,255), 3, 8, 0 );
                
                int halfSide = candidates[i].GetRadius();
                int begX = frame.cols/4 + candidates[i].GetCenter()->x - halfSide;
                int begY = frame.rows/4 + candidates[i].GetCenter()->y - halfSide;
                
                std::string nameMetal = getMetal ( frame( Rect ( begX,
                                                                 begY,
                                                                 2*halfSide,
                                                                 2*halfSide ) ) );
                
                rectangle ( frame, cv::Point(begX + halfSide, begY - 20),
                                   cv::Point(begX + halfSide + 100, begY + 20),
                                   cv::Scalar(255,255,255), -1 );
                putText  ( frame,  nameMetal, cv::Point(begX + halfSide , begY + 10),
                           cv::FONT_HERSHEY_SIMPLEX, 1 , cv::Scalar(0,0,0) );
                
            }
            
            
            rectangle ( frame, Point (  frame.cols/4,  frame.rows/4),
                        Point (3*frame.cols/4,3*frame.rows/4),
                        cvScalar(0,0,0) );
            
            
            
            imshow ( framName, frame );
            cv::moveWindow ( framName, 720-frame.cols/2, 0 );

            

            keyboard = cvWaitKey(pace_time);

        }
    }
    
}


bool checkNewCircle (Circle tempCircle){
    double radius = tempCircle.GetRadius();
    Point centre = *tempCircle.GetCenter();
    
    if ( (radius > 35)           &&
         (radius < gray.rows/2)  &&
         (centre.x - radius >= 0) &&
         (centre.y - radius >= 0) &&
         (centre.x + radius <= gray.cols) &&
         (centre.y + radius <= gray.rows) )
    {
        return true;
    }else{
        return false;
    }
}



