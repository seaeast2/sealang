#include <assert.h>
#include "parser.h"
#include <stdlib.h>

using namespace Lexer;

namespace Parser {
  eResult SyntaxAnalyzer::TraverseRule(int entry) {
    Rule rule = rules_[entry];
    eResult res;
    int tok_pos;

    switch(rule.action_) {
      case Repeat:
        {
          while(true) {
            tok_pos = tokenizer_->GetTokPos(); // backup token position
            for (int i = 0; rule.sub_rules_[i] > -1; i++) {
              res = TraverseRule(rule.sub_rules_[i]);
              if (res == False)
                if (rules_[rule.sub_rules_[i]].action_ != Options) {
                  tokenizer_->SetTokPos(tok_pos); // restore token position.
                  // Run action
                  (this->*rule_actions_[entry])();
                  return True;
                }
              else if (res == Error)
                return Error;
            }
          };
        }
        break;

      case Select:
        {
          tok_pos = tokenizer_->GetTokPos(); // backup token position
          for (int i = 0; rule.sub_rules_[i] > -1; i++) {
            res = TraverseRule(rule.sub_rules_[i]);
            if (res == True) {
              // Run action
              (this->*rule_actions_[entry])();
              return True; // found matching
            }
            else if (res == False) {
              tokenizer_->SetTokPos(tok_pos); // restore token position.
              return False;
            }
            else if (res == Error)
              return Error;
          }
          return False;
        }
        break;

      case Sequence:
        {
          int matching_count = 0;
          int i = 0;
          tok_pos = tokenizer_->GetTokPos(); // backup token position
          for (; rule.sub_rules_[i] > -1; i++) {
            res = TraverseRule(rule.sub_rules_[i]);
            if (res == True) { // matched
              matching_count++;
            }
            else if (res == False) { // not matching
              if (rules_[rule.sub_rules_[i]].action_ == Options) {
                matching_count++;
              }
            }
            else
              return Error;
          }

          if (matching_count == i) {
            // if every rules are matching, run action.
            (this->*rule_actions_[entry])();
            return True;
          }
          tokenizer_->SetTokPos(tok_pos); // restore token position.
          return False; // unmatching
        }
        break;

      case Options:
        {
          int matching_count = 0;
          int i = 0;
          tok_pos = tokenizer_->GetTokPos(); // backup token position
          for (; rule.sub_rules_[i] > -1; i++) {
            res = TraverseRule(rule.sub_rules_[i]);
            if (res == True) {
              matching_count++;
            }
            else
              return Error;
          }

          if (matching_count == i) {
            // Run action
            (this->*rule_actions_[entry])();
            return True;
          }

          tokenizer_->SetTokPos(tok_pos); // restore token position.
          return False; // unmatching
        }
        break;

      case Terminal:
        {
          if(tokenizer_->isToken(0, Lexer::TokenType(rule.sub_rules_[0]))) {
            // Run action
            (this->*rule_actions_[rule.sub_rules_[0]])();
            tokenizer_->ConsumeToken(1);
            return True;
          }
          return False;
        }
        break;

      default:
        assert("Error wrong rule action.");
        break;
    }
  }

  SyntaxAnalyzer::SyntaxAnalyzer(SyntaxAction* sa, Tokenizer* tk, 
      ErrorDiag::Diagnosis* ed) {
    tokenizer_ = tk;
    action_ = sa;
    err_diag_ = ed;
  }

  SyntaxAnalyzer::~SyntaxAnalyzer() {
  }


