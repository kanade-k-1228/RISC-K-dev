import { Token } from "./type.ts";

export const trim = (s: string) => {
  const idx = s.indexOf(";");
  return idx === -1 ? s : s.substring(0, idx);
};

export const slice = (s: string) => {
  return s.split(/\s/).filter((s) => s !== "");
};

export const tokenize = (s: string[], idx: number): Token[] => {
  if (s.length === 0) return [];
  const s0 = s.at(0);
  if (s.at(0)?.endsWith(":")) {
    const name = s[0].slice(0, -1);
    return [{ line: idx, kind: "lab_pc", name }];
  }
  if (s.at(0)?.startsWith("#")) {
    const name = s[1];
    const value = parseInt(s[0].slice(1, -1));
    return [{ line: idx, kind: "lab_const", name, value }];
  }
  if (s.at(0)?.startsWith("@")) {
    const name = s[1];
    const value = parseInt(s[0].slice(1, -1));
    return [{ line: idx, kind: "lab_var", name, value }];
  }
  if (true) {
    const op = s[0];
    const args = s.slice(1);
    return [{ line: idx, kind: "opr", op, args }];
  }
};

export const fill_pc = (toks: Token[]) => {
  let pc = 0;
  const ret: Token[] = [];
  for (const t of toks) {
    if (t.kind === "opr") {
      ret.push({ ...t, pc: pc++ });
    } else if (t.kind === "lab_pc") {
      ret.push({ ...t, value: pc });
    } else {
      ret.push(t);
    }
  }
  return ret;
};
