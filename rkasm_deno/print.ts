import { blue, green, red, yellow } from "https://deno.land/std/fmt/colors.ts";
import { Token, TokenLabConst, TokenLabVar } from "./type.ts";

export const print = (toks: Token[]) => {
  console.log("--------------------------------------------------");
  print_lab_vars(toks);
  console.log("--------------------------------------------------");
  print_lab_const(toks);
  console.log("--------------------------------------------------");
  print_program(toks);
};

const print_program = (toks: Token[]) => {
  for (const t of toks) {
    if (t.kind === "opr")
      console.log(`${hex(t.pc ?? 0)} | ${red(right(t.op, 5))} `);
    if (t.kind === "lab_pc")
      console.log(green(`${hex(t.value ?? 0)}: ${t.name}`));
  }
};

const print_lab_vars = (toks: Token[]) => {
  for (const l of toks.filter((t) => t.kind === "lab_var") as TokenLabVar[]) {
    console.log(`${blue(hex(l.value))} <- ${l.name}`);
  }
};

const print_lab_const = (toks: Token[]) => {
  for (const l of toks.filter(
    (t) => t.kind === "lab_const"
  ) as TokenLabConst[]) {
    console.log(`${yellow(hex(l.value))} == ${l.name}`);
  }
};

const hex = (n: number) =>
  "0x" + ("0000" + n.toString(16).toUpperCase()).substr(-4);

const right = (s: string, n: number) => (" ".repeat(n) + s).substr(-n);
