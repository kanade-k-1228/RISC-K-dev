// ----------------------------------------------------------------------------
// Registor
// TODO : binary map

#[derive(Debug)]
enum Reg {
    ZERO,
    IRQ,
    PC,
    SP,
    RA,
    FP,
    A0,
    A1,
    T0,
    T1,
    T2,
    T3,
    S0,
    S1,
    S2,
    S3,
}

impl Reg {
    fn str_to_reg(s: &str) -> Option<Reg> {
        match s {
            "zero" => Option::Some(Reg::ZERO),
            "irq" => Option::Some(Reg::IRQ),
            "pc" => Option::Some(Reg::PC),
            "sp" => Option::Some(Reg::SP),
            "ra" => Option::Some(Reg::RA),
            "fp" => Option::Some(Reg::FP),
            "a0" => Option::Some(Reg::A0),
            "a1" => Option::Some(Reg::A1),
            "t0" => Option::Some(Reg::T0),
            "t1" => Option::Some(Reg::T1),
            "t2" => Option::Some(Reg::T2),
            "t3" => Option::Some(Reg::T3),
            "s0" => Option::Some(Reg::S0),
            "s1" => Option::Some(Reg::S1),
            "s2" => Option::Some(Reg::S2),
            "s3" => Option::Some(Reg::S3),
            _ => Option::None,
        }
    }
    fn print(&self) -> String {
        match self {
            Reg::ZERO => "zero",
            Reg::IRQ => "irq",
            Reg::PC => "pc",
            Reg::SP => "sp",
            Reg::RA => "ra",
            Reg::FP => "fp",
            Reg::A0 => "a0",
            Reg::A1 => "a1",
            Reg::T0 => "t0",
            Reg::T1 => "t1",
            Reg::T2 => "t2",
            Reg::T3 => "t3",
            Reg::S0 => "s0",
            Reg::S1 => "s1",
            Reg::S2 => "s2",
            Reg::S3 => "s3",
            _ => "?",
        }
        .to_string()
    }
}

// ----------------------------------------------------------------------------
// Immidiate Value

#[derive(Debug)]
enum Imm {
    Literal(u16),
    UnknownLabel(String),
    OprLabel(String, u16),
    ConstLabel(String, u16),
    AddrLabel(String, u16),
}

impl Imm {
    fn parse_imm(s: &str) -> Option<Imm> {
        Option::None
    }
}

// ----------------------------------------------------------------------------
// Assembly Operations

enum Field {
    Calc(Reg, Reg, Reg, u8),
    CalcI(Reg, Reg, u16, u8),
    Load(),
    Store(),
    Ctrl(),
}

enum Arg {
    Reg,
    Imm,
}

struct Asm {
    name: &'static str,
    args: [Arg; 3],
    bit: Field,
}

macro_rules! asm {
    () => {
        Asm {
            name: "addi",
            args: [Arg::Reg, Arg::Reg, Arg::Reg],
            bit: Field::Calc(Reg::T0, Reg::T0, Reg::T0, 0),
        }
    };
}

const ASMS: [Asm; 3] = [
    Asm {
        name: "add",
        args: [Arg::Reg, Arg::Reg, Arg::Reg],
        bit: Field::Calc(Reg::T0, Reg::T0, Reg::T0, 0),
    },
    Asm {
        name: "sub",
        args: [Arg::Reg, Arg::Reg, Arg::Reg],
        bit: Field::Calc(Reg::T0, Reg::T0, Reg::T0, 0),
    },
    asm!(),
];

// ----------------------------------------------------------------------------
// Assembly line

#[derive(Debug)]
pub struct Line {
    file: String,
    idx: usize,
    line: String,
    code: String,
    comment: String,
    stmt: Option<Stmt>,
}

#[derive(Debug)]
enum Stmt {
    Add(Add),
    Sub(Sub),
    Load(Load),
    Store(Store),
    AddrLabel(AddrLabel),
    CodeLabel(CodeLabel),
    ConstLabel(ConstLabel),
}

#[derive(Debug)]
pub struct Add {
    rd: Reg,
    rs1: Reg,
    rs2: Reg,
}

#[derive(Debug)]
pub struct Sub {
    rd: Reg,
    rs1: Reg,
    rs2: Reg,
}

#[derive(Debug)]
pub struct Load {
    rd: Reg,
    rs1: Reg,
    imm: Imm,
}

#[derive(Debug)]
pub struct Store {
    rd: Reg,
    rs1: Reg,
    imm: Imm,
}

#[derive(Debug)]
pub struct CodeLabel {
    label: String,
    value: Option<u16>,
}

#[derive(Debug)]
pub struct AddrLabel {
    label: String,
    value: u16,
}

