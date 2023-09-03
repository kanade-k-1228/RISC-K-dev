import { readKeypressSync } from "https://deno.land/x/keypress@0.0.11/mod.ts";
import { Model } from "./model.ts";
import { right } from "./utils.ts";
import { red } from "https://deno.land/std@0.170.0/fmt/colors.ts";

export const step = async (hex_file: string, time_limit = 0x1_0000) => {
  console.log(`Emulate: ${hex_file}`);
  const hex_text = await Deno.readTextFile(hex_file).catch(() => {
    console.log(`Cannot open input file: ${hex_file}`);
    Deno.exit(-1);
  });
  console.log(hex_text.split("\n"));

  const model = new Model(hex_text.split("\n"));

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
      console.log(red(`[${right(4, `${time}`, "0")}] Interrupt ==============================`));
      model.intr();
      console.log(model.print(++time));
    }

    // エミュレータを終了
    if (key.key === "escape" || (key.key === "c" && key.ctrlKey)) {
      console.log(`[${right(4, `${time}`, "0")}] Exit`);
      return;
    }
    if (time >= time_limit || !Number.isSafeInteger(time)) {
      console.log(`[${right(4, `${time}`, "0")}] Exceed time limit`);
      return;
    }
  }
};
