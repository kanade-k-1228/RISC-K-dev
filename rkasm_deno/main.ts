import { Command } from "https://deno.land/x/cliffy@v0.25.7/command/mod.ts";
import { build_op_arg, build_op_bin, fill_pc, interpret } from "./path.ts";
import { print } from "./print.ts";
import {} from "./error.ts";
import { ConstLabel, Operation, PCLabel, VarLabel } from "./type.ts";

await new Command()
  .name("rkasm")
  .version("v0.0.0")
  .description(
    "Asembler for RK16 ISA \n https://github.com/kanade-k-1228/RISC-K"
  )
  .arguments("<rkasm_file>")
  .action(async (_, rkasm_file) => {
    console.log("--------------------------------------------------");
    console.log(`Assemble: ${rkasm_file}`);
    const rkasm_text = await Deno.readTextFile(rkasm_file).catch(() => {
      console.log(`Cannot open input file: ${rkasm_file}`);
      console.log("--------------------------------------------------");
      Deno.exit(-1);
    });

    // テキストを行と空白で分割
    const preproc = rkasm_text
      .split("\n") //行で分割
      .map((s) => (s.indexOf(";") === -1 ? s : s.substring(0, s.indexOf(";")))) // コメント削除
      .map((s) => s.split(/\s/).filter((s) => s !== "")); // 空白文字で分割
    console.table(preproc);

    // 各文を解釈
    const statements = preproc.flatMap(interpret);
    // console.table(statements);

    // 各文のプログラムのカウンタの値を補完
    const pc_filled = fill_pc(statements);
    // console.table(pc_filled);

    const labels = pc_filled.filter(
      (l) =>
        l.kind === "lab_const" || l.kind === "lab_pc" || l.kind === "lab_var"
    ) as (PCLabel | VarLabel | ConstLabel)[];

    // 命令文をビルド
    const built = pc_filled
      .map((stmt) => (stmt.kind === "opr" ? build_op_arg(stmt, labels) : stmt)) // 引数のレジスタ名とラベルの解決
      .map((stmt) => (stmt.kind === "opr" ? build_op_bin(stmt) : stmt));

    print(built);
  })

  .parse(Deno.args);
