//
//  Circle.hpp
//  CV_LAB_6
//
//  Created by Elijah Ozhmegov on 07/02/2018.
//  Copyright © 2018 Elijah Ozhmegov. All rights reserved.
//

#ifndef Circle_hpp
#define Circle_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

class Circle
{
private:
    double CalcCircle(Point *pt1, Point *pt2, Point *pt3);
    bool IsPerpendicular(Point *pt1, Point *pt2, Point *pt3);
    double m_dRadius;
    Point m_Center;
    
public:
    double GetRadius();
    Point* GetCenter();
    Circle(Point *p1, Point *p2, Point *p3);    // p1, p2, p3 are co-planar
    Circle();
    double length(Point Centre, Point *pt1);
    virtual ~Circle();
    
    bool operator ==(Circle const &right)
    {
        if ( (this->m_Center == right.m_Center) &&
             (this->m_dRadius == right.m_dRadius) )
        {
            return true;
        }else{
            return false;
        }
    }
};


#endif /* Circle_hpp */
