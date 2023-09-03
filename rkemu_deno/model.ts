import { green } from "https://deno.land/std@0.170.0/fmt/colors.ts";

export class Model {
  // State
  pc: number;
  ram: number[];

  // Initial State
  constructor() {
    this.pc = 0;
    this.ram = Array(0x1_0000).fill(0);
  }

  intr = () => {
    this.pc = 0x0010;
  };

  next = () => {
    this.pc++;
  };

  print = () => {
    return [
      " +-----------+----------+----------+----------+",
      ` |  ${green(`PC: ${hex(4, this.pc)}`)} | RA: ${hex(4, this.ram[4])} | T0: ${hex(4, this.ram[8])} | S0: ${hex(4, this.ram[12])} |`,
      ` | IRA: ${hex(4, this.ram[1])} | FP: ${hex(4, this.ram[5])} | T1: ${hex(4, this.ram[9])} | S1: ${hex(4, this.ram[13])} |`,
      ` | CSR: ${hex(4, this.ram[2])} | A0: ${hex(4, this.ram[6])} | T2: ${hex(4, this.ram[10])} | S2: ${hex(4, this.ram[14])} |`,
      ` |  SP: ${hex(4, this.ram[3])} | A1: ${hex(4, this.ram[7])} | T3: ${hex(4, this.ram[11])} | S3: ${hex(4, this.ram[15])} |`,
      " +-----------+----------+----------+----------+",
    ].join("\n");
  };
}

const hex = (w: number, n: number) => right(w, n.toString(16).toUpperCase(), "0");

const right = (n: number, s: string, fill = " ") => {
  const offset = n - s.length;
  return offset >= 0 ? fill.repeat(n - s.length) + s : s;
};

const left = (n: number, s: string, fill = " ") => {
  const offset = n - s.length;
  return offset >= 0 ? s + fill.repeat(n - s.length) : s;
};
