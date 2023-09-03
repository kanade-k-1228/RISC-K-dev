import { StatementError } from "./error.ts";
import { get_arg_format, mnemonics, regs } from "../rkisa/mnemonics.ts";
import { Arg, Statement, ConstLabel, PCLabel, VarLabel, Operation } from "./type.ts";

export const interpret = (file: string, str: string[], idx: number): Statement[] => {
  const str0 = str.at(0);
  const line = idx + 1;
  if (!str0) return [];
  if (str0.endsWith(":")) {
    const label = str0.slice(0, -1);
    return [{ file, line, str, kind: "lab_pc", label }];
  } else if (str0.startsWith("#")) {
    const label = str[1];
    const value = parseInt(str[0].slice(1));
    return [{ file, line, str, kind: "lab_const", label, value }];
  } else if (str0.startsWith("@")) {
    const label = str[1];
    const value = parseInt(str[0].slice(1));
    return [{ file, line, str, kind: "lab_var", label, value }];
  } else {
    const op = str0;
    const args = str.slice(1).map((str) => ({ str }));
    return [{ file, line, str, kind: "opr", op, args }];
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

export const build_op_arg = (
  opr: Operation,
  label: (PCLabel | VarLabel | ConstLabel)[]
): Operation => {
  const opinfo = mnemonics[opr.op];
  if (opinfo === undefined) throw new StatementError(opr, `Undefined mnemonic`, opr.op);
  const arg_format = get_arg_format(opinfo[0]);

  const arg_built = arg_format.map((format, i) => {
    const arg = opr.args.at(i);
    if (!arg) throw new StatementError(opr, `More argument required`, arg_format.toString());
    else if (format === "r") {
      // レジスタ名を解決
      // Resolute registor name
      const reg = regs[arg.str];
      if (reg === undefined) throw new StatementError(opr, `Undefined register name`, arg.str);
      return { ...arg, kind: "reg", val: reg } as Arg;
    } else if (format === "i") {
      // 即値を解決
      // Resolute immidiate
      const int = parseInt(arg.str);
      if (isNaN(int)) {
        const lab = label.find((l) => l.label === arg.str);
        if (lab === undefined) throw new StatementError(opr, `Undefined label`, arg.str);
        return { ...arg, kind: lab.kind, val: lab.value } as Arg;
      } else {
        return { ...arg, kind: "imm", val: int } as Arg;
      }
    } else throw new StatementError(opr, `Invalid mnemonic format`, opr.op);
  });

  return { ...opr, args: arg_built };
};

export const build_op_bin = (opr: Operation): Operation => {
  return { ...opr, bin: to_bin(opr) };
};

const to_bin = (opr: Operation): number => {
  const opinfo = mnemonics[opr.op];
  if (opinfo === undefined) throw new StatementError(opr, `Undefined mnemonic`, opr.op);

  const encode = opinfo[1];
  const bits = encode.map((e) => {
    if (typeof e === "number") {
      return e;
    } else {
      const val = opr.args.at(parseInt(e))?.val;
      if (val === undefined) {
        console.log(opr);
        throw new StatementError(opr, `More argument required`);
      }
      return val;
    }
  });

  return (
    ((bits[0] & 0xf) |
      ((bits[1] & 0xf) << 4) |
      ((bits[2] & 0xf) << 8) |
      ((bits[3] & 0xf) << 12) |
      ((bits[4] & 0xffff) << 16)) >>>
    0
  );
  // >>> 0 : cast to uint32
};
