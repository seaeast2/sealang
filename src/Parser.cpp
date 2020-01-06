#include <assert.h>
#include <stdlib.h>
#include "Parser.h"

namespace Parser {
  SyntaxAnalyzer::SyntaxAnalyzer(AST::ASTContext* ac, Lexer::Tokenizer* tk, 
      ErrorDiag::Diagnosis* ed) {
    tokenizer_ = tk;
    ac_ = ac;
    err_diag_ = ed;

    InitBasicRule();
    InitSuccessAction();
    InitFailAction();
  }

  SyntaxAnalyzer::~SyntaxAnalyzer() {
  }

  eResult SyntaxAnalyzer::TraverseRule(RuleName entry) {
    Rule rule = rules_[entry];
    eResult res;
    int tok_pos;
    int lookahead = rule.lookahead_; 
    bool check_user_type = false;

    // turn on check user type
    if (rule.lookahead_ == 16) {
      lookahead = 1;
      check_user_type = true;
    }

    switch(rule.action_) {
      case RepeatStar: // Repeat more than zero
        {
          while(true) {
            int matching_count = 0;
            int i = 0;
            tok_pos = tokenizer_->GetTokPos(); // backup token position
            
            // check look ahead
            for (int j = 0; j < lookahead; j++) {
              if (TestRule(rule.sub_rules_[j], check_user_type) == False) {
                tokenizer_->SetTokPos(tok_pos); // restore token position.
                return True; // anyway return true in repeat star
              }
            }

            tokenizer_->SetTokPos(tok_pos); // restore token position.
            for (; rule.sub_rules_[i] > TokUnknown; i++) {
              res = TraverseRule(rule.sub_rules_[i]);
              if (res == True) // matched
                matching_count++;
              else if (res == False) { // unmatched
                // Special actions for grammer fail situation.
                if (rule_fail_actions_[entry]) {
                  if ((this->*rule_fail_actions_[entry])() == Error)
                    return Error;
                }
                tokenizer_->SetTokPos(tok_pos);
                return True;
              }
              else {
                assert(0 && "Error on RepeatStar");
                return Error;
              }
            }

            if (matching_count == i) {
              // when every rules are matched, run action.
              if (rule_actions_[entry]) {
                if ((this->*rule_actions_[entry])() == Error) {
                  assert(0 && "Error on RepeatStar");
                  return Error;
                }
                /*if ((this->*rule_actions_[entry])() == Error) {
                  assert(0 && "Error on RepeatStar");
                  return Error;
                }*/
              }
            }
          };
        }
        break;

      case RepeatDagger: // Repeat more than once
        {
          while(true) {
            bool success = false;
            int matching_count = 0;
            int i = 0;
            tok_pos = tokenizer_->GetTokPos(); // backup token position

            // check look ahead
            for (int j = 0; j < lookahead; j++) {
              if (TestRule(rule.sub_rules_[j], check_user_type) == False) {
                tokenizer_->SetTokPos(tok_pos); // restore token position.
                return True; // anyway return true in repeat star
              }
            }

            tokenizer_->SetTokPos(tok_pos); // restore token position.
            for (; rule.sub_rules_[i] > TokUnknown; i++) {
              res = TraverseRule(rule.sub_rules_[i]);
              if (res == True) // matched
                matching_count++;
              else if (res == False) { // Rereached unmached point 
                if (rule_fail_actions_[entry]) { // Special actions for grammer fail situation.
                  if ((this->*rule_fail_actions_[entry])() == Error)
                    return Error;
                }
                tokenizer_->SetTokPos(tok_pos);
                if (success)
                  return True;
                return False;
              }
              else {
                assert(0 && "Error on RepeatDagger");
                return Error;
              }
            }

            if (matching_count == i) {
              // if every rules are matching, run action.
              if (rule_actions_[entry]) {
                if ((this->*rule_actions_[entry])() == Error)
                  return Error;
              }
              success = true;
            }
          };
        }
        break;

      case Select: // select one of list
        {
          tok_pos = tokenizer_->GetTokPos(); // backup token position
          for (int i = 0; rule.sub_rules_[i] > TokUnknown; i++) {
            res = TraverseRule(rule.sub_rules_[i]);
            if (res == True) {
              if (rule_actions_[entry]) {
                if ((this->*rule_actions_[entry])() == Error)
                  return Error;
              }
              return True; // found matching
            }
            else if (res == False) {
              if (rule_fail_actions_[entry]) { // Special actions for grammer fail situation.
                if ((this->*rule_fail_actions_[entry])() == Error)
                  return Error;
              }

              tokenizer_->SetTokPos(tok_pos); // restore token position.
            }
            else {
              assert(0 && "Error on Select");
              return Error;
            }
          }
          return False;
        }
        break;

      case Sequence:
        {
          int matching_count = 0;
          int i = 0;
          tok_pos = tokenizer_->GetTokPos(); // backup token position

          // check look ahead
          for (int j = 0; j < lookahead; j++) {
            if (TestRule(rule.sub_rules_[j], check_user_type) == False) {
              tokenizer_->SetTokPos(tok_pos); // restore token position.
              return False;
            }
          }

          tokenizer_->SetTokPos(tok_pos); // restore token position.
          for (; rule.sub_rules_[i] > TokUnknown; i++) {
            res = TraverseRule(rule.sub_rules_[i]);
            if (res == True) // matched
              matching_count++;
            else if (res == False) { // unmatched
              if (rule_fail_actions_[entry]) { // Special actions for grammer fail situation.
                if ((this->*rule_fail_actions_[entry])() == Error)
                  return Error;
              }
              tokenizer_->SetTokPos(tok_pos); // restore token position.
              return False;
            }
            else {
              assert(0 && "Error on Sequence");
              return Error;
            }
          }

          if (matching_count == i) {
            if (rule_actions_[entry]) {
              if ((this->*rule_actions_[entry])() == Error)
                return Error;
            }
            return True; // matched
          }
          tokenizer_->SetTokPos(tok_pos);
          return False;
        }
        break;

      case Options:
        {
          int matching_count = 0;
          int i = 0;
          tok_pos = tokenizer_->GetTokPos(); // backup token position

          // check look ahead
          for (int j = 0; j < lookahead; j++) {
            if (TestRule(rule.sub_rules_[j], check_user_type) == False) {
              tokenizer_->SetTokPos(tok_pos); // restore token position.
              return True;
            }
          }

          tokenizer_->SetTokPos(tok_pos); // restore token position.
          for (; rule.sub_rules_[i] > TokUnknown; i++) {
            res = TraverseRule(rule.sub_rules_[i]);
            if (res == True)
              matching_count++;
            else if (res == False)  {
              if (rule_fail_actions_[entry]) { // Special actions for grammer fail situation.
                if ((this->*rule_fail_actions_[entry])() == Error)
                  return Error;
              }
              tokenizer_->SetTokPos(tok_pos);
              return True; // unmatched but return true
            }
            else if (res == Error) {
              assert(0 && "Error on Options");
              return Error;
            }
          }

          if (matching_count == i) { // matched
            if (rule_actions_[entry]) {
              if ((this->*rule_actions_[entry])() == Error)
                return Error;
            }
            return True;
          }
        }
        break;

      case Terminal:
        {
          if(tokenizer_->isToken(0, Lexer::TokenType(rule.sub_rules_[0]))) {
            // Run action
            if (rule_actions_[rule.sub_rules_[0]]) {
              if ((this->*rule_actions_[rule.sub_rules_[0]])() == Error)
                return Error;
            }
            tokenizer_->ConsumeToken(1);
            return True;
          }

          if (rule_fail_actions_[entry]) { // Special actions for grammer fail situation.
            if ((this->*rule_fail_actions_[entry])() == Error)
              return Error;
          }

          return False;
        }
        break;

      default:
        assert(0 && "Error wrong rule action.");
        break;
    }

    assert(0 && "Unexpected parsing error");
    return Error;
  }

  eResult SyntaxAnalyzer::TestRule(RuleName entry, bool check_user_type) {
    Rule rule = rules_[entry];
    eResult res;
    int tok_pos;

    switch(rule.action_) {
      case RepeatStar: // Repeat more than zero
        {
          while(true) {
            int matching_count = 0;
            int i = 0;
            tok_pos = tokenizer_->GetTokPos(); // backup token position
            for (; rule.sub_rules_[i] > TokUnknown; i++) {
              res = TestRule(rule.sub_rules_[i], check_user_type);
              if (res == True) // matched
                matching_count++;
              else if (res == False) { // not matching
                tokenizer_->SetTokPos(tok_pos); // restore token position.
                return True; // reached unmatching point.
              }
              else {
                assert(0 && "Error on Lookahead RepeatStar.");
                return Error;
              }
            }
          };
        }
        break;

      case RepeatDagger: // Repeat more than once
        {
          while(true) {
            bool success = false;
            int matching_count = 0;
            int i = 0;
            tok_pos = tokenizer_->GetTokPos(); // backup token position
            for (; rule.sub_rules_[i] > TokUnknown; i++) {
              res = TestRule(rule.sub_rules_[i], check_user_type);
              if (res == True) // matched
                matching_count++;
              else if (res == False) {// Rereached unmached point 
                tokenizer_->SetTokPos(tok_pos);
                if (success)
                  return True;
                return False;
              }
              else {
                assert(0 && "Error on Lookahead RepeatDagger.");
                return Error;
              }
            }

            if (matching_count == i) {
              success = true;
            }
          };
        }
        break;

      case Select: // select one of list
        {
          tok_pos = tokenizer_->GetTokPos(); // backup token position
          for (int i = 0; rule.sub_rules_[i] > TokUnknown; i++) {
            res = TestRule(rule.sub_rules_[i], check_user_type);
            if (res == True) {
              return True; // found matching
            }
            else if (res == False)
              tokenizer_->SetTokPos(tok_pos); // restore token position.
            else {
              assert(0 && "Error on Lookahead Select.");
              return Error;
            }
          }
          return False; // unmatched
        }
        break;

      case Sequence:
        {
          int matching_count = 0;
          int i = 0;
          tok_pos = tokenizer_->GetTokPos();
          for (; rule.sub_rules_[i] > TokUnknown; i++) {
            res = TestRule(rule.sub_rules_[i], check_user_type);
            if (res == True) // matched
              matching_count++;
            else if (res == False) {
              tokenizer_->SetTokPos(tok_pos);
              return False; // unmatched
            }
            else {
              assert(0 && "Error on Lookahead Sequence.");
              return Error;
            }
          }

          if (matching_count == i) {
            return True; // matched
          }
          tokenizer_->SetTokPos(tok_pos);
          return False; // unmatching
        }
        break;

      case Options:
        {
          int matching_count = 0;
          int i = 0;
          tok_pos = tokenizer_->GetTokPos(); // backup token position
          for (; rule.sub_rules_[i] > TokUnknown; i++) {
            res = TestRule(rule.sub_rules_[i], check_user_type);
            if (res == True)
              matching_count++;
            else if (res == False)  {
              tokenizer_->SetTokPos(tok_pos);
              return True; // unmatched but return true
            }
            else{
              assert(0 && "Error on Lookahead Options.");
              return Error;
            }
          }

          if (matching_count == i) // matched
            return True;
        }
        break;

      case Terminal:
        {
          if (!check_user_type) { // normal situation
            if(tokenizer_->isToken(0, Lexer::TokenType(rule.sub_rules_[0]))) {
              tokenizer_->ConsumeToken(1);
              return True;
            }
          }
          else { // need to check user type
            if(tokenizer_->isToken(0, Lexer::TokenType(rule.sub_rules_[0]))) {
              Lexer::Token tok = tokenizer_->GetCurToken(0);
              char* usertype_name = new char[tok.len + 1];
              memset(usertype_name, 0, tok.len + 1);
              strncpy(usertype_name, tok.c, tok.len);
              if (ac_->GetType(usertype_name)) {
                delete[] usertype_name;
                tokenizer_->ConsumeToken(1);
                return True;
              }
              delete[] usertype_name;
            }
          }

          return False;
        }
        break;

      default:
        assert(0 && "Error wrong rule action.");
        break;
    }

    assert(0 && "Unexpected Lookahead parsing error");
    return Error;
  }

