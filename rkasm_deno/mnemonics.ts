export const alu: Record<string, number> = { add: 0, not: 1, sl: 2, lrot: 3, and: 4, xor: 5, or: 6, sub: 7, eq: 8, neq: 9, ltu: 10, lts: 11, sru: 12, srs: 13, rrot: 14 };
export const opc: Record<string, number> = { calc: 0, calci: 1, load: 2, store: 3, calif: 4 };
export const regs: Record<string, number> = { zero: 0, ira: 1, pc: 2, sp: 3, ra: 4, fp: 5, a0: 6, a1: 7, t0: 8, t1: 9, t2: 10, t3: 11, s0: 12, s1: 13, s2: 14, s3: 15 };

type Encode<T = string | number | "arg0" | "arg1" | "arg2"> = [T, T, T, T, T];

export const mnemonics: Record<string, [string, Encode]> = {
  // レジスタ間演算
  add: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.add]],
  not: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.not]],
  sl: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.sl]],
  lrot: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.lrot]],
  and: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.and]],
  xor: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.xor]],
  or: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.or]],
  sub: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.sub]],
  eq: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.eq]],
  neq: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.neq]],
  ltu: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.ltu]],
  lts: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.lts]],
  sru: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.sru]],
  srs: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.srs]],
  rrot: ["r.r.r", [opc.calc, "arg1", "arg2", "arg0", alu.rrot]],

  nop: ["", [opc.calc, regs.zero, regs.zero, regs.zero, alu.add]],
  mov: ["r.r", [opc.calc, "arg1", regs.zero, "arg0", alu.add]],

  // 即値演算
  addi: ["r.r.r", [opc.calci, "arg1", alu.add, "arg0", "arg2"]],
  noti: ["r.r.r", [opc.calci, "arg1", alu.not, "arg0", "arg2"]],
  sli: ["r.r.r", [opc.calci, "arg1", alu.sl, "arg0", "arg2"]],
  lroti: ["r.r.r", [opc.calci, "arg1", alu.lrot, "arg0", "arg2"]],
  andi: ["r.r.r", [opc.calci, "arg1", alu.and, "arg0", "arg2"]],
  xori: ["r.r.r", [opc.calci, "arg1", alu.xor, "arg0", "arg2"]],
  ori: ["r.r.r", [opc.calci, "arg1", alu.or, "arg0", "arg2"]],
  subi: ["r.r.r", [opc.calci, "arg1", alu.sub, "arg0", "arg2"]],
  eqi: ["r.r.r", [opc.calci, "arg1", alu.eq, "arg0", "arg2"]],
  neqi: ["r.r.r", [opc.calci, "arg1", alu.neq, "arg0", "arg2"]],
  ltui: ["r.r.r", [opc.calci, "arg1", alu.ltu, "arg0", "arg2"]],
  ltsi: ["r.r.r", [opc.calci, "arg1", alu.lts, "arg0", "arg2"]],
  srui: ["r.r.r", [opc.calci, "arg1", alu.sru, "arg0", "arg2"]],
  srsi: ["r.r.r", [opc.calci, "arg1", alu.srs, "arg0", "arg2"]],
  rroti: ["r.r.r", [opc.calci, "arg1", alu.rrot, "arg0", "arg2"]],

  loadi: ["r.i", [opc.calci, regs.zero, alu.add, "arg0", "arg1"]],

  // ロード
  load: ["r.r.i", [opc.load, "arg1", alu.add, "arg0", "arg2"]],
  pop: ["r", [opc.load, regs.zero, alu.add, "arg0", 1]],

  // ストア
  store: ["r.r.i", [opc.store, "arg1", "arg0", alu.add, "arg2"]],
  push: ["r", [opc.store, "arg1", "arg0", alu.sub, 0]],

  // 制御
  if: ["r.r.i", [opc.calif, "arg1", "arg0", regs.zero, "arg2"]],
  jump: ["i", [opc.calif, regs.zero, regs.zero, regs.zero, "arg0"]],
  call: ["i", [opc.calif, regs.zero, regs.zero, regs.ra, "arg0"]],
  ret: ["", [opc.calif, regs.ra, regs.zero, regs.zero, 0]],
  iret: ["", [opc.calif, regs.ira, regs.zero, regs.zero, 0]],
};
