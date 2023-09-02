export const alu: Record<string, number> = { add: 0, not: 1, sl: 2, lrot: 3, and: 4, xor: 5, or: 6, sub: 7, eq: 8, neq: 9, ltu: 10, lts: 11, sru: 12, srs: 13, rrot: 14 };
export const opc: Record<string, number> = { calc: 0, calci: 1, load: 2, store: 3, calif: 4 };
export const regs: Record<string, number> = { zero: 0, ira: 1, pc: 2, sp: 3, ra: 4, fp: 5, a0: 6, a1: 7, t0: 8, t1: 9, t2: 10, t3: 11, s0: 12, s1: 13, s2: 14, s3: 15 };

type Encode<T = string | number | "arg0" | "arg1" | "arg2"> = [T, T, T, T, T];

export const mnemonics: Record<string, { arg: string; bitfield: Encode }> = {
  // レジスタ間演算
  add: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.add] },
  not: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.not] },
  sl: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.sl] },
  lrot: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.lrot] },
  and: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.and] },
  xor: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.xor] },
  or: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.or] },
  sub: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.sub] },
  eq: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.eq] },
  neq: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.neq] },
  ltu: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.ltu] },
  lts: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.lts] },
  sru: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.sru] },
  srs: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.srs] },
  rrot: { arg: "r.r.r", bitfield: [opc.calc, "arg1", "arg2", "arg0", alu.rrot] },

  nop: { arg: "", bitfield: [opc.calc, regs.zero, regs.zero, regs.zero, alu.add] },
  mov: { arg: "r.r", bitfield: [opc.calc, "arg1", regs.zero, "arg0", alu.add] },

  // 即値演算
  addi: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.add, "arg0", "arg2"] },
  noti: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.not, "arg0", "arg2"] },
  sli: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.sl, "arg0", "arg2"] },
  lroti: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.lrot, "arg0", "arg2"] },
  andi: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.and, "arg0", "arg2"] },
  xori: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.xor, "arg0", "arg2"] },
  ori: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.or, "arg0", "arg2"] },
  subi: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.sub, "arg0", "arg2"] },
  eqi: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.eq, "arg0", "arg2"] },
  neqi: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.neq, "arg0", "arg2"] },
  ltui: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.ltu, "arg0", "arg2"] },
  ltsi: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.lts, "arg0", "arg2"] },
  srui: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.sru, "arg0", "arg2"] },
  srsi: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.srs, "arg0", "arg2"] },
  rroti: { arg: "r.r.r", bitfield: [opc.calci, "arg1", alu.rrot, "arg0", "arg2"] },

  loadi: { arg: "r.i", bitfield: [opc.calci, regs.zero, alu.add, "arg0", "arg1"] },

  // ロード
  load: { arg: "r.r.i", bitfield: [opc.load, "arg1", alu.add, "arg0", "arg2"] },
  pop: { arg: "r", bitfield: [opc.load, regs.zero, alu.add, "arg0", 1] },

  // ストア
  store: { arg: "r.r.i", bitfield: [opc.store, "arg1", "arg0", alu.add, "arg2"] },
  push: { arg: "r", bitfield: [opc.store, "arg1", "arg0", alu.sub, 0] },

  // 制御
  if: { arg: "r.r.i", bitfield: [opc.calif, "arg1", "arg0", regs.zero, "arg2"] },
  jump: { arg: "i", bitfield: [opc.calif, regs.zero, regs.zero, regs.zero, "arg0"] },
  call: { arg: "i", bitfield: [opc.calif, regs.zero, regs.zero, regs.ra, "arg0"] },
  ret: { arg: "", bitfield: [opc.calif, regs.ra, regs.zero, regs.zero, 0] },
  iret: { arg: "", bitfield: [opc.calif, regs.ira, regs.zero, regs.zero, 0] },
};
