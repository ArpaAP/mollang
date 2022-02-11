import exitMessage from "./exit.js";

//1: Int Literal
//2: Variable with Initialize
//3: Print
//4: Input
//5: PrintMode :Character
//6: Goto Start
//7: Goto End
//8: Solo Variable Token
//9: If Start
//10: Exit Code

class Parsed {
    constructor(tokens, line_backpoint) {
        this.tokens = tokens;
        this.line_backpoint = line_backpoint;
    }
}

function parseLiteral(script, i) {
    let ends = [];
    let cur = 0;
    for (; i < script.length; i++) {
        if (script[i] == "?") cur++;
        else if (script[i] == "!") cur--;
        else if (script[i] == ".") {
            ends.push(cur);
            cur = 0;
        } else break;
    }
    ends.push(cur);
    let res = ends[0];
    for (let i = 1; i < ends.length; i++) res *= ends[i];
    return [res, i];
}

export default function parse(script) {
    let result = new Parsed([], []);
    let line_number = 0;
    let if_jumppoint = [];
    result.line_backpoint.push(0);

    for (let i = 0; i < script.length; i++) {
        if (script[i] == "?" || script[i] == "!") {
            let ret = parseLiteral(script, i);
            result.tokens.push([1, ret[0], -1]);
            i = ret[1];
            i--;
        } else if (script[i] == ".") {
            exitMessage(1, ".", [line_number, i]);
        } else if (script[i] == "몰" || script[i] == "모") {
            let length = 1;
            while (
                i < script.length &&
                !(script[i] == "몰" || script[i] == "올")
            ) {
                i++;
                length++;
            }
            i++;
            let init = 0;
            if (i < script.length && (script[i] == "?" || script[i] == "!")) {
                let ret = parseLiteral(script, i);
                init = ret[0];
                i = ret[1];
                result.tokens.push([2, length, init]);
            } else {
                result.tokens.push([8, length, -1]);
            }
            i--;
        } else if (script[i] == "루") {
            if (i + 1 < script.length && script[i + 1] == "?") {
                if (result.tokens[-1][0] == 8) {
                    let length = result.tokens[-1][1];
                    result.tokens.pop();
                    result.tokens.push([4, length, -1]);
                } else {
                    exitMessage(3, "루", [line_number, i]);
                }
            } else {
                result.tokens.push([3, -1, -1]);
            }
        } else if (script[i] == "아") {
            result.tokens.push([5, -1, -1]);
        } else if (script[i] == "가") {
            result.tokens.push([6, -1, -1]);
        } else if (script[i] == "은") {
            if (
                i + 2 < script.length &&
                script[i + 1] == "?" &&
                script[i + 2] == "행"
            ) {
                if_jumppoint.push(result.tokens.length);
                result.tokens.push([9, -1, -1]);
            } else {
                exitMessage(2, "?행", [line_number, i]);
            }
            i += 2;
        } else if (script[i] == "털") {
            if (
                i + 2 < script.length &&
                script[i + 1] == "!" &&
                script[i + 2] == "자"
            ) {
                result.tokens[if_jumppoint[0]][1] = result.tokens.length;
                if_jumppoint.pop();
            } else {
                exitMessage(2, "?자", [line_number, i]);
            }
            i += 2;
        } else if (script[i] == "0") {
            if (
                i + 2 < script.length &&
                script[i + 1] == "ㅅ" &&
                script[i + 2] == "0"
            ) {
                let value = 0;
                let mode = 0;
                if (
                    i + 3 < script.length &&
                    (script[i + 2] == "!" || script[i + 3] == "?")
                ) {
                    let ret = parseLiteral(script, i + 3);
                    value = ret[0];
                    i = ret[1] - 1;
                } else if (
                    (i + 3 < script.length && script[i + 3] == "몰") ||
                    script[i + 3] == "모"
                ) {
                    mode = 1;
                    value = 1;
                    i += 3;
                    while (
                        i < script.length &&
                        !(script[i] == "몰" || script[i] == "올")
                    ) {
                        value++;
                        i++;
                    }
                    i--;
                } else {
                    i += 2;
                }
                result.tokens.push([10, mode, value]);
            } else {
                exitMessage(2, "ㅅ0", [line_number, i]);
            }
        } else if (script[i] == "자") {
            if (i + 1 < script.length && script[i + 1] == "!") {
                result.tokens.push([7, -1, -1]);
            } else {
                exitMessage(2, "!", [line_number, i]);
            }
        } else if (script[i] == "\n") {
            line_number++;
            result.line_backpoint.push(result.tokens.length);
        } else if (script[i] == " ") {
            pass;
        } else {
            exitMessage(4, script[i], [line_number, i]);
        }
    }
    return result;
}
