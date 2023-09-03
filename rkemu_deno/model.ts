import { green } from "https://deno.land/std@0.170.0/fmt/colors.ts";
import { hex, right } from "./utils.ts";

export class Model {
  // State
  pc: number;
  ram: number[];
  rom: number[];

  // Initial State
  constructor() {
    this.pc = 0;
    this.ram = Array(0x1_0000).fill(0);
    this.rom = Array(0x1_0000).fill(0);
  }

  intr = () => {
    this.pc = 0x0010;
  };

  next = () => {
    this.pc++;
  };

  print = (time: number) => {
    return [
      " +-----------+----------+----------+----------+",
      ` |  ${green(`PC: ${hex(4, this.pc)}`)} | RA: ${hex(4, this.ram[4])} | T0: ${hex(4, this.ram[8])} | S0: ${hex(4, this.ram[12])} |`,
      ` | IRA: ${hex(4, this.ram[1])} | FP: ${hex(4, this.ram[5])} | T1: ${hex(4, this.ram[9])} | S1: ${hex(4, this.ram[13])} |`,
      ` | CSR: ${hex(4, this.ram[2])} | A0: ${hex(4, this.ram[6])} | T2: ${hex(4, this.ram[10])} | S2: ${hex(4, this.ram[14])} |`,
      ` |  SP: ${hex(4, this.ram[3])} | A1: ${hex(4, this.ram[7])} | T3: ${hex(4, this.ram[11])} | S3: ${hex(4, this.ram[15])} |`,
      " +-----------+----------+----------+----------+",
      `[${right(4, time.toString(), "0")}] ${hex(8, this.rom[this.pc])}`,
    ].join("\n");
  };
}

const decode = (code: number) => {};
