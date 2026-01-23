use std::process::Command;

pub fn ex(com: &Vec<String>) -> std::io::Result<()> {
    let coms: Vec<Vec<String>> = split_by_seps(&com);

    for c in coms {
        let status = match c.len() {
            1 => Command::new(&c[0]).status()?,
            _ => Command::new(&c[0]).args(&c[1..]).status()?
        };

        if !status.success() {
            eprintln!("{status}");
        }
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
