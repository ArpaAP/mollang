import exitMessage from "./exit";
import * as readline from "readline-sync";

export default function run(
    recursionMax: number,
    output: (msg: string) => void,
    tokens: [number, number, number][],
    line_backpoint: number[]
) {
    let runtime = [];
    let vars: number[] = [];
    let recursion: number[] = [];
    let characterPrint = false;

    for (let i = 0; i < tokens.length; i++) {
        let cur = tokens[i];
        let f = cur[0];
        let s = cur[1];
        let t = cur[2];

        if (f === 1) {
            runtime.push(s);
        } else if (f === 2) {
            if (vars[s] !== undefined) {
                vars[s] += t;
            } else {
                vars[s] = t;
            }
        } else if (f === 3) {
            if (!runtime) {
                exitMessage(3, "", []);
            }
            if (characterPrint) {
                output(String.fromCharCode(runtime[runtime.length - 1]));
            } else {
                output(String(runtime[runtime.length - 1]));
            }
            runtime.pop();
            characterPrint = false;
        } else if (f === 4) {
            let tmp = Number(readline.question());
            vars[s] = tmp;
        } else if (f === 5) {
            characterPrint = true;
        } else if (f === 7) {
            if (!runtime) {
                exitMessage(3, "", []);
            }
            if (recursion[runtime[0]] === undefined) {
                recursion[runtime[0]] = 1;
            } else if (recursion[runtime[0]] === recursionMax) {
                exitMessage(5, "", [runtime[0] - 1, 0]);
                return 0;
            } else {
                recursion[runtime[0]] += 1;
            }
            i = line_backpoint[runtime[0] - 1] - 1;
            runtime.pop();
        } else if (f === 8) {
            if (vars[s] !== undefined) {
                runtime.push(vars[s]);
            } else {
                vars[s] = 0;
            }
        } else if (f === 9) {
            if (!runtime) {
                exitMessage(3, "", []);
            }
            if (runtime[0] !== 0) {
                i = s - 1;
            }
            runtime.pop();
        } else if (f === 10) {
            if (s === 0) {
                return t;
            } else {
                return vars[t];
            }
        }
    }
    return 0;
}
