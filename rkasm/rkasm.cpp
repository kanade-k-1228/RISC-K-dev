#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <regex>
#include <map>

enum TokenType{
    OP,   // 命令ニーモニック
    REG,  // レジスタ
    IMM,  // 即値
    LABP, // ラベルの指す場所
    LABR, // ラベルの参照
};

struct Token{
    TokenType token_type;
    std::string str; // 文字列
    uint16_t value; // 即値の場合は値
};

struct MCode{
    uint32_t bin;
    uint8_t opc;
    uint8_t rs1;
    uint8_t rs2;
    uint8_t rd;
    uint16_t imm;
    bool imm_is_label;
    bool labelled;
};

std::string pretty_hex(uint16_t);
std::vector<std::string> split(const std::string &, char);
std::vector<Token> tokenize(std::vector<std::string>);
// MCode asm_to_mcode(const std::vector<Token>);

int main(){
    std::map<std::string, uint16_t> labels;
    std::vector<MCode> mcodes;
    std::string line;
    uint16_t pc = 0;
    while(std::getline(std::cin,line)){

        // コメント削除
        auto pos = line.find(";"); // 最初にヒットした ; の位置
        if(pos != std::string::npos) line = line.erase(pos);

        // 空行削除
        std::regex non_empty(R"(\S)"); // 空でない文字にヒット
        if(!std::regex_search(line,non_empty)) continue; // 空文字しかない場合、この行は飛ばす

        // トークン化
        std::vector<std::string> splited_code = split(line,' ');
        std::vector<Token> tokenized_code = tokenize(splited_code);

        // 機械語化
        // MCode mcode = asm_to_mcode(tokenized_code);
        // mcodes.push_back(mcode);

        // ラベル行の場合、ラベルの指す位置をリストに記録
        // 命令行の場合、PCをカウントアップする。
        bool is_label = tokenized_code.size() != 0 && tokenized_code.at(0).token_type == LABP;
        if(is_label){
            if(labels.contains(tokenized_code.at(0).str)){
                std::cout << "\033[31m" << "----------------------------------------" << std::endl;
                std::cout << " ERROR! Find Same Label: " << "\033[m" << tokenized_code.at(0).str << std::endl << std::endl;
                exit(EXIT_FAILURE);
            }
            labels.insert(std::map<std::string,uint16_t>::value_type(tokenized_code.at(0).str,pc));
        }
        else pc++;
        
        // ----------------------------------------------------------------------------------------------------
        // きれいに表示
        if(!is_label) std::cout << pretty_hex(pc-1) << " | ";
        if(!is_label) std::cout << pretty_hex(0) << " |"; //mcode.bin
        for(int j=0; j < tokenized_code.size(); j++){
            Token token = tokenized_code.at(j);
            TokenType t = token.token_type;
            if(t==OP)  std::cout << "\033[31m" << std::setw(7) << token.str << "\033[m"; // red
            if(t==REG) std::cout << "\033[34m" << std::setw(8) << token.str << "\033[m"; // blue
            if(t==IMM) std::cout << "\033[33m" << std::setw(8) << pretty_hex(token.value) << "\033[m"; // yellow
            if(t==LABP || t==LABR) std::cout << "\033[32m" << token.str << "\033[m"; // green
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "Label List" << std::endl;
    for(const auto &label : labels){
        std::cout << std::setw(8) << label.first << std::setw(8) << pretty_hex(label.second) << std::endl;
    }

    return EXIT_SUCCESS;
}

std::string pretty_hex(uint16_t n){
    std::ostringstream ss;
    ss.setf(std::ios::hex,std::ios::basefield);
    char fill_save = ss.fill('0');
    ss << "0x" << std::setw(4) << n; // yellow
    ss.fill(fill_save);
    return ss.str();
}


// 区切り文字 sep で分割
std::vector<std::string> split(const std::string &str, char sep){
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buffer;
    while(std::getline(ss,buffer,sep)){
        if(buffer=="") continue; // 区切り文字が連続した場合、削除
        v.push_back(buffer);
    }
    return v;
}

std::vector<Token> tokenize(std::vector<std::string> asm_code){
    std::vector<Token> ret;
    std::regex op(R"(^((add)|(addi)|(sub)|(subi)|(store)|(load)|(loadi)|(be)|(bl)|(jump)|(jumpr))$)");
    std::regex reg(R"(^((x\d)|(a\d)|(s\d)|(zero)|(ra)|(sp))$)");
    std::regex imm(R"(^((0x)|(0b)|\d|(-\d)))");
    std::regex labp(R"(:$)");
    for(int i=0; i<asm_code.size(); i++){
        std::string word = asm_code.at(i);
        Token token;
        token.str=word;
        if(std::regex_search(word,op)){
            token.token_type=OP;
        }else if(std::regex_search(word,reg)){
            token.token_type=REG;
        }else if(std::regex_search(word,labp)){
            token.token_type=LABP;
        }else if(std::regex_search(word,imm)){
            token.token_type=IMM;
            token.value=std::stoi(word);
        }else{
            token.token_type=LABR;
        }
        ret.push_back(token);
    }
    return ret;
}

// MCode asm_to_mcode(const std::vector<Token> code){
//     MCode ret;
//     if(code.size()==0) return 0;
//     if(code.at(0).token_type == OP){
//         std::string op = code.at(0).str;
//         if(op=="add"){
//             return 0x0000;
//         }
//         if(op=="sub"){
//             return 0x0000;
//         }
//         if(op=="addi"){
//             return 0x0000;
//         }
//         if(op=="store"||op=="s"){
//             return 0x0000;
//         }
//         if(op=="load"||op=="l"){
//             return 0x0000;
//         }
//         if(op=="loadi"||op=="li"){
//             return 0x0000;
//         }
//         if(op=="jump"||op=="j"){
//             return 0x0000;
//         }
//         if(op=="jumpr"||op=="jr"){
//             return 16;
//         }
//     }
//     if(code.at(0).token_type == LABP){
//         return 0;
//     }
//     std::cout << "Invalid Syntax" << std::endl;
//     exit(EXIT_FAILURE);
// }