#[derive(Debug)]
pub struct ConstLabel {
    label: String,
    value: u16,
}

impl Line {
    pub fn new(file: &str, idx: usize, line: &str) -> Line {
        let (code, comment) = line.split_once(';').unwrap_or((&line, ""));
        return Line {
            file: file.to_string(),
            idx,
            line: line.to_string(),
            code: code.to_string(),
            comment: comment.to_string(),
            stmt: Stmt::new(&code),
        };
    }
}

impl Stmt {
    fn new(code: &str) -> Option<Stmt> {
        let words: Vec<&str> = code.split_whitespace().collect();
        if words.len() == 0 {
            return Option::None;
        }

        // Switch by kind
        let key = words.get(0).unwrap();
        let head = key.chars().nth(0).unwrap();
        let tail = key.chars().last().unwrap();

        if head == '@' {
            let label = words.get(1).unwrap().to_string();
            let value = parse_with_prefix(key.get(1..).unwrap());
            return Option::Some(Stmt::AddrLabel(AddrLabel { label, value }));
        }

        if head == '#' {
            let label = words.get(1).unwrap().to_string();
            let value = parse_with_prefix(key.get(1..).unwrap());
            return Option::Some(Stmt::ConstLabel(ConstLabel { label, value }));
        }

        if tail == ':' {
            let label_c = words.get(0).unwrap().to_string();
            let label = label_c.get(0..label_c.len() - 1).unwrap().to_string();
            let value = Option::None;
            return Option::Some(Stmt::CodeLabel(CodeLabel { label, value }));
        }

        let asm = match *key {
            "add" => Option::Some(Stmt::Add(Add {
                rs1: Reg::str_to_reg(words.get(1).unwrap()).unwrap(),
                rs2: Reg::str_to_reg(words.get(2).unwrap()).unwrap(),
                rd: Reg::str_to_reg(words.get(3).unwrap()).unwrap(),
            })),
            "sub" => Option::Some(Stmt::Sub(Sub {
                rs1: Reg::str_to_reg(words.get(1).unwrap()).unwrap(),
                rs2: Reg::str_to_reg(words.get(2).unwrap()).unwrap(),
                rd: Reg::str_to_reg(words.get(3).unwrap()).unwrap(),
            })),
            _ => Option::None,
        };

        return asm;
    }
}

fn parse_with_prefix(s: &str) -> u16 {
    let radix = match s.chars().nth(1).unwrap().to_ascii_lowercase() {
        'b' => 2,
        'o' => 8,
        'x' => 16,
        _ => 10,
    };
    u16::from_str_radix(s.get(2..).unwrap(), radix).unwrap()
}

// ----------------------------------------------------------------------------
// Print

use std::fmt::format;

use color_print::cformat;

impl Line {
    pub fn cprint(&self) -> String {
        let stmt = match &self.stmt {
            Some(stmt) => match stmt {
                Stmt::AddrLabel(lab) => lab.cprint(),
                Stmt::CodeLabel(lab) => lab.cprint(),
                Stmt::ConstLabel(lab) => lab.cprint(),
                Stmt::Add(add) => add.cprint(),
                Stmt::Sub(sub) => sub.cprint(),
                _ => format!("{:?}", stmt),
            },
            None => format!("{}", self.code),
        };
        format!("{:>4} | {}", self.idx, stmt)
    }
}

impl CodeLabel {
    fn cprint(&self) -> String {
        cformat!("<green>{}:</>", self.label)
    }
}

impl AddrLabel {
    fn cprint(&self) -> String {
        cformat!("<blue>{:04X} = {}</>", self.value, self.label)
    }
}

impl ConstLabel {
    fn cprint(&self) -> String {
        cformat!("<yellow>{:04X} = {}</>", self.value, self.label)
    }
}

impl Add {
    fn cprint(&self) -> String {
        cformat!(
            "    <red>{:<6}</><blue>{:<4}{:<4}{:<4}</>",
            "add",
            self.rs1.print(),
            self.rs2.print(),
            self.rd.print()
        )
    }
}

impl Sub {
    fn cprint(&self) -> String {
        cformat!(
            "    <red>{:<6}</><blue>{:<4}{:<4}{:<4}</>",
            "sub",
            self.rs1.print(),
            self.rs2.print(),
            self.rd.print()
        )
    }
}

impl Load {
    fn cprint(&self) -> String {
        cformat!(
            "    <red>{:<6}</><blue>{:<4}{:<4}{:<4}</>",
            "sub",
            self.rs1.print(),
            self.rs1.print(),
            // self.imm.print(),
            self.rd.print()
        )
    }
}
