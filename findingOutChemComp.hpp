//
//  findingOutChemComp.hpp
//  CV_LAB_6
//
//  Created by Elijah Ozhmegov on 04/02/2018.
//  Copyright © 2018 Elijah Ozhmegov. All rights reserved.
//

#ifndef findingOutChemComp_hpp
#define findingOutChemComp_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include "Circle.hpp"


void findingOut ();
void on_trackbar( int, void* );
void calibration ();

void buildingSelectionVector ( cv::Mat &frame );

bool checkNewCircle (Circle tempCircle);

void findingSolution (std::vector<Circle> &candidate);

void checkMemAccess (Point centre, double radius,
                     Point &beginEnvironment, int &width, int &height,
                     Point &beginAction );

int max( std::vector<float> &distribution );

void updateDistribution (float Betha, std::vector<float> &distribution, int imax);

#endif /* findingOutChemComp_hpp */
