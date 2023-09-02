export type Statement = Operation | PCLabel | VarLabel | ConstLabel;

export interface Operation {
  line: number;
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
  line: number;
  kind: "lab_pc";
  label: string;
  value?: number;
}

export interface VarLabel {
  line: number;
  kind: "lab_var";
  label: string;
  value: number;
}

export interface ConstLabel {
  line: number;
  kind: "lab_const";
  label: string;
  value: number;
}