  eResult SyntaxAnalyzer::CompilationUnit(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ImportStmts(void) {
    return True;
  }

  eResult SyntaxAnalyzer::TopDefs(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Name(void) {
    // check if stack top is Identifier.
    ParseInfo pi = parse_stack_.Top();
    if(pi.type_ == ParseInfo::Identifier)
      return True;
    return Error;
  }

  eResult SyntaxAnalyzer::Storage(void) {
    return False;
  }

  eResult SyntaxAnalyzer::Type(void) {
    return True;
  }

  eResult SyntaxAnalyzer::TypeRef(void) {
    // <<== working here
    return False;
  }
  eResult SyntaxAnalyzer::Act_seq_unassigned_array(void) {
    // <<== working here
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_assigned_array(void) {
    // <<== working here
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_ptr(void) {
    // <<== working here
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_func_ptr(void) {
    // <<== working here
    return True;
  }

  eResult SyntaxAnalyzer::TypeRefBase(void) {
    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ == ParseInfo::ASTType) {
      return True;
    }
    return False;
  }
  // create primitive types
  eResult SyntaxAnalyzer::Act_seq_void(void) {
    AST::Type* ty = (AST::Type*)AST::VoidType::Get(action_->GetContext());
    PushType(ty);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_char(void){
    AST::Type* ty = (AST::Type*)AST::CharType::Get(action_->GetContext(), 
        AST::IntegerType::Signed);
    PushType(ty);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_short(void){
    AST::Type* ty = (AST::Type*)AST::ShortType::Get(action_->GetContext(), 
        AST::IntegerType::Signed);
    PushType(ty);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_int(void){
    AST::Type* ty = (AST::Type*)AST::IntType::Get(action_->GetContext(), 
        AST::IntegerType::Signed);
    PushType(ty);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_long(void){
    AST::Type* ty = (AST::Type*)AST::LongType::Get(action_->GetContext(), 
        AST::IntegerType::Signed);
    PushType(ty);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_char(void){
    AST::Type* ty = (AST::Type*)AST::CharType::Get(action_->GetContext(), 
        AST::IntegerType::Unsigned);
    PushType(ty);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_short(void){
    AST::Type* ty = (AST::Type*)AST::ShortType::Get(action_->GetContext(), 
        AST::IntegerType::Unsigned);
    PushType(ty);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_int(void){
    AST::Type* ty = (AST::Type*)AST::IntType::Get(action_->GetContext(), 
        AST::IntegerType::Unsigned);
    PushType(ty);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_long(void){
    AST::Type* ty = (AST::Type*)AST::LongType::Get(action_->GetContext(), 
        AST::IntegerType::Unsigned);
    PushType(ty);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_float(void){
    AST::Type* ty = (AST::Type*)AST::FloatType::Get(action_->GetContext());
    PushType(ty);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_double(void){
    AST::Type* ty = (AST::Type*)AST::DoubleType::Get(action_->GetContext());
    PushType(ty);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_class_identifier(void){
    // forward class declaration.
    ParseInfo pi_iden = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_iden.type_ == ParseInfo::Identifier) {
      char iden[256];
      memset(iden, 0, sizeof(iden));
      strncpy(iden, pi_iden.data_.cstr_, pi_iden.cstr_len_);
      AST::ClassType* cty = AST::ClassType::Get(action_->GetContext(), iden);
      if (cty) {
        PushType(cty);
        return True;
      }
    }
    return Error;
  }

  eResult SyntaxAnalyzer::TypeDef(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ParamTypeRefs(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Block(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Param(void) {
    return False;
  }

  eResult SyntaxAnalyzer::FixedParams(void) {
    return False;
  }

  eResult SyntaxAnalyzer::Params(void) {
    return True;
  }

  eResult SyntaxAnalyzer::DefFunc(void) {
    return True;
  }

  eResult SyntaxAnalyzer::DefVars(void) {
    return True;
  }

  eResult SyntaxAnalyzer::DefVarList(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Term(void) {
    // actually do nothing
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on term casting");
      return Error;
    }
    return True;
  }

  //"(" type ")" term          // type casting 
  eResult SyntaxAnalyzer::Act_seq_type_term(void) {
    // get term
    ParseInfo pi_term = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_term.type_ != ParseInfo::ASTNode || 
        !pi_term.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on term casting");
      return Error;
    }

    // get type
    ParseInfo pi_type = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_type.type_ != ParseInfo::ASTNode || 
        !pi_type.data_.node_->IsKindOf(AST::BaseNode::TypeNodeTy)) {
      assert("Error on term TypeNode");
      return Error;
    }

    AST::CastNode* node = 
      new AST::CastNode((AST::ExprNode*)pi_term.data_.node_,
          (AST::TypeNode*)pi_type.data_.node_);

    PushNode(node);
    return True;
  }

  // unary
  eResult SyntaxAnalyzer::Unary(void) {
    // actually do nothing
    if (parse_stack_.IsEmpty())  {
      assert("Error on Postfix() : Needed parsing info!");
      return Error;
    }

    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::ASTNode) {
      assert("Error on Postfix() : Needed ASTNode in stack!");
      return Error;
    }

    return True;
  }

  // "++" unary
  eResult SyntaxAnalyzer::Act_seq_preinc_unary(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary prefix increase");
      return Error;
    }

    AST::PrefixOpNode* node = 
      new AST::PrefixOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Inc);

    PushNode(node);
    return True;
  }

  // "--" unary
  eResult SyntaxAnalyzer::Act_seq_predec_unary(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary prefix increase");
      return Error;
    }

    AST::PrefixOpNode* node = 
      new AST::PrefixOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Dec);

    PushNode(node);
    return True;
  }

  // "+" term 
  eResult SyntaxAnalyzer::Act_seq_pos_term(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::UnaryOpNode* node = 
      new AST::UnaryOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Pos);

    PushNode(node);
    return True;
  }

  // "-" term 
  eResult SyntaxAnalyzer::Act_seq_neg_term(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::UnaryOpNode* node = 
      new AST::UnaryOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Neg);
    
    PushNode(node);
    return True;
  }

  // "!" term 
  eResult SyntaxAnalyzer::Act_seq_not_term(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::UnaryOpNode* node = 
      new AST::UnaryOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Not);

    PushNode(node);
    return True;
  }

  // "*" term 
  eResult SyntaxAnalyzer::Act_seq_ptr_term(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::DereferenceNode* node = 
      new AST::DereferenceNode((AST::ExprNode*)pi_expr.data_.node_);

    PushNode(node);
    return True;
  }

  // "&" term 
  eResult SyntaxAnalyzer::Act_seq_adr_term(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::AddressNode * node = 
      new AST::AddressNode((AST::ExprNode*)pi_expr.data_.node_);

    PushNode(node);
    return True;
  }

  // <SIZEOF> "(" type ")"
  eResult SyntaxAnalyzer::Act_seq_sizeof_type(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::SizeofTypeNode* node = 
      new AST::SizeofTypeNode((AST::ExprNode*)pi_expr.data_.node_);

    PushNode(node);
    return True;
  }

  // <SIZEOF> unary
  eResult SyntaxAnalyzer::Act_seq_sizeof_unary(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on unary positive");
      return Error;
    }

    AST::SizeofExprNode* node = 
      new AST::SizeofExprNode((AST::ExprNode*)pi_expr.data_.node_);

    PushNode(node);
    return True;
  }

  eResult SyntaxAnalyzer::Postfix(void) {
    if (parse_stack_.IsEmpty())  {
      assert("Error on Postfix() : Needed parsing info!");
      return Error;
    }

    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::ASTNode) {
      assert("Error on Postfix() : Needed ASTNode in stack!");
      return Error;
    }

    return True;
  }

  // "++"
  eResult SyntaxAnalyzer::Act_seq_post_inc(void) {
    ParseInfo pi_expr;
    pi_expr = parse_stack_.Top(); // get stored Expr
    parse_stack_.Pop();
    if (!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on postfix unary increase");
      return Error;
    }

    // create suffix node
    AST::SuffixOpNode* node = 
      new AST::SuffixOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Inc);

    PushNode(node);
    return True;
  }

  // "--"
  eResult SyntaxAnalyzer::Act_seq_post_dec(void) {
    ParseInfo pi_expr;
    pi_expr = parse_stack_.Top(); // get stored Expr
    parse_stack_.Pop();
    
    if (pi_expr.type_ != ParseInfo::ASTNode) {
      assert("Error on postfix unary decrease");
      return Error;
    }
    
    if (!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on postfix unary decrease");
      return Error;
    }

    AST::SuffixOpNode* node = 
      new AST::SuffixOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Dec);

    PushNode(node);
    return True;
  }

  //"[" expr "]"
  eResult SyntaxAnalyzer::Act_seq_array_reference(void) {
    // get array size expr
    ParseInfo pi_arrsize_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_arrsize_expr.type_ != ParseInfo::ASTNode) {
      assert("Error on postfix array reference");
      return Error;
    }
    if(!pi_arrsize_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on postfix array reference");
      return Error;
    }

    // get expr
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode) {
      assert("Error on postfix array reference");
      return Error;
    }
    if(!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on postfix array reference");
      return Error;
    }

    AST::ArrayRefNode * arnode = 
      new AST::ArrayRefNode((AST::ExprNode*)pi_expr.data_.node_, 
                            (AST::ExprNode*)pi_arrsize_expr.data_.node_);
    PushNode(arnode);
    return True;
  }
  
  // "." name
  eResult SyntaxAnalyzer::Act_seq_dot_name(void) {
    // get member name
    ParseInfo pi_name = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_name.type_ != ParseInfo::Identifier) {
      assert("Error on Postfix member reference");
      return Error;
    }

    // get expr
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode) {
      assert("Error on postfix array reference");
      return Error;
    }
    if(!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on postfix array reference");
      return Error;
    }

