export type OPCName = "calc" | "calci" | "load" | "store" | "ctrl";

export const opc: Record<OPCName, number> = {
  calc: 0b0000,
  calci: 0b0001,
  load: 0b0011,
  store: 0b0111,
  ctrl: 0b1111,
};

export type ALUFunc =
  | "add"
  | "not"
  | "lrot"
  | "and"
  | "sl"
  | "xor"
  | "or"
  | "sub"
  | "eq"
  | "neq"
  | "ltu"
  | "lts"
  | "sru"
  | "srs"
  | "rrot";

export const alu: Record<ALUFunc, number> = {
  add: 0,
  not: 1,
  sl: 2,
  lrot: 3,
  and: 4,
  xor: 5,
  or: 6,
  sub: 7,
  eq: 8,
  neq: 9,
  ltu: 10,
  lts: 11,
  sru: 12,
  srs: 13,
  rrot: 14,
};

export type RegName =
  | "zero"
  | "ira"
  | "pc"
  | "sp"
  | "ra"
  | "fp"
  | "a0"
  | "a1"
  | "t0"
  | "t1"
  | "t2"
  | "t3"
  | "s0"
  | "s1"
  | "s2"
  | "s3";

export const regs: Record<RegName, number> = {
  zero: 0,
  ira: 1,
  pc: 2,
  sp: 3,
  ra: 4,
  fp: 5,
  a0: 6,
  a1: 7,
  t0: 8,
  t1: 9,
  t2: 10,
  t3: 11,
  s0: 12,
  s1: 13,
  s2: 14,
  s3: 15,
};

type Encode<T = number | "0" | "1" | "2"> = [T, T, T, T, T];

export const get_arg_format = (str: string) =>
  str.split(".").filter((s) => s !== "") as ("r" | "i")[];

export const mnemonics: Record<string, [string, Encode]> = {
  // レジスタ間演算
  add: ["r.r.r", [opc.calc, "1", "2", "0", alu.add]],
  not: ["r.r.r", [opc.calc, "1", "2", "0", alu.not]],
  sl: ["r.r.r", [opc.calc, "1", "2", "0", alu.sl]],
  lrot: ["r.r.r", [opc.calc, "1", "2", "0", alu.lrot]],
  and: ["r.r.r", [opc.calc, "1", "2", "0", alu.and]],
  xor: ["r.r.r", [opc.calc, "1", "2", "0", alu.xor]],
  or: ["r.r.r", [opc.calc, "1", "2", "0", alu.or]],
  sub: ["r.r.r", [opc.calc, "1", "2", "0", alu.sub]],
  eq: ["r.r.r", [opc.calc, "1", "2", "0", alu.eq]],
  neq: ["r.r.r", [opc.calc, "1", "2", "0", alu.neq]],
  ltu: ["r.r.r", [opc.calc, "1", "2", "0", alu.ltu]],
  lts: ["r.r.r", [opc.calc, "1", "2", "0", alu.lts]],
  sru: ["r.r.r", [opc.calc, "1", "2", "0", alu.sru]],
  srs: ["r.r.r", [opc.calc, "1", "2", "0", alu.srs]],
  rrot: ["r.r.r", [opc.calc, "1", "2", "0", alu.rrot]],

  nop: ["", [opc.calc, regs.zero, regs.zero, regs.zero, alu.add]],
  mov: ["r.r", [opc.calc, "1", regs.zero, "0", alu.add]],

  // 即値演算
  addi: ["r.r.i", [opc.calci, "1", alu.add, "0", "2"]],
  noti: ["r.r.i", [opc.calci, "1", alu.not, "0", "2"]],
  sli: ["r.r.i", [opc.calci, "1", alu.sl, "0", "2"]],
  lroti: ["r.r.i", [opc.calci, "1", alu.lrot, "0", "2"]],
  andi: ["r.r.i", [opc.calci, "1", alu.and, "0", "2"]],
  xori: ["r.r.i", [opc.calci, "1", alu.xor, "0", "2"]],
  ori: ["r.r.i", [opc.calci, "1", alu.or, "0", "2"]],
  subi: ["r.r.i", [opc.calci, "1", alu.sub, "0", "2"]],
  eqi: ["r.r.i", [opc.calci, "1", alu.eq, "0", "2"]],
  neqi: ["r.r.i", [opc.calci, "1", alu.neq, "0", "2"]],
  ltui: ["r.r.i", [opc.calci, "1", alu.ltu, "0", "2"]],
  ltsi: ["r.r.i", [opc.calci, "1", alu.lts, "0", "2"]],
  srui: ["r.r.i", [opc.calci, "1", alu.sru, "0", "2"]],
  srsi: ["r.r.i", [opc.calci, "1", alu.srs, "0", "2"]],
  rroti: ["r.r.i", [opc.calci, "1", alu.rrot, "0", "2"]],

  loadi: ["r.i", [opc.calci, regs.zero, alu.add, "0", "1"]],

  // ロード
  load: ["r.r.i", [opc.load, "1", alu.add, "0", "2"]],
  pop: ["r", [opc.load, regs.zero, alu.add, "0", 1]],

  // ストア
  store: ["r.r.i", [opc.store, "1", "0", alu.add, "2"]],
  push: ["r", [opc.store, "1", "0", alu.sub, 0]],

  // 制御
  if: ["r.i", [opc.ctrl, regs.zero, "0", regs.zero, "1"]], // 絶対アドレス
  ifr: ["r.i", [opc.ctrl, regs.pc, "0", regs.zero, "1"]], // 相対アドレス
  jump: ["i", [opc.ctrl, regs.zero, regs.zero, regs.zero, "0"]], // 絶対アドレス
  jumpr: ["i", [opc.ctrl, regs.pc, regs.zero, regs.zero, "0"]], // 相対アドレス
  call: ["i", [opc.ctrl, regs.zero, regs.zero, regs.ra, "0"]],
  ret: ["", [opc.ctrl, regs.ra, regs.zero, regs.zero, 0]],
  iret: ["", [opc.ctrl, regs.ira, regs.zero, regs.zero, 0]],
};

