export type Token = TokenOp | TokenLabPc | TokenLabVar | TokenLabConst;

export interface TokenOp {
  line: number;
  kind: "opr";
  op: string;
  args: string[];
  pc?: number;
  func?: string;
}

export interface TokenLabPc {
  line: number;
  kind: "lab_pc";
  name: string;
  value?: number;
}

export interface TokenLabVar {
  line: number;
  kind: "lab_var";
  name: string;
  value: number;
}

export interface TokenLabConst {
  line: number;
  kind: "lab_const";
  name: string;
  value: number;
}
