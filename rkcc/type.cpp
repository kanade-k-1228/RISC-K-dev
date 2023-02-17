#include "type.hpp"
#include "../utils/utils.hpp"

int type_size(Node* node) {
  switch(node->type) {
  case Node::Type::TypePrim:  // 整数型 : 1
    return 1;
  case Node::Type::TypePointer:  // ポインタ : 1
    return 1;
  case Node::Type::TypeArray:  // 配列型 : ベース型のN倍
    return type_size(node->type_base()) * node->array_len();
  case Node::Type::TypeStruct:  // 構造体 : メンバの型の合計
    int ret;
    for(auto t : node->type_members())
      ret += type_size(t.second);
    return ret;
  case Node::Type::TypeFunc:  // 関数型 : RAM上になし
    return 0;
  default:
    error("This node is not type: ");
    return 0;
  }
}