export interface Operation {
  opc: OPCName;
  func: ALUFunc;
  rs1: RegName;
  rs2: RegName;
  rd: RegName;
  imm: string;
}

export const encode = (op: Operation): string => {
  return "00000000";
};

export const decode = (code: string): Operation => {
  if (code.length !== 8) throw Error(`Decode error`);
  const op = parseInt(code[7], 16);

  const ret = {
    rs1: decode_reg(code[6]),
    rs2: decode_reg(code[5]),
    rd: decode_reg(code[4]),
    imm: code.slice(0, 4),
  };

  switch (op) {
    case opc.calc:
      return { ...ret, opc: "calc", func: decode_alu(code[3]) };

    case opc.calci:
      return { ...ret, opc: "calci", func: decode_alu(code[5]) };

    case opc.load:
      return { ...ret, opc: "load", func: "add" };

    case opc.store:
      return { ...ret, opc: "store", func: "add" };

    case opc.ctrl:
      return { ...ret, opc: "ctrl", func: "add" };

    default:
      throw Error(`Undefined opecode: ${op}`);
  }
};

const decode_alu = (code: string) => {
  const alu_code = parseInt(code, 16);
  const alu_func = Object.entries(alu).find(([_, code]) => code === alu_code);
  if (!alu_func) throw Error(`Undefined ALU code: ${alu_code}`);
  return alu_func[0] as ALUFunc;
};

const decode_reg = (code: string) => {
  const reg_code = parseInt(code, 16);
  const reg = Object.entries(regs).find(([_, code]) => code === reg_code);
  if (!reg) throw Error(`Undefined Registor code: ${reg_code}`);
  return reg[0] as RegName;
};

export const alu_emu = (a: number, b: number, c: ALUFunc) => {
  switch (c) {
    case "add":
      return a + b;
    case "not":
      return ~a;
    case "lrot":
      return ((a << 1) & 0xffff) | ((a >> 15) & 1);
    case "and":
      return a & b;
    case "sl":
      return (a << 1) & 0xffff;
    case "xor":
      return a ^ b;
    case "or":
      return a | b;
    case "sub":
      return a - b;
    case "eq":
      return a === b ? 0xffff : 0x0000;
    case "neq":
      return a !== b ? 0xffff : 0x0000;
    case "ltu":
      return a < b ? 0xffff : 0x0000;
    case "lts":
      return a < b ? 0xffff : 0x0000;
    case "sru":
      return a >>> 1;
    case "srs":
      return a >> 1;
    case "rrot":
      return a >>> 1;
  }
};
