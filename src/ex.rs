use std::process::Command;

pub fn ex(com: &Vec<String>) -> std::io::Result<()> {
    let status = match com.len() {
        1 => Command::new(&com[0]).status()?,
        _ => Command::new(&com[0]).args(&com[1..]).status()?
    };

    if !status.success() {
        eprintln!("{status}");
    }
    Ok(())
}
