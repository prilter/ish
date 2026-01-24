use std::{env, path::Path};

const NOHOME: &str = "No HOME path";

pub fn getdir() -> String {
    return match env::current_dir() {
        Ok(p) => {p.display().to_string().replace(&env::var("HOME").expect(NOHOME), "~")},
        _     => {eprintln!("Cannot take current dir"); String::from("~")}
    }
}

pub fn chdir(new: &String, lastdir: &mut String) {
    /* GET HOME SYSTEM VARIABLE */
    let home = match env::var("HOME") {
        Ok(p) => p.to_string(),
        _     => {eprintln!("{}", NOHOME); String::new()}
    };

    /* GET CLEAN PATH */
    let path: String = match new.is_empty() {
        true  => home.clone(),
        false => new.replace("-", lastdir).replace("~", &home)
    };

    /* CHANGING DIRECTORY */
    if let Err(e) = env::set_current_dir(Path::new(&path)) {
        eprintln!("{e}");
    }

    /* CHANGE LAST DIRECTORY */
    if new == "-" {
        *lastdir = path.clone();
    }
}
