use clap::Parser;
use color_print::cformat;
use std::fs::File;
use std::io::BufRead;
use std::io::BufReader;

#[derive(Parser)]
#[clap(
    name = "RK16 Assembler",
    author = "kanade-k-1228",
    version = "v1.0.0",
    about = "Assembler for RISC-K 16 ISA"
)]
struct AppArg {
    #[clap()]
    input: Vec<String>,
    #[clap(short = 'o', long = "output", default_value = "out.rk.bin")]
    output: String,
}

#[derive(Debug)]
struct Line {
    file: String,
    line: usize,
    code: String,
}

fn main() {
    let arg: AppArg = AppArg::parse();

    println!("----------------------------------------------------");
    println!("RK16 Assembler by kanade-k-1228");
    println!("1. Read Files: ");

    let lines: Vec<Line> = arg
        .input
        .iter()
        .flat_map(|fpath| {
            println!("  - {}", fpath);
            let file =
                File::open(fpath).expect(&cformat!("<red,bold>File not found</>: {}", fpath));
            let buf = BufReader::new(file);
            buf.lines()
                .enumerate()
                .map(|(idx, line)| {
                    let s = line.unwrap();
                    // println!("{:04} | {}", idx, s);
                    return Line {
                        file: fpath.to_string(),
                        line: idx,
                        code: s,
                    };
                })
                .collect::<Vec<Line>>()
        })
        .collect();

    for line in lines {
        println!("{}:{:04} | {}", line.file, line.line, line.code);
    }

    println!("2. Parse Statements: ");

    println!("3. Generate Binary: ");
}
