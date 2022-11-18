enum Reg {
  ZERO = 0x00,
  RA = 0x01,
  SP = 0x02,
  FP = 0x03,
  S0 = 0x04,
  S1 = 0x05,
  S2 = 0x06,
  S3 = 0x07,
  T0 = 0x08,
  T1 = 0x09,
  T2 = 0x0A,
  T3 = 0x0B,
  A0 = 0x0C,
  A1 = 0x0D,
  A2 = 0x0E,
  A3 = 0x0F,
  CSR = 0x10,
  IID = 0x11,  // TODO 廃止
  IRA = 0x12,
  COUT = 0x30,
};

enum OP {
  CALC = 0x0,
  CALCI = 0x1,
  LOAD = 0x2,
  LOADI = 0x3,
  STORE = 0x4,
  JUMP = 0x5,
  BREQ = 0x6,
  BRLT = 0x7,
};

enum Func {
  ADD = 0x0,
  SUB = 0x1,
  AND = 0x2,
  OR = 0x3,
  XOR = 0x4,
  NOT = 0x5,
  LROT = 0x06,
  RROT = 0x07,
};

enum CSRBit {
  IEN = 0b0000'0000'0001,
  INTR = 0b0000'0000'0010,
  INTR0 = 0b0000'0000'0100,
  INTR1 = 0b0000'0000'1000,
  INTR2 = 0b0000'0001'0000,
  INTR3 = 0b0000'0010'0000,
  SRCV = 0b0000'0100'0000,
  SSEND = 0b0000'1000'0000,
  CSTOP = 0b0001'0000'0000,
};
