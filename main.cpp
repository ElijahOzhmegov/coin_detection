//
//  main.cpp
//  CV_LAB_6
//
//  Created by Elijah Ozhmegov on 04/02/2018.
//  Copyright © 2018 Elijah Ozhmegov. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <pthread.h>
#include "findingOutChemComp.hpp"
//#include "comparingCoins.hpp"
#include "skeletonisation.hpp"

int main(int argc, const char * argv[]) {
    
    std::cout << "OpenCV Version: " << CV_VERSION << std::endl;
    
    findingOut ();
    
    
    
//    makeSkelet ();
    
    return 0;
}
