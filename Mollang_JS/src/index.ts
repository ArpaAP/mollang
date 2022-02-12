#!/usr/bin/env node
import * as fs from "fs";
import { Command } from "commander";
import parse from "./parse.js";
import run from "./runtime.js";

async function main(code: string) {
    code = code
        .split("\n")
        .map((line: string) => line.trim())
        .join("\n");
    return run(...parse(code));
}

const program = new Command();
program
    .name("mollang")
    .argument("<filename>")
    .action(async (fn: string) => {
        if (fn.endsWith(".molu")) {
            if (fs.existsSync(fn)) {
                fs.readFile(fn, { encoding: "utf8" }, (err, data) => {
                    if (err) {
                        program.error(err.message);
                    } else {
                        main(data);
                    }
                });
            } else {
                program.error("File not found");
            }
        } else {
            program.error("Unknown file type");
        }
    });

program.parse();
