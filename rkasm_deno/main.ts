import { Command } from "https://deno.land/x/cliffy@v0.25.7/command/mod.ts";
import { fill_pc, slice, tokenize, trim } from "./path.ts";
import { print } from "./print.ts";
import {} from "./error.ts";

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
    const rkasm_lines = rkasm_text.split("\n").map(trim).map(slice);
    const tokenized = rkasm_lines.flatMap(tokenize);
    const token_all = fill_pc(tokenized);
    print(token_all);
  })

  .parse(Deno.args);
