import { readKeypressSync } from "https://deno.land/x/keypress@0.0.11/mod.ts";
import { Model } from "./model.ts";

export const interactive = () => {
  const cpu = new Model();

  console.log(["Interactive emulator", " - next (\\n)", " - interrupt (0~3)", " - exit (e,ctrl+c)", ""].join("\n"));

  console.log(cpu.print());

  let timer = 0;

  for (const key of readKeypressSync()) {
    // 次の命令を実行
    if (key.key === "return") {
      console.log(`[${timer++}]`);
      cpu.next();
      console.log(cpu.print());
    }

    // 割り込み発生
    if (key.key === "i") {
      console.log(`[${timer++}] Interrupt`);
      cpu.intr();
      console.log(cpu.print());
    }

    // エミュレータを終了
    if (key.key === "escape" || (key.key === "c" && key.ctrlKey)) {
      console.log(`[${timer}] Exit Emulator`);
      return;
    }
  }
};
