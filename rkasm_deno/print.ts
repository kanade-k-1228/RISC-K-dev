import { Arg, Statement } from "./type.ts";
import { blue, cyan, green, red, yellow } from "https://deno.land/std@0.170.0/fmt/colors.ts";

export const print_hex = (stmts: Statement[]) =>
  stmts.flatMap((stmt) => (stmt.kind === "opr" ? [hex(8, stmt.bin ?? 0)] : [])).join("\n");

export const print_asm = (stmts: Statement[]) =>
  stmts
    .flatMap((stmt) => {
      if (stmt.kind === "opr") {
        const pc = hex(4, stmt.pc ?? 0);
        const bin = str_ins(hex(8, stmt.bin ?? 0), 4, "_");
        return [`${pc} | ${bin} | ${red(right(5, stmt.op))}  ${print_arg(stmt.args)}`];
      }
      if (stmt.kind === "lab_pc") {
        return [green(`${hex(4, stmt.value ?? 0)}: ${stmt.label}`)];
      } else return [];
    })
    .join("\n");

const print_arg = (args: Arg[]) => {
  const arg_imm = args.find(({ kind }) => kind !== "reg");
  const imm = arg_imm ? print_imm(arg_imm) : undefined;

  const reg0 = args.at(0)?.kind === "reg" ? args.at(0)?.str : undefined;
  const reg1 = args.at(1)?.kind === "reg" ? args.at(1)?.str : undefined;
  const reg2 = args.at(2)?.kind === "reg" ? args.at(2)?.str : undefined;

  const str0 = reg0 ? blue(right(4, reg0)) : " ".repeat(4);
  const str1 = reg1 ? blue(right(4, reg1)) : " ".repeat(4);
  const str2 = reg2 ? blue(right(4, reg2)) : imm ? imm : "";

  return `${str0}  ${str1}  ${str2}`;
};

const print_imm = (imm: Arg) => {
  if (imm.kind === "lab_pc") return green(`${hex(4, imm.val ?? 0)} : ${imm.str}`);
  if (imm.kind === "lab_var") return cyan(`${hex(4, imm.val ?? 0)} < ${imm.str}`);
  if (imm.kind === "lab_const") return yellow(`${hex(4, imm.val ?? 0)} = ${imm.str}`);
  if (imm.kind === "imm") return yellow(hex(4, imm.val ?? 0));
};

export const print_var_labels = (stmts: Statement[]) =>
  stmts
    .flatMap((stmt) =>
      stmt.kind === "lab_var" ? [`${hex(4, stmt.value)} <- ${cyan(stmt.label)}`] : []
    )
    .join("\n");

export const print_const_labels = (stmts: Statement[]) =>
  stmts
    .flatMap((stmt) =>
      stmt.kind === "lab_const" ? [`${hex(4, stmt.value)} =: ${yellow(stmt.label)}`] : []
    )
    .join("\n");

const hex = (w: number, n: number) => right(w, n.toString(16).toUpperCase(), "0");

const right = (n: number, s: string, fill = " ") => {
  const offset = n - s.length;
  return offset >= 0 ? fill.repeat(n - s.length) + s : s;
};

const left = (n: number, s: string, fill = " ") => {
  const offset = n - s.length;
  return offset >= 0 ? s + fill.repeat(n - s.length) : s;
};

const str_ins = (str: string, idx: number, ins: string) => str.slice(0, idx) + ins + str.slice(idx);
