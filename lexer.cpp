#include <cstdio>
#include <cstring>

#include "lexer.h"

using namespace std;

namespace Lexer {
#define MoveNext(c, l) { (c) = (c + l); }

#define UpAlpha(c) ((c)>66 && (c)<91 ? true : false)
#define LowAlpha(c) ((c)>96 && (c)<123 ? true : false)
#define isAlpha(c) (UpAlpha(c) || LowAlpha(c) ? true : false)
#define isNumber(c) ((c)>47 && (c)<58 ? true : false)

  struct TokenInfo {
    char* str;
    char* name;
    int length;
  };

  TokenInfo TokInfo[] = {
    { "eof",    "TokEof",     3 }, // 0

    { "import", "TokImport",  6 }, // 1
    { "static", "TokStatic",  6 }, // 2

    { "if",     "TokIf",      2 }, // 3
    { "else",   "TokElse",    4 }, // 4
    { "while",  "TokWhile",   5 }, // 5
    { "return", "TokReturn",  6 }, // 6
    { "for",    "TokFor",     3 }, // 7
    { "break",  "TokBreak",   5 }, // 8

    { "byte",   "TokByte",    4 }, // 9
    { "int",    "TokInt",     3 }, // 10
    { "string", "TokString",  6 }, // 11
    { "real",   "TokReal",    4 }, // 12
    { "void",   "TokVoid",    4 }, // 13
    { "class", "TokClass",  6 }, // 14

    { "<<=",    "TokComBitShiftL", 3 }, // 15
    { ">>=",    "TokComBitShiftR", 3 }, // 16

    { "<<",     "TokBitShiftL", 2 }, // 17
    { ">>",     "TokBitShiftR", 2 }, // 18
    { "++",     "TokUnaryInc",2 }, // 19
    { "--",     "TokUnaryDec",2 }, // 20
    { "+=",     "TokComAdd",  2 }, // 21
    { "-=",     "TokComSub",  2 }, // 22
    { "*=",     "TokComMul",  2 }, // 23
    { "/=",     "TokComDiv",  2 }, // 24
    { "&=",     "TokComBitAnd", 2 }, // 25
    { "|=",     "TokComBitOr", 2 },  // 26
    { "^=",     "TokComBitXor", 2 }, // 27
    { "==",     "TokEqual",   2 },   // 28
    { "!=",     "TokNotEqual", 2 },  // 29
    { "<=",     "TokLessThanEqual", 2 }, // 30
    { ">=",     "TokGreatorThenEqual", 2 }, // 31
    { "&&",     "TokConAnd",  2 }, // 32
    { "||",     "TokConOr",   2 }, // 33

    { "+",      "TokAdd",     1 }, // 34
    { "-",      "TokSub",     1 }, // 35
    { "*",      "TokMul",     1 }, // 36
    { "/",      "TokDiv",     1 }, // 37
    { "%",      "TokMod",     1 }, // 38
    { "=",      "TokAssign",  1 }, // 39
    { "&",      "TokBitAnd",  1 }, // 40
    { "|",      "TokBitOr",   1 }, // 41
    { "^",      "TokBitXor",  1 }, // 42
    { "~",      "TokBitNot",  1 }, // 43
    { "<",      "TokLessThan",1 }, // 44
    { ">",      "TokGreatorThan", 1 }, // 45
    { "!",      "TokConNot",  1 }, // 46
    { "?",      "TokQuestion", 1 }, // 47

    { "(",      "TokParenOpen", 1 }, // 48
    { ")",      "TokParenClose", 1 }, // 49
    { "{",      "TokBraceOpen", 1 }, // 50
    { "}",      "TokBraceClose", 1 }, // 51
    { "[",      "TokBracketOpen", 1 }, // 52
    { "]",      "TokBracketClose", 1 }, // 53
    
    { ",",      "TokComma",   1 }, // 54
    { ".",      "TokDot",     1 }, // 55
    { ";",      "TokSemiColon", 1 }, // 56
    { "\"",     "TokDoubleQuot", 1 }, // 57
    { "\'",     "TokSingleQuot", 1 }, // 58
   
    { "/*",     "TokStarRemark", 2 }, // 59
    { "//",     "TokLineRemark", 2 }, // 60

    { "ID",     "TokIdentifier", 0 }, // 61
  };