    // create new pi
    AST::MemberRefNode* memref_node = 
      new AST::MemberRefNode((AST::ExprNode*)pi_expr.data_.node_, pi_name.data_.cstr_);
    PushNode(memref_node);
    return True;
  }

  // "->" name
  eResult SyntaxAnalyzer::Act_seq_arrow_name(void) {
    // get member name
    ParseInfo pi_name = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_name.type_ != ParseInfo::Identifier) {
      assert("Error on Postfix member reference");
      return Error;
    }

    // get expr
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode) {
      assert("Error on postfix array reference");
      return Error;
    }
    if(!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on postfix array reference");
      return Error;
    }

    // create new pi
    AST::PtrMemberRefNode* ptrmemref_node = 
      new AST::PtrMemberRefNode((AST::ExprNode*)pi_expr.data_.node_, pi_name.data_.cstr_);
    PushNode(ptrmemref_node);
    return True;
  }

  //"(" args ")"
  eResult SyntaxAnalyzer::Act_seq_po_args_pc(void) {
    // get args
    ParseInfo pi_args = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_args.type_ != ParseInfo::ASTNode)
      return Error;
    if (!pi_args.data_.node_->IsKindOf(AST::BaseNode::ArgsNodeTy))
      return Error;
    // get function 
    ParseInfo pi_func = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_func.type_ != ParseInfo::ASTNode)
      return Error;
    if (!pi_func.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy))
      return Error;

    // create new pi
    AST::FuncCallNode* fcall= 
      new AST::FuncCallNode((AST::ExprNode*)pi_func.data_.node_, 
          (AST::ArgsNode*)pi_args.data_.node_);
    PushNode(fcall);
    return True;
  }

  eResult SyntaxAnalyzer::Primary(void) {
    if (parse_stack_.IsEmpty())  {
      assert("Error on Primary() : Need to child data!");
      return Error;
    }
    
    ParseInfo top_pi = parse_stack_.Top();
    parse_stack_.Pop();
    if (top_pi.type_ == ParseInfo::Integer) {
      AST::IntegerLiteralNode * node = 
        new AST::IntegerLiteralNode(AST::IntegerLiteralNode::Int, top_pi.data_.integer_);
      PushNode(node);
    }
    else if (top_pi.type_ == ParseInfo::Character) {
      AST::IntegerLiteralNode * node = 
        new AST::IntegerLiteralNode(AST::IntegerLiteralNode::Char, top_pi.data_.integer_);
      PushNode(node);
    }
    else if (top_pi.type_ == ParseInfo::String) {
      AST::StringLiteralNode* node = 
        new AST::StringLiteralNode(top_pi.data_.cstr_, top_pi.cstr_len_);
      PushNode(node);
    }
    else if (top_pi.type_ == ParseInfo::Identifier) {
      AST::VariableNode* node = 
        new AST::VariableNode(top_pi.data_.cstr_, top_pi.cstr_len_);
      PushNode(node);
    }
    else if (top_pi.type_ == ParseInfo::ASTNode &&
        top_pi.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      // Do nothing
    }
    else
      return Error;

    return True;
  }

  // "(" expr ")" 
  eResult SyntaxAnalyzer::Act_seq_po_expr_pc(void) {
    ParseInfo pi = parse_stack_.Top();
    // Check if top has expr ASTNode
    if(pi.type_ == ParseInfo::ASTNode && 
        pi.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy))
      return True;
    return False;
  }

  // args 
  //   : [expr ("," expr)*] 
  eResult SyntaxAnalyzer::Args(void) {
    // get ArgsNode
    ParseInfo pi_arg_node = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_arg_node.type_ == ParseInfo::ASTNode && 
        pi_arg_node.data_.node_->IsKindOf(AST::BaseNode::ArgsNodeTy)) {
      return True;
    }

    // if there isn't appropriate ArgsNode, create new one.
    AST::ArgsNode* args_node = new AST::ArgsNode();
    PushNode(args_node);
    return True;
  }
  
  // expr // first function argument
  eResult SyntaxAnalyzer::Act_seq_args_expr(void) {
    // get first argument expr
    ParseInfo pi_arg_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_arg_expr.type_ != ParseInfo::ASTNode || 
        !pi_arg_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on Args first expr");
      return Error;
    }

    // create new pi
    AST::ArgsNode* args_node = new AST::ArgsNode();
    args_node->Add((AST::ExprNode*)pi_arg_expr.data_.node_);
    PushNode(args_node);
    return True;
  }

  // ("," expr)*
  eResult SyntaxAnalyzer::Act_rep_args_expr(void) {
    // get argument expr
    ParseInfo pi_arg_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_arg_expr.type_ != ParseInfo::ASTNode || 
        !pi_arg_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on Args first expr");
      return Error;
    }

    // get ArgsNode
    ParseInfo pi_arg_node = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_arg_node.type_ != ParseInfo::ASTNode || 
      !pi_arg_node.data_.node_->IsKindOf(AST::BaseNode::ArgsNodeTy)) {
      assert("Error on Args first expr");
      return Error;
    }

    // create new pi
    AST::ArgsNode* args_node = (AST::ArgsNode*)pi_arg_node.data_.node_;
    args_node->Add((AST::ExprNode*)pi_arg_expr.data_.node_);
    PushNode(args_node);
    return True;
  }

  eResult SyntaxAnalyzer::OpAssignOp(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr10(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr9(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr8(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr7(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr6(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr5(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr4(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr3(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr2(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Expr1(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Stmts(void) {
    return True;
  }

  eResult SyntaxAnalyzer::Stmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::LabeledStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::IfStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::WhileStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::DoWhileStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ForStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::BreakStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ContinueStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::GotoStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ReturnStmt(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ActTokIntegerLiteral(void) {
    ParseInfo pi;
    Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::Integer;
    pi.data_.integer_ = atol(tok.c);
    pi.token_idx_ = tokenizer_->GetTokPos();
    parse_stack_.Push(pi);
    return True;
  }

  eResult SyntaxAnalyzer::ActTokCharacterLiteral(void) {
    ParseInfo pi;
    Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::Character;
    pi.data_.character_ = *tok.c;
    pi.token_idx_ = tokenizer_->GetTokPos();
    parse_stack_.Push(pi);
    return True;
  }

  eResult SyntaxAnalyzer::ActTokStringLiteral(void) {
    ParseInfo pi;
    Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::String;
    pi.data_.cstr_ = tok.c;
    pi.cstr_len_ = tok.len; // string length
    pi.token_idx_ = tokenizer_->GetTokPos();
    parse_stack_.Push(pi);
    return True;
  }

  eResult SyntaxAnalyzer::ActIdentifier(void) {
    ParseInfo pi;
    Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::Identifier;
    pi.data_.cstr_ = tok.c;
    pi.token_idx_ = tokenizer_->GetTokPos();
    pi.cstr_len_ = tok.len; // string length
    parse_stack_.Push(pi);
    return True;
  }

  void SyntaxAnalyzer::PushType(AST::Type* type) {
    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTType;
    pi_new.data_.type_ = type;
    parse_stack_.Push(pi_new);
  }

  void SyntaxAnalyzer::PushToken(void) {
    ParseInfo pi;
    Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::TokenType;
    pi.data_.tok_type_ = tok.type;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::PushNode(AST::BaseNode* node) {
    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::ASTNode;
    pi_new.data_.node_ = node;
    parse_stack_.Push(pi_new);
  }

  void SyntaxAnalyzer::DebugPrint(void) {
  }

}
