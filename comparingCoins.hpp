//
//  comparingCoins.hpp
//  CV_LAB_6
//
//  Created by Elijah Ozhmegov on 19/02/2018.
//  Copyright © 2018 Elijah Ozhmegov. All rights reserved.
//

#ifndef comparingCoins_hpp
#define comparingCoins_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

//enum Metal {
//    Unknown = "Unknown",
//    Nickel  = "Nickel",
//    Brass   = "Brass"
//
//};

std::string getMetal ( cv::Mat imagePart );

void getHist ( const cv::Mat img, cv::MatND &hist_img);

#endif /* comparingCoins_hpp */
