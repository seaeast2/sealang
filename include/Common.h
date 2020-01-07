#ifndef _common_h_
#define _common_h_

#include <vector>
#include <string>
#include "core/simple_vector.h"
#include "core/hash.h"
#include "ASTType.h"

typedef std::vector<std::string> VecStr;

template <class T1, class T2> 
struct Pair {
  T1 first_;
  T2 second_;
};

namespace AST {
  class BaseNode;
  class NamedDecl;
  class StmtNode;
  class ExprNode;
  class BlockNode;
  class LabelNode;
  class ExprStmtNode;
  class IfNode;
  class WhileNode;
  class DoWhileNode;
  class ForNode;
  class CaseNode;
  class SwitchNode;
  class BreakNode;
  class ContinueNode;
  class GotoNode;
  class ReturnNode;
  class AssignNode;
  class OpAssignNode;
  class AddressNode;
  class BinaryOpNode;
  class LogicalAndNode;
  class LogicalOrNode;
  class CastNode;
  class CondExprNode;
  class FuncCallNode;
  class ArrayRefNode;
  class DereferenceNode;
  class MemberRefNode;
  class PtrMemberRefNode;
  class VariableNode;
  class IntegerLiteralNode;
  class RealLiteralNode;
  class StringLiteralNode;
  class SizeofExprNode;
  class SizeofTypeNode;
  class UnaryOpNode;
  class PrefixOpNode;
  class SuffixOpNode;
  class FunctionDecl;
  class VariableDecl;
  class ConstantDecl;
  class TypeNode;
  class ParamNode;
  class ImportNode;
  class ArgsNode;
  //class ClassNode;
  class RecordDecl;
  class TypedefNode;
  class Declarations;

  typedef SimpleVector<TypeNode*>     TypeNodes;
  typedef SimpleVector<FunctionDecl*> FunctionDecls;
  typedef SimpleVector<ConstantDecl*> ConstantDecls;
  typedef SimpleVector<VariableDecl*> VariableDecls;
  typedef SimpleVector<ParamNode*>    ParamNodes;
  typedef SimpleVector<StmtNode*>     StmtNodes;
  typedef SimpleVector<ExprNode*>     ExprNodes;
  typedef SimpleVector<CaseNode*>     CaseNodes;
  //typedef SimpleVector<ClassNode*>    ClassNodes;
  typedef SimpleVector<RecordDecl*>    RecordDecls;
  typedef SimpleVector<TypedefNode*>  TypedefNodes;
  typedef SimpleVector<ImportNode*>   ImportNodes;

  typedef HashTable<Type*, 64> TypeTable;
}
#endif
