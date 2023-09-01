import { Command } from "https://deno.land/x/cliffy@v0.25.7/command/mod.ts";
import { fill_label, slice, tokenize, trim } from "./path.ts";

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
  .action(async ({ warn, consts, vars }, rkasm_file) => {
    console.log("--------------------------------------------------");
    console.log(`Assemble: ${rkasm_file}`);
    const rkasm_text = await Deno.readTextFile(rkasm_file).catch(() => {
      console.log(`Cannot open input file: ${rkasm_file}`);
      console.log("--------------------------------------------------");
      Deno.exit(-1);
    });
    const rkasm_lines = rkasm_text.split("\n").map(trim).map(slice);
    console.log(rkasm_lines);
    const tokenized = rkasm_lines.flatMap(tokenize);
    console.log(tokenized);
    console.table(tokenized);
    const token_list = fill_label(tokenized);
    console.table(token_list);

    const oprs = token_list.filter((t) => t.kind === "opr");
    console.table(oprs);

    const lab_pc = token_list.filter((t) => t.kind === "lab_pc");
    console.table(lab_pc);

    const lab_var = token_list.filter((t) => t.kind === "lab_var");
    console.table(lab_var);

    const lab_const = token_list.filter((t) => t.kind === "lab_const");
    console.table(lab_const);
  })

  .parse(Deno.args);
