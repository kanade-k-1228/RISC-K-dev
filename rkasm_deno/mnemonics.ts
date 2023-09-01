export enum ALU {
  add,
  not,
  sl,
  lrot,
  and,
  xor,
  or,
  sub,
  eq,
  neq,
  ltu,
  lts,
  sru,
  srs,
  rrot,
}

type op = Record<string, { func?: number; arg: string }>;

export const mnemonics: op = {
  nop: { func: ALU.add, arg: "" },
  mov: { func: ALU.add, arg: "r.r" },
  loadi: { func: ALU.add, arg: "r.i" },

  add: { func: ALU.add, arg: "r.r.r" },
  not: { func: ALU.not, arg: "r.r.r" },
  sl: { func: ALU.sl, arg: "r.r.r" },
  lrot: { func: ALU.lrot, arg: "r.r.r" },
  and: { func: ALU.and, arg: "r.r.r" },
  xor: { func: ALU.xor, arg: "r.r.r" },
  or: { func: ALU.or, arg: "r.r.r" },
  sub: { func: ALU.sub, arg: "r.r.r" },
  eq: { func: ALU.eq, arg: "r.r.r" },
  neq: { func: ALU.neq, arg: "r.r.r" },
  ltu: { func: ALU.ltu, arg: "r.r.r" },
  lts: { func: ALU.lts, arg: "r.r.r" },
  sru: { func: ALU.sru, arg: "r.r.r" },
  srs: { func: ALU.srs, arg: "r.r.r" },
  rrot: { func: ALU.rrot, arg: "r.r.r" },

  addi: { func: ALU.add, arg: "r.r.i" },
  noti: { func: ALU.not, arg: "r.r.i" },
  sli: { func: ALU.sl, arg: "r.r.i" },
  lroti: { func: ALU.lrot, arg: "r.r.i" },
  andi: { func: ALU.and, arg: "r.r.i" },
  xori: { func: ALU.xor, arg: "r.r.i" },
  ori: { func: ALU.or, arg: "r.r.i" },
  subi: { func: ALU.sub, arg: "r.r.i" },
  eqi: { func: ALU.eq, arg: "r.r.i" },
  neqi: { func: ALU.neq, arg: "r.r.i" },
  ltui: { func: ALU.ltu, arg: "r.r.i" },
  ltsi: { func: ALU.lts, arg: "r.r.i" },
  srui: { func: ALU.sru, arg: "r.r.i" },
  srsi: { func: ALU.srs, arg: "r.r.i" },
  rroti: { func: ALU.rrot, arg: "r.r.i" },

  load: { func: 0, arg: "r.r.i" },
  pop: { func: 1, arg: "r" },

  store: { func: 0, arg: "r.r.i" },
  push: { func: 0, arg: "r" },

  if: { arg: "r.r.i" },
  jump: { arg: "i" },
  call: { arg: "i" },
  ret: { arg: "" },
  iret: { arg: "" },
};

const regs: Record<string, number> = {
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

export const read = (op: string, arg: string[]) => {
  const opinfo = mnemonics[op];
  const arg_format = opinfo.arg.split(".");
  const arg_int = arg_format.map((f, i) => {
    const arg_str = arg.at(i);
    if (!arg_str) throw "argument required";
    if (f === "r") {
      return resolute_reg(arg_str);
    }
    if (f === "i") {
      const int = parseInt(arg_str);
      return isNaN(int) ? resolute_label(arg_str) : int;
    }
  });
};

const resolute_reg = (s: string) => {
  const ret = regs[s];
  if (ret === undefined) throw "Undefined register name";
  return ret;
};

const resolute_label = (s: string) => 0;