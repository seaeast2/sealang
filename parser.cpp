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
                else {
                  tokenizer_->SetTokPos(tok_pos); // restore token position.
                  return True; // reached unmatching point.
                }
              }
              else
                return Error;
            }

            if (matching_count == i) {
              // if every rules are matching, run action.
              (this->*rule_actions_[entry])();
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
            else
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
            else if (res == Error)
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

  eResult SyntaxAnalyzer::TestRule(int entry) {
    Rule rule = rules_[entry];
    eResult res;
    int tok_pos;


    switch(rule.action_) {
      case Repeat:
        {
          while(true) {
            int matching_count = 0;
            int i = 0;
            tok_pos = tokenizer_->GetTokPos(); // backup token position
            for (; rule.sub_rules_[i] > -1; i++) {
              res = TestRule(rule.sub_rules_[i]);
              if (res == True) { // matched
                matching_count++;
              }
              else if (res == False) { // not matching
                if (rules_[rule.sub_rules_[i]].action_ == Options) {
                  matching_count++;
                }
                else {
                  tokenizer_->SetTokPos(tok_pos); // restore token position.
                  return True; // reached unmatching point.
                }
              }
              else
                return Error;
            }
          };
        }
        break;

      case Select:
        {
          tok_pos = tokenizer_->GetTokPos(); // backup token position
          for (int i = 0; rule.sub_rules_[i] > -1; i++) {
            res = TestRule(rule.sub_rules_[i]);
            if (res == True) {
              // Run action
              return True; // found matching
            }
            else if (res == False) {
              tokenizer_->SetTokPos(tok_pos); // restore token position.
              return False;
            }
            else
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
            res = TestRule(rule.sub_rules_[i]);
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
            res = TestRule(rule.sub_rules_[i]);
            if (res == True)
              matching_count++;
            else if (res == Error)
              return Error;
          }

          if (matching_count == i)
            return True;

          tokenizer_->SetTokPos(tok_pos); // restore token position.
          return False; // unmatching
        }
        break;

      case Terminal:
        {
          if(tokenizer_->isToken(0, Lexer::TokenType(rule.sub_rules_[0]))) {
            // Run action
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
    return True;
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

  // import_stmt 
  //    : <IMPORT> name ("." name)* ";" 
  // ex) import aaa.bbb.ccc;
  eResult SyntaxAnalyzer::ImportStmt() {
    int cur_tok_pos = tokenizer_->GetTokPos(); // backup start token position.
    std::string import_path, tmp;
    Token tok;

    // check if first token is 'import'.
    if (!tokenizer_->isToken(0, Lexer::TokImport))
      return False;
    tokenizer_->ConsumeToken(1); // Move next

    // name("."name)* ";"
    // check if identifier.
    if (Name() != True) { // something wrong in path
      err_diag_->Print(ErrorDiag::Err_Parser_NoIdentifier, 
          tok.line, tok.col, "Wrong import path");
      return Error;
    }

    // insert first import path to string.
    Token tok_name = tokenizer_->GetCurToken(0);
    tokenizer_->ConsumeToken(1); // move next
    tmp.assign(tok_name.c, tok_name.len);
    import_path = tmp;

    while(true) {
      // "."name)* ";"
      if (tokenizer_->isToken(0, Lexer::TokDot)) {
        tokenizer_->ConsumeToken(1); // move next
        // name)* ";"
        if (Name() != True) { // something wrong in path
          err_diag_->Print(ErrorDiag::Err_Parser_NoIdentifier, 
              tok.line, tok.col, "Wrong import path");
          return Error;
        }

        import_path += "/";
        tok_name = tokenizer_->GetCurToken(0);
        tmp.assign(tok_name.c, tok_name.len);
        import_path += tmp;
        tokenizer_->ConsumeToken(1);
      }
      // ";"
      else if (tokenizer_->isToken(0, Lexer::TokSemiColon)) {
        tokenizer_->ConsumeToken(1);
        break;
      }
      else
        return Error;// some error on it
    }

    action_->ActOnImport(import_path);
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
    ParseInfo pi_new;
    pi_new.type_ = ParseInfo::StorageInfo; 
    pi_new.data_.boolean_ = true;
    parse_stack_.Push(pi_new);
    return True;
  }

  eResult SyntaxAnalyzer::Type(void) {
    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ == ParseInfo::ASTType)
      return True;
    return Error;
  }

  eResult SyntaxAnalyzer::TypeRef(void) {
    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ == ParseInfo::ASTType)
      return True;
    return Error;
  }

  eResult SyntaxAnalyzer::Act_seq_unassigned_array(void) {
    ParseInfo pi = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi.type_ == ParseInfo::ASTType) {
      AST::Type* basety = pi.data_.type_;
      AST::ArrayType* arrty = AST::ArrayType::Get(action_->GetContext(), basety);
      PushType((AST::Type*)arrty, RuleName::seq_unassigned_array);
      return True;
    }
    return Error;
  }
  eResult SyntaxAnalyzer::Act_seq_assigned_array(void) {
    ParseInfo pi_basety, pi_int;  
    pi_int = parse_stack_.Top();
    if (pi_int.type_ == ParseInfo::Integer) {
      parse_stack_.Pop();
      pi_basety = parse_stack_.Top();
      parse_stack_.Pop();
      if (pi_basety.type_ == ParseInfo::ASTType) {
        AST::Type* basety = pi_basety.data_.type_;
        long size = pi_int.data_.integer_;
        if (size < 0) {
          // invalid array size.
          return Error;
        }
        AST::ArrayType* arrty = AST::ArrayType::Get(action_->GetContext(), basety, size);
        PushType((AST::Type*)arrty, RuleName::seq_assigned_array);
        return True;
      }
    }
    return Error;
  }

  eResult SyntaxAnalyzer::Act_seq_ptr(void) {
    ParseInfo pi_basety;
    pi_basety = parse_stack_.Top();
    if (pi_basety.type_ == ParseInfo::ASTType) {
      parse_stack_.Pop();
      AST::Type* basety = pi_basety.data_.type_;

      AST::PointerType* ptrty = AST::PointerType::Get(action_->GetContext(), basety);
      PushType((AST::Type*)ptrty, RuleName::seq_ptr);
      return True;
    }
    return Error;
  }

  eResult SyntaxAnalyzer::Act_seq_func(void) {
    ParseInfo pi_params, pi_retty;
    pi_params = parse_stack_.Top();
    if (pi_params.type_ == ParseInfo::TypeList) {
      parse_stack_.Pop();
      SimpleVector<AST::Type*>* params = pi_params.data_.types_;

      pi_retty = parse_stack_.Top();
      if (pi_retty.type_ == ParseInfo::ASTType) {
        parse_stack_.Pop();
        AST::Type* retty = pi_retty.data_.type_;
        AST::FunctionType* fnty = AST::FunctionType::Get(action_->GetContext(), retty, *params);
        PushType((AST::Type*)fnty, RuleName::seq_func);
        delete params; // delete parameter type list container.
        return True;
      }
      return Error; // TODO : Need to error message for invalid return type.
    }
    return Error; // TODO : Need to error message for invalid parameter types
  }

  eResult SyntaxAnalyzer::TypeRefBase(void) {
    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::ASTType)
      return Error;

    SetRuleNameForPI(RuleName::typeref_base);
    return True;
  }
  // create primitive types
  eResult SyntaxAnalyzer::Act_seq_void(void) {
    AST::Type* ty = (AST::Type*)AST::VoidType::Get(action_->GetContext());
    PushType(ty, RuleName::seq_void);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_char(void){
    AST::Type* ty = (AST::Type*)AST::CharType::Get(action_->GetContext(), 
        AST::IntegerType::Signed);
    PushType(ty, RuleName::seq_char);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_short(void){
    AST::Type* ty = (AST::Type*)AST::ShortType::Get(action_->GetContext(), 
        AST::IntegerType::Signed);
    PushType(ty, RuleName::seq_short);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_int(void){
    AST::Type* ty = (AST::Type*)AST::IntType::Get(action_->GetContext(), 
        AST::IntegerType::Signed);
    PushType(ty, RuleName::seq_int);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_long(void){
    AST::Type* ty = (AST::Type*)AST::LongType::Get(action_->GetContext(), 
        AST::IntegerType::Signed);
    PushType(ty, RuleName::seq_long);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_char(void){
    AST::Type* ty = (AST::Type*)AST::CharType::Get(action_->GetContext(), 
        AST::IntegerType::Unsigned);
    PushType(ty, RuleName::seq_unsigned_char);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_short(void){
    AST::Type* ty = (AST::Type*)AST::ShortType::Get(action_->GetContext(), 
        AST::IntegerType::Unsigned);
    PushType(ty, RuleName::seq_unsigned_short);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_int(void){
    AST::Type* ty = (AST::Type*)AST::IntType::Get(action_->GetContext(), 
        AST::IntegerType::Unsigned);
    PushType(ty, RuleName::seq_unsigned_int);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_long(void){
    AST::Type* ty = (AST::Type*)AST::LongType::Get(action_->GetContext(), 
        AST::IntegerType::Unsigned);
    PushType(ty, RuleName::seq_unsigned_long);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_float(void){
    AST::Type* ty = (AST::Type*)AST::FloatType::Get(action_->GetContext());
    PushType(ty, RuleName::seq_float);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_double(void){
    AST::Type* ty = (AST::Type*)AST::DoubleType::Get(action_->GetContext());
    PushType(ty, RuleName::seq_double);
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
        PushType(cty, RuleName::seq_class_identifier);
        return True;
      }
    }
    return Error;
  }

  eResult SyntaxAnalyzer::TypeDef(void) {
    return True;
  }

  eResult SyntaxAnalyzer::ParamTypeRefs(void) {
    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::TypeList)
      return Error;
    
    if (pi.rule_name_ != RuleName::seq_param_void &&
        pi.rule_name_ != RuleName::seq_param_type_list)
      return Error;

    SetRuleNameForPI(RuleName::param_typerefs);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_param_void(void) {
    AST::Type* ty = (AST::Type*)AST::VoidType::Get(action_->GetContext());
    SimpleVector<AST::Type*>* params = new SimpleVector<AST::Type*>();
    params->PushBack(ty);
    PushTypeList(params, RuleName::seq_param_void);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_param_type_list(void) {
    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::TypeList)
      return Error;

    SetRuleNameForPI(RuleName::seq_param_type_list);
    return True;
  }

  eResult SyntaxAnalyzer::Act_rep_param_comma_type(void) {
    ParseInfo pi_type = parse_stack_.Top();
    if (pi_type.type_ == ParseInfo::ASTType) {
      parse_stack_.Pop();
      ParseInfo pi_type_list = parse_stack_.Top();
      if (pi_type_list.type_ == ParseInfo::TypeList) {
        parse_stack_.Pop();
        SimpleVector<AST::Type*>* params = pi_type_list.data_.types_;
        params->PushBack(pi_type.data_.type_);
        PushTypeList(params, RuleName::rep_param_comma_type);
        return True;
      }
    }
    return Error;
  }

  eResult SyntaxAnalyzer::Act_opt_vararg_type(void) {
    // It indicates this function has vararg type.
    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ == ParseInfo::TypeList) {
      parse_stack_.Pop();
      SimpleVector<AST::Type*>* params = pi.data_.types_;
      AST::Type* ty = (AST::Type*)AST::VarArgType::Get(action_->GetContext());
      params->PushBack(ty);
      PushTypeList(params, RuleName::opt_vararg_type);
      return True;
    }
    return Error;
  }

  eResult SyntaxAnalyzer::ParamType(void) {
    // This indicates this is first parameter type.
    ParseInfo pi_type = parse_stack_.Top();
    SimpleVector<AST::Type*>* params = nullptr;
    if (pi_type.type_ == ParseInfo::ASTType) {
      parse_stack_.Pop();

      params = new SimpleVector<AST::Type*>();

      params->PushBack(pi_type.data_.type_);
      PushTypeList(params, RuleName::param_type);
      return True;
    }
    return Error;
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
    ParseInfo pi;
    AST::BaseNode* expr_node = nullptr;
    SimpleVector<AST::VariableDecl*>* vardecls = 
      new SimpleVector<AST::VariableDecl*>();

    while(true) {
      pi = parse_stack_.Top();
      // Read variable initializer Expr
      expr_node = nullptr;
      if (pi.type_ == ParseInfo::ASTNode) {
        expr_node = pi.data_.node_;
        if (!expr_node->IsKindOf(AST::BaseNode::ExprNodeTy))
          return Error;
        parse_stack_.Pop();
        pi = parse_stack_.Top(); // read variable name
      }

      // Read variable name
      if (pi.type_ == ParseInfo::Identifier) {
        AST::VariableDecl* new_var = new AST::VariableDecl();
        if(expr_node)
          new_var->SetInit((AST::ExprNode*)expr_node);
        new_var->SetName(pi.data_.cstr_, pi.cstr_len_);

        vardecls->PushBack(new_var);
      }

      // Read variable type
      if (pi.type_ == ParseInfo::ASTType) {
        bool is_static = false;
        AST::Type* ty = pi.data_.type_;
        parse_stack_.Pop();

        // Read storage info if possible
        pi = parse_stack_.Top();
        if (pi.type_ == ParseInfo::StorageInfo) {
          is_static = true;
          parse_stack_.Pop();
        }

        if (vardecls->GetSize() == 0)
          return Error;

        for (int i = 0; i < vardecls->GetSize(); i++) {
          (*vardecls)[i]->SetStorage(is_static);
          (*vardecls)[i]->SetType(ty);
        }
        vardecls->Reverse();
        PushVarDecls(vardecls, RuleName::defvars);
        return True;
      }
    }

    return Error;
  }

  eResult SyntaxAnalyzer::DefVarList(void) {
    return True;
  }


  eResult SyntaxAnalyzer::Term(void) {
    // actually do nothing
    ParseInfo pi_expr = parse_stack_.Top();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on term casting");
      return Error;
    }

    SetRuleNameForPI(RuleName::term);// mark pi with term.
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

    PushNode(node, RuleName::seq_type_term);
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

    PushNode(node, RuleName::seq_preinc_unary);
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

    PushNode(node, RuleName::seq_predec_unary);
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

    PushNode(node, RuleName::seq_pos_term);
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
    
    PushNode(node, RuleName::seq_neg_term);
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

    PushNode(node, RuleName::seq_not_term);
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

    PushNode(node, RuleName::seq_ptr_term);
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

    PushNode(node, RuleName::seq_adr_term);
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

    PushNode(node, RuleName::seq_sizeof_type);
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

    PushNode(node, RuleName::seq_sizeof_unary);
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

    PushNode(node, RuleName::seq_post_inc);
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

    PushNode(node, RuleName::seq_post_dec);
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
    PushNode(arnode, RuleName::seq_array_reference);
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
    PushNode(memref_node, RuleName::seq_dot_name);
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
    PushNode(ptrmemref_node, RuleName::seq_arrow_name);
    return True;
  }

  //"(" args ")"
  eResult SyntaxAnalyzer::Act_seq_fncall(void) {
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
    PushNode(fcall, RuleName::seq_fncall);
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
      PushNode(node, RuleName::primary);
    }
    else if (top_pi.type_ == ParseInfo::Character) {
      AST::IntegerLiteralNode * node = 
        new AST::IntegerLiteralNode(AST::IntegerLiteralNode::Char, top_pi.data_.integer_);
      PushNode(node, RuleName::primary);
    }
    else if (top_pi.type_ == ParseInfo::String) {
      AST::StringLiteralNode* node = 
        new AST::StringLiteralNode(top_pi.data_.cstr_, top_pi.cstr_len_);
      PushNode(node, RuleName::primary);
    }
    else if (top_pi.type_ == ParseInfo::Identifier) {
      AST::VariableNode* node = 
        new AST::VariableNode(top_pi.data_.cstr_, top_pi.cstr_len_);
      PushNode(node, RuleName::primary);
    }
    else if (top_pi.type_ == ParseInfo::ASTNode &&
        top_pi.rule_name_ == RuleName::expr &&
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
    PushNode(args_node, RuleName::args);
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
    PushNode(args_node, RuleName::seq_args_expr);
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
    PushNode(args_node, RuleName::rep_args_expr);
    return True;
  }

  eResult SyntaxAnalyzer::Expr(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert("Error on term casting");
      return Error;
    }

    if (pi_expr.rule_name_ != RuleName::seq_assign_value && 
        pi_expr.rule_name_ != RuleName::seq_opassign_value &&
        pi_expr.rule_name_ != RuleName::expr10)
      return Error;

    SetRuleNameForPI(RuleName::expr);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_assign_value(void) {
    ParseInfo pi_term, pi_expr;
    AST::BaseNode* rhs = nullptr, *lhs = nullptr;
    
    // Read RHS expr
    pi_expr = parse_stack_.Top();
    if (pi_expr.type_ == ParseInfo::ASTNode &&
        pi_expr.rule_name_ == RuleName::expr){
      rhs = pi_expr.data_.node_;
      if (!rhs->IsKindOf(AST::BaseNode::ExprNodeTy))
        return Error;
      parse_stack_.Pop();
    }
    else 
      return Error;

    // Read LHS term
    pi_term = parse_stack_.Top();
    if (pi_term.type_ == ParseInfo::ASTNode && 
        pi_term.rule_name_ == RuleName::term){
      lhs = pi_term.data_.node_;
      if (!rhs->IsKindOf(AST::BaseNode::ExprNodeTy))
        return Error;
      parse_stack_.Pop();
    }
    else 
      return Error;

    // Create AssignNode
    AST::AssignNode* assign_node = 
      new AST::AssignNode((AST::ExprNode*)lhs, (AST::ExprNode*)rhs);
    PushNode(assign_node, RuleName::seq_assign_value);

    SetRuleNameForPI(RuleName::seq_assign_value);
    return True;;
  }

  eResult SyntaxAnalyzer::Act_seq_opassign_value(void){
    ParseInfo pi_expr, pi_opassign, pi_term;
    AST::BaseNode* rhs = nullptr, *lhs = nullptr;
    AST::OpAssignNode::AssignOp op;
    
    // Read RHS expr
    pi_expr = parse_stack_.Top();
    if (pi_expr.type_ == ParseInfo::ASTNode &&
        pi_expr.rule_name_ == RuleName::expr){
      rhs = pi_expr.data_.node_;
      if (!rhs->IsKindOf(AST::BaseNode::ExprNodeTy))
        return Error;
      parse_stack_.Pop();
    }
    else 
      return Error;

    // Read opassign
    pi_opassign = parse_stack_.Top();
    if (pi_opassign.type_ == ParseInfo::TokenType &&
        pi_opassign.rule_name_ == RuleName::opassign_op){
      parse_stack_.Pop();

      switch(pi_opassign.data_.tok_type_) {
        case TokComAdd:
          op = AST::OpAssignNode::AssignAdd;
          break;
        case TokComSub:
          op = AST::OpAssignNode::AssignSub;
          break;
        case TokComMul:
          op = AST::OpAssignNode::AssignMul;
          break;
        case TokComDiv:
          op = AST::OpAssignNode::AssignDiv;
          break;
        case TokComMod:
          op = AST::OpAssignNode::AssignMod;
          break;
        case TokComBitAnd:
          op = AST::OpAssignNode::AssignBitAnd;
          break;
        case TokComBitOr:
          op = AST::OpAssignNode::AssignBitOr;
          break;
        case TokComBitXor:
          op = AST::OpAssignNode::AssignBitXor;
          break;
        case TokComBitShiftL:
          op = AST::OpAssignNode::AssignBitShiftL;
          break;
        case TokComBitShiftR:
          op = AST::OpAssignNode::AssignBitShiftR;
          break;
        default:
          return Error;
      }
    }
    else 
      return Error;

    // Read LHS term
    pi_term = parse_stack_.Top();
    if (pi_term.type_ == ParseInfo::ASTNode && 
        pi_term.rule_name_ == RuleName::term){
      lhs = pi_term.data_.node_;
      if (!rhs->IsKindOf(AST::BaseNode::ExprNodeTy))
        return Error;
      parse_stack_.Pop();
    }
    else 
      return Error;

    // Create OpAssign
    AST::OpAssignNode* opassign = 
      new AST::OpAssignNode((AST::ExprNode*)lhs, op, (AST::ExprNode*)rhs);
    PushNode(opassign, RuleName::seq_opassign_value);

    SetRuleNameForPI(RuleName::seq_opassign_value);
    return True;
  }

  eResult SyntaxAnalyzer::OpAssignOp(void) {
    // Get OpAssign token.
    PushToken(-1, RuleName::opassign_op);
    return True;
  }

  eResult SyntaxAnalyzer::Expr10(void) {
    ParseInfo pi_expr9, pi_ternary;
    AST::CondExprNode* cond_expr = nullptr;

    // check if first expr is ternary op.
    pi_ternary = parse_stack_.Top();
    if (pi_ternary.type_ == ParseInfo::ASTNode && 
        pi_ternary.rule_name_ == RuleName::opt_ternaryop) {
      parse_stack_.Pop();
      cond_expr = (AST::CondExprNode*)pi_ternary.data_.node_;
    }

    // Read condition expr
    pi_expr9 = parse_stack_.Top();
    if (pi_expr9.type_ != ParseInfo::ASTNode ||
        pi_expr9.rule_name_ != RuleName::expr9) {
      return Error;
    }

    if (cond_expr) {
      parse_stack_.Pop(); // pop expr9
      cond_expr->SetCond((AST::ExprNode*)pi_expr9.data_.node_);
      PushNode(cond_expr, expr10);
    }
    else
      SetRuleNameForPI(RuleName::expr10); // just change rule name.

    return True;
  }

  // ["?" expr ":" expr10] 
  eResult SyntaxAnalyzer::Act_opt_ternaryop(void) {
    ParseInfo pi_then, pi_else;

    // Read else expr
    pi_else = parse_stack_.Top();
    if (pi_else.type_ != ParseInfo::ASTNode || 
        pi_else.rule_name_ != RuleName::expr10)
      return Error;
    parse_stack_.Pop();

    // Read then expr
    pi_then = parse_stack_.Top();
    if (pi_then.type_ != ParseInfo::ASTNode || 
        pi_then.rule_name_ != RuleName::expr)
      return Error;
    parse_stack_.Pop();


    // Create Conditional expression node
    // Cond expr will be set up expr10 action.
    AST::CondExprNode* cond_expr = new AST::CondExprNode(nullptr, 
        (AST::ExprNode*)pi_then.data_.node_, (AST::ExprNode*)pi_else.data_.node_);

    PushNode(cond_expr, RuleName::opt_ternaryop);
    return True;
  }

  eResult SyntaxAnalyzer::Expr9(void) {
    ParseInfo pi_expr8;

    // Check if top is right.
    pi_expr8 = parse_stack_.Top();
    if (pi_expr8.type_ != ParseInfo::ASTNode ||
        (pi_expr8.rule_name_ != RuleName::expr8) && 
        (pi_expr8.rule_name_ != RuleName::rep_or_expr8))
      return Error;

    SetRuleNameForPI(RuleName::expr9);
    return True;
  }

  eResult SyntaxAnalyzer::Act_rep_or_expr8(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr8)
      return Error;
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr8 &&
         pi_lhs.rule_name_ != RuleName::rep_or_expr8))
      return Error;
    parse_stack_.Pop();

    //Create node
    AST::LogicalOrNode* logic_or = 
      new AST::LogicalOrNode((AST::ExprNode*)pi_lhs.data_.node_, 
                             (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(logic_or, RuleName::rep_or_expr8);
    return True;
  }

  eResult SyntaxAnalyzer::Expr8(void) {
    ParseInfo pi_expr7;

    // Check if top is right.
    pi_expr7 = parse_stack_.Top();
    if (pi_expr7.type_ != ParseInfo::ASTNode ||
        (pi_expr7.rule_name_ != RuleName::expr7) && 
        (pi_expr7.rule_name_ != RuleName::rep_and_expr7))
      return Error;

    SetRuleNameForPI(RuleName::expr9);
    return True;
  }

  eResult SyntaxAnalyzer::Act_rep_and_expr7(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr7)
      return Error;
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr7 &&
         pi_lhs.rule_name_ != RuleName::rep_and_expr7))
      return Error;
    parse_stack_.Pop();

    //Create node
    AST::LogicalAndNode* logic_and = 
      new AST::LogicalAndNode((AST::ExprNode*)pi_lhs.data_.node_, 
                             (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(logic_and, RuleName::rep_and_expr7);
    return True;
  }

  eResult SyntaxAnalyzer::Expr7(void) {
    ParseInfo pi_expr7;

    // Check if top is right.
    pi_expr7 = parse_stack_.Top();
    if (pi_expr7.type_ != ParseInfo::ASTNode ||
        (pi_expr7.rule_name_ != RuleName::expr7 &&
         pi_expr7.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_expr7.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_expr7.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_expr7.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_expr7.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_expr7.rule_name_ != RuleName::seq_neq_expr6 ))
      return Error;

    SetRuleNameForPI(RuleName::expr7);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_gr_expr6(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr6)
      return Error;
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr7 &&
         pi_lhs.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_neq_expr6 ))
      return Error;
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* greator = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::GreatorThan,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(greator, RuleName::seq_gr_expr6);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_ls_expr6(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr6)
      return Error;
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr7 &&
         pi_lhs.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_neq_expr6 ))
      return Error;
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* less = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::LessThan,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(less, RuleName::seq_ls_expr6);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_geq_expr6(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr6)
      return Error;
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr7 &&
         pi_lhs.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_neq_expr6 ))
      return Error;
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* gr_eq = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::GreatorThanEq,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(gr_eq, RuleName::seq_geq_expr6);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_leq_expr6(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr6)
      return Error;
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr7 &&
         pi_lhs.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_neq_expr6 ))
      return Error;
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* ls_eq = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::LessThanEq,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(ls_eq, RuleName::seq_leq_expr6);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_eq_expr6(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr6)
      return Error;
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr7 &&
         pi_lhs.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_neq_expr6 ))
      return Error;
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* eq = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::Equal,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(eq, RuleName::seq_eq_expr6);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_neq_expr6(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr6)
      return Error;
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr7 &&
         pi_lhs.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_neq_expr6 ))
      return Error;
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* neq = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::NotEqual,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(neq, RuleName::seq_neq_expr6);
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

  void SyntaxAnalyzer::PushType(AST::Type* type, RuleName rname) {
    ParseInfo pi;
    pi.type_ = ParseInfo::ASTType;
    pi.data_.type_ = type;
    pi.rule_name_ = rname;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::PushToken(int pos_offset, RuleName rname) {
    ParseInfo pi;
    Token tok = tokenizer_->GetCurToken(pos_offset);

    pi.type_ = ParseInfo::TokenType;
    pi.data_.tok_type_ = tok.type;
    pi.rule_name_ = rname;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::PushNode(AST::BaseNode* node, RuleName rname) {
    ParseInfo pi;
    pi.type_ = ParseInfo::ASTNode;
    pi.data_.node_ = node;
    pi.rule_name_ = rname;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::PushTypeList(SimpleVector<AST::Type*>* ty_list,
      RuleName rname) {
    ParseInfo pi;
    pi.type_ = ParseInfo::TypeList;
    pi.data_.types_ = ty_list;
    pi.rule_name_ = rname;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::PushVarDecls(SimpleVector<AST::VariableDecl*>* var_list,
      RuleName rname) {
    ParseInfo pi;
    pi.type_ = ParseInfo::VarDeclList;
    pi.data_.vardecls_ = var_list;
    pi.rule_name_ = rname;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::SetRuleNameForPI(RuleName rname) {
    ParseInfo pi = parse_stack_.Top();
    parse_stack_.Pop();

    pi.rule_name_ = rname;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::DebugPrint(void) {
  }

}
