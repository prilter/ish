use std::{env, path::Path};

pub fn getdir() -> String {
    return match env::current_dir() {
        Ok(p) => {p.display().to_string()},
        _     => {eprintln!("Cannot take current dir"); String::from("~")}
    }
}

pub fn chdir(new: &String) {
    let home = match env::var("HOME") {
        Ok(p) => p.to_string(),
        _     => {eprintln!("No HOME path"); String::new()}
    };

    let path: String = match new.is_empty() {
        true  => home,
        false => new.replace("~", &home)
    };

    if let Err(e) = env::set_current_dir(Path::new(&path)) {
        eprintln!("{e}");
    }
}
