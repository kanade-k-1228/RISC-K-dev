#include "dfs.hpp"

DFS::DFS(Node* root) { s.push(root); }

Node* DFS::next() {
  Node* ret = s.top();
  s.pop();
  for(auto itr = ret->childs.rbegin(); itr != ret->childs.rend(); ++itr) {
    s.push(*itr);
  }
  return ret;
}


SweepDFS::SweepDFS(Node* root) { s.emplace(root, 0); }

std::pair<Node*, int> SweepDFS::next() {
  std::pair<Node*, int> ret = s.top();
  s.pop();
  for(int i = ret.first->childs.size() - 1; i >= 0; --i) {
    s.emplace(ret.first->childs.at(i), i);
  }
  return ret;
}
