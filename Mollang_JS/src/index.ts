#!/usr/bin/env node
import * as fs from "fs";
import { Command } from "commander";
import parse from "./parse";
import run from "./runtime";

/**
 * Mollang 실행
 * @param code 실행할 Mollang 코드
 * @param parseOut 파싱 결과 출력 파일 (ex: parsed.json)
 * @param recursion GOTO 반복 최대 가능 횟수
 * @param cli 커맨드 라인 여부 (true: 리턴값 없음, false: 리턴값 출력)
 * @returns cli 여부에 따라 출력 문자열 또는 없음
 */
export default function main(
    code: string,
    parseOut: string = "",
    recursion: number = 100000,
    cli: boolean = false
) {
    code = code
        .split("\n")
        .map((line: string) => line.trim())
        .join("\n");

    const parsed = parse(code);
    if (parseOut) {
        fs.writeFile(
            parseOut,
            JSON.stringify(parsed),
            { encoding: "utf8" },
            (err) => {
                if (err) {
                    throw err;
                }
            }
        );
    }
    if (cli) {
        return run(
            recursion,
            (msg) => {
                process.stdout.write(msg.toString());
            },
            ...parsed
        );
    } else {
        const output: string[] = [];
        run(
            recursion,
            (msg) => {
                output.push(msg.toString());
            },
            ...parsed
        );
        return output;
    }
}

if (require.main === module) {
    const program = new Command();
    program
        .name("mollang")
        .argument("<filename>")
        .option("-p, --parse <outfile>", "Write parsed data to file", false)
        .option(
            "-r, --recursion <times>",
            "Max number of recursions",
            Number,
            100000
        )
        .action((fn: string, options) => {
            if (fn.endsWith(".molu")) {
                if (fs.existsSync(fn)) {
                    fs.readFile(fn, { encoding: "utf8" }, (err, data) => {
                        if (err) {
                            throw err;
                        } else {
                            main(data, options.parse, options.recursion, true);
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
}
