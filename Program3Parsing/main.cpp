#include <iostream>
#include <string>
#include <fstream>
#include "projlex.h"
//#include "parse.h"
#include <fstream>
using std::ifstream;
#include <map>
using std::map;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

double parseFormula();
double parseSum();
double parseProduct();
double parseFactor();
double parseNumber();

char const * x;

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
    } else {
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

//---------------------------INT MAIN()--------------------------------
int main(int argc, const char * argv[]) {
    
    int lineNumber = 0;
    Token tok;
    
    istream *in = &cin;
    ifstream file;
    bool tFlag = false, fFlag = false;
    
    int tokCount = 0;
    int stringCount = 0;
    map<TType,int> counts;
    map<string,bool> allStrings;
    map<string,int> identCount;
    
    for( int i = 1; i < argc; i++ ) {
        string arg( argv[i] );
        if( arg == "-t" ) { tFlag = true; }
        else if( arg[0] == '-' ) {
            cerr << "INVALID FLAG " << arg << endl; return 0;
        } else if ( in != &cin ) {
            cerr << "TOO MANY FILE NAMES" << endl;  return 0;
        } else {
            file.open(arg);
            if( file.is_open() == false ) {
                cerr << "UNABLE TO OPEN " << arg << endl; return 0;
            }
            in = &file;
        }
    }
    
//------------------------------------------------------------------------
    cout<<"getting next token\n";
    while( (tok = getNextToken(in, &lineNumber)) != DONE && tok != ERR ){
        cout<<"-----starting cout token\n";
        if( tFlag )
            cout << tok << endl;
        
        ++tokCount;
        if( tok == SCONST ) {
            stringCount++;
            allStrings[ tok.GetLexeme() ] = true;
        }
        else if( tok == IDENT )
            identCount[ tok.GetLexeme() ]++;
    }
    cout<<"end of token\n";

//TOKEN = error on tokens
    if( tok == ERR ) {
        cout << "Error on line " << lineNumber << " (" << tok.GetLexeme() << ")" << endl;
        return 0;
    }
    cout<<"end of error token\n";
    cout<<"starting common token\n";
//TOKEN = for finding the most common identifier
    if( tFlag && identCount.size() > 0 ) {
        cout << "Most Common Identifier: ";
        // what is the most common identifier?
        // first, find the largest counter in the map
        int maxcnt = 0;
        for( auto it = identCount.begin(); it != identCount.end(); it++ )
            if( it->second > maxcnt )
                maxcnt = it->second;
        
        // now, print all the identifiers with that length
        bool printed = false;
        for( auto it = identCount.begin(); it != identCount.end(); it++ ) {
            if( it->second != maxcnt )
                continue;
            if( printed )
                cout << ", ";
            printed = true;
            cout << it->first;
        }
        cout << endl;
    }
    cout<<"ending of common token\n";
    
    cout<<"starting stats token\n";
//TOKEN = total lines/tokens/strings and length of long string
    if( tFlag ) {
        cout << "Total lines: " << lineNumber << endl;
        cout << "Total tokens: " << tokCount << endl;
        if( tokCount > 0 ) {
            cout << "Total strings: " << stringCount << endl;
            if( stringCount > 0 ) {
                unsigned maxSlen = 0;
                for( auto it = allStrings.begin(); it != allStrings.end(); it++ ) {
                    if( it->first.length() > maxSlen )
                        maxSlen = it->first.length();
                }
                cout << "Length of longest string: " << maxSlen << endl;
            }
        }
    }
    cout<<"ending stats token\n";
    
    
//------------------------------------------------------------------------
    if (tFlag) {
        cout<<"Trace activated\n";
    }
    
    x = "10*2.5+1.25+(1.25*3)";
    double result = parseFormula();
    cout<<result<<endl;
    
    return 0;
}
