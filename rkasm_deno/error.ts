import { red } from "https://deno.land/std/fmt/colors.ts";

export class AsemblerError extends Error {
  file: string;
  line: number;
  constructor(message: string, file: string, line: number) {
    super(message);
    this.name = "AsemblerError";
    this.file = file;
    this.line = line;
  }
  print = () => {
    return `${red("ERROR")} at ${this.file}:${this.line}\n${this.message}`;
  };
}
