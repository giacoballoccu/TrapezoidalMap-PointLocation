#include "geoutils.h"

namespace geoutils {

/*
Cross product to understand if the point is above or below line, formula returns 0 if segment and point are collinear.
1 if the point is above the segment

*/
int isPointAbove(cg3::Point2d p, cg3::Segment2d s){
    double dotProduct = ((s.p2().x() - s.p1().x())*(p.y() - s.p1().y()) - (s.p2().y() - s.p1().y())*(p.x() - s.p1().x()));
    if(dotProduct > 0){
        return 1;
    }else if (dotProduct< 0){
        return 0;
    }else{
        return -1;
    }
}

double sixDecimal(double number){
    return round( number * 100000.0 ) / 100000.0;
}

bool arePointsEqual(cg3::Point2d p1, cg3::Point2d p2){
    double threshold = 0.1;
    if(abs(p1.x() - p2.x()) <= threshold){
        if(abs(p1.y() - p2.y()) <= threshold){
            return true;
        }

    }
    return false;
}

/*

This function calculate the y value in a given x of a segment from the line equation passing from two point and intersection

*/
double calculateYCoord(cg3::Segment2d s, double x){
    cg3::Point2d p1 = s.p1();
    cg3::Point2d p2 = s.p2();
    float m = (p2.y() - p1.y())/(p2.x()-p1.x());
    return sixDecimal(m*(x - p1.x()) + p1.y());
}

/*

This function check if the segment is valid, if isn't swap the points. (Valid is defined by the fact that p1.x is > of p2.x)

*/
void validateSegment(cg3::Segment2d& s){
    if (s.p1().x() > s.p2().x()){
        cg3::Point2d tmp = s.p2();
        s.setP2(s.p1());
        s.setP1(tmp);
    }
    return;
}
}
