//
//  Zhang-Suen.cpp
//  CV_LAB_6
//
//  Created by Elijah Ozhmegov on 22/02/2018.
//  Copyright © 2018 Elijah Ozhmegov. All rights reserved.
//

#include "Zhang-Suen.hpp"

using namespace cv;


void zhangSuen (const Mat img, OutputArray &result)
{
    Mat tempResult;

    img.copyTo( tempResult );
    
    for (int i=1; i<img.rows-2; i++)
    {
        for (int j=1; j<img.cols-2; j++)
        {
            Mat square = tempResult ( Rect(j-1, i-1, 3, 3));
            
            int B2W, notZero;
            
            getNeighInfo (square, B2W, notZero);
            
            int P_4n6 = square.at<uchar>(5) & square.at<uchar>(7);
            
            if ( (tempResult.at<uchar>(i,j) != 0) &&
                 (B2W == 1)                       &&
                 (notZero >= 2)                   &&
                 (notZero <= 6)                   &&
                !(square.at<uchar>(1) & P_4n6)    &&
                !(square.at<uchar>(3) & P_4n6) )
            {
                tempResult.at<uchar>(i,j) = 0;
            }
            
        }
    }
    
    tempResult.copyTo( result );
}

void getNeighInfo (Mat cells, int &sum, int &nonZero)
{
    uchar row[9] = { cells.at<uchar>(1), cells.at<uchar>(2), cells.at<uchar>(5),
                     cells.at<uchar>(8), cells.at<uchar>(7), cells.at<uchar>(6),
                     cells.at<uchar>(4), cells.at<uchar>(0), cells.at<uchar>(1) };
    
    sum = 0;
    nonZero = 0;
    
    for(int i=0; i<8; i++)
    {
        sum += ( (row[i] == 0) && (row[i+1] != 0) );
        nonZero += row[i] / 255;
        
    }
}






