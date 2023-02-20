#pragma once
#include "ast.hpp"

// 深さ優先探索
class DFS {
  std::stack<Node*> s;
public:
  DFS(Node* root);
  Node* next();
  Node* begin() { return s.top(); }
  bool end() { return s.empty(); }
};

// 親ノードに戻る深さ優先探索
class SweepDFS {
  std::stack<std::pair<Node*, int>> s;
public:
  SweepDFS(Node* root);
  std::pair<Node*, int> next();
  std::pair<Node*, int> begin() { return s.top(); }
  bool end() { return s.empty(); }
};
