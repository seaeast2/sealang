#ifndef _lexer_h_
#define _lexer_h_

#include <vector>
#include "error_diagnosis.h"

namespace Lexer {
  enum TokenType {
    TokEof,
    // Keywords
    TokImport,
    TokStatic,

    // Conditions : if else while return for break
    TokIf,
    TokElse,
    TokWhile,
    TokReturn,
    TokFor,
    TokBreak,

    // Types : byte int string double void struct
    TokByte,
    TokInt,
    TokString,
    TokReal,
    TokVoid,
    TokClass,

    // Operators
    TokComBitShiftL,  // <<=
    TokComBitShiftR,  // >>=

    TokBitShiftL,     // <<
    TokBitShiftR,     // >>
    TokUnaryInc,      // ++
    TokUnaryDec,      // --
    TokComAdd,        // +=
    TokComSub,        // -= 
    TokComMul,        // *=
    TokComDiv,        // /=
    TokComBitAnd,     // &=
    TokComBitOr,      // |=
    TokComBitXor,     // ^=
    TokEqual,         // ==
    TokNotEqual,      // !=
    TokLessThanEqual, // <=
    TokGreatorThenEqual, // >=
    TokConAnd,        // &&
    TokConOr,         // ||

    TokAdd,           // +
    TokSub,           // -
    TokMul,           // *
    TokDiv,           // /
    TokMod,           // %
    TokAssign,        // =
    TokBitAnd,        // &
    TokBitOr,         // |
    TokBitXor,        // ^
    TokBitNot,        // ~
    TokLessThan,      // < 
    TokGreatorThan,   // >
    TokConNot,        // !
    TokQuestion,      // ?

    //  Parentheses
    TokParenOpen,     // (
    TokParenClose,    // )
    TokBraceOpen,     // {
    TokBraceClose,    // }
    TokBracketOpen,   // [
    TokBracketClose,  // ]

    /* Etc : , . ; */
    TokComma,         // ,
    TokDot,           // .
    TokSemiColon,     // ;

    // Quoto
    TokDoubleQuot,    // "
    TokSingleQuot,    // '

    /* Remark */
    TokStarRemark,    // /*
    TokLineRemark,    // //

    /* Identifier */
    TokIdentifier,
    TokUnknown
  };

  struct Token {
    TokenType type;
    int line; // line at token.
    int col; // colunm at token.

    int len; // token length
    const char *c; // token position in text stream
  };

  // Produce text stream to token.
  class Tokenizer {
    const char* src_; // source file
    int src_len_;// source length

    int cur_tok_; // last token position
    std::vector<Token> tokens_; // token made

    ErrorDiag::LexerDiag* err_diag_; // error diagnosis

    private:
    // Produce token from text stream.
    Token MakeToken(const char*& read_pos, int& line, int& col);
    Tokenizer() {}
    public:
    Tokenizer(const char* src);
    ~Tokenizer();

    bool CompileTokens();
    Token GetToken(int look=0);
    void ConsumeToken(int count=1);

    // check if current token is 'toktype'.
    bool isToken(int look, TokenType toktype);
    bool isName(int look);
    bool isStorage(int look);
    bool isType(int look);
  };

  void PrintToken(Token& tok);
};
#endif
