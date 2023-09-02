import { Command } from "https://deno.land/x/cliffy@v0.25.7/command/mod.ts";
import { build_op_arg, build_op_bin, fill_pc, interpret } from "./path.ts";
import { print_bin, print_const_labels, print_var_labels } from "./print.ts";
import {} from "./error.ts";
import { ConstLabel, PCLabel, VarLabel } from "./type.ts";

await new Command()
  .name("rkasm")
  .version("v0.0.0")
  .description(
    "Asembler for RK16 ISA \n https://github.com/kanade-k-1228/RISC-K"
  )
  .arguments("<rkasm_file>")
  .option("-w, --warn", "print Warning : 警告を表示")
  .option("-c, --consts", "print Const list : 定数リストを表示")
  .option("-v, --vars", "print Var list : 変数リストを表示")
  .action(async ({ consts, vars }, rkasm_file) => {
    console.log("--------------------------------------------------");
    console.log(`Assemble: ${rkasm_file}`);
    const rkasm_text = await Deno.readTextFile(rkasm_file).catch(() => {
      console.log(`Cannot open input file: ${rkasm_file}`);
      console.log("--------------------------------------------------");
      Deno.exit(-1);
    });

    // テキストを分割
    const preproc = rkasm_text
      .split("\n") //行で分割
      .map((s) => (s.indexOf(";") === -1 ? s : s.substring(0, s.indexOf(";")))) // コメント削除
      .map((s) => s.split(/\s/).filter((s) => s !== "")); // 空白文字で分割
    // console.table(preproc);

    const comments = rkasm_text
      .split("\n")
      .flatMap((s, i) =>
        s.indexOf(";") === -1
          ? []
          : [{ line: i, comment: s.slice(s.indexOf(";")) }]
      );
    console.log(comments);

    // 各文を解釈
    const statements = preproc.flatMap(interpret);
    // console.table(statements);

    // 各文のプログラムのカウンタの値を補完
    const pc_filled = fill_pc(statements);
    // console.table(pc_filled);

    // ラベル一覧
    const labels = pc_filled.filter(
      (l) =>
        l.kind === "lab_const" || l.kind === "lab_pc" || l.kind === "lab_var"
    ) as (PCLabel | VarLabel | ConstLabel)[];

    // 命令文をビルド
    const built = pc_filled
      .map((stmt) => (stmt.kind === "opr" ? build_op_arg(stmt, labels) : stmt)) // 引数のレジスタ名とラベルの解決
      .map((stmt) => (stmt.kind === "opr" ? build_op_bin(stmt) : stmt));

    // きれいに表示
    if (vars) {
      console.log("--------------------------------------------------");
      print_var_labels(built);
    }
    if (consts) {
      console.log("--------------------------------------------------");
      print_const_labels(built);
    }
    console.log("--------------------------------------------------");
    print_bin(built);
    console.log("--------------------------------------------------");
  })

  .command("format", "Format asembly file")
  .arguments("<rkasm_file>")
  .action(async (_, rkasm_file) => {
    console.log(`Format: ${rkasm_file}`);
    const rkasm_text = await Deno.readTextFile(rkasm_file).catch(() => {
      console.log(`Cannot open input file: ${rkasm_file}`);
      Deno.exit(-1);
    });
    console.log(rkasm_text);
  })

  .parse(Deno.args);
