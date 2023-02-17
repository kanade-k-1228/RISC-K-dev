#include "ast.hpp"
#include "code.hpp"

extern Code code;
void gen(Node* node);

void gen_gvar(Node* node);
void gen_func(Node* node);
