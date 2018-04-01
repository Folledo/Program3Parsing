//
//  main.cpp
//  Program3Parsing
//
//  Created by Samuel Folledo on 3/30/18.
//  Copyright © 2018 Samuel Folledo. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

double parseFormula();
double parseSum();
double parseProduct();
double parseFactor();
double parseNumber();

char * x;

double parseFormula() {
    double result = parseSum();
    if (*x == '\0') { return result; }
    else {
        cout<<"expected end but found "<<*x;
        return -1;
    }
}

double parseSum() {
    double left = parseProduct();
    while (*x == '+') {
        ++x;
        double right = parseProduct();
        left += right;
    }
    return left;
}

double parseProduct() {
    double left = parseFactor();
    while (*x == '*') {
        ++x;
        double right = parseFactor();
        left *= right;
    }
    return left;
}

double parseFactor() {
    if (*x >= '0' &&  *x <= '9') { return parseNumber(); }
    else if (*x == '(') {
        ++x; //consume (
        double sum = parseSum();
        ++x; //consume )
        return sum;
    } //else if (*x == ')'){ ++x; return -1;}
    else {
        //if (*x == ')'){ ++x;}
        cout<<*x<<" is not a digit\n";
        return -1;
    }
}

double parseNumber() {
    double num = 0;
    while (*x >= '0' && *x <= '9') {
        num = num * 10;
        num = num + *x - '0';
        ++x;
    }
    if (*x == '.') {
        ++x; double weight = 1;
        while (*x >= '0' && *x <= '9') {
            weight /= 10; //weight = weight / 10
            double scaled = (*x - '0') * weight;
            num += scaled; //num = num + scaled
            ++x;
        }
    }
    return num;
}


int main(int argc, const char * argv[]) {
    
    x = "10*2.5+1.25+(1.25*3)";
    double result = parseFormula();
    cout<<result<<endl;
    
    return 0;
}
