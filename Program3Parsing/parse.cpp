/*
 * parse.cpp
 */

#include "parse.h"

namespace Parser {
    bool pushed_back = false;
    Token    pushed_token;
    
    static Token GetNextToken(istream *in, int *line) {
        if( pushed_back ) {
            pushed_back = false;
            return pushed_token;
        }
        return getNextToken(in, line);
    }
    
    static void PushBackToken(Token& t) {
        if( pushed_back ) {
            abort();
        }
        pushed_back = true;
        pushed_token = t;
    }
    
}

static int error_count = 0;

void ParseError(int line, string msg) {}

ParseTree *Prog(istream *in, int *line)
{
    ParseTree *sl = Slist(in, line);
    
    if( sl == 0 )
        ParseError(*line, "No statements in program");
    
    if( error_count )
        return 0;
    
    return sl;
}

// Slist is a Statement followed by a Statement List
ParseTree *Slist(istream *in, int *line) {
    ParseTree *s = Stmt(in, line);
    if( s == 0 )
        return 0;
    
    Token sc = Parser::GetNextToken(in, line);
    if( sc != SC ) {
        ParseError(*line, "Missing semicolon");
        delete s;
        return 0;
    }
    
    ParseTree *sl = Slist(in, line);
    if( sl == 0 ) { return s; }
    return new StmtList(s, sl);
}

ParseTree *Stmt(istream *in, int *line) { return 0; }

ParseTree *VarStmt(istream *in, int *line) { return 0; }

ParseTree *SetStmt(istream *in, int *line) { return 0; }

ParseTree *PrintStmt(istream *in, int *line) {
    int l = *line;
    ParseTree *ex = Expr(in, line);
    if( ex == 0 ) {
        ParseError(*line, "Missing expression after print");
        return 0;
    }
    return new Print(l, ex);
}

ParseTree *RepeatStmt(istream *in, int *line) { return 0; }

ParseTree *Expr(istream *in, int *line) {
    ParseTree *t1 = Term(in, line);
    if( t1 == 0 ) { return 0; }
    while ( true ) {
        Token t = Parser::GetNextToken(in, line);
        
        if( t != PLUS && t != MINUS ) {
            Parser::PushBackToken(t);
            return t1;
        }
        ParseTree *t2 = Term(in, line);
        if( t2 == 0 ) {
            ParseError(*line, "Missing expression after operator");
            return 0;
        }
        /*
-----------------------------------------------------------
        if( t == PLUS )
            t1 = new PlusExpr(t.GetLinenum(), t1, t2);
        else
            t1 = new MinusExpr(t.GetLinenum(), t1, t2);
         */
    }
}

ParseTree *Term(istream *in, int *line) { return 0; }

ParseTree *Factor(istream *in, int *line) { return 0; }

ParseTree *Primary(istream *in, int *line) { return 0; }