  void PrintToken(Token& tok) {
    printf("TokenType : %s, line : %d, col : %d \n", 
        TokInfo[tok.type].name, tok.line, tok.col);
    //printf("scr : %s \n", tok.c);
  }

  Tokenizer::Tokenizer(const char* src) : src_(src) {
    src_len_ = strlen(src_);

    err_diag_ = new ErrorDiag::LexerDiag();
  }

  Tokenizer::~Tokenizer() {
    delete err_diag_;
  }

  bool Tokenizer::CompileTokens() {
    Token tok;

    const char* srcpos = src_;
    int line = 1, col = 1;
    while (true) {
      tok = MakeToken(srcpos, line, col);
      if (tok.type == TokUnknown)
        return false;

      tokens_.push_back(tok);
      if (tok.type == TokEof)
        break;
    }

    // vector shrink
    std::vector<Token> tmp(tokens_);
    tokens_.swap(tmp);
    return true;
  }

  Token Tokenizer::MakeToken(const char*& read_pos, int& line, int& col) {
    Token tok;
    tok.type = TokUnknown;

    // Remove space
    while(true) {
      if (*read_pos == ' ' || *read_pos == '\t' || *read_pos == '\r') {
        col++;
        read_pos++;
      }
      else if (*read_pos == '\n') {
        line++;
        col = 1;
        read_pos++;
      }
      else
        break;
    }

    // check eof
    if (*read_pos == '\0') {
      tok.type = TokEof;
      tok.line = line;
      tok.col = col;
      tok.len = 1;
      tok.c = read_pos;
      return tok;
    }

    // check whether token is keyword or type
    for (int i = TokImport; i < TokClass + 1; i++) {
      char match = 0;
      for (int j = 0; j < TokInfo[i].length; j++) {
        match |= (*(read_pos + j) ^ TokInfo[i].str[j]);
      }
      if (!match) {
        // check if it is keywork
        if (!isAlpha(*(read_pos+TokInfo[i].length)) && 
            !isNumber(*(read_pos+TokInfo[i].length))) {
          tok.type = (TokenType)i;
          tok.line = line;
          tok.col = col;
          tok.len = TokInfo[i].length;
          tok.c = read_pos;
          
          col += TokInfo[i].length;
          MoveNext(read_pos, TokInfo[i].length);
          return tok;
        }
      }
    }

    /* Star Remark */
    char match = 0;
    for (int j = 0; j < TokInfo[TokStarRemark].length; j++) {
      match |= (*(read_pos + j) ^ TokInfo[TokStarRemark].str[j]);
    }
    if (!match) {
      tok.type = (TokenType)TokStarRemark;
      tok.line = line;
      tok.col = col;
      tok.c = read_pos;
      
      col += TokInfo[TokStarRemark].length;
      MoveNext(read_pos, TokInfo[TokStarRemark].length);
      for (;;) {
        if (*read_pos == '*' && *(read_pos+1) == '/') { // match
          col += TokInfo[TokStarRemark].length;
          read_pos += TokInfo[TokStarRemark].length;
          return tok;
        }
        
        if (*read_pos == '\n') {
          line++;
          col = 1;
          read_pos++;
        }

        if (*read_pos == '\0') {
          err_diag_->Print(ErrorDiag::Err_Lexer_UnexpectedEof, tok.line, tok.col);
          tok.type = TokUnknown; // check error
          return tok;
        }

        // move forward
        tok.len++;
        col++;
        read_pos++;
      }
    }

    /* line Remark '//' */
    match = 0;
    for (int j = 0; j < TokInfo[TokLineRemark].length; j++) {
      match |= (*(read_pos + j) ^ TokInfo[TokLineRemark].str[j]);
    }
    if (!match) {
      tok.type = (TokenType)TokLineRemark;
      tok.line = line;
      tok.col = col;
      tok.len = TokInfo[TokLineRemark].length;
      tok.c = read_pos;
      
      col += TokInfo[TokLineRemark].length;
      MoveNext(read_pos, TokInfo[TokLineRemark].length);
      while(true) { 
        if (*read_pos == '\n') {
          line++;
          col = 1;
          read_pos++;
          return tok;
        }

        if (*read_pos == '\0') {
          err_diag_->Print(ErrorDiag::Err_Lexer_UnexpectedEof, tok.line, tok.col);
          tok.type = TokUnknown;
          return tok;
        }

        // move forward
        tok.len++;
        col++;
        read_pos++;
      }
    }

    /* check if it is bin op*/
    for (int i = TokComBitShiftL; 
        i < TokSemiColon + 1; i++) {
      char match = 0;
      for (int j = 0; j < TokInfo[i].length; j++) {
        match |= (*(read_pos + j) ^ TokInfo[i].str[j]);
      }
      if (!match) {
        tok.type = (TokenType)i;
        tok.line = line;
        tok.col = col;
        tok.len = TokInfo[i].length;
        tok.c = read_pos;
        
        col += TokInfo[i].length;
        MoveNext(read_pos, TokInfo[i].length);
        return tok;
      }
    }

    /* Double quotation */
    if (*read_pos == *TokInfo[TokDoubleQuot].str) {
      tok.type = (TokenType)TokDoubleQuot;
      tok.line = line;
      tok.col = col;
      tok.len = 0;
      tok.c = read_pos + TokInfo[TokDoubleQuot].length;
      MoveNext(read_pos, TokInfo[TokDoubleQuot].length);
      while(*read_pos != *TokInfo[TokDoubleQuot].str) {
        if (*read_pos == '\0') {
          err_diag_->Print(ErrorDiag::Err_Lexer_UnexpectedEof, tok.line, tok.col);
          tok.type = TokUnknown;
          return tok;
        }
        if (*read_pos == '\n') {
          line++;
          col = 1;
        }

        tok.len++;
        col++;
        read_pos++;
      }
      MoveNext(read_pos, TokInfo[TokDoubleQuot].length);
      return tok;
    }

    /* Single quotation */
    if (*read_pos == *TokInfo[TokSingleQuot].str) {
      tok.type = (TokenType)TokSingleQuot;
      tok.line = line;
      tok.col = col;
      tok.len = 0;
      tok.c = read_pos + TokInfo[TokSingleQuot].length;
      MoveNext(read_pos, TokInfo[TokSingleQuot].length);
      while(*read_pos != *TokInfo[TokSingleQuot].str) {
        if (*read_pos == '\0') {
          err_diag_->Print(ErrorDiag::Err_Lexer_UnexpectedEof, tok.line, tok.col);
          tok.type = TokUnknown;
          return tok;
        }

        col++;
        tok.len++;
        read_pos++;
      }
      MoveNext(read_pos, TokInfo[TokSingleQuot].length);
    }

    /* Identifier */
    tok.type = (TokenType)TokIdentifier;
    tok.line = line;
    tok.col = col;
    tok.len = 0;
    tok.c = read_pos;
    while(isAlpha(*read_pos) || isNumber(*read_pos)) {
      col++;
      tok.len++;
      read_pos++;
    }

    return tok;
  }

  Token Tokenizer::GetToken(int look) {
    return tokens_[cur_tok_+look];
  }

  void Tokenizer::ConsumeToken(int count) {
    cur_tok_ += count;
  }

  //=============================================================================
  // Tokenizer
  bool Tokenizer::isToken(int look, TokenType toktype) {
    return (GetToken(look).type == toktype);
  }

  // name : id
  bool Tokenizer::isName(int look) {
    return isToken(look, Lexer::TokIdentifier);
  }

  bool Tokenizer::isStorage(int look) {
    return isToken(look, Lexer::TokStatic);
  }

  bool Tokenizer::isType(int look) {
    if (isToken(look, TokByte))
      return true;
    else if (isToken(look, TokInt))
      return true;
    else if (isToken(look, TokString))
      return true;
    else if (isToken(look, TokReal))
      return true;
    else if (isToken(look, TokVoid))
      return true;
    else if (isToken(look, TokClass))
      return true;

    return false;
  }

}; // end namespace Lexer

