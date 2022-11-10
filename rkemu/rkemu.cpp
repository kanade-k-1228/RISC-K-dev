#include "../rkisa/rkisa.hpp"
#include "cpu.hpp"
#include <unistd.h>

// int main(int argc, char* argv[]) {
//   std::string fname = argv[1];  // 入力ファイル名
//   // バイナリの読み込み
//   CPU cpu;
//   cpu.load_rom(fname);
//   // 実行
//   while(true) {
//     cpu.dump();
//     cpu.step();
//     sleep(1);
//   }
//   return 0;
// }

// こういうふうに、命令を手書きしてもシュミレーションできる

int main() {
  CPU cpu;
  cpu.loadi(T0, 0x1111);
  cpu.loadi(T1, 0x2222);
  cpu.add(T0, T0, T1);
  cpu.dump();

  return 0;
}
