#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
using std::vector;
using std::map;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE };

class ParseTree {
    int            linenum;
    ParseTree    *left;
    ParseTree    *right;
    
public:
    ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
    : linenum(linenum), left(l), right(r) {}
    
    virtual ~ParseTree() {
        delete left;
        delete right;
    }
    
    int GetLineNumber() const { return linenum; }
    
    virtual NodeType GetType() const { return ERRTYPE; }
};

class StmtList : public ParseTree {
    
public:
    StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
};

class VarDecl : public ParseTree {};

class Assignment : public ParseTree {};

class Print : public ParseTree {
public:
    Print(int line, ParseTree *e) : ParseTree(line, e) {}
};

class Repeat : public ParseTree {};

class PlusExpr : public ParseTree {};

class MinusExpr : public ParseTree {};

class TimesExpr : public ParseTree {};

class SliceExpr : public ParseTree {};

class SliceOperand : public ParseTree {};

class IConst : public ParseTree {
    int val;
    
public:
    IConst(Token& t) : ParseTree(t.GetLinenum()) {
        val = stoi(t.GetLexeme());
    }
    
    NodeType GetType() const { return INTTYPE; }
};

class SConst : public ParseTree { };

class Ident : public ParseTree { };


#endif /* PARSETREE_H_ */
