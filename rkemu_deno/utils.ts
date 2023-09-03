export const hex = (w: number, n: number) => right(w, n.toString(16).toUpperCase(), "0");

export const right = (n: number, s: string, fill = " ") => {
  const offset = n - s.length;
  return offset >= 0 ? fill.repeat(n - s.length) + s : s;
};

export const left = (n: number, s: string, fill = " ") => {
  const offset = n - s.length;
  return offset >= 0 ? s + fill.repeat(n - s.length) : s;
};
