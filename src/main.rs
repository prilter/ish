mod dir;
mod ex;
mod rl;

use rl::{create_editor, ReadlineError};
use colored::Colorize;
use whoami::username;
//use whoami::hostname;

fn main() {
    let mut rl = create_editor().expect("Failed to create editor");
    let mut conversation: String;

    loop {
        conversation = format!("{} {} $ ", username(), /*hostname(),*/ dir::getdir().green().bold());
        match rl.readline(&conversation) {
            Ok(line) => {
                let _ = rl.add_history_entry(&line);

                let inp: Vec<String> = line.split_whitespace()
                    .map(|s: &str| s.to_owned())
                    .collect();

                if inp.len() > 0 {
                    let _ = match inp[0].as_str() {
                        "cd" => Ok(dir::chdir(inp.get(1).map_or(&"~".to_string(), |s| s))),
                        "exit" => break,
                        _ => ex::ex(&inp)
                    };
                }
            }
            Err(ReadlineError::Interrupted) => continue,
            Err(ReadlineError::Eof)         => break,
            Err(err)                        => { eprintln!("Error: {:?}", err); break; }
        }
    }

    let _ = rl.save_history(&rl::get_history_path());
}
