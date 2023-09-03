import { Command } from "https://deno.land/x/cliffy@v0.25.7/command/mod.ts";
import { interactive } from "./main.ts";

await new Command()
  .name("rkemu")
  .version("v0.0.0")
  .description("Emulator for RK16 \n https://github.com/kanade-k-1228/RISC-K")
  .arguments("<bin_file>")
  .option("-b, --break", "Break point : 停止するPCを指定")
  .option("-c, --consts", "print Const list : 定数リストを表示")
  .option("-v, --vars", "print Var list : 変数リストを表示")
  .action(() => interactive())
  .parse(Deno.args);
