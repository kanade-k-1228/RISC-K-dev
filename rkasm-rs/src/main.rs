use clap::Parser;
use color_print::cformat;
use std::fs::File;
use std::io::BufRead;
use std::io::BufReader;

mod parser;

fn main() {
    let args: AppArgs = AppArgs::parse();

    println!("----------------------------------------------------");
    println!("RK16 Assembler by kanade-k-1228");
    println!("1. Parse Files: ");
    let lines: Vec<parser::Line> = args
        .input
        .iter()
        .flat_map(|fpath| {
            println!("  - {}", fpath);
            let file =
                File::open(fpath).expect(&cformat!("<red,bold>File not found</>: {}", fpath));
            let buf = BufReader::new(file);
            buf.lines()
                .enumerate()
                .map(|(idx, line)| parser::Line::new(fpath, idx, &line.unwrap()))
                .collect::<Vec<parser::Line>>()
        })
        .collect();

    for line in &lines {
        println!("{}", line.cprint());
    }

    println!("2. Resolve Label: ");

    println!("3. Generate Binary: ");
}

#[derive(Parser)]
#[clap(
    name = "RK16 Assembler",
    author = "kanade-k-1228",
    version = "v1.0.0",
    about = "Assembler for RISC-K 16 ISA"
)]
struct AppArgs {
    #[clap()]
    input: Vec<String>,
    #[clap(short = 'o', long = "output", default_value = "out.rk.bin")]
    output: String,
}
