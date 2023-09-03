export const alu: Record<string, number> = { add: 0, not: 1, sl: 2, lrot: 3, and: 4, xor: 5, or: 6, sub: 7, eq: 8, neq: 9, ltu: 10, lts: 11, sru: 12, srs: 13, rrot: 14 };
export const opc: Record<string, number> = { calc: 0b0000, calci: 0b0001, load: 0b0011, store: 0b0111, ctrl: 0b1111 };
export const regs: Record<string, number> = { zero: 0, ira: 1, pc: 2, sp: 3, ra: 4, fp: 5, a0: 6, a1: 7, t0: 8, t1: 9, t2: 10, t3: 11, s0: 12, s1: 13, s2: 14, s3: 15 };

type Encode<T = number | "0" | "1" | "2"> = [T, T, T, T, T];

export const get_arg_format = (str: string) => str.split(".").filter((s) => s !== "") as ("r" | "i")[];

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
