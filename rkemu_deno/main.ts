import { readKeypressSync } from "https://deno.land/x/keypress@0.0.11/mod.ts";
import { Model } from "./model.ts";
import { right } from "./utils.ts";

export const step = (time_limit = 0x1_0000) => {
  const model = new Model();

  console.log(["Interactive emulator", " - next (\\n)", " - interrupt (0~3)", " - exit (e,ctrl+c)", " - run to next break point", ""].join("\n"));

  let time = 0;

  console.log(model.print(time));

  for (const key of readKeypressSync()) {
    // 次の命令を実行
    if (key.key === "return") {
      model.next();
      console.log(model.print(++time));
    }

    // 割り込み発生
    if (key.key === "i") {
      console.log(`[${right(4, `${time}`)}] Interrupt`);
      model.intr();
      console.log(model.print(++time));
    }

    // エミュレータを終了
    if (key.key === "escape" || (key.key === "c" && key.ctrlKey)) {
      console.log(`[${right(4, `${time}`)}] Exit`);
      return;
    }
    if (time >= time_limit || !Number.isSafeInteger(time)) {
      console.log(`[${right(4, `${time}`)}] Exceed time limit`);
      return;
    }
  }
};
