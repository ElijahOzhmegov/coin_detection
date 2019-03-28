//
//  skeletonisation.cpp
//  CV_LAB_6
//
//  Created by Elijah Ozhmegov on 22/02/2018.
//  Copyright © 2018 Elijah Ozhmegov. All rights reserved.
//

#include "skeletonisation.hpp"

using namespace cv;
using namespace std;

void makeSkelet ()
{
    string path = "/Users/Elijah/Desktop/Computer Vision/Lab SIX/CV_LAB_6/CV_LAB_6/skelet/";
    
    Mat iMtest_1 = imread ( path + "test_1.jpg", IMREAD_GRAYSCALE);
    Mat iMtest_2 = imread ( path + "test_2",     IMREAD_GRAYSCALE);
    
    Mat result_1;
    zhangSuen (iMtest_1, result_1);
    
    
    imshow("123", result_1);
    
    cvWaitKey();
}
