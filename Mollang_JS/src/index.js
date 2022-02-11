import fs from "fs";
import parse from "./parse.js";
import run from "./runtime.js";

async function main(code) {
    code = code
        .split("\n")
        .map((line) => line.trim())
        .join("\n");
    return run(parse(code));
}

const fn = process.argv[process.argv.length - 1];
if (fn) {
    if (fn.endsWith(".molu")) {
        if (fs.existsSync(fn)) {
            fs.readFile(fn, { encoding: "utf8" }, (_err, data) => {
                main(data);
            });
        } else {
            console.error("File not found");
        }
    } else {
        console.error("Unknown file type");
    }
} else {
    console.error("No file specified");
}
