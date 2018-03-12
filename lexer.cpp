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
    { "eof",      "TokEof",     3 },
    // Keywords
    { "import",   "TokImport",  6 },
    { "static",   "TokStatic",  6 },
    { "if",       "TokIf",      2 },
    { "else",     "TokElse",    4 },
    { "do",       "TokDo",      2 },
    { "while",    "TokWhile",   5 },
    { "return",   "TokReturn",  6 },
    { "for",      "TokFor",     3 },
    { "break",    "TokBreak",   5 },
    { "typedef",  "TokTypeDef", 7 },
    { "const",    "TokConst",   5 },
    { "switch",   "TokSwitch",  5 },
    { "case",     "TokCase",    4 },
    { "default",  "TokDefault", 7 },
    { "continue", "TokContinue",8 },
    { "goto",     "TokGoto",    4 },
    { "sizeof",   "TokSizeOf",  6 },
    // Types : byte int string double void struct
    { "signed",   "TokSigned",  6 },
    { "unsigned", "TokUnsigned", 8 },
    { "char",     "TokChar",    4 },
    { "short",    "TokShort",   5 },
    { "int",      "TokInt",     3 },
    { "long",     "TokLong",    4 },
    { "float",    "TokFloat",   5 },
    { "double",   "TokDouble",  6 },
    { "void",     "TokVoid",    4 },
    { "class",    "TokClass",   5 },
    // Operators
    { "<<=",      "TokComBitShiftL",  3 },
    { ">>=",      "TokComBitShiftR",  3 },
    { "...",      "TokDotDotDot",     3 },
    
    { "<<",       "TokBitShiftL",     2 },
    { ">>",       "TokBitShiftR",     2 },
    { "++",       "TokUnaryInc",      2 },
    { "--",       "TokUnaryDec",      2 },
    { "+=",       "TokComAdd",        2 },
    { "-=",       "TokComSub",        2 },
    { "*=",       "TokComMul",        2 },
    { "/=",       "TokComDiv",        2 },
    { "&=",       "TokComBitAnd",     2 },
    { "|=",       "TokComBitOr",      2 },
    { "^=",       "TokComBitXor",     2 },
    { "==",       "TokEqual",         2 },
    { "!=",       "TokNotEqual",      2 },
    { "<=",       "TokLessThanEqual", 2 },
    { ">=",       "TokGreatorThenEqual", 2 },
    { "&&",       "TokConAnd",        2 },
    { "||",       "TokConOr",         2 },
    { "->",       "TokRightArrow",    2 },

    { "+",        "TokAdd",     1 },
    { "-",        "TokSub",     1 },
    { "*",        "TokMul",     1 },
    { "/",        "TokDiv",     1 },
    { "%",        "TokMod",     1 },
    { "=",        "TokAssign",  1 },
    { "&",        "TokBitAnd",  1 },
    { "|",        "TokBitOr",   1 },
    { "^",        "TokBitXor",  1 },
    { "~",        "TokBitNot",  1 },
    { "<",        "TokLessThan",1 },
    { ">",        "TokGreatorThan", 1 },
    { "!",        "TokConNot",  1 },
    { "?",        "TokQuestion", 1 },

    { "(",        "TokParenOpen", 1 },
    { ")",        "TokParenClose", 1 },
    { "{",        "TokBraceOpen", 1 },
    { "}",        "TokBraceClose", 1 },
    { "[",        "TokBracketOpen", 1 },
    { "]",        "TokBracketClose", 1 },
    
    { ",",        "TokComma",   1 },
    { ".",        "TokDot",     1 },
    { ":",        "TokColon",   1 },
    { ";",        "TokSemiColon", 1 },

    { "\"",       "TokDoubleQuot", 1 },
    { "\'",       "TokSingleQuot", 1 },
   
    { "/*",       "TokStarRemark", 2 },
    { "//",       "TokLineRemark", 2 },

    { "ID",       "TokIdentifier", 0 },
    { "IL",       "TokIntegerLiteral", 0},
    { "SL",       "TokStringLiteral", 0},
    { "CL",       "TokCharactorLiteral", 0},
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
    /* Numbers */
    if (isNumber(*read_pos)) {
      tok.type = (TokenType)TokIntegerLiteral;
      tok.line = line;
      tok.col = col;
      tok.len = 0;
      tok.c = read_pos;
      while(isAlpha(*read_pos) || isNumber(*read_pos)) {
        col++;
        tok.len++;
        read_pos++;
      }
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

  Token Tokenizer::GetToken(int pos) {
    return tokens_[pos];
  }

  Token Tokenizer::GetCurToken(int look) {
    return tokens_[cur_tok_+look];
  }

  void Tokenizer::ConsumeToken(int count) {
    cur_tok_ += count;
  }

  bool Tokenizer::isToken(int look, TokenType toktype) {
    return (GetCurToken(look).type == toktype);
  }

}; // end namespace Lexer

