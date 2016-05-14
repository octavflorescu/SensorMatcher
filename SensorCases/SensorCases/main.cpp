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
    Line("2", {-3.725, 0}, {9, 69}),
    Line("3", {0, 0}, {0, 20}),
    Line("4", {3.725, 0}, {-9, 69}),
    Line("5", {8, 0}, {8, 24.5}),
    Line("a", {-4, 0}, {-9, 12}),
//    Line("b", {0, 0}, {0, 20}),
    Line("c", {4, 0}, {9, 8})
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

map<string, vector<vector<point>>> findSubCases() {
    map<string, vector<vector<point>>> subCases = map<string, vector<vector<point>>>();
    vector<vector<point>> subCasesValues;
    vector<point> subcaseVals;
    point pointOfSubCase;
    bool didAddToExistentSubCase;
    vector<square20> squares;
    string lineString;
    square20 square = square20({0,-500});
    square20 lastSquare = square20({0,-500});
    
    for (std::map<vector<Line>, vector<square20>>::iterator it=cases.begin(); it!=cases.end(); ++it) {
        lineString = stringFromLines(it->first);
        subCasesValues = subCases[lineString];
        
        //        cout << lineString << "\n";
        squares = it->second;
        
        if (subCasesValues.empty()) {
            subcaseVals = vector<point>();
            lastSquare = squares[0];
            
            subcaseVals.push_back(lastSquare.c);
            
            subCasesValues.push_back(subcaseVals);
        }
        
        
        for (int i = 1; i < squares.size(); i++) {
            square = squares[i];
            
            //            cout << "(" << square.c.x << "," << square.c.y << ") : ";
            //
            //            cout << square.p1.x << "," << square.p2.y << " -- ";
            //            cout << square.p2.x << "," << square.p3.y << " -- ";
            //            cout << square.p3.x << "," << square.p4.y << " -- ";
            //            cout << square.p4.x << "," << square.p1.y << "\n";
            
            if (lastSquare.c == square.c) { // rotation diffs will not be taken into consideration
                continue;
            }
            didAddToExistentSubCase = false;
            
            for (int j = 0; j < subCasesValues.size(); j++) {
                subcaseVals = subCasesValues[j];
                pointOfSubCase = subcaseVals.back();
                
                if (pointOfSubCase.x + kRobotSize / 4 > square.c.x && pointOfSubCase.y + kRobotSize / 4 > square.c.y ) {
                    didAddToExistentSubCase = true;
                    subcaseVals.push_back(square.c);
                    
                    subCasesValues[j] = subcaseVals;
                    
                    break;
                }
            }
            if (!didAddToExistentSubCase) {
                subcaseVals = vector<point>();
                subcaseVals.push_back(square.c);
                subCasesValues.push_back(subcaseVals);
            }
            
            lastSquare = square;
        }
        subCases[lineString] = subCasesValues;
        
        cout << "\n";
        //        cout << squares.size() << "\n\n";
    }
    return subCases;
}

map<string, vector<square20>> convertToVisualCases(map<string, vector<vector<point>>> source) {
    map<string, vector<square20>> rezMap = map<string, vector<square20>>();
    vector<square20> rezSubCases;
    square20 rezSquare = square20({0,0}); // pattern; center == center of pattern / refference center
    
    string lineString;
    vector<vector<point>> subCasesValues;
    vector<point> subcaseVals;
    point pointOfSubCase;
    
    for (std::map<string, vector<vector<point>>>::iterator it=source.begin(); it!=source.end(); ++it) {
        lineString = it->first;
        rezSubCases = vector<square20>();
        
        subCasesValues = it->second;
        
        for (int i = 0; i < subCasesValues.size(); i++) {
            subcaseVals = subCasesValues[i];
            pointOfSubCase = subcaseVals[0];
            rezSquare = square20(pointOfSubCase, pointOfSubCase, pointOfSubCase, pointOfSubCase, pointOfSubCase);
            
            for (int j = 0; j < subcaseVals.size(); j++) {
                pointOfSubCase = subcaseVals[j];
                
                if (pointOfSubCase.x <= rezSquare.p1.x && pointOfSubCase.y > rezSquare.p1.y) {
                    rezSquare.p1 = pointOfSubCase;
                }
                if (pointOfSubCase.x >= rezSquare.p2.x && pointOfSubCase.y < rezSquare.p2.y) {
                    rezSquare.p2 = pointOfSubCase;
                }
                if (pointOfSubCase.x >= rezSquare.p3.x && pointOfSubCase.y > rezSquare.p3.y) {
                    rezSquare.p3 = pointOfSubCase;
                }
                if (pointOfSubCase.x <= rezSquare.p4.x && pointOfSubCase.y < rezSquare.p4.y) {
                    rezSquare.p4 = pointOfSubCase;
                }
            }
            rezSquare.c = {static_cast<float>((rezSquare.p1.x + rezSquare.p2.x + rezSquare.p3.x + rezSquare.p4.x) / 4.0),
                           static_cast<float>((rezSquare.p1.y + rezSquare.p2.y + rezSquare.p3.y + rezSquare.p4.y) / 4.0)};
            
            rezSubCases.push_back(rezSquare);
        }
        rezMap[lineString] = rezSubCases;
    }
    return rezMap;
}


// MAIN //


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    vector<Line> vecLine;
    square20 square = square20({0,0});
    
    // move on X
    for (int i = -kRange; i < kRange; i++) {
        
        // move on Y
        for (int j = -kSelfMargin; j < kRange; j++) {
            square = square20({(float)i, (float)j});
            
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
    
    map<string, vector<vector<point>>> subCases = findSubCases();
    map<string, vector<square20>> visualCases = convertToVisualCases(subCases);
    
    return 0;
}
