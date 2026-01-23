use crate::dir;
use std::process::Command;

pub fn run_command(com: &Vec<String>) -> std::io::Result<()> {
    let coms: Vec<Vec<String>> = split_by_seps(&com);

    for c in coms {
        let _ = match c[0].as_str() {
            "cd" => Ok(dir::chdir(c.get(1).map_or(&"~".to_string(), |s| s))),
            _    => ex(c),
        };
    }

    Ok(())

}

fn ex(c: Vec<String>) -> std::io::Result<()> {
    let status = match c.len() {
        1 => Command::new(&c[0]).status()?,
        _ => Command::new(&c[0]).args(&c[1..]).status()?
    };

    if !status.success() {
        eprintln!("{status}");
    }

    Ok(())
}

fn split_by_seps(ss: &Vec<String>) -> Vec<Vec<String>> {
    return merge_backslash(ss).split(|s| s == "&&" || s == ";") // &&, ;
    .map(|slice| slice.to_vec())
        .collect();
}

fn merge_backslash(vec: &Vec<String>) -> Vec<String> {
    vec.into_iter()
        .fold(Vec::new(), |mut acc, s| {
            if let Some(last) = acc.last_mut() {
                if last.ends_with('\\') {
                    last.pop(); /* DELETE '\' */
                    last.push(' ');
                    last.push_str(&s); /* ADD CURRENT ELEMENT */
                    return acc;
                }
            }

            /* DEFAULT */
            acc.push(s.to_string());
            acc
        })
}
