import { green, red } from "https://deno.land/std@0.170.0/fmt/colors.ts";
import { hex, left, right } from "./utils.ts";
import { Operation, alu_emu, decode, regs } from "../rkisa/mnemonics.ts";

export class Model {
  // State
  pc: number;
  ram: number[];
  rom: string[];

  // Initial State
  constructor(rom_init: string[]) {
    this.pc = 0;
    this.ram = Array(0x1_0000).fill(0);
    this.rom = Array(0x1_0000)
      .fill("00000000")
      .map((_, i) => (i < rom_init.length ? rom_init[i] : "00000000"));
  }

  intr = () => {
    this.pc = 0x0010;
    this.execute(this.rom[this.pc]);
  };

  next = () => {
    this.execute(this.rom[this.pc]);
  };

  print = (time: number) => {
    return [
      " +-----------+----------+----------+----------+",
      ` |  ${green(`PC: ${hex(4, this.pc)}`)} | RA: ${hex(4, this.ram[4])} | T0: ${hex(4, this.ram[8])} | S0: ${hex(4, this.ram[12])} |`,
      ` | IRA: ${hex(4, this.ram[1])} | FP: ${hex(4, this.ram[5])} | T1: ${hex(4, this.ram[9])} | S1: ${hex(4, this.ram[13])} |`,
      ` | CSR: ${hex(4, this.ram[2])} | A0: ${hex(4, this.ram[6])} | T2: ${hex(4, this.ram[10])} | S2: ${hex(4, this.ram[14])} |`,
      ` |  SP: ${hex(4, this.ram[3])} | A1: ${hex(4, this.ram[7])} | T3: ${hex(4, this.ram[11])} | S3: ${hex(4, this.ram[15])} |`,
      " +-----------+----------+----------+----------+",
      `[${right(4, time.toString(), "0")}] ${this.rom[this.pc]} ${print_op(decode(this.rom[this.pc]))} `,
    ].join("\n");
  };
  execute = (hex: string) => {
    const op = decode(hex);

    const rs1 = this.ram[regs[op.rs1]];
    const rs2 = this.ram[regs[op.rs2]];
    const imm = parseInt(op.imm, 16);
    const alu_out = alu_emu(rs1, op.opc === "calc" ? rs2 : imm, op.func);

    switch (op.opc) {
      case "calc":
        this.ram[regs[op.rd]] = alu_out;
        this.pc++;
        return;

      case "calci":
        this.ram[regs[op.rd]] = alu_out;
        this.pc++;
        return;

      case "load":
        this.ram[regs[op.rd]] = alu_out;
        if (op.rs1 === "sp") this.ram[regs.sp]++; // pop
        this.pc++;
        return;

      case "store":
        this.ram[alu_out] = rs2;
        if (op.rs1 === "sp") this.ram[regs.sp]--; // push
        this.pc++;
        return;

      case "ctrl":
        this.pc = rs2 === 0 ? alu_out : this.pc + 1;
        return;
    }
  };
}

export const print_op = (op: Operation): string => {
  return `${red(left(6, op.opc))} `;
};
