import { mnemonics, regs } from "./mnemonics.ts";
import { Arg, Statement, ConstLabel, PCLabel, VarLabel, Operation } from "./type.ts";

export const interpret = (str: string[], idx: number): Statement[] => {
  const str0 = str.at(0);
  if (!str0) return [];
  if (str0.endsWith(":")) {
    const label = str0.slice(0, -1);
    return [{ line: idx, kind: "lab_pc", label }];
  } else if (str0.startsWith("#")) {
    const label = str[1];
    const value = parseInt(str[0].slice(1));
    return [{ line: idx, kind: "lab_const", label, value }];
  } else if (str0.startsWith("@")) {
    const label = str[1];
    const value = parseInt(str[0].slice(1));
    return [{ line: idx, kind: "lab_var", label, value }];
  } else {
    const op = str0;
    const args = str.slice(1).map((str) => ({ str }));
    return [{ line: idx, kind: "opr", op, args }];
  }
};

export const fill_pc = (toks: Statement[]) => {
  let pc = 0;
  const ret: Statement[] = [];
  for (const t of toks) {
    if (t.kind === "opr") {
      ret.push({ ...t, pc: pc++ });
    } else if (t.kind === "lab_pc") {
      ret.push({ ...t, value: pc });
    } else {
      ret.push(t);
    }
  }
  return ret;
};

export const build_op_arg = (opr: Operation, label: (PCLabel | VarLabel | ConstLabel)[]): Operation => {
  const op = opr.op;
  const args = opr.args;

  const opinfo = mnemonics[op];
  if (opinfo === undefined) throw `Undefined mnemonic @${opr.line} : ${op}`;
  const arg_format = opinfo[0].split(".").filter((s) => s !== "");

  const arg_built = arg_format.map((format, i) => {
    const arg = args.at(i);
    if (!arg) throw `Argument required @${opr.line} : ${arg_format}`;
    else if (format === "r") {
      // レジスタ名を解決
      // Resolute registor name
      const reg = regs[arg.str];
      if (reg === undefined) {
        throw `Undefined register name @${opr.line} : ${arg.str}`;
      }
      return { ...arg, kind: "reg", val: reg } as Arg;
    } else if (format === "i") {
      // 即値を解決
      // Resolute immidiate
      const int = parseInt(arg.str);
      if (isNaN(int)) {
        const lab = label.find((l) => l.label === arg.str);
        if (lab === undefined) throw `Undefined label name @${opr.line} : ${arg}`;
        return { ...arg, kind: lab.kind, val: lab.value } as Arg;
      } else {
        return { ...arg, kind: "imm", val: int } as Arg;
      }
    } else throw `Format error: ${format}`;
  });

  return { ...opr, args: arg_built };
};

export const build_op_bin = (opr: Operation): Operation => {
  return { ...opr, bin: to_bin(opr.op) };
};

const to_bin = (op: string): number => {
  if (["add", "sub"].includes(op)) pack(0, 0, 0, 0, 0);
  if (op === "sub") return 0;
  return 0;
};

const pack = (u4_0: number, u4_1: number, u4_2: number, u4_3: number, u16: number) => (u4_0 & 0x0f) | ((u4_1 & 0x0f) << 4) | ((u4_2 & 0x0f) << 8) | ((u4_3 & 0x0f) << 12) | ((u16 & 0xffff) << 16);
