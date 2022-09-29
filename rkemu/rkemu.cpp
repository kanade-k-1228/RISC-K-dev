#include<iostream>
#include<array>
#include<unistd.h>

std::array<uint16_t, 0xffff> mem;
uint16_t pc = 0;
std::array<uint16_t, 0xffff> rom;

void next(uint16_t code){
    uint16_t opc = code & 0x000f;
    uint16_t rs1 = code > 4 & 0x000f;
    uint16_t rs2 = code > 8 & 0x000f;
    uint16_t rd  = code > 12 & 0x000f;
    uint16_t imm = get_imm(code,opc);
    switch (opc){
    case 0:
        break;
    case 1:
        break;

    default:
        break;
    }
}

// 即値デコーダ
uint16_t get_imm(uint16_t code, uint16_t opc){

}

int main(){
    // バイナリ読み込み

    // 実行
    while(true){
        next(rom[pc]);
        sleep(1);
    }
}
