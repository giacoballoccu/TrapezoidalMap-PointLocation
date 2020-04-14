#include "geoutils.h"
namespace geoutils {

/*
Cross product to understand if the point is above or below line, formula returns 0 if segment and point are collinear.
1 if the point is above the segment

*/
bool isAbove(cg3::Point2d p, cg3::Segment2d s){
     return ((s.p2().x() - s.p1().x())*(p.y() - s.p1().y()) - (s.p2().y() - s.p1().y())*(p.x() - s.p1().x())) > 0;
}

/*

This function calculate the y value in a given x of a segment from the line equation passing from two point and intersection

*/
float calculateYCoord(cg3::Segment2d s, float x){
    cg3::Point2d p1 = s.p1();
    cg3::Point2d p2 = s.p2();
    float m = (p2.y() - p1.y())/(p2.x()-p1.x());
    return (m*(x - p1.x()) + p1.y());
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
