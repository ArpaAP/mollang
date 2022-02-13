import * as fs from "fs";
import * as readline from "readline-sync";
import { Command } from "commander";
import main from "mollang-core";

const program = new Command();
program
    .name("mollang")
    .argument("<filename>")
    .option("-p, --parse <outfile>", "Write parsed data to file", false)
    .option("-r, --recursion <times>", "Max number of recursions", Number, 100000)
    .action((fn: string, options) => {
        if (fn.endsWith(".molu")) {
            if (fs.existsSync(fn)) {
                fs.readFile(fn, { encoding: "utf8" }, (err, data) => {
                    if (err) {
                        throw err;
                    } else {
                        const [_outputs, _errorCodes, _errorPositions, parsed] = main(data, {
                            maxRecursion: options.recursion,
                            outputFn: (msg) => process.stdout.write(msg),
                            errorFn: (code, position) =>
                                process.stderr.write(
                                    `* ${position}번째 줄에서 오류 코드 ${code} (이)가 발생했습니다.\n`
                                ),
                            inputFn: readline.question,
                        });
                        if (options.parse) {
                            fs.writeFileSync(options.parse, parsed);
                        }
                    }
                });
            } else {
                throw new Error("File not found");
            }
        } else {
            throw new Error("Unknown file type");
        }
    });

program.parse();
