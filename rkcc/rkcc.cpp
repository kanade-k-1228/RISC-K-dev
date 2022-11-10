#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

class Token {
public:
  enum class Type {
    Reserved,
    OperatorAdd,   // +
    OperatorSub,   // -
    ParOpen,       // (
    ParClose,      // )
    BracketOpen,   //
    BracketClose,  //
    CurlyOpen,     // {
    CurlyClose,    // }
    AngleOpen,     // <
    AngleClose,    //
    Number,
    Identifier,
  };

  Type type;
  std::string str;
  int val;
  Token(Type type, std::string str, int val) : type(type), str(str), val(val) {}
  std::string print() {
    if(type == Type::Reserved)
      return "\033[31m" + str + "\033[m";
    else
      return str;
  }
};

class Node {
public:
  enum class Type {
    Add,
    Sub,
    Mul,
    Div,
    Num
  };
  // 二項演算子のコンストラクタ
  Node(Type type, Node* lhs, Node* rhs)
      : type(type), lhs(lhs), rhs(rhs) {}
  // 数値のコンストラクタ
  Node(int val)
      : type(Type::Num), val(val) {}
  Type type;
  Node *lhs, *rhs;
  int val;
  std::string print() {
    if(lhs && rhs) {
      std::string op;
      if(type == Type::Add) op = " + ";
      if(type == Type::Sub) op = " - ";
      if(type == Type::Mul) op = " * ";
      if(type == Type::Div) op = " / ";
      return "( " + lhs->print() + op + rhs->print() + " )";
    } else {
      return std::to_string(val);
    }
  }
  int reduct() {
    if(type == Type::Add) return lhs->reduct() + rhs->reduct();
    if(type == Type::Sub) return lhs->reduct() - rhs->reduct();
    if(type == Type::Mul) return lhs->reduct() * rhs->reduct();
    if(type == Type::Div) return lhs->reduct() / rhs->reduct();
    if(type == Type::Num) return val;
  }
};

std::vector<Token> tokens;

bool consume_if_token_is(std::string str) {
  if(tokens.size() > 0 && tokens.at(0).str == str) {
    tokens.erase(tokens.begin());
    return true;
  } else
    return false;
}

void expect_token_is() {
  std::cout << "Parse Error" << std::endl;
  exit(EXIT_FAILURE);
}

std::string token_head() {
  if(tokens.size() == 0) return "";
  return tokens.at(0).str;
}

Node* expr();
Node* mul();
Node* primary();

Node* expr() {
  //   std::cout << "Expr    " << token_head() << std::endl;
  Node* node = mul();
  for(;;) {
    if(consume_if_token_is("+")) {
      node = new Node(Node::Type::Add, node, mul());
    } else if(consume_if_token_is("-")) {
      node = new Node(Node::Type::Sub, node, mul());
    } else
      return node;
  }
}

Node* mul() {
  //   std::cout << "Mul     " << token_head() << std::endl;
  Node* node = primary();
  for(;;) {
    if(consume_if_token_is("*")) {
      node = new Node(Node::Type::Mul, node, primary());
    } else if(consume_if_token_is("/")) {
      node = new Node(Node::Type::Div, node, primary());
    } else
      return node;
  }
}

Node* primary() {
  //   std::cout << "Primary " << token_head() << std::endl;
  if(consume_if_token_is("(")) {
    Node* node = expr();
    tokens.erase(tokens.begin());
    return node;
  } else {
    Node* node = new Node(tokens.at(0).val);
    tokens.erase(tokens.begin());
    return node;
  }
}

int main(int argc, char* argv[]) {

  // ファイルの読み取り
  std::string fname = argv[1];
  std::ifstream fin(fname, std::fstream::in);
  if(!fin) exit(EXIT_FAILURE);

  // トークン化
  // 先頭から読み取っていく
  const std::regex reserved("^(\\+|-|\\*|/|\\(|\\))");
  const std::regex num("^([0-9]+)");
  const std::regex blank("^(\\s)");
  std::string line;
  std::smatch match;
  while(std::getline(fin, line)) {
    for(; line.length() > 0;) {
      if(std::regex_search(line, match, reserved)) {
        std::string match_str = match.str();
        int match_length = match.length();
        // 読み取った分だけ読み込みを進める
        line.erase(0, match_length);
        // トークンに追加
        Token new_token(Token::Type::Reserved, match_str, 0);
        tokens.push_back(new_token);
      } else if(std::regex_search(line, match, num)) {
        std::string match_str = match.str();
        int match_length = match.length();
        // 読み取った分だけ読み込みを進める
        line.erase(0, match_length);
        // トークンに追加
        Token new_token(Token::Type::Number, match_str, std::stoi(match_str));
        tokens.push_back(new_token);
      } else if(std::regex_search(line, blank)) {
        // 空白文字を消去
        line.erase(0, 1);
      }
    }

    for(auto token : tokens) std::cout << token.print() << " ";
    std::cout << " --> ";

    // 構文木
    Node* root
        = expr();
    std::cout << root->print();
    // 計算結果
    std::cout << " = " << root->reduct() << std::endl;
  }
}
