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
    // Keywords
    { "import", "TokImport",  6 }, // 1
    { "static", "TokStatic",  6 }, // 2
    // Conditions : if else while return for break
    { "if",     "TokIf",      2 }, // 3
    { "else",   "TokElse",    4 }, // 4
    { "while",  "TokWhile",   5 }, // 5
    { "return", "TokReturn",  6 }, // 6
    { "for",    "TokFor",     3 }, // 7
    { "break",  "TokBreak",   5 }, // 8
    // Types : byte int string double void struct
    { "unsigned","TokUnsigned", 8 }, // 9
    { "char",   "TokChar",    4 }, // 10
    { "short",  "TokShort",   5 }, // 11
    { "int",    "TokInt",     3 }, // 12
    { "long",   "TokLong",    4 }, // 13
    { "float",  "TokFloat",   5 }, // 14
    { "double", "TokDouble",  6 }, // 15
    { "void",   "TokVoid",    4 }, // 16
    { "class",  "TokClass",   5 }, // 17
    // Operators
    { "<<=",    "TokComBitShiftL", 3 }, // 18
    { ">>=",    "TokComBitShiftR", 3 }, // 19

    { "<<",     "TokBitShiftL", 2 }, // 20
    { ">>",     "TokBitShiftR", 2 }, // 21
    { "++",     "TokUnaryInc",2 }, // 22
    { "--",     "TokUnaryDec",2 }, // 23
    { "+=",     "TokComAdd",  2 }, // 24
    { "-=",     "TokComSub",  2 }, // 25
    { "*=",     "TokComMul",  2 }, // 26
    { "/=",     "TokComDiv",  2 }, // 27
    { "&=",     "TokComBitAnd", 2 }, // 28
    { "|=",     "TokComBitOr", 2 },  // 29
    { "^=",     "TokComBitXor", 2 }, // 30
    { "==",     "TokEqual",   2 },   // 31
    { "!=",     "TokNotEqual", 2 },  // 32
    { "<=",     "TokLessThanEqual", 2 }, // 33
    { ">=",     "TokGreatorThenEqual", 2 }, // 34
    { "&&",     "TokConAnd",  2 }, // 35
    { "||",     "TokConOr",   2 }, // 36

    { "+",      "TokAdd",     1 }, // 37
    { "-",      "TokSub",     1 }, // 38
    { "*",      "TokMul",     1 }, // 39
    { "/",      "TokDiv",     1 }, // 40
    { "%",      "TokMod",     1 }, // 41
    { "=",      "TokAssign",  1 }, // 42
    { "&",      "TokBitAnd",  1 }, // 43
    { "|",      "TokBitOr",   1 }, // 44
    { "^",      "TokBitXor",  1 }, // 45
    { "~",      "TokBitNot",  1 }, // 46
    { "<",      "TokLessThan",1 }, // 47
    { ">",      "TokGreatorThan", 1 }, // 48
    { "!",      "TokConNot",  1 }, // 49
    { "?",      "TokQuestion", 1 }, // 50

    { "(",      "TokParenOpen", 1 }, // 51
    { ")",      "TokParenClose", 1 }, // 52
    { "{",      "TokBraceOpen", 1 }, // 53
    { "}",      "TokBraceClose", 1 }, // 54
    { "[",      "TokBracketOpen", 1 }, // 55
    { "]",      "TokBracketClose", 1 }, // 56
    
    { ",",      "TokComma",   1 }, // 57
    { ".",      "TokDot",     1 }, // 58
    { ";",      "TokSemiColon", 1 }, // 59

    { "\"",     "TokDoubleQuot", 1 }, // 60
    { "\'",     "TokSingleQuot", 1 }, // 61
   
    { "/*",     "TokStarRemark", 2 }, // 62
    { "//",     "TokLineRemark", 2 }, // 63

    { "ID",     "TokIdentifier", 0 }, // 64
    { "IL",     "TokIntegerLiteral", 0}, // 65
    { "SL",     "TokStringLiteral", 0}, // 66
    { "CR",     "TokCharactorLiteral", 0}, // 67
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

    // Double quotation : StringLiteral ex) "abcd012" => abcd012
    if (*read_pos == *TokInfo[TokDoubleQuot].str) {
      tok.type = (TokenType)TokStringLiteral; 
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

    // Single quotation : CharactorLiteral ex) 'A' => A
    // Charactorliteral allow only 1 charactor.
    if (*read_pos == *TokInfo[TokSingleQuot].str) {
      tok.type = (TokenType)TokCharactorLiteral;
      tok.line = line;
      tok.col = col;
      tok.len = 0;
      tok.c = read_pos + TokInfo[TokSingleQuot].length;
      MoveNext(read_pos, 2);
      col+=2;
      tok.len = 1;
      if (*read_pos != *TokInfo[TokSingleQuot].str) {
        if (*read_pos == '\0') {
          err_diag_->Print(ErrorDiag::Err_Lexer_UnexpectedEof, tok.line, tok.col);
          tok.type = TokUnknown;
          return tok;
        }
        else {
          err_diag_->Print(ErrorDiag::Err_Lexer_WrongCharLiteral, tok.line, tok.col);
          tok.type = TokUnknown;
          return tok;
        }
      }

      MoveNext(read_pos, TokInfo[TokSingleQuot].length);
    }

    /* Identifier */
    tok.type = (TokenType)TokIdentifier;
    tok.line = line;
    tok.col = col;
    tok.len = 0;
    tok.c = read_pos;

    bool have_alpha = false;
    while(isAlpha(*read_pos) || isNumber(*read_pos)) {
      if (isAlpha(*read_pos)) 
        have_alpha = true;
      col++;
      tok.len++;
      read_pos++;
    }
    if (!have_alpha) { // only number
      tok.type = (TokenType)TokIntegerLiteral;
    }

    return tok;
  }

  Token Tokenizer::GetToken(int look) {
    return tokens_[cur_tok_+look];
  }

  void Tokenizer::ConsumeToken(int count) {
    cur_tok_ += count;
  }

  bool Tokenizer::isToken(int look, TokenType toktype) {
    return (GetToken(look).type == toktype);
  }

}; // end namespace Lexer

