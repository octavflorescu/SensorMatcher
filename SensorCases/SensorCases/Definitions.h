//
//  Definitions.h
//  SensorCases
//
//  Created by OctavF on 05/05/16.
//  Copyright © 2016 skobbler. All rights reserved.
//

#ifndef Definitions_h
#define Definitions_h

#include <math.h>

#define kRobotSize 20.0

using namespace std;

struct point {
    float x, y;
    
    bool isBetweenPoints(point p1, point p2) {
        bool isBetweenOnX = ((p1.x <= x) && (p2.x >= x)) || ((p1.x >= x) && (p2.x <= x));
        bool isBetweenOnY = ((p1.y <= y) && (p2.y >= y)) || ((p1.y >= y) && (p2.y <= y));
        
        if (p1.x == p2.x) {
            return isBetweenOnY;
        }
        if (p1.y == p2.y) {
            return isBetweenOnX;
        }
        // match the gradients
        return isBetweenOnX && isBetweenOnY;
    }
    
    friend bool operator==(const point lhs, const point rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

struct square20 {
    point p1, p2, p3, p4, c;
    
    square20();
    
    square20(point point1, point point2, point point3, point point4, point center) {
        p1 = point1;
        p2 = point2;
        p3 = point3;
        p4 = point4;
        c = center;
    }
    
    square20(point center) {
        c = center;
        
        p1 = {static_cast<float>(center.x - (kRobotSize / 2.0)),
            static_cast<float>(center.y + kRobotSize / 2.0)};
        p2 = {static_cast<float>(center.x + kRobotSize / 2.0),
            static_cast<float>(center.y + kRobotSize / 2.0)};
        p3 = {static_cast<float>(center.x + kRobotSize / 2.0),
            static_cast<float>(center.y - kRobotSize / 2.0)};
        p4 = {static_cast<float>(center.x - kRobotSize / 2.0),
            static_cast<float>(center.y - kRobotSize / 2.0)};
    }
    
    void rotateAtAngle(float angle)
    {
        float s = sin(angle);
        float cZ = cos(angle);
        
        float xnew;
        float ynew;
        
        // rotate point 1
        p1.x = p1.x - c.x;
        p1.y = p1.y - c.y;
        xnew = p1.x * cZ - p1.y * s;
        ynew = p1.x * s + p1.y * cZ;
        p1.x = xnew + c.x;
        p1.y = ynew + c.y;
        
        // rotate point 2
        p2.x = p2.x - c.x;
        p2.y = p2.y - c.y;
        xnew = p2.x * cZ - p2.y * s;
        ynew = p2.x * s + p2.y * cZ;
        p2.x = xnew + c.x;
        p2.y = ynew + c.y;
        
        // rotate point 3
        p3.x = p3.x - c.x;
        p3.y = p3.y - c.y;
        xnew = p3.x * cZ - p3.y * s;
        ynew = p3.x * s + p3.y * cZ;
        p3.x = xnew + c.x;
        p3.y = ynew + c.y;
        
        // rotate point 4
        p4.x = p4.x - c.x;
        p4.y = p4.y - c.y;
        xnew = p4.x * cZ - p4.y * s;
        ynew = p4.x * s + p4.y * cZ;
        p4.x = xnew + c.x;
        p4.y = ynew + c.y;
    }
};

#endif /* Definitions_h */
