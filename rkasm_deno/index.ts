import { Command } from "https://deno.land/x/cliffy@v0.25.7/command/mod.ts";
import { main } from "./main.ts";

await new Command()
  .name("rkasm")
  .version("v0.0.0")
  .description("Asembler for RK16 ISA \n https://github.com/kanade-k-1228/RISC-K")
  .arguments("<rkasm_file>")
  .option("-w, --warn", "print Warning : 警告を表示")
  .option("-c, --consts", "print Const list : 定数リストを表示")
  .option("-v, --vars", "print Var list : 変数リストを表示")
  .action(({ consts, vars }, rkasm_file) =>
    main({ consts, vars }, rkasm_file).catch((e) => {
      if (e.name === "AsemblerError") {
        console.log("--------------------------------------------------");
        console.log(e.print());
        console.log("--------------------------------------------------");
        Deno.exit(-1);
      }
    })
  )
  .parse(Deno.args);
