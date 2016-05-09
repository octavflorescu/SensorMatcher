//
//  main.cpp
//  SensorCases
//
//  Created by OctavF on 05/05/16.
//  Copyright © 2016 skobbler. All rights reserved.
//

#include <iostream>

#include <vector>
#include <map>
#include "Line.h"

#define kRange 130
#define kSelfMargin kRobotSize / 2.0

map<vector<Line>, vector<square20>> cases = map<vector<Line>, vector<square20>>();
map<string, vector<Line>> caseKeys = map<string, vector<Line>>();
vector<Line> caseKey;

Line lines[] = {
    Line("1", {-8, 0}, {-8, 24.5}),
    Line("2", {-3.725, 0}, {0, 24.5}),
    Line("3", {0, 0}, {0, 24.5}),
    Line("4", {3.725, 0}, {0, 24.5}),
    Line("5", {8, 0}, {8, 24.5})//,
//    Line("a", {-4, 0}, {-9, 18.24}),
//    Line("b", {0, 0}, {0, 20}),
//    Line("c", {4, 0}, {9, 18.24})
};
unsigned int numberOfLines = sizeof(lines) / sizeof(Line);

// FUNCTIONS //

string stringFromLines(vector<Line> lines) {
    string cKey = "";
    
    for (int i = 0; i < lines.size(); i++) {
        cKey += lines[i].getLineID();
    }
    return cKey;
}

bool isSquareValid(square20 sq) {
    if ((sq.p1.y < 0 && sq.p1.x < kSelfMargin && sq.p1.x > -kSelfMargin)
        || (sq.p2.y < 0 && sq.p2.x < kSelfMargin && sq.p2.x > -kSelfMargin)
        || (sq.p3.y < 0 && sq.p3.x < kSelfMargin && sq.p3.x > -kSelfMargin)
        || (sq.p4.y < 0 && sq.p4.x < kSelfMargin && sq.p4.x > -kSelfMargin)) {
        return false;
    }
    return true;
}

vector<Line> linesWhichPassSquare(square20 square) {
    vector<Line> vecLine = vector<Line>();
    Line currentLine;
    
    if (isSquareValid(square)) {
        // check each line
        for (int l = 0; l < numberOfLines; l++) {
            currentLine = lines[l];
            
            if (currentLine.intersectsSquare(square)) {
                vecLine.push_back(currentLine);
            }
        }
    }
    return vecLine;
}

void decideAddVecLineWithSquare(vector<Line> vecLine, square20 square) {
    string stringCaseKey;
    
    if (!vecLine.empty()) {
        stringCaseKey = stringFromLines(vecLine);
        caseKey = caseKeys[stringCaseKey];
        
        // if the re is no line vector in main map, add it to both
        if (caseKey.empty()) {
            caseKeys[stringCaseKey] = vecLine;
            cases[vecLine].push_back(square);
            
        } else { // otherwise just append the square to the main map
            cases[caseKey].push_back(square);
        }
    }
}


// MAIN //


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    vector<Line> vecLine;
    square20 square = square20({0,0});
    
    // move on X
    for (float i = -kRange; i < kRange; i++) {
        
        // move on Y
        for (float j = kRobotSize / 2.0; j < kRange; j++) {
            square = square20({i, j});
            
            vecLine = linesWhichPassSquare(square);
            decideAddVecLineWithSquare(vecLine, square);
            
            // rotate square
            for (float k = 0.1; k <= 1.7; k += 0.2) {
                square.rotateAtAngle(0.2);
                
                vecLine = linesWhichPassSquare(square);
                decideAddVecLineWithSquare(vecLine, square);
            }
        }
    }
    vector<square20> squares;
    
    for (std::map<vector<Line>, vector<square20>>::iterator it=cases.begin(); it!=cases.end(); ++it) {
        cout << stringFromLines(it->first) << " - ";
        squares = it->second;
        
//        for (int i = 0; i < squares.size(); i++) {
//            square = squares[i];
//            
//            cout << "(" << square.c.x << "," << square.c.y << ") : ";
//            
//            cout << square.p1.x << "," << square.p2.y << " -- ";
//            cout << square.p2.x << "," << square.p3.y << " -- ";
//            cout << square.p3.x << "," << square.p4.y << " -- ";
//            cout << square.p4.x << "," << square.p1.y << "\n";
//        }
//        cout << "\n";
        cout << squares.size() << "\n\n";
    }
    return 0;
}
