//
//  Zhang-Suen.hpp
//  CV_LAB_6
//
//  Created by Elijah Ozhmegov on 22/02/2018.
//  Copyright © 2018 Elijah Ozhmegov. All rights reserved.
//

#ifndef Zhang_Suen_hpp
#define Zhang_Suen_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

void zhangSuen (const  cv::Mat img, cv::OutputArray &result);

void getNeighInfo (cv::Mat cells, int &sum, int &nonZero);

#endif /* Zhang_Suen_hpp */
