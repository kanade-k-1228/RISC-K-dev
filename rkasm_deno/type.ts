export type Statement = Operation | PCLabel | VarLabel | ConstLabel;

export interface Operation {
  file: string;
  line: number;
  raw: string;
  kind: "opr";
  op: string;
  args: Arg[];
  pc?: number;
  bin?: number;
}

export interface Arg {
  str: string;
  kind?: "reg" | "imm" | "lab_pc" | "lab_var" | "lab_const";
  val?: number;
}

export interface PCLabel {
  file: string;
  line: number;
  raw: string;
  kind: "lab_pc";
  label: string;
  value?: number;
}

export interface VarLabel {
  file: string;
  line: number;
  raw: string;
  kind: "lab_var";
  label: string;
  value: number;
}

export interface ConstLabel {
  file: string;
  line: number;
  raw: string;
  kind: "lab_const";
  label: string;
  value: number;
}
