import { blue, green, red } from "https://deno.land/std/fmt/colors.ts";

export const print_err = (file: string, line: number, msg: string) => {
  console.log("--------------------------------------------------");
  console.log(`${red("ERROR!")} @${file}:${line}`);
  console.log(msg);
  console.log("--------------------------------------------------");
};
