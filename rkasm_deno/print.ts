import { Arg, Statement, ConstLabel, VarLabel, Operation } from "./type.ts";
import {
  blue,
  cyan,
  green,
  red,
  yellow,
} from "https://deno.land/std@0.170.0/fmt/colors.ts";

export const print = (toks: Statement[]) => {
  console.log("--------------------------------------------------");
  print_lab_vars(toks);
  console.log("--------------------------------------------------");
  print_lab_const(toks);
  console.log("--------------------------------------------------");
  print_bin(toks);
};

const print_bin = (toks: Statement[]) => {
  for (const t of toks) {
    if (t.kind === "opr")
      console.log(
        `${hex(4, t.pc ?? 0)} | ${hex(8, t.bin ?? 0)} | ${red(
          left(5, t.op)
        )} ${print_arg(t.args)}`
      );
    if (t.kind === "lab_pc")
      console.log(green(`${hex(4, t.value ?? 0)}: ${t.label}`));
  }
};

const print_arg = (args: Arg[]) => {
  const arg_imm = args.find(({ kind }) => kind !== "reg");
  const imm = arg_imm ? print_imm(arg_imm) : undefined;

  const reg0 = args.at(0)?.kind === "reg" ? args.at(0)?.str : undefined;
  const reg1 = args.at(1)?.kind === "reg" ? args.at(1)?.str : undefined;
  const reg2 = args.at(2)?.kind === "reg" ? args.at(2)?.str : undefined;

  const str0 = reg0 ? blue(left(4, reg0)) : " ".repeat(4);
  const str1 = reg1 ? blue(left(4, reg1)) : " ".repeat(4);
  const str2 = reg2 ? blue(left(6, reg2)) : imm ? imm : "";

  return `${str0} ${str1} ${str2}`;
};

const print_imm = (imm: Arg) => {
  if (imm.kind === "lab_pc") {
    return green(`${hex(4, imm.val ?? 0)} : ${imm.str}`);
  }
  if (imm.kind === "lab_var") {
    return cyan(`${hex(4, imm.val ?? 0)} <- ${imm.str}`);
  }
  if (imm.kind === "lab_const") {
    return yellow(`${hex(4, imm.val ?? 0)} == ${imm.str}`);
  }
  if (imm.kind === "imm") {
    return yellow(hex(4, imm.val ?? 0));
  }
};

const print_lab_vars = (toks: Statement[]) => {
  for (const l of toks.filter((t) => t.kind === "lab_var") as VarLabel[]) {
    console.log(`${hex(4, l.value)} <- ${cyan(l.label)}`);
  }
};

const print_lab_const = (toks: Statement[]) => {
  for (const l of toks.filter((t) => t.kind === "lab_const") as ConstLabel[]) {
    console.log(`${hex(4, l.value)} =: ${yellow(l.label)}`);
  }
};

export const format = (stmt: Statement) => {
  if (stmt.kind === "opr") return `    ${left(5, stmt.op)}`;
};

const hex = (w: number, n: number) =>
  "0x" + right(w, n.toString(16).toUpperCase(), "0");

const right = (n: number, s: string, fill = " ") => {
  const offset = n - s.length;
  return offset >= 0 ? fill.repeat(n - s.length) + s : s;
};

const left = (n: number, s: string, fill = " ") => {
  const offset = n - s.length;
  return offset >= 0 ? s + fill.repeat(n - s.length) : s;
};
