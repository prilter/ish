pub use rustyline::error::ReadlineError;
use rustyline::completion::{Completer, FilenameCompleter, Pair};
use rustyline::{Editor, Config, CompletionType, Context};
use rustyline::highlight::Highlighter;
use rustyline::hint::Hinter;
use rustyline::validate::Validator;
use rustyline::Helper;
use rustyline::history::DefaultHistory;

pub struct MyHelper {
    completer: FilenameCompleter,
}

impl Completer for MyHelper {
    type Candidate = Pair;

    fn complete(
        &self,
        line: &str,
        pos: usize,
        ctx: &Context<'_>,
    ) -> rustyline::Result<(usize, Vec<Pair>)> {
        self.completer.complete(line, pos, ctx)
    }
}

impl Hinter for MyHelper {
    type Hint = String;
}

impl Highlighter for MyHelper {}
impl Validator for MyHelper {}
impl Helper for MyHelper {}

pub type ShellEditor = Editor<MyHelper, DefaultHistory>;

pub fn create_editor() -> rustyline::Result<ShellEditor> {
    let config = Config::builder()
        .completion_type(CompletionType::List)
        .build();

    let helper = MyHelper {
        completer: FilenameCompleter::new(),
    };

    let mut rl = Editor::with_config(config)?;
    rl.set_helper(Some(helper));

    /* LOAD HISTORY */
    let _ = rl.load_history(&get_history_path());

    Ok(rl)
}

use std::path::PathBuf;
pub fn get_history_path() -> PathBuf {
    dirs::home_dir()
        .unwrap_or_else(|| PathBuf::from("."))
        .join(".ish_history")
}
