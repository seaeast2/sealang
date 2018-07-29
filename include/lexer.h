#ifndef _lexer_h_
#define _lexer_h_

#include <vector>
#include "error_diagnosis.h"

namespace Lexer {
  enum TokenType {
    TokUnknown = 0,
    TokEof,
    TokImport, // Keywords
    TokStatic,
    TokIf,
    TokElse,
    TokDo,
    TokWhile,
    TokReturn,
    TokFor,
    TokBreak,
    TokTypeDef,
    TokConst,
    TokSwitch,
    TokCase,
    TokDefault,
    TokContinue,
    TokGoto,
    TokSizeOf,
    TokSigned,    // unsigned  // Types : byte int string double void struct
    TokUnsigned,  // signed
    TokChar,      // char
    TokShort,     // short
    TokInt,       // int
    TokLong,      // long
    TokFloat,     // float
    TokDouble,    // double
    TokVoid,      // void
    TokClass,     // class
    TokComBitShiftL,  // <<= // Operators
    TokComBitShiftR,  // >>=
    TokDotDotDot,     // ... // Triple operator
    TokBitShiftL,     // << // Double operator
    TokBitShiftR,     // >>
    TokUnaryInc,      // ++
    TokUnaryDec,      // --
    TokComAdd,        // +=
    TokComSub,        // -= 
    TokComMul,        // *=
    TokComDiv,        // /=
    TokComMod,        // %=
    TokComBitAnd,     // &=
    TokComBitOr,      // |=
    TokComBitXor,     // ^=
    TokEqual,         // ==
    TokNotEqual,      // !=
    TokLessThanEqual, // <=
    TokGreatorThenEqual,// >=
    TokConAnd,        // &&
    TokConOr,         // ||
    TokRightArrow,    // ->
    TokAdd,           // + // Single operators
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
    TokParenOpen,     // ( //  Parentheses
    TokParenClose,    // )
    TokBraceOpen,     // {
    TokBraceClose,    // }
    TokBracketOpen,   // [
    TokBracketClose,  // ]
    TokComma,         // , /* Etc : , . ; */
    TokDot,           // .
    TokColon,         // :
    TokSemiColon,     // ;
    TokDoubleQuot,    // "  // Quoto
    TokSingleQuot,    // '
    TokStarRemark,    // /* /* Remark */
    TokLineRemark,    // //
    TokIdentifier,    // ex) abcd012  /* Identifier */
    TokIntegerLiteral, // ex) 12345
    TokStringLiteral, // ex) "test"
    TokCharactorLiteral, // ex) 'A'
    TokEnd // 84
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
    Token MakeToken(const char*& src, int& line, int& col);
    Tokenizer() {}
    public:
    Tokenizer(const char* src);
    ~Tokenizer();

    bool CompileTokens();
    Token GetToken(int pos);
    Token GetCurToken(int look=0);
    void ConsumeToken(int count=1);

    int GetTokPos() { return cur_tok_; }
    void SetTokPos(int tokpos) { cur_tok_ = tokpos; }

    // check if current token is 'toktype'.
    bool isToken(int look, TokenType toktype);
  };

  void PrintToken(Token& tok);
};
#endif