  eResult SyntaxAnalyzer::StartParser() {
    return TraverseRule(RuleName::compilation_unit);
  }

  eResult SyntaxAnalyzer::CompilationUnit(void) {
    ParseInfo pi;
    AST::Declarations* decl = new AST::Declarations();

    // one of these
    // import : node
    // function : node
    // variable : variable vector
    // constant : node
    // class : node
    // typedef : node
    while(!parse_stack_.IsEmpty()) {
      pi = parse_stack_.Top();
      if (pi.type_ == ParseInfo::ASTNode) {
        if (pi.rule_name_ == RuleName::import_stmts)
          decl->AddImport((AST::ImportNode*)pi.data_.node_);
        if (pi.rule_name_ == RuleName::deffunc)
          decl->AddFunction((AST::FunctionDecl*)pi.data_.node_);
        if (pi.rule_name_ == RuleName::defconst)
          decl->AddConstant((AST::ConstantDecl*)pi.data_.node_);
        if (pi.rule_name_ == RuleName::defclass)
          decl->AddClass((AST::ClassNode*)pi.data_.node_);
        if (pi.rule_name_ == RuleName::typedef_)
          decl->AddTypedef((AST::TypedefNode*)pi.data_.node_);
      }
      else if (pi.type_ == ParseInfo::VarDeclList) {
        if (pi.rule_name_ == RuleName::defvars) {
          AST::VariableDecls *vars = pi.data_.vardecls_;
          for (int i = 0; i < vars->GetSize(); i++) {
            decl->AddVariable((*vars)[i]);
          }
          delete vars;
        }
      }
      else {
        assert(0 && "Unidentified grammar.");
        return Error;
      }

      parse_stack_.Pop();
    }

    ac_->SetLocalDecl(decl);

    return True;
  }

  eResult SyntaxAnalyzer::ImportStmts(void) {
    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::ASTNode ||
        pi.rule_name_ != RuleName::import_stmt) {
      assert(0 && "Wrong import_stmt.");
      return Error;
    }

