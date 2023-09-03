import { red } from "https://deno.land/std@0.199.0/fmt/colors.ts";
import { Statement } from "./type.ts";

export class StatementError extends Error {
  stmt: Statement;
  highlight: string;
  constructor(stmt: Statement, message: string, highlight = "") {
    super(message);
    this.stmt = stmt;
    this.name = "AsemblerError";
    this.highlight = highlight;
  }
  print = () => {
    const raw = this.stmt.str.join(" ");
    const idx = raw.indexOf(this.highlight);
    const highlighted = this.stmt.str.map((s) => (s === this.highlight ? red(s) : s)).join(" ");
    const underline =
      idx === -1
        ? red("~".repeat(raw.length))
        : " ".repeat(idx) + red("~".repeat(this.highlight.length));
    const comment = " ".repeat(idx === -1 ? 0 : idx) + "^ " + this.message;

    return [
      ``,
      ` ${red(`ERROR`)} at ${this.stmt.file}:${this.stmt.line}`,
      "",
      `    ${highlighted}`,
      `    ${underline}`,
      `    ${comment}`,
      "",
    ]
      .map((s) => red("|") + s)
      .join("\n");
  };
}
