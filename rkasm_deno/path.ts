export const trim = (s: string) => {
  const idx = s.indexOf(";");
  return idx === -1 ? s : s.substring(0, idx);
};

export const slice = (s: string) => {
  return s.split(/\s/).filter((s) => s !== "");
};

interface TokenOp {
  line: number;
  kind: "opr";
  op: string;
  args: string[];
  pc?: number;
  func?: string;
}

interface TokenLabPc {
  line: number;
  kind: "lab_pc";
  name: string;
  value?: number;
}

interface TokenLabVar {
  line: number;
  kind: "lab_var";
  name: string;
  value: number;
}

interface TokenLabConst {
  line: number;
  kind: "lab_const";
  name: string;
  value: number;
}

export type Token = TokenOp | TokenLabPc | TokenLabVar | TokenLabConst;

export const tokenize = (s: string[], idx: number): Token[] => {
  if (s.length === 0) return [];
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

export const fill_label = (toks: Token[]) => {
  let pc = 0;
  const ret: Token[] = [];
  for (const t of toks) {
    if (t.kind === "opr") {
      ++pc;
      ret.push({ ...t, pc });
    } else if (t.kind === "lab_pc") {
      ret.push({ ...t, value: pc + 1 });
    } else {
      ret.push(t);
    }
  }
  return ret;
};
