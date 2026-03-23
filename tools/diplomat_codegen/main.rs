use std::env;
use std::path::PathBuf;

use diplomat_tool::config::Config;
use diplomat_tool::DocsUrlGenerator;

fn main() -> std::io::Result<()> {
    let mut args = env::args().skip(1);
    let target_language = args
        .next()
        .expect("usage: diplomat_codegen <target> <out_dir> [--entry <path>]");
    let out_dir = PathBuf::from(
        args.next()
            .expect("usage: diplomat_codegen <target> <out_dir> [--entry <path>]"),
    );

    let mut entry = PathBuf::from("src/lib.rs");
    let mut config = Config::default();
    let mut cli_config = Vec::new();
    while let Some(arg) = args.next() {
        match arg.as_str() {
            "--entry" => {
                entry = PathBuf::from(
                    args.next()
                        .expect("--entry requires a path argument"),
                );
            }
            "--config" => {
                cli_config.push(
                    args.next()
                        .expect("--config requires a key=value argument"),
                );
            }
            _ => panic!("unknown argument: {arg}"),
        }
    }
    config.read_cli_settings(cli_config);

    diplomat_tool::gen(
        &entry,
        &target_language,
        &out_dir,
        &DocsUrlGenerator::with_base_urls(None, Default::default()),
        config,
        false,
    )
}