    SetRuleNameForPI(RuleName::import_stmts);
    return True;
  }

  eResult SyntaxAnalyzer::ImportStmt() {
    ParseInfo pi_import = parse_stack_.Top();
    if(pi_import.type_ != ParseInfo::ASTNode ||
        pi_import.rule_name_ != RuleName::rep_dot_name) {
      assert(0 && "Wrong import path.");
      return Error;
    }
    parse_stack_.Pop();

    AST::ImportNode* im = (AST::ImportNode*)pi_import.data_.node_;
    im->Reverse();

    PushNode(im, RuleName::import_stmt);
    return True;
  }

  eResult SyntaxAnalyzer::Act_rep_dot_name(void) {
    ParseInfo pi_name, pi_import;
    AST::ImportNode* import_node = nullptr;
    
    // Read import path name
    pi_name = parse_stack_.Top();
    if (pi_name.type_ != ParseInfo::Identifier ||
        pi_name.rule_name_ != RuleName::name) {
      assert(0 && "Unidentified import path name.");
      return Error;
    }
    parse_stack_.Pop();

    // Read first path name or ImportNode
    pi_import = parse_stack_.Top();
    if(pi_import.type_ == ParseInfo::ASTNode && 
        pi_import.rule_name_ == RuleName::rep_dot_name) {
      import_node = (AST::ImportNode*)pi_import.data_.node_;
    }
    else if (pi_import.type_ == ParseInfo::Identifier &&
        pi_import.rule_name_ == RuleName::name) {
      import_node = new AST::ImportNode();
      
      char* first_import_path = new char[pi_import.cstr_len_ + 1];
      memset(first_import_path, 0, pi_import.cstr_len_);
      strncpy(first_import_path, pi_import.data_.cstr_,pi_import.cstr_len_);
      import_node->AddImportPath(first_import_path);
      delete[] first_import_path;
    }
    else {
      assert(0 && "Unidentified import path name.");
      return Error;
    }
    parse_stack_.Pop();

    char* import_path = new char[pi_name.cstr_len_ + 1];
    memset(import_path, 0, pi_name.cstr_len_ + 1);
    strncpy(import_path, pi_name.data_.cstr_, pi_name.cstr_len_);
    import_node->AddImportPath(import_path);
    delete[] import_path;

    PushNode(import_node, RuleName::rep_dot_name);
    return True;
  }

  eResult SyntaxAnalyzer::TopDefs(void) {
    ParseInfo pi = parse_stack_.Top();

    if (pi.rule_name_ != RuleName::deffunc &&
        pi.rule_name_ != RuleName::defvars &&
        pi.rule_name_ != RuleName::defconst && 
        pi.rule_name_ != RuleName::defclass &&
        pi.rule_name_ != RuleName::typedef_) {
      assert(0 && "Unidentified top level feature.");
      return Error;
    }

    return True;
  }

  eResult SyntaxAnalyzer::Name(void) {
    // check if stack top is Identifier.
    ParseInfo pi = parse_stack_.Top();
    if(pi.type_ != ParseInfo::Identifier) {
      assert(0 && "Unidentified Identifier.");
      return Error; 
    }

    SetRuleNameForPI(RuleName::name);
    return True;
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
    if (pi.type_ != ParseInfo::ASTType || 
        pi.rule_name_ != RuleName::typeref) {
      assert(0 && "Unidentified typeref.");
      return Error;
    }
    parse_stack_.Pop();
    
    // convert type to TypeNode 
    AST::TypeNode* ty = new AST::TypeNode(pi.data_.type_);
    PushNode(ty, RuleName::type);
    return True;
  }

  eResult SyntaxAnalyzer::TypeRef(void) {
    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::ASTType || 
         (pi.rule_name_ != RuleName::typeref_base && 
          pi.rule_name_ != RuleName::seq_unassigned_array &&
          pi.rule_name_ != RuleName::seq_assigned_array &&
          pi.rule_name_ != RuleName::seq_ptr &&
          pi.rule_name_ != RuleName::seq_func)) {
      assert(0 && "Unidentified type.");
      return Error;
    }

    SetRuleNameForPI(RuleName::typeref);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_unassigned_array(void) {
    ParseInfo pi = parse_stack_.Top();
    
    // Read base type
    if (pi.type_ != ParseInfo::ASTType || 
         (pi.rule_name_ != RuleName::typeref_base && 
          pi.rule_name_ != RuleName::seq_unassigned_array &&
          pi.rule_name_ != RuleName::seq_assigned_array &&
          pi.rule_name_ != RuleName::seq_ptr &&
          pi.rule_name_ != RuleName::seq_func)) {
      assert(0 && "Unidentified base type.");
      return Error;
    }
    parse_stack_.Pop();

    AST::Type* basety = pi.data_.type_;
    AST::ArrayType* arrty = AST::ArrayType::Get(ac_, basety);
    PushType((AST::Type*)arrty, RuleName::seq_unassigned_array);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_assigned_array(void) {
    ParseInfo pi_basety, pi_int;  

    // Read array size
    pi_int = parse_stack_.Top();
    if (pi_int.type_ != ParseInfo::Integer) {
      assert(0 && "Unidentified array size.");
      return Error;
    }
    parse_stack_.Pop();
    
    // Read base type
    pi_basety = parse_stack_.Top();
    if (pi_basety.type_ != ParseInfo::ASTType ||
         (pi_basety.rule_name_ != RuleName::typeref_base && 
          pi_basety.rule_name_ != RuleName::seq_unassigned_array &&
          pi_basety.rule_name_ != RuleName::seq_assigned_array &&
          pi_basety.rule_name_ != RuleName::seq_ptr &&
          pi_basety.rule_name_ != RuleName::seq_func)) {
      assert(0 && "Unidentified base type.");
      return Error;
    }
    parse_stack_.Pop();

    AST::Type* basety = pi_basety.data_.type_;
    
    long array_size = pi_int.data_.integer_;
    if (array_size < 0) {
      assert(0 && "Invalid array size.");
      return Error;
    }

    AST::ArrayType* arrty = AST::ArrayType::Get(ac_, basety, array_size);
    PushType((AST::Type*)arrty, RuleName::seq_assigned_array);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_ptr(void) {
    ParseInfo pi_basety;

    // read base type
    pi_basety = parse_stack_.Top();
    if (pi_basety.type_ != ParseInfo::ASTType ||
       (pi_basety.rule_name_ != RuleName::typeref_base && 
        pi_basety.rule_name_ != RuleName::seq_unassigned_array &&
        pi_basety.rule_name_ != RuleName::seq_assigned_array &&
        pi_basety.rule_name_ != RuleName::seq_ptr &&
        pi_basety.rule_name_ != RuleName::seq_func)) {
      assert(0 && "Unidentified base type.");
      return Error;
    }
    parse_stack_.Pop();

    AST::Type* basety = pi_basety.data_.type_;

    AST::PointerType* ptrty = AST::PointerType::Get(ac_, basety);
    PushType((AST::Type*)ptrty, RuleName::seq_ptr);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_func(void) {
    ParseInfo pi_params, pi_retty;

    // Read param list
    pi_params = parse_stack_.Top();
    if (pi_params.type_ != ParseInfo::Types || 
        pi_params.rule_name_ != param_typerefs) {
      assert(0 && "Unidentified param type list.");
      return Error;
    }
    parse_stack_.Pop();

    AST::Types* params = pi_params.data_.types_;

    // Read return type
    pi_retty = parse_stack_.Top();
    if (pi_retty.type_ != ParseInfo::ASTType || 
       (pi_retty.rule_name_ != RuleName::typeref_base && 
        pi_retty.rule_name_ != RuleName::seq_unassigned_array &&
        pi_retty.rule_name_ != RuleName::seq_assigned_array &&
        pi_retty.rule_name_ != RuleName::seq_ptr &&
        pi_retty.rule_name_ != RuleName::seq_func)) {
      assert(0 && "Unidentified return type.");
      return Error;
    }
    parse_stack_.Pop();

    AST::Type* retty = pi_retty.data_.type_; // get return type

    // create function type
    AST::FunctionType* fnty = AST::FunctionType::Get(ac_, retty, *params);
    PushType((AST::Type*)fnty, RuleName::seq_func);
    
    delete params; // delete parameter type list container.
    return True;
  }

  eResult SyntaxAnalyzer::TypeRefBase(void) {
    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::ASTType ||
        (pi.rule_name_ != RuleName::seq_void && 
         pi.rule_name_ != RuleName::seq_char &&
         pi.rule_name_ != RuleName::seq_short && 
         pi.rule_name_ != RuleName::seq_int &&
         pi.rule_name_ != RuleName::seq_long &&
         pi.rule_name_ != RuleName::seq_unsigned_char &&
         pi.rule_name_ != RuleName::seq_unsigned_short &&
         pi.rule_name_ != RuleName::seq_unsigned_int &&
         pi.rule_name_ != RuleName::seq_unsigned_long &&
         pi.rule_name_ != RuleName::seq_float &&
         pi.rule_name_ != RuleName::seq_double &&
         pi.rule_name_ != RuleName::seq_class_identifier &&
         pi.rule_name_ != RuleName::seq_user_type)) {
      assert(0 && "Unidentified typeref base type.");
      return Error;
    }

    SetRuleNameForPI(RuleName::typeref_base);
    return True;
  }
  // create primitive types
  eResult SyntaxAnalyzer::Act_seq_void(void) {
    AST::Type* ty = (AST::Type*)AST::VoidType::Get(ac_);
    PushType(ty, RuleName::seq_void);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_char(void){
    AST::Type* ty = (AST::Type*)AST::CharType::Get(ac_, 
        AST::IntegerType::Signed);
    PushType(ty, RuleName::seq_char);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_short(void){
    AST::Type* ty = (AST::Type*)AST::ShortType::Get(ac_, 
        AST::IntegerType::Signed);
    PushType(ty, RuleName::seq_short);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_int(void){
    AST::Type* ty = (AST::Type*)AST::IntType::Get(ac_, 
        AST::IntegerType::Signed);
    PushType(ty, RuleName::seq_int);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_long(void){
    AST::Type* ty = (AST::Type*)AST::LongType::Get(ac_, 
        AST::IntegerType::Signed);
    PushType(ty, RuleName::seq_long);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_char(void){
    AST::Type* ty = (AST::Type*)AST::CharType::Get(ac_, 
        AST::IntegerType::Unsigned);
    PushType(ty, RuleName::seq_unsigned_char);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_short(void){
    AST::Type* ty = (AST::Type*)AST::ShortType::Get(ac_, 
        AST::IntegerType::Unsigned);
    PushType(ty, RuleName::seq_unsigned_short);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_int(void){
    AST::Type* ty = (AST::Type*)AST::IntType::Get(ac_, 
        AST::IntegerType::Unsigned);
    PushType(ty, RuleName::seq_unsigned_int);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_unsigned_long(void){
    AST::Type* ty = (AST::Type*)AST::LongType::Get(ac_, 
        AST::IntegerType::Unsigned);
    PushType(ty, RuleName::seq_unsigned_long);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_float(void){
    AST::Type* ty = (AST::Type*)AST::FloatType::Get(ac_);
    PushType(ty, RuleName::seq_float);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_double(void){
    AST::Type* ty = (AST::Type*)AST::DoubleType::Get(ac_);
    PushType(ty, RuleName::seq_double);
    return True;
  }
  eResult SyntaxAnalyzer::Act_seq_class_identifier(void){
    // forward class declaration.
    ParseInfo pi_iden = parse_stack_.Top();
    if (pi_iden.type_ != ParseInfo::Identifier) {
      assert(0 && "Unidentified Identifier");
      return Error;
    }
    parse_stack_.Pop();

    char* iden = new char[pi_iden.cstr_len_ + 1];
    memset(iden, 0, pi_iden.cstr_len_ + 1);
    strncpy(iden, pi_iden.data_.cstr_, pi_iden.cstr_len_);
    AST::ClassType* cty = AST::ClassType::Get(ac_, iden);
    delete[] iden;
    if (!cty) {
      assert(0 && "Failed to assign ClassType.");
      return Error;
    }
    PushType(cty, RuleName::seq_class_identifier);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_user_type(void) {
    // Read user type
    ParseInfo pi_iden = parse_stack_.Top();
    if (pi_iden.type_ != ParseInfo::Identifier) {
      assert(0 && "Unidentified Identifier");
      return Error;
    }
    parse_stack_.Pop();

    char* iden = new char[pi_iden.cstr_len_ + 1];
    memset(iden, 0, pi_iden.cstr_len_ + 1);
    strncpy(iden, pi_iden.data_.cstr_, pi_iden.cstr_len_);
    AST::Type* ty = ac_->GetType(iden);
    delete[] iden;
    if (!ty || !ty->IsKindOf(AST::Type::UserTy)) {
      assert(0 && "Can't find UserType.");
      return Error;
    }
    PushType(ty, RuleName::seq_user_type);
    return True;
  }

  eResult SyntaxAnalyzer::TypeDef(void) {
    ParseInfo pi_id, pi_type;

    // read new type name
    pi_id = parse_stack_.Top();
    if (pi_id.type_ != ParseInfo::Identifier) {
      assert(0 && "Unidentified Identifier");
      return Error;
    }
    parse_stack_.Pop();

    // read type
    pi_type = parse_stack_.Top();
    if(pi_type.type_ != ParseInfo::ASTType ||
       pi_type.rule_name_ != RuleName::typeref) {
      assert(0 && "Unidentified typeref");
      return Error;
    }
    parse_stack_.Pop();

    // create temp new typename char buff
    char* tyname = new char[pi_id.cstr_len_ + 1];
    memset(tyname, 0, pi_id.cstr_len_ + 1);
    strncpy(tyname, pi_id.data_.cstr_, pi_id.cstr_len_);

    // create TypeNode
    AST::UserType* ut = AST::UserType::Get(ac_, pi_type.data_.type_, tyname);
    AST::TypeNode* user_type = new AST::TypeNode(ut);

    AST::TypedefNode* typedef_node = new AST::TypedefNode(user_type, tyname);
    delete[] tyname;

    PushNode(typedef_node, RuleName::typedef_);
    return True;
  }

  eResult SyntaxAnalyzer::ParamTypeRefs(void) {
    ParseInfo pi = parse_stack_.Top();
    
    if (pi.type_ != ParseInfo::Types ||
        (pi.rule_name_ != RuleName::seq_param_type_void &&
        pi.rule_name_ != RuleName::seq_param_type_list)) {
      assert(0 && "Unidentified Param typeref");
      return Error;
    }

    SetRuleNameForPI(RuleName::param_typerefs);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_param_type_void(void) {
    // create void type
    AST::Type* voidty = (AST::Type*)AST::VoidType::Get(ac_);
    // Create type node list
    AST::Types* param_types = new AST::Types();

    param_types->PushBack(voidty);
    PushTypes(param_types, RuleName::seq_param_type_void);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_param_type_list(void) {
    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::Types || 
        (pi.rule_name_ != RuleName::rep_param_comma_type && 
         pi.rule_name_ != RuleName::opt_vararg_type)) {
      assert(0 && "Unidentified param list or vararg");
      return Error;
    }

    SetRuleNameForPI(RuleName::seq_param_type_list);
    return True;
  }

  eResult SyntaxAnalyzer::Act_rep_param_comma_type(void) {
    ParseInfo pi_type, pi_type_list;
    AST::Types* param_types = nullptr;

    // read type
    if (pi_type.type_ != ParseInfo::ASTType ||
        pi_type.rule_name_ != RuleName::typeref) {
      assert(0 && "Unidentified typeref");
      return Error;
    }
    parse_stack_.Pop();

    pi_type_list = parse_stack_.Top();
    if (pi_type_list.type_ == ParseInfo::Types &&
        pi_type_list.rule_name_ == RuleName::rep_param_comma_type) {
      param_types = pi_type_list.data_.types_;
    }
    else if(pi_type_list.type_ == ParseInfo::ASTType && 
        pi_type_list.rule_name_ == RuleName::typeref) {
      // in case this is just first param type
      param_types = new AST::Types();
      param_types->PushBack(pi_type_list.data_.type_);
    }
    else {
      assert(0 && "Unidentified type list");
      return Error;
    }
    parse_stack_.Pop();

    param_types->PushBack(pi_type.data_.type_);// push new param type
    PushTypes(param_types, RuleName::rep_param_comma_type);
    return True;
  }

  eResult SyntaxAnalyzer::Act_opt_vararg_type(void) {
    ParseInfo pi = parse_stack_.Top();

    // read param list
    if (pi.type_ != ParseInfo::Types ||
        pi.rule_name_ != RuleName::rep_param_comma_type) {
      assert(0 && "Unidentified param list");
      return Error;
    }
    parse_stack_.Pop();

    AST::Types* param_types = pi.data_.types_;
    AST::Type* ty = (AST::Type*)AST::VarArgType::Get(ac_);
    param_types->PushBack(ty);
    
    PushTypes(param_types, RuleName::opt_vararg_type);
    return True;
  }

  eResult SyntaxAnalyzer::DefFunc(void) {
    ParseInfo pi_block, pi_params, pi_name, pi_type, pi_storage;
    AST::BlockNode* body = nullptr;
    AST::ParamNodes* parms = nullptr;
    char* fn_name = nullptr;
    AST::Type* ret_type = nullptr;

    // Read block (function body)
    pi_block = parse_stack_.Top();
    if (pi_block.type_ != ParseInfo::ASTNode || 
        pi_block.rule_name_ != RuleName::block) {
      assert(0 && "Invalid block");
      return Error;
    }
    parse_stack_.Pop();
    body = (AST::BlockNode*)pi_block.data_.node_;
    
    // Read params
    pi_params = parse_stack_.Top();
    if (pi_params.type_ != ParseInfo::ParamNodeList || 
        pi_params.rule_name_ != RuleName::params) {
      assert(0 && "Invalid function parameters. ");
      return Error;
    }
    parse_stack_.Pop();
    parms = pi_params.data_.param_nodes_;

    // Read name
    pi_name = parse_stack_.Top();
    if (pi_name.type_ != ParseInfo::Identifier || 
        pi_name.rule_name_ != RuleName::name) {
      assert(0 && "Invalid function name");
      return Error;
    }
    parse_stack_.Pop();
    fn_name = new char[pi_name.cstr_len_+1];
    memset(fn_name, 0, pi_name.cstr_len_+1);
    strncpy(fn_name, pi_name.data_.cstr_, pi_name.cstr_len_);

    // Read return type
    pi_type = parse_stack_.Top();
    if (pi_type.type_ != ParseInfo::ASTType || 
        pi_type.rule_name_ != RuleName::typeref) {
      assert(0 && "Invalid function return type");
      return Error;
    }
    parse_stack_.Pop();
    ret_type = pi_type.data_.type_;
    AST::TypeNode* ret_ty_node = new AST::TypeNode(ret_type);

    // Read pi_storage
    bool is_storage = false;
    pi_storage = parse_stack_.Top();
    if (pi_storage.type_ == ParseInfo::StorageInfo && 
        pi_storage.rule_name_ == RuleName::storage) {
      // in case there is storage info
      parse_stack_.Pop();
      is_storage = true;
    }

    // create function 
    AST::FunctionDecl* func = 
      new AST::FunctionDecl(is_storage, ret_ty_node, fn_name, parms, body);
    delete[] fn_name;
    delete parms;

    PushNode(func, RuleName::deffunc);
    return True;
  }

  eResult SyntaxAnalyzer::Block(void) {
    ParseInfo pi;
    AST::BlockNode* blk = new AST::BlockNode();
    AST::StmtNodes* all_stmts = new AST::StmtNodes();

    /// Read stmts
    pi = parse_stack_.Top();
    while(pi.rule_name_ == RuleName::defvar_list ||
          pi.rule_name_ == RuleName::stmts) {
      parse_stack_.Pop();

      if (pi.type_ == ParseInfo::VarDeclList) {
        for (int i = 0; i < pi.data_.vardecls_->GetSize(); i++) {
          blk->AddVariable((*pi.data_.vardecls_)[i]);
        }
        delete pi.data_.vardecls_;
      }
      else if (pi.type_ == ParseInfo::StmtNodeList) {
        for (int i = 0; i < pi.data_.stmt_nodes_->GetSize(); i++) {
          all_stmts->PushBack((*pi.data_.stmt_nodes_)[i]);
        }
        delete pi.data_.stmt_nodes_;
      }
      else  {
        assert(0 && "Unidentified stmt in block.");
        return Error;
      }

      pi = parse_stack_.Top();
    }

    // Check brace open
    if (pi.type_ != ParseInfo::TokenType || 
        pi.rule_name_ != RuleName::TokBraceOpen) {
      assert(0 && "Unmatched brace.");
      return Error;
    }
    parse_stack_.Pop();

    all_stmts->Reverse();
    for (int i = 0; i < all_stmts->GetSize(); i++) {
      blk->AddStmt((*all_stmts)[i]);
    }
    delete all_stmts;

    PushNode((AST::BaseNode*)blk, RuleName::block);
    return True;
  }

  eResult SyntaxAnalyzer::DefClass(void) {
    ParseInfo pi_class_member, pi_brace_open, pi_class_name;
    AST::ClassNode* class_node = new AST::ClassNode();

    pi_class_member = parse_stack_.Top();
    while(pi_class_member.rule_name_ == RuleName::seq_class_member_variable || 
        pi_class_member.rule_name_ == RuleName::seq_class_member_function) {
      parse_stack_.Pop();

      if (pi_class_member.rule_name_ == RuleName::seq_class_member_variable) {
        AST::VariableDecls* vars;
        if (pi_class_member.type_ == ParseInfo::VarDeclList) {
          vars = pi_class_member.data_.vardecls_;
          for (int i=vars->GetSize()-1; i >= 0; i--) {
            class_node->AddMemVariable((*vars)[i]);
          }
          delete vars; // removes simplevector
        }
        else  {
          assert(0 && "Invalid class member variable list.");
          return Error;
        }
      }
      else {
        if (pi_class_member.type_ == ParseInfo::ASTNode) {
          AST::FunctionDecl* fd = (AST::FunctionDecl*)pi_class_member.data_.node_;
          class_node->AddMemFunction(fd);
        }
        else  {
          assert(0 && "Invalid class member function list.");
          return Error;
        }
      }
      pi_class_member = parse_stack_.Top();
    }

    // Check brace matching
    pi_brace_open = parse_stack_.Top();
    if (pi_brace_open .type_ != ParseInfo::TokenType || 
        pi_brace_open .rule_name_ != RuleName::TokBraceOpen) {
      assert(0 && "Unmatched brace.");
      return Error;
    }
    parse_stack_.Pop();

    // Read Class type name
    pi_class_name = parse_stack_.Top();
    if (pi_class_name.type_ != ParseInfo::Identifier ||
        pi_class_name.rule_name_ != RuleName::name) {
      assert(0 && "Invalid class name.");
      return Error;
    }
    parse_stack_.Pop();

    // Set class name
    char* classname = new char[pi_class_name.cstr_len_ + 1];
    memset(classname, 0, pi_class_name.cstr_len_ + 1);
    strncpy(classname, pi_class_name.data_.cstr_, pi_class_name.cstr_len_);
    class_node->SetTypeName(classname);
    // Set class type
    AST::ClassType* clsty = AST::ClassType::Get(ac_, classname);
    AST::TypeNode* class_ty_node = new AST::TypeNode(clsty);
    class_node->SetType(class_ty_node);
    delete[] classname;

    // reverse order
    class_node->ReverseVariableOrder();
    class_node->ReverseFunctionOrder();

    PushNode(class_node, RuleName::defclass);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_class_member_variable(void) {
    ParseInfo pi_vars = parse_stack_.Top();

    // Read class memeber variable
    if (pi_vars.type_ != ParseInfo::VarDeclList|| 
        pi_vars.rule_name_ != RuleName::defvars) {
      assert(0 && "Invalid class name.");
      return Error;
    }

    SetRuleNameForPI(RuleName::seq_class_member_variable);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_class_member_function(void) {
    ParseInfo pi_func = parse_stack_.Top();

    // Read class memeber variable
    if (pi_func.type_ != ParseInfo::ASTNode || 
        pi_func.rule_name_ != RuleName::deffunc) {
      assert(0 && "Invalid class name.");
      return Error;
    }

    SetRuleNameForPI(RuleName::seq_class_member_function);
    return True;
  }

  eResult SyntaxAnalyzer::Param(void) {
    ParseInfo pi_type, pi_name;

    // Read name
    pi_name = parse_stack_.Top();
    if (pi_name.type_ != ParseInfo::Identifier ||
        pi_name.rule_name_ != RuleName::name) {
      assert(0 && "Invalid param name.");
      return Error;
    }
    parse_stack_.Pop();

    // Read type
    pi_type = parse_stack_.Top();
    if (pi_type.type_ != ParseInfo::ASTNode ||
        pi_type.rule_name_ != RuleName::type) {
      assert(0 && "Invalid param type.");
      return Error;
    }
    parse_stack_.Pop();

    char* param_name = new char[pi_name.cstr_len_+1];
    memset(param_name, 0, pi_name.cstr_len_+1);
    strncpy(param_name, pi_name.data_.cstr_, pi_name.cstr_len_);
    AST::ParamNode* param_node = 
      new AST::ParamNode((AST::TypeNode*)pi_type.data_.node_, param_name, false);
    delete[] param_name;

    PushNode(param_node, RuleName::param);
    return True;
  }

  eResult SyntaxAnalyzer::Params(void) {
    ParseInfo pi = parse_stack_.Top();

    // confirm param list 
    if (pi.type_ != ParseInfo::ParamNodeList ||
       (pi.rule_name_ != RuleName::seq_param_void &&
        pi.rule_name_ != RuleName::seq_param_list)) {
      assert(0 && "Invalid param list.");
      return Error;
    }

    SetRuleNameForPI(RuleName::params);
    return True;
  }
        
  eResult SyntaxAnalyzer::Act_seq_param_void(void) {
    AST::ParamNodes* params = new AST::ParamNodes();

    AST::VoidType* voidty = AST::VoidType::Get(ac_);
    AST::TypeNode* void_ty_node = new AST::TypeNode(voidty);

    AST::ParamNode* param_void= new AST::ParamNode(void_ty_node, "", false);
    params->PushBack(param_void);
    PushParamNodes(params, RuleName::seq_param_void);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_param_list(void) {
    ParseInfo pi = parse_stack_.Top();

    if (pi.type_ != ParseInfo::ParamNodeList ||
        (pi.rule_name_ != RuleName::opt_vararg &&
         pi.rule_name_ != RuleName::fixedparams)) {
      assert(0 && "Invalid parameters");
      return Error; 
    }

    SetRuleNameForPI(RuleName::seq_param_list);
    return True;
  }
        
  eResult SyntaxAnalyzer::Act_opt_vararg(void) {
    ParseInfo pi = parse_stack_.Top();

    // read param list
    if (pi.type_ != ParseInfo::ParamNodeList ||
        pi.rule_name_ != RuleName::fixedparams) {
      assert(0 && "Invalid fixed param list.");
      return Error;
    }
    parse_stack_.Pop();

    AST::ParamNodes* params = pi.data_.param_nodes_;
    AST::ParamNode* var_arg = new AST::ParamNode();
    var_arg->SetVarArgs(true);
    var_arg->SetName("...");
    params->PushBack(var_arg);
    
    PushParamNodes(params, RuleName::opt_vararg);
    return True;
  }
  
  eResult SyntaxAnalyzer::FixedParams(void) {
    ParseInfo pi_params;
    AST::ParamNodes* params = nullptr;

    pi_params = parse_stack_.Top();
    // single parameter
    if (pi_params.type_ == ParseInfo::ASTNode && 
        pi_params.rule_name_ == RuleName::param) {
      parse_stack_.Pop();
      // In case node is first param
      params = new AST::ParamNodes();
      params->PushBack((AST::ParamNode*)pi_params.data_.node_);
      PushParamNodes(params, RuleName::fixedparams);
      return True;
    }

    // multi parameters
    if (pi_params.type_ == ParseInfo::ParamNodeList &&
         pi_params.rule_name_ == RuleName::rep_comma_param) {
      SetRuleNameForPI(RuleName::fixedparams);
      return True;
    }

    assert(0 && "Invalid fixed param.");
    return Error;
  }

  eResult SyntaxAnalyzer::Act_rep_comma_param(void) {
    ParseInfo pi_paramlist_or_param, pi_param;
    AST::ParamNodes* params = nullptr;
    
    // Read last param
    pi_param = parse_stack_.Top();
    if (pi_param.type_ != ParseInfo::ASTNode ||
        pi_param.rule_name_ != RuleName::param ) {
      assert(0 && "Invalid param.");
      return Error;
    }
    parse_stack_.Pop();

    // Read first param or param list
    pi_paramlist_or_param = parse_stack_.Top();
    if (pi_paramlist_or_param.type_ == ParseInfo::ASTNode && 
        pi_paramlist_or_param.rule_name_ == RuleName::param ) {
      // In case node is first param
      params = new AST::ParamNodes();
      params->PushBack((AST::ParamNode*)pi_paramlist_or_param.data_.node_);
    }
    else if (pi_paramlist_or_param.type_ == ParseInfo::ParamNodeList && 
        pi_paramlist_or_param.rule_name_ == RuleName::fixedparams) {
      params = pi_paramlist_or_param.data_.param_nodes_;
    }
    else {
      assert(0 && "Unidentified param.");
      return Error;
    }
    parse_stack_.Pop();
    params->PushBack((AST::ParamNode*)pi_param.data_.node_);

    PushParamNodes(params, RuleName::rep_comma_param);
    return True;
  }


  eResult SyntaxAnalyzer::DefVars(void) {
    ParseInfo pi;
    AST::BaseNode* expr_node = nullptr;
    AST::VariableDecls* vardecls = new AST::VariableDecls();

    while(!parse_stack_.IsEmpty()) {
      expr_node = nullptr;

      // In case we have variable initializer.
      pi = parse_stack_.Top();
      if (pi.type_ == ParseInfo::ASTNode &&
          pi.rule_name_ == RuleName::expr) {
        parse_stack_.Pop();
        expr_node = pi.data_.node_;
        if (!expr_node->IsKindOf(AST::BaseNode::ExprNodeTy)) {
          assert(0 && "Unidentified variable initializer.");
          return Error;
        }
      }

      // Read variable name
      pi = parse_stack_.Top(); // read variable name
      if (pi.type_ != ParseInfo::Identifier ||
          pi.rule_name_ != RuleName::name) {
        assert(0 && "Unidentified variable name.");
        return Error;
      }
      parse_stack_.Pop();
      AST::VariableDecl* new_var = new AST::VariableDecl();
      if(expr_node)
        new_var->SetInit((AST::ExprNode*)expr_node);
      new_var->SetName(pi.data_.cstr_, pi.cstr_len_);
      vardecls->PushBack(new_var);

      // Read variable type
      pi = parse_stack_.Top(); // read variable type
      if (pi.type_ == ParseInfo::ASTNode && 
          pi.rule_name_ == RuleName::type) {
        parse_stack_.Pop();
        bool is_static = false;
        AST::TypeNode* ty = (AST::TypeNode*)pi.data_.node_;

        // Read storage info if possible
        pi = parse_stack_.Top();
        if (pi.type_ == ParseInfo::StorageInfo) {
          is_static = true;
          parse_stack_.Pop();
        }

        if (vardecls->GetSize() == 0) {
          assert(0 && "No defined variable name.");
          return Error;
        }

        for (int i = 0; i < vardecls->GetSize(); i++) {
          // In order not to share same typenode pointer, we create every single new typenode.
          AST::TypeNode* tn = new AST::TypeNode(ty->GetType());
          (*vardecls)[i]->SetStorage(is_static);
          (*vardecls)[i]->SetType(tn);
        }
        delete ty;

        vardecls->Reverse();
        PushVarDecls(vardecls, RuleName::defvars);
        return True;
      }
    }

    assert(0 && "Error on variable definition.");
    return Error;
  }

  eResult SyntaxAnalyzer::DefConst(void) {
    ParseInfo pi_expr, pi_name, pi_type;
    
    // Read constant initializer
    pi_expr = parse_stack_.Top();
    if(pi_expr.type_ != ParseInfo::ASTNode || 
       pi_expr.rule_name_ != RuleName::expr) {
      assert(0 && "Invalid constant initializer.");
      return Error;
    }
    parse_stack_.Pop();
    
    // Read name 
    pi_name = parse_stack_.Top();
    if(pi_name.type_ != ParseInfo::Identifier || 
       pi_name.rule_name_ != RuleName::name) {
      assert(0 && "Invalid constant name.");
      return Error;
    }
    parse_stack_.Pop();

    // Read type 
    pi_type = parse_stack_.Top();
    if(pi_type.type_ != ParseInfo::ASTNode || 
       pi_type.rule_name_ != RuleName::type) {
      assert(0 && "Invalid constant type.");
      return Error;
    }
    parse_stack_.Pop();

    char* const_name = new char[pi_name.cstr_len_+1];
    memset(const_name, 0, pi_name.cstr_len_+1);
    strncpy(const_name, pi_name.data_.cstr_, pi_name.cstr_len_);
    AST::ConstantDecl* constdecl = new AST::ConstantDecl((AST::TypeNode*)pi_type.data_.node_,
        const_name, (AST::ExprNode*)pi_expr.data_.node_);
    delete[] const_name;

    PushNode(constdecl, RuleName::defconst);
    return True;
  }

  eResult SyntaxAnalyzer::DefVarList(void) {
    // Read first vars
    AST::VariableDecls *all_vars = nullptr, *new_vars = nullptr;

    // read variables
    ParseInfo pi_vars = parse_stack_.Top();
    if (pi_vars.type_ != ParseInfo::VarDeclList ||
        pi_vars.rule_name_ != RuleName::defvars) {
      assert( 0 && "Invalid variable definitions.");
      return Error;
    }
    parse_stack_.Pop();
    new_vars = pi_vars.data_.vardecls_;
    
    pi_vars = parse_stack_.Top();
    if (pi_vars.type_ != ParseInfo::VarDeclList ||
        pi_vars.rule_name_ != RuleName::defvar_list) {
      // In case, first variable list.
      PushVarDecls(new_vars, RuleName::defvar_list);
    }
    else {
      // second variable list.
      parse_stack_.Pop();
      all_vars = pi_vars.data_.vardecls_;
      for(int i = 0; i < new_vars->GetSize() ; i++) {
        all_vars->PushBack((*new_vars)[i]);
      }
      delete new_vars; // removes simplevector
      PushVarDecls(all_vars, RuleName::defvar_list);
    }
    return True;
  }

  eResult SyntaxAnalyzer::Term(void) {
    // actually do nothing
    ParseInfo pi_expr = parse_stack_.Top();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy) ||
        (pi_expr.rule_name_ != RuleName::seq_type_term && 
         pi_expr.rule_name_ != RuleName::unary)) {
      assert(0 && "Error on term casting");
      return Error;
    }

    SetRuleNameForPI(RuleName::term);// mark top with term.
    return True;
  }

  //"(" type ")" term          // type casting 
  eResult SyntaxAnalyzer::Act_seq_type_term(void) {
    // get term
    ParseInfo pi_term = parse_stack_.Top();
    if (pi_term.type_ != ParseInfo::ASTNode || 
        !pi_term.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy) ||
        pi_term.rule_name_ != RuleName::term) {
      assert(0 && "Error on term casting");
      return Error;
    }
    parse_stack_.Pop();

    // get type
    ParseInfo pi_type = parse_stack_.Top();
    if (pi_type.type_ != ParseInfo::ASTNode || 
        !pi_type.data_.node_->IsKindOf(AST::BaseNode::TypeNodeTy) ||
        pi_type.rule_name_ != RuleName::type) {
      assert(0 && "Error on term TypeNode");
      return Error;
    }
    parse_stack_.Pop();

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
      assert(0 && "Error on Postfix() : Needed parsing info!");
      return Error;
    }

    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::ASTNode || 
       (pi.rule_name_ != RuleName::seq_preinc_unary && 
        pi.rule_name_ != RuleName::seq_predec_unary &&
        pi.rule_name_ != RuleName::seq_pos_term &&
        pi.rule_name_ != RuleName::seq_neg_term &&
        pi.rule_name_ != RuleName::seq_not_term &&
        pi.rule_name_ != RuleName::seq_bitnot_term &&
        pi.rule_name_ != RuleName::seq_ptr_term &&
        pi.rule_name_ != RuleName::seq_adr_term &&
        pi.rule_name_ != RuleName::seq_sizeof_type &&
        pi.rule_name_ != RuleName::seq_sizeof_unary &&
        pi.rule_name_ != RuleName::primary)) {
      assert(0 && "Error on Postfix() : Needed ASTNode in stack!");
      return Error;
    }

    SetRuleNameForPI(RuleName::unary);
    return True;
  }

  // "++" unary
  eResult SyntaxAnalyzer::Act_seq_preinc_unary(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert(0 && "Error on unary prefix increase");
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
      assert(0 && "Error on unary prefix increase");
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
      assert(0 && "Error on unary positive");
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
      assert(0 && "Error on unary positive");
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
      assert(0 && "Error on unary positive");
      return Error;
    }

    AST::UnaryOpNode* node = 
      new AST::UnaryOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::Not);

    PushNode(node, RuleName::seq_not_term);
    return True;
  }

  // "~" term 
  eResult SyntaxAnalyzer::Act_seq_bitnot_term(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        pi_expr.rule_name_ != RuleName::term) {
      assert(0 && "Error on unary positive");
      return Error;
    }
    parse_stack_.Pop();

    AST::UnaryOpNode* node = 
      new AST::UnaryOpNode((AST::ExprNode*)pi_expr.data_.node_, AST::UnaryOpNode::BitNot);

    PushNode(node, RuleName::seq_bitnot_term);
    return True;
  }

  // "*" term 
  eResult SyntaxAnalyzer::Act_seq_ptr_term(void) {
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        !pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert(0 && "Error on unary positive");
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
      assert(0 && "Error on unary positive");
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
      assert(0 && "Error on unary positive");
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
      assert(0 && "Error on unary positive");
      return Error;
    }

    AST::SizeofExprNode* node = 
      new AST::SizeofExprNode((AST::ExprNode*)pi_expr.data_.node_);

    PushNode(node, RuleName::seq_sizeof_unary);
    return True;
  }

  eResult SyntaxAnalyzer::Postfix(void) {
    if (parse_stack_.IsEmpty())  {
      assert(0 && "Error on Postfix() : Needed parsing info!");
      return Error;
    }

    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::ASTNode) {
      assert(0 && "Error on Postfix() : Needed ASTNode in stack!");
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
      assert(0 && "Error on postfix unary increase");
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
      assert(0 && "Error on postfix unary decrease");
      return Error;
    }
    
    if (!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert(0 && "Error on postfix unary decrease");
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
      assert(0 && "Error on postfix array reference");
      return Error;
    }
    if(!pi_arrsize_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert(0 && "Error on postfix array reference");
      return Error;
    }

    // get expr
    ParseInfo pi_array_base = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_array_base.type_ != ParseInfo::ASTNode) {
      assert(0 && "Error on postfix array reference");
      return Error;
    }
    if(!pi_array_base.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert(0 && "Error on postfix array reference");
      return Error;
    }

    AST::ArrayRefNode * arnode = 
      new AST::ArrayRefNode((AST::ExprNode*)pi_array_base.data_.node_, 
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
      assert(0 && "Error on Postfix member reference");
      return Error;
    }

    // get expr
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode) {
      assert(0 && "Error on postfix array reference");
      return Error;
    }
    if(!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert(0 && "Error on postfix array reference");
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
      assert(0 && "Error on Postfix member reference");
      return Error;
    }

    // get expr
    ParseInfo pi_expr = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_expr.type_ != ParseInfo::ASTNode) {
      assert(0 && "Error on postfix array reference");
      return Error;
    }
    if(!pi_expr.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert(0 && "Error on postfix array reference");
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
    if (pi_args.type_ != ParseInfo::ASTNode) {
      assert(0 && "Invalid function call argument");
      return Error;
    }
    if (!pi_args.data_.node_->IsKindOf(AST::BaseNode::ArgsNodeTy)) {
      assert(0 && "Invalid function call argument");
      return Error;
    }
    // get function 
    ParseInfo pi_func = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_func.type_ != ParseInfo::ASTNode) {
      assert(0 && "Invalid function function call");
      return Error;
    }
    if (!pi_func.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert(0 && "Invalid function function call");
      return Error;
    }

    // create new pi
    AST::FuncCallNode* fcall= 
      new AST::FuncCallNode((AST::ExprNode*)pi_func.data_.node_, 
          (AST::ArgsNode*)pi_args.data_.node_);
    PushNode(fcall, RuleName::seq_fncall);
    return True;
  }

  eResult SyntaxAnalyzer::Primary(void) {
    if (parse_stack_.IsEmpty())  {
      assert(0 && "Error on Primary() : Need to child data!");
      return Error;
    }
    
    ParseInfo top_pi = parse_stack_.Top();

    if (top_pi.type_ == ParseInfo::Integer) {
      parse_stack_.Pop();

      AST::IntegerLiteralNode * node = 
        new AST::IntegerLiteralNode(AST::IntegerLiteralNode::Int, top_pi.data_.integer_);
      PushNode(node, RuleName::primary);
    }
    else if (top_pi.type_ == ParseInfo::Character) {
      parse_stack_.Pop();

      AST::IntegerLiteralNode * node = 
        new AST::IntegerLiteralNode(AST::IntegerLiteralNode::Char, top_pi.data_.integer_);
      PushNode(node, RuleName::primary);
    }
    else if (top_pi.type_ == ParseInfo::String) {
      parse_stack_.Pop();

      AST::StringLiteralNode* node = 
        new AST::StringLiteralNode(top_pi.data_.cstr_, top_pi.cstr_len_);
      PushNode(node, RuleName::primary);
    }
    else if (top_pi.type_ == ParseInfo::Identifier) {
      // In case type is idenfier, we decide it is a variable.
      parse_stack_.Pop();

      char* tmp = new char[top_pi.cstr_len_+1];
      memset(tmp, 0, top_pi.cstr_len_+1);
      strncpy(tmp, top_pi.data_.cstr_, top_pi.cstr_len_);
      AST::VariableNode* node = new AST::VariableNode(tmp);
      PushNode(node, RuleName::primary);
      delete[] tmp;
    }
    else if (top_pi.type_ == ParseInfo::ASTNode &&
        top_pi.rule_name_ == RuleName::expr &&
        top_pi.data_.node_->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      SetRuleNameForPI(RuleName::primary);
    }
    else {
      assert(0 && "Unidentified primary");
      return Error;
    }

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
      assert(0 && "Error on Args first expr");
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
      assert(0 && "Error on Args first expr");
      return Error;
    }

    // get ArgsNode
    ParseInfo pi_arg_node = parse_stack_.Top();
    parse_stack_.Pop();
    if (pi_arg_node.type_ != ParseInfo::ASTNode || 
      !pi_arg_node.data_.node_->IsKindOf(AST::BaseNode::ArgsNodeTy)) {
      assert(0 && "Error on Args first expr");
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
      assert(0 && "Error on term casting");
      return Error;
    }

    if (pi_expr.rule_name_ != RuleName::seq_assign_value &&
        pi_expr.rule_name_ != RuleName::seq_opassign_value &&
        pi_expr.rule_name_ != RuleName::expr10) {
      assert(0 && "Unidentified assign Expression");
      return Error;
    }

    SetRuleNameForPI(RuleName::expr);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_assign_value(void) {
    ParseInfo pi_term, pi_expr;
    AST::BaseNode* rhs = nullptr, *lhs = nullptr;
    
    // Read RHS expr
    pi_expr = parse_stack_.Top();
    if (pi_expr.type_ != ParseInfo::ASTNode ||
        pi_expr.rule_name_ != RuleName::expr){
      assert(0 && "Invalid RHS Expression");
      return Error;
    }
    parse_stack_.Pop();
    rhs = pi_expr.data_.node_;
    if (!rhs->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert(0 && "Unidentified assign Expression");
      return Error;
    }

    // Read LHS term
    pi_term = parse_stack_.Top();
    if (pi_term.type_ != ParseInfo::ASTNode ||
        pi_term.rule_name_ != RuleName::term) {
      assert(0 && "Invalid LHS Expression");
      return Error;
    }
    parse_stack_.Pop();
    lhs = pi_term.data_.node_;
    if (!rhs->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert(0 && "Unidentified assign Expression");
      return Error;
    }

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
    if (pi_expr.type_ != ParseInfo::ASTNode || 
        pi_expr.rule_name_ != RuleName::expr){
      assert(0 && "Invalid RHS Expression");
      return Error;
    }
    parse_stack_.Pop();
    rhs = pi_expr.data_.node_;
    if (!rhs->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert(0 && "Unidentified assign Expression");
      return Error;
    }

    // Read opassign
    pi_opassign = parse_stack_.Top();
    if (pi_opassign.type_ != ParseInfo::TokenType ||
        pi_opassign.rule_name_ != RuleName::opassign_op){
      assert(0 && "Unidentified OpAssign");
      return Error;
    }
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
        {
          assert(0 && "Unidentified assign sign");
          return Error;
        }
    }

    // Read LHS term
    pi_term = parse_stack_.Top();
    if (pi_term.type_ != ParseInfo::ASTNode ||
        pi_term.rule_name_ != RuleName::term) {
      assert(0 && "Unidentified opassign LHS");
      return Error;
    }
    parse_stack_.Pop();
    lhs = pi_term.data_.node_;
    if (!rhs->IsKindOf(AST::BaseNode::ExprNodeTy)) {
      assert(0 && "Unidentified opassign LHS");
      return Error;
    }

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
      assert(0 && "Invalid condition Expression");
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
        pi_else.rule_name_ != RuleName::expr10) {
      assert(0 && "Invalid Else Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read then expr
    pi_then = parse_stack_.Top();
    if (pi_then.type_ != ParseInfo::ASTNode || 
        pi_then.rule_name_ != RuleName::expr) {
      assert(0 && "Invalid Then Expression");
      return Error;
    }
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
        (pi_expr8.rule_name_ != RuleName::expr8 && 
        pi_expr8.rule_name_ != RuleName::rep_or_expr8)) {
      assert(0 && "Invalid Or Expression");
      return Error;
    }

    SetRuleNameForPI(RuleName::expr9);
    return True;
  }

  eResult SyntaxAnalyzer::Act_rep_or_expr8(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr8) {
      assert(0 && "Invalid Or RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr8 &&
         pi_lhs.rule_name_ != RuleName::rep_or_expr8)) {
      assert(0 && "Invalid Or LHS Expression");
      return Error;
    }
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
        (pi_expr7.rule_name_ != RuleName::expr7 && 
         pi_expr7.rule_name_ != RuleName::rep_and_expr7)) {
      assert(0 && "Invalid And Expression");
      return Error;
    }

    SetRuleNameForPI(RuleName::expr8);
    return True;
  }

  eResult SyntaxAnalyzer::Act_rep_and_expr7(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr7) {
      assert(0 && "Invalid And RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr7 &&
         pi_lhs.rule_name_ != RuleName::rep_and_expr7)) {
      assert(0 && "Invalid And LHS Expression");
      return Error;
    }
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
        (pi_expr7.rule_name_ != RuleName::expr6 &&
         pi_expr7.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_expr7.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_expr7.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_expr7.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_expr7.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_expr7.rule_name_ != RuleName::seq_neq_expr6 )) {
      assert(0 && "Invalid Comparison Expression");
      return Error;
    }

    SetRuleNameForPI(RuleName::expr7);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_gr_expr6(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr6) {
      assert(0 && "Invalid Greater RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_neq_expr6 )) {
      assert(0 && "Invalid Greater LHS Expression");
      return Error;
    }
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
        pi_rhs.rule_name_ != RuleName::expr6) {
      assert(0 && "Invalid Less RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_neq_expr6 )) {
      assert(0 && "Invalid Less LHS Expression");
      return Error;
    }
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
        pi_rhs.rule_name_ != RuleName::expr6) {
      assert(0 && "Invalid Greater Equal RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_neq_expr6 )) {
      assert(0 && "Invalid Greater Equal LHS Expression");
      return Error;
    }
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
        pi_rhs.rule_name_ != RuleName::expr6) {
      assert(0 && "Invalid Less Equal RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_neq_expr6 )) {
      assert(0 && "Invalid Less Equal LHS Expression");
      return Error;
    }
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
        pi_rhs.rule_name_ != RuleName::expr6) {
      assert(0 && "Invalid Equal RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_neq_expr6 )) {
      assert(0 && "Invalid Equal LHS Expression");
      return Error;
    }
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
        pi_rhs.rule_name_ != RuleName::expr6) {
      assert(0 && "Invalid Not Equal RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_gr_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_ls_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_geq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_leq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_eq_expr6 &&
         pi_lhs.rule_name_ != RuleName::seq_neq_expr6 )) {
      assert(0 && "Invalid Not Equal LHS Expression");
      return Error;
    }
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
    ParseInfo pi_expr5;

    // Check if top is right.
    pi_expr5 = parse_stack_.Top();
    if (pi_expr5.type_ != ParseInfo::ASTNode ||
        (pi_expr5.rule_name_ != RuleName::expr5 &&
         pi_expr5.rule_name_ != RuleName::rep_bitor_expr5)) {
      assert(0 && "Invalid Bit Or Expression");
      return Error;
    }

    SetRuleNameForPI(RuleName::expr6);
    return True;
  }

  eResult SyntaxAnalyzer::Act_rep_bitor_expr5(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr5) {
      assert(0 && "Invalid Bit Or LHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr5 &&
         pi_lhs.rule_name_ != RuleName::rep_bitor_expr5)) {
      assert(0 && "Invalid Bit Or RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* bit_or = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::BitOr,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(bit_or, RuleName::rep_bitor_expr5);
    return True;
  }

  eResult SyntaxAnalyzer::Expr5(void) {
    ParseInfo pi_expr4;

    // Check if top is right.
    pi_expr4 = parse_stack_.Top();
    if (pi_expr4.type_ != ParseInfo::ASTNode ||
        (pi_expr4.rule_name_ != RuleName::expr4 &&
         pi_expr4.rule_name_ != RuleName::rep_bitxor_expr4)) {
      assert(0 && "Invalid Bit Xor Expression");
      return Error;
    }

    SetRuleNameForPI(RuleName::expr5);
    return True;
  }

  eResult SyntaxAnalyzer::Act_rep_bitxor_expr4(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr4) {
      assert(0 && "Invalid Bit Xor RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr4 &&
         pi_lhs.rule_name_ != RuleName::rep_bitxor_expr4)) {
      assert(0 && "Invalid Bit Xor LHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* bit_xor = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::BitOr,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(bit_xor, RuleName::rep_bitxor_expr4);
    return True;
  }

  eResult SyntaxAnalyzer::Expr4(void) {
    ParseInfo pi_expr3;

    // Check if top is right.
    pi_expr3 = parse_stack_.Top();
    if (pi_expr3.type_ != ParseInfo::ASTNode ||
        (pi_expr3.rule_name_ != RuleName::expr3 &&
         pi_expr3.rule_name_ != RuleName::rep_bitand_expr3)) {
      assert(0 && "Invalid Bit And Expression");
      return Error;
    }

    SetRuleNameForPI(RuleName::expr4);
    return True;
  }
  
  eResult SyntaxAnalyzer::Act_rep_bitand_expr3(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr3) {
      assert(0 && "Invalid Bit And RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr3 &&
         pi_lhs.rule_name_ != RuleName::rep_bitand_expr3)) {
      assert(0 && "Invalid Bit And LHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* bit_and = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::BitAnd,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(bit_and, RuleName::rep_bitand_expr3);
    return True;
  }

  eResult SyntaxAnalyzer::Expr3(void) {
    ParseInfo pi_expr2;

    // Check if top is right.
    pi_expr2 = parse_stack_.Top();
    if (pi_expr2.type_ != ParseInfo::ASTNode ||
        (pi_expr2.rule_name_ != RuleName::expr2 &&
         pi_expr2.rule_name_ != RuleName::seq_rshft_expr2&&
         pi_expr2.rule_name_ != RuleName::seq_lshft_expr2)) {
      assert(0 && "Invalid Shift Expression");
      return Error;
    }

    SetRuleNameForPI(RuleName::expr3);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_rshft_expr2(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr2) {
      assert(0 && "Invalid Shift RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr2 &&
         pi_lhs.rule_name_ != RuleName::seq_rshft_expr2 &&
         pi_lhs.rule_name_ != RuleName::seq_lshft_expr2)) {
      assert(0 && "Invalid Shift LHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* bit_shift_right = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::BitShiftRight,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(bit_shift_right, RuleName::seq_rshft_expr2);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_lshft_expr2(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr2) {
      assert(0 && "Invalid Shift RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr2 &&
         pi_lhs.rule_name_ != RuleName::seq_rshft_expr2 &&
         pi_lhs.rule_name_ != RuleName::seq_lshft_expr2)) {
      assert(0 && "Invalid Shift LHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* bit_shift_left = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::BitShiftLeft,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(bit_shift_left, RuleName::seq_lshft_expr2);
    return True;
  }

  eResult SyntaxAnalyzer::Expr2(void) {
    ParseInfo pi_expr1;

    // Check if top is right.
    pi_expr1 = parse_stack_.Top();
    if (pi_expr1.type_ != ParseInfo::ASTNode ||
        (pi_expr1.rule_name_ != RuleName::expr1 &&
         pi_expr1.rule_name_ != RuleName::seq_sum_expr1&&
         pi_expr1.rule_name_ != RuleName::seq_sub_expr1)) {
      assert(0 && "Invalid Sum Sub Expression");
      return Error;
    }

    SetRuleNameForPI(RuleName::expr2);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_sum_expr1(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr1) {
      assert(0 && "Invalid Sum Sub RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr1 &&
         pi_lhs.rule_name_ != RuleName::seq_sum_expr1 &&
         pi_lhs.rule_name_ != RuleName::seq_sub_expr1)) {
      assert(0 && "Invalid Sum Sub LHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* bin_sum = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::BinSum,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(bin_sum, RuleName::seq_sum_expr1);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_sub_expr1(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::expr1) {
      assert(0 && "Invalid Sub RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::expr1 &&
         pi_lhs.rule_name_ != RuleName::seq_sum_expr1 &&
         pi_lhs.rule_name_ != RuleName::seq_sub_expr1)) {
      assert(0 && "Invalid Sub LHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* bin_sub = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::BinSub,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(bin_sub, RuleName::seq_sub_expr1);
    return True;
  }

  eResult SyntaxAnalyzer::Expr1(void) {
    ParseInfo pi_term;

    // Check if top is right.
    pi_term= parse_stack_.Top();
    if (pi_term.type_ != ParseInfo::ASTNode ||
        (pi_term.rule_name_ != RuleName::term &&
         pi_term.rule_name_ != RuleName::seq_mul_term &&
         pi_term.rule_name_ != RuleName::seq_div_term &&
         pi_term.rule_name_ != RuleName::seq_mod_term)) {
      assert(0 && "Invalid Mul Div Mod Expression");
      return Error;
    }

    SetRuleNameForPI(RuleName::expr1);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_mul_term(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::term) {
      assert(0 && "Invalid Mul Div Mod RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::term &&
         pi_lhs.rule_name_ != RuleName::seq_mul_term &&
         pi_lhs.rule_name_ != RuleName::seq_div_term &&
         pi_lhs.rule_name_ != RuleName::seq_mod_term)) {
      assert(0 && "Invalid Mul Div Mod LHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* bin_mul = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::BinMul,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(bin_mul, RuleName::seq_mul_term);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_div_term(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::term) {
      assert(0 && "Invalid Div RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::term &&
         pi_lhs.rule_name_ != RuleName::seq_mul_term &&
         pi_lhs.rule_name_ != RuleName::seq_div_term &&
         pi_lhs.rule_name_ != RuleName::seq_mod_term)) {
      assert(0 && "Invalid Div LHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* bin_div = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::BinDiv,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(bin_div, RuleName::seq_div_term);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_mod_term(void) {
    ParseInfo pi_lhs, pi_rhs;

    // Read right side expr
    pi_rhs = parse_stack_.Top();
    if (pi_rhs.type_ != ParseInfo::ASTNode ||
        pi_rhs.rule_name_ != RuleName::term) {
      assert(0 && "Invalid Mod RHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    // Read left side expr
    pi_lhs = parse_stack_.Top();
    if (pi_lhs.type_ != ParseInfo::ASTNode ||
        (pi_lhs.rule_name_ != RuleName::term &&
         pi_lhs.rule_name_ != RuleName::seq_mul_term &&
         pi_lhs.rule_name_ != RuleName::seq_div_term &&
         pi_lhs.rule_name_ != RuleName::seq_mod_term)) {
      assert(0 && "Invalid Mod LHS Expression");
      return Error;
    }
    parse_stack_.Pop();

    //Create node
    AST::BinaryOpNode* bin_mod = 
      new AST::BinaryOpNode((AST::ExprNode*)pi_lhs.data_.node_, 
                            AST::BinaryOpNode::BinMod,
                            (AST::ExprNode*)pi_rhs.data_.node_);

    PushNode(bin_mod, RuleName::seq_mod_term);
    return True;
  }

  eResult SyntaxAnalyzer::Stmts(void) {
    ParseInfo pi = parse_stack_.Top();
    AST::StmtNodes* sm = new AST::StmtNodes();

    while(pi.rule_name_ == RuleName::stmt) {
      if (pi.type_ != ParseInfo::ASTNode) {
        assert(0 && "Invalid AST Node in stmts");
        return Error;
      }
      parse_stack_.Pop();

      sm->PushBack((AST::StmtNode*)pi.data_.node_);
      pi = parse_stack_.Top();
    }

    sm->Reverse();
    PushStmtNodes(sm, RuleName::stmts);
    return True;
  }

  eResult SyntaxAnalyzer::Stmt(void) {
    ParseInfo pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::ASTNode || 
        (pi.rule_name_ != labeled_stmt &&
         pi.rule_name_ != expr_stmt &&
         pi.rule_name_ != block &&
         pi.rule_name_ != if_stmt &&
         pi.rule_name_ != while_stmt &&
         pi.rule_name_ != dowhile_stmt &&
         pi.rule_name_ != for_stmt &&
         pi.rule_name_ != switch_stmt &&
         pi.rule_name_ != break_stmt &&
         pi.rule_name_ != continue_stmt &&
         pi.rule_name_ != goto_stmt &&
         pi.rule_name_ != return_stmt)) {
      assert(0 && "Unidentified stmts");
      return Error;
    }

    SetRuleNameForPI(RuleName::stmt);
    return True;
  }

  eResult SyntaxAnalyzer::LabeledStmt(void) {
    ParseInfo pi_stmt, pi_label_name;

    // Read stmt
    pi_stmt = parse_stack_.Top();
    if (pi_stmt.type_ != ParseInfo::ASTNode ||
        pi_stmt.rule_name_ != RuleName::stmt) { 
      assert(0 && "Invalid label statements");
      return Error;
    }
    parse_stack_.Pop();

    // Read label name
    pi_label_name = parse_stack_.Top();
    if (pi_label_name.type_ != ParseInfo::Identifier) {
      assert(0 && "Invalid label name");
      return Error;
    }
    parse_stack_.Pop();

    // Create label stmt 
    char* lname = new char[pi_label_name.cstr_len_+1];
    memset(lname, 0, pi_label_name.cstr_len_+1);
    strncpy(lname, pi_label_name.data_.cstr_, pi_label_name.cstr_len_);
    AST::LabelNode* label = new AST::LabelNode(lname, (AST::StmtNode*)pi_stmt.data_.node_);
    delete[] lname;

    PushNode((AST::BaseNode*)label, RuleName::labeled_stmt);
    return True;
  }

  eResult SyntaxAnalyzer::ExprStmt(void) {
    ParseInfo pi;

    // Read Expr
    pi = parse_stack_.Top();
    if (pi.type_ != ParseInfo::ASTNode ||
        pi.rule_name_ != RuleName::expr) {
      assert(0 && "Invalid expr in statement");
      return Error;
    }
    parse_stack_.Pop();

    AST::ExprStmtNode* expr_stmt_node = 
      new AST::ExprStmtNode((AST::ExprNode*)pi.data_.node_);

    PushNode(expr_stmt_node, RuleName::expr_stmt);
    return True;
  }
  
  eResult SyntaxAnalyzer::IfStmt(void) {
    ParseInfo pi_cond, pi_thenbody, pi_elsebody;
    bool is_else_body = false;

    // Read else body if it is exist.
    pi_elsebody = parse_stack_.Top();
    if (pi_elsebody.type_ == ParseInfo::ASTNode &&
        pi_elsebody.rule_name_ == RuleName::opt_else_stmt) {
      // in case top is else body.
      parse_stack_.Pop();
      is_else_body = true;
    }

    // read then body.
    pi_thenbody = parse_stack_.Top();
    if (pi_elsebody.type_ != ParseInfo::ASTNode ||
        pi_elsebody.rule_name_ != RuleName::stmt) {
      assert(0 && "Invalid then body in if_stmt");
      return Error;
    }
    parse_stack_.Pop();

    // read Conditional expr
    pi_cond = parse_stack_.Top();
    if (pi_cond.type_ != ParseInfo::ASTNode || 
        pi_cond.rule_name_ != RuleName::expr)  {
      assert(0 && "Invalid conditinal in if_stmt");
      return Error;
    }
    parse_stack_.Pop();

    // Create if node
    AST::IfNode* if_node = new AST::IfNode((AST::ExprNode*)pi_cond.data_.node_, 
        (AST::StmtNode*)pi_thenbody.data_.node_, 
        (AST::StmtNode*)(is_else_body ? pi_elsebody.data_.node_ : nullptr));

    PushNode(if_node, RuleName::if_stmt);
    return True;
  }
        
  eResult SyntaxAnalyzer::Act_opt_else_stmt(void) {
    ParseInfo pi_elsebody = parse_stack_.Top();

    // Check if there is stmt.
    if (pi_elsebody.type_ != ParseInfo::ASTNode || 
        pi_elsebody.rule_name_ != RuleName::stmt)  {
      assert(0 && "Invalid else body in if_stmt");
      return Error;
    }

    SetRuleNameForPI(RuleName::opt_else_stmt);
    return True;
  }

  eResult SyntaxAnalyzer::WhileStmt(void) {
    ParseInfo pi_cond, pi_body;

    // Read while body
    pi_body = parse_stack_.Top();
    if (pi_body.type_ != ParseInfo::ASTNode ||
        pi_body.rule_name_ != RuleName::stmt)  {
      assert(0 && "Invalid while body in while_stmt");
      return Error;
    }
    parse_stack_.Pop();

    // Read conditional expr
    pi_cond = parse_stack_.Top();
    if (pi_cond.type_ != ParseInfo::ASTNode ||
        pi_cond.rule_name_ != RuleName::expr)  {
      assert(0 && "Invalid while conditinal in while_stmt");
      return Error;
    }
    parse_stack_.Pop();

    // Create while stmt 
    AST::WhileNode* while_node = new AST::WhileNode((AST::ExprNode*)pi_cond.data_.node_,
        (AST::StmtNode*)pi_body.data_.node_);
    
    PushNode(while_node, RuleName::while_stmt);
    return True;
  }

  eResult SyntaxAnalyzer::DoWhileStmt(void) {
    ParseInfo pi_cond, pi_body;

    // Read conditional expr
    pi_cond = parse_stack_.Top();
    if (pi_cond.type_ != ParseInfo::ASTNode ||
        pi_cond.rule_name_ != RuleName::expr)  {
      assert(0 && "Invalid do while conditinal in do_while_stmt");
      return Error;
    }
    parse_stack_.Pop();

    // Read while body
    pi_body = parse_stack_.Top();
    if (pi_body.type_ != ParseInfo::ASTNode ||
        pi_body.rule_name_ != RuleName::stmt) {
      assert(0 && "Invalid do while body in do_while_stmt");
      return Error;
    }
    parse_stack_.Pop();

    // Create while stmt 
    AST::DoWhileNode* dowhile_node = 
      new AST::DoWhileNode((AST::StmtNode*)pi_body.data_.node_, 
          (AST::ExprNode*)pi_cond.data_.node_);
    
    PushNode(dowhile_node, RuleName::dowhile_stmt);
    return True;
  }

  eResult SyntaxAnalyzer::ForStmt(void) {
    ParseInfo pi_init, pi_cond, pi_inc, pi_body;

    // Read while body
    pi_body = parse_stack_.Top();
    if (pi_body.type_ != ParseInfo::ASTNode ||
        pi_body.rule_name_ != RuleName::stmt)  {
      assert(0 && "Invalid for body in for_stmt");
      return Error;
    }
    parse_stack_.Pop();

    // Read increase expr
    pi_inc = parse_stack_.Top();
    if (pi_inc.type_ != ParseInfo::ASTNode ||
        pi_inc.rule_name_ != RuleName::opt_for_inc_expr)  {
      assert(0 && "Invalid for inc in for_stmt");
      return Error;
    }
    parse_stack_.Pop();

    // Read conditional expr
    pi_cond = parse_stack_.Top();
    if (pi_cond.type_ != ParseInfo::ASTNode ||
        pi_cond.rule_name_ != RuleName::opt_for_cond_expr)  {
      assert(0 && "Invalid for cond in for_stmt");
      return Error;
    }
    parse_stack_.Pop();

    // Read init expr
    pi_init= parse_stack_.Top();
    if (pi_init.type_ != ParseInfo::ASTNode ||
        pi_init.rule_name_ != RuleName::opt_for_init_expr)  {
      assert(0 && "Invalid for init in for_stmt");
      return Error;
    }
    parse_stack_.Pop();

    // Create for node
    AST::ForNode* for_stmt = new AST::ForNode(
        (AST::ExprNode*)pi_init.data_.node_,
        (AST::ExprNode*)pi_cond.data_.node_,
        (AST::ExprNode*)pi_inc.data_.node_,
        (AST::StmtNode*)pi_body.data_.node_);

    PushNode(for_stmt, RuleName::for_stmt);
    return True;
  }

  eResult SyntaxAnalyzer::Act_opt_for_init_expr(void) {
    ParseInfo pi_init= parse_stack_.Top();

    // Read expr
    if (pi_init.type_ != ParseInfo::ASTNode ||
        pi_init.rule_name_ != RuleName::expr) {
      assert(0 && "Invalid for init expr in for_stmt");
      return Error;
    }

    SetRuleNameForPI(RuleName::opt_for_init_expr);
    return True;
  }

  eResult SyntaxAnalyzer::Act_opt_for_cond_expr(void) {
    ParseInfo pi_cond = parse_stack_.Top();

    // Read expr
    if (pi_cond.type_ != ParseInfo::ASTNode ||
        pi_cond.rule_name_ != RuleName::expr) {
      assert(0 && "Invalid for cond expr in for_stmt");
      return Error;
    }

    SetRuleNameForPI(RuleName::opt_for_cond_expr);
    return True;
  }

  eResult SyntaxAnalyzer::Act_opt_for_inc_expr(void) {
    ParseInfo pi_inc = parse_stack_.Top();

    // Read expr
    if (pi_inc.type_ != ParseInfo::ASTNode ||
        pi_inc.rule_name_ != RuleName::expr) {
      assert(0 && "Invalid for inc expr in for_stmt");
      return Error;
    }

    SetRuleNameForPI(RuleName::opt_for_inc_expr);
    return True;
  }

  eResult SyntaxAnalyzer::SwitchStmt(void) {
    ParseInfo pi_cases, pi_cond;

    pi_cases = parse_stack_.Top();
    if (pi_cases.type_ != ParseInfo::CaseNodeList ||
        pi_cases.rule_name_ != RuleName::case_clauses) {
      assert(0 && "Invalid case clauses");
      return Error;
    }
    parse_stack_.Pop();

    // case cond
    pi_cond = parse_stack_.Top();
    if (pi_cond.type_ != ParseInfo::ASTNode ||
        pi_cond.rule_name_ != RuleName::expr) {
      assert(0 && "Invalid case cond");
      return Error;
    }
    parse_stack_.Pop();

    AST::SwitchNode* sn = new AST::SwitchNode((AST::ExprNode*)pi_cond.data_.node_,
        pi_cases.data_.case_nodes_);

    delete pi_cases.data_.case_nodes_;

    PushNode(sn, RuleName::switch_stmt);
    return True;
  }

  eResult SyntaxAnalyzer::CaseClauses(void) {
    ParseInfo pi_case = parse_stack_.Top();
    AST::CaseNodes* cv = new AST::CaseNodes();

    while(pi_case.rule_name_ == RuleName::case_clause || 
        pi_case.rule_name_ == RuleName::default_clause) {
      if (pi_case.type_ != ParseInfo::ASTNode) {
        assert(0 && "Invalid case clause ast node");
        return Error;
      }
      parse_stack_.Pop();

      cv->PushBack((AST::CaseNode*)pi_case.data_.node_);
      pi_case = parse_stack_.Top();
    }

    cv->Reverse();
    PushCaseNodes(cv, RuleName::case_clauses);
    return True;
  }
  
  eResult SyntaxAnalyzer::DefaultClause(void) {
    ParseInfo pi_case_body;

    // read case body
    pi_case_body = parse_stack_.Top();
    if (pi_case_body.type_ != ParseInfo::ASTNode ||
        pi_case_body.rule_name_ != RuleName::case_body)  {
      assert(0 && "Invalid case body");
      return Error;
    }
    parse_stack_.Pop();

    AST::CaseNode* case_node = new AST::CaseNode( nullptr,
        (AST::StmtNode*)pi_case_body.data_.node_);

    PushNode(case_node, RuleName::default_clause);
    return True;
  }

  eResult SyntaxAnalyzer::CaseClause(void) {
    ParseInfo pi_case_body, pi_case_list;

    // read case body
    pi_case_body = parse_stack_.Top();
    if (pi_case_body.type_ != ParseInfo::ASTNode ||
        pi_case_body.rule_name_ != RuleName::case_body)  {
      assert(0 && "Invalid case body");
      return Error;
    }
    parse_stack_.Pop();

    // read case list
    pi_case_list = parse_stack_.Top();
    if (pi_case_list.type_ != ParseInfo::ExprNodeList ||
        pi_case_list.rule_name_ != RuleName::case_list)  {
      assert(0 && "Invalid case list");
      return Error;
    }
    parse_stack_.Pop();

    AST::CaseNode* case_node = new AST::CaseNode(
        (AST::ExprNodes*)pi_case_list.data_.expr_nodes_,
        (AST::StmtNode*)pi_case_body.data_.node_);
    delete pi_case_list.data_.expr_nodes_;

    PushNode(case_node, RuleName::case_clause);
    return True;
  }

  eResult SyntaxAnalyzer::CaseList(void) {
    ParseInfo pi_value, pi_case_list;
    AST::ExprNodes* case_values = nullptr;
    
    // Read case values
    pi_value = parse_stack_.Top();
    if (pi_value.type_ != ParseInfo::ASTNode ||
        pi_value.rule_name_ != RuleName::primary)  {
      assert(0 && "Invalid case value");
      return Error;
    }
    parse_stack_.Pop();

    // Read Case list
    pi_case_list = parse_stack_.Top(); 
    if (pi_case_list.type_ != ParseInfo::ExprNodeList ||
        pi_case_list.rule_name_ != RuleName::case_list) {
      case_values = new AST::ExprNodes();
    }
    else {
      parse_stack_.Pop();

      case_values = pi_case_list.data_.expr_nodes_;
    }

    case_values->PushBack((AST::ExprNode*)pi_value.data_.node_);
    PushExprNodes(case_values, RuleName::case_list);
    return True;
  }

  eResult SyntaxAnalyzer::CaseBody(void) {
    ParseInfo pi = parse_stack_.Top();

    // Read stmt
    if (pi.type_ != ParseInfo::ASTNode ||
        pi.rule_name_ != RuleName::stmt)  {
      assert(0 && "Invalid case body stmt");
      return Error;
    }

    SetRuleNameForPI(RuleName::case_body);
    return True;
  }

  eResult SyntaxAnalyzer::BreakStmt(void) {
    AST::BreakNode* bk = new AST::BreakNode();
    PushNode(bk, RuleName::break_stmt);
    return True;
  }

  eResult SyntaxAnalyzer::ContinueStmt(void) {
    AST::ContinueNode* cn = new AST::ContinueNode();
    PushNode(cn, RuleName::continue_stmt);
    return True;
  }

  eResult SyntaxAnalyzer::GotoStmt(void) {
    ParseInfo pi_target = parse_stack_.Top();

    if(pi_target.type_ != ParseInfo::Identifier) {
      assert(0 && "Invalid goto target");
      return Error;
    }
    parse_stack_.Pop();

    char* target = new char[pi_target.cstr_len_ + 1];
    memset(target, 0, pi_target.cstr_len_ + 1);
    strncpy(target, pi_target.data_.cstr_, pi_target.cstr_len_);
    AST::GotoNode* gn = new AST::GotoNode(target);
    delete target;

    PushNode(gn, RuleName::goto_stmt);
    return True;
  }

  eResult SyntaxAnalyzer::ReturnStmt(void) {
    ParseInfo pi = parse_stack_.Top();
    if(pi.type_ != ParseInfo::ASTNode ||
        (pi.rule_name_ != seq_return && 
         pi.rule_name_ != seq_return_expr)) {
      assert(0 && "Invalid return expr");
      return Error;
    }

    SetRuleNameForPI(RuleName::return_stmt);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_return(void) {
    AST::ReturnNode* rn = new AST::ReturnNode();
    PushNode(rn, RuleName::seq_return);
    return True;
  }

  eResult SyntaxAnalyzer::Act_seq_return_expr(void) {
    ParseInfo pi = parse_stack_.Top();
    if(pi.type_ != ParseInfo::ASTNode || 
        pi.rule_name_ != RuleName::expr) {
      assert(0 && "Invalid return expr");
      return Error;
    }
    parse_stack_.Pop();

    AST::ReturnNode* rn = new AST::ReturnNode((AST::ExprNode*)pi.data_.node_);

    PushNode(rn, RuleName::seq_return_expr);
    return True;
  }

  eResult SyntaxAnalyzer::ActTokIntegerLiteral(void) {
    ParseInfo pi;
    Lexer::Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::Integer;
    pi.data_.integer_ = atol(tok.c);
    pi.token_idx_ = tokenizer_->GetTokPos();

    parse_stack_.Push(pi);
    return True;
  }

  eResult SyntaxAnalyzer::ActTokCharacterLiteral(void) {
    ParseInfo pi;
    Lexer::Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::Character;
    pi.data_.character_ = *tok.c;
    pi.token_idx_ = tokenizer_->GetTokPos();

    parse_stack_.Push(pi);
    return True;
  }

  eResult SyntaxAnalyzer::ActTokStringLiteral(void) {
    ParseInfo pi;
    Lexer::Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::String;
    pi.data_.cstr_ = tok.c;
    pi.cstr_len_ = tok.len; // string length
    pi.token_idx_ = tokenizer_->GetTokPos();

    parse_stack_.Push(pi);
    return True;
  }

  eResult SyntaxAnalyzer::ActTokIdentifier(void) {
    ParseInfo pi;
    Lexer::Token tok = tokenizer_->GetCurToken(0);

    pi.type_ = ParseInfo::Identifier;
    pi.data_.cstr_ = tok.c;
    pi.cstr_len_ = tok.len; // string length
    pi.token_idx_ = tokenizer_->GetTokPos();

    parse_stack_.Push(pi);
    return True;
  }

  eResult SyntaxAnalyzer::ActTokBraceOpen(void) {
    PushToken(0, RuleName::TokBraceOpen);
    return True;
  }

  eResult SyntaxAnalyzer::FailTopDefs(void) {
    // In case top_defs meet eof, it is okay.
    // But top_defs encounter unrecognized grammer, it is error.
    Lexer::Token tok = tokenizer_->GetCurToken();
    if (tok.type == Lexer::TokEof)
      return False;

    // TODO : Unrecognizable error here
    return Error;
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
    Lexer::Token tok = tokenizer_->GetCurToken(pos_offset);

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

  void SyntaxAnalyzer::PushTypeNodes(AST::TypeNodes* type_nodes, RuleName rname) {
    ParseInfo pi;
    pi.type_ = ParseInfo::TypeNodeList;
    pi.data_.type_nodes_ = type_nodes;
    pi.rule_name_ = rname;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::PushVarDecls(AST::VariableDecls* var_list, RuleName rname) {
    ParseInfo pi;
    pi.type_ = ParseInfo::VarDeclList;
    pi.data_.vardecls_ = var_list;
    pi.rule_name_ = rname;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::PushParamNodes(AST::ParamNodes* param_list, RuleName rname) {
    ParseInfo pi;
    pi.type_ = ParseInfo::ParamNodeList;
    pi.data_.param_nodes_ = param_list;
    pi.rule_name_ = rname;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::PushExprNodes(AST::ExprNodes* expr_list, RuleName rname) {
    ParseInfo pi;
    pi.type_ = ParseInfo::ExprNodeList;
    pi.data_.expr_nodes_ = expr_list;
    pi.rule_name_ = rname;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::PushCaseNodes(AST::CaseNodes* case_values, RuleName rname) {
    ParseInfo pi;
    pi.type_ = ParseInfo::CaseNodeList;
    pi.data_.case_nodes_ = case_values;
    pi.rule_name_ = rname;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::PushStmtNodes(AST::StmtNodes* stmt_list, RuleName rname) {
    ParseInfo pi;
    pi.type_ = ParseInfo::StmtNodeList;
    pi.data_.stmt_nodes_ = stmt_list;
    pi.rule_name_ = rname;
    parse_stack_.Push(pi);
  }

  void SyntaxAnalyzer::PushTypes(AST::Types* types, RuleName rname) {
    ParseInfo pi;
    pi.type_ = ParseInfo::Types;
    pi.data_.types_ = types;
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
