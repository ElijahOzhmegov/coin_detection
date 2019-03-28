//
//  comparingCoins.cpp
//  CV_LAB_6
//
//  Created by Elijah Ozhmegov on 19/02/2018.
//  Copyright © 2018 Elijah Ozhmegov. All rights reserved.
//

#include "comparingCoins.hpp"


std::string path = "/Users/Elijah/Desktop/Computer Vision/Lab SIX/CV_LAB_6/CV_LAB_6/coins/";

cv::Mat cold = cv::imread ( path + "nickel.png", cv::IMREAD_COLOR);
cv::Mat warm = cv::imread ( path + "brass.png",  cv::IMREAD_COLOR);

bool isRequiredBefore = false;

// Histograms
cv::MatND hist_cold;
cv::MatND hist_warm;

std::string  getMetal ( cv::Mat imagePart )
{
    if (!isRequiredBefore)
    {
        isRequiredBefore = true;

        getHist ( cold, hist_cold );
        getHist ( warm, hist_warm );
        
    }
    
    cv::MatND hist_imgPart;
    
    getHist ( imagePart, hist_imgPart );
    
    
    double img_cold = compareHist( hist_cold, hist_imgPart, cv::HISTCMP_CORREL );
    double img_warm = compareHist( hist_warm, hist_imgPart, cv::HISTCMP_CORREL );
    
//    std::cout << "img n cold:" << img_cold << std::endl;
//    std::cout << "img n warm:" << img_warm << std::endl;
//
//    std::cout << std::endl;
    
    if (img_cold > img_warm) return "Nickel";

    if (img_cold < img_warm) return "Brass";
    
    return "Unknown";
    
}

void getHist ( const cv::Mat img, cv::MatND &hist_img)
{
    cv::Mat img_hsv;
    cvtColor ( img, img_hsv, CV_RGB2HSV );
    
    // Using 50 bins for hue and 60 for saturation
    int h_bins = 50; int s_bins = 60;
    int histSize[] = { h_bins, s_bins };
    
    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };
    
    const float* ranges[] = { h_ranges, s_ranges };
    
    // Use the o-th and 1-st channels
    int channels[] = { 0, 1 };
    
    int numb_img = 1;
    unsigned int hist_dim = 2;
    
    // Calculate the histograms for the HSV images
    calcHist( &img_hsv, numb_img, channels, cv::Mat(), hist_img, hist_dim, histSize, ranges );
    normalize( hist_img, hist_img, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
    
}





