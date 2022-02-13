import { Errors } from "./exit";

let error: (code: number, position: number) => void;

export class KEY {
    static LITERAL = -1;
    static PLUS = -1;
    static MINUS = -2;
    static MULTIPLY = 0;
    static PAIR_KEYWORD = 32767;
}

export class LiteralParsed {
    text: string;
    content: number[] = [];
    constructor(token: string, position: number) {
        this.text = token;
        for (let i = 0; i < token.length; i++) {
            switch (token[i]) {
                case "?":
                    this.content.push(KEY.PLUS);
                    break;
                case "!":
                    this.content.push(KEY.MINUS);
                    break;
                case ".":
                    this.content.push(KEY.MULTIPLY);
                    break;
                case "몰":
                    this.content.push(1);
                case "모":
                    let len = 2;
                    let t = i + 1;
                    while (token[t] === "오") {
                        len++;
                        t++;
                    }
                    if (token[t] !== "올") {
                        error(Errors.WRONG_EXPRESSION, position);
                    }
                    this.content.push(len);
                    i = t;
                    break;
                default:
                    error(Errors.WRONG_EXPRESSION, position);
            }
        }
    }
}

export class Tokenized {
    tokens: number[][] = [];
    tokens_position: number[] = [];
    literals: LiteralParsed[] = [];
    gotoPoint: number[] = [];
}

const literal_char = ["몰", "모", "오", "올", "?", "!", "."];
const keywords = ["루?", "루", "0ㅅ0"];
const pair_keywords = [
    ["아", "루"],
    ["은?행", "털!자"],
    ["가", "자!"],
];

function literalToken(script: string, idx: number) {
    let length = 0;
    for (let i = idx; i < script.length; i++) {
        if (literal_char.includes(script[i])) {
            length++;
        } else {
            break;
        }
    }
    return length;
}

export default function tokenize(
    script: string,
    errorFn: (code: number, position: number) => void
) {
    error = errorFn;
    let ret: Tokenized = new Tokenized();
    ret.gotoPoint.push(0);
    let closePair: [number, string][] = [];
    let line = 0;
    for (let i = 0; i < script.length; i++) {
        let cur = script[i];
        let foundToken: boolean = false;
        if (cur === "\n") {
            ret.gotoPoint.push(ret.tokens.length);
            line++;
            continue;
        }
        if (cur === " ") {
            continue;
        }
        if (literal_char.includes(cur)) {
            let length = literalToken(script, i);
            ret.tokens.push([KEY.LITERAL, ret.literals.length, -1]);
            ret.tokens_position.push(line);
            ret.literals.push(new LiteralParsed(script.slice(i, i + length), line));
            i += length - 1;
            continue;
        }
        if (closePair.length > 0) {
            if (
                script.slice(i, i + closePair[closePair.length - 1][1].length) ===
                closePair[closePair.length - 1][1]
            ) {
                ret.tokens[closePair[closePair.length - 1][0]][1] = ret.tokens.length - 1;
                closePair.pop();
                continue;
            }
        }
        for (let t = 0; t < keywords.length; t++) {
            if (script.slice(i, i + keywords[t].length) === keywords[t]) {
                foundToken = true;
                ret.tokens.push([t, -1, -1]);
                ret.tokens_position.push(line);
                i += keywords[t].length - 1;
                break;
            }
        }
        if (foundToken) {
            continue;
        }
        for (let t = 0; t < pair_keywords.length; t++) {
            if (script.slice(i, i + pair_keywords[t][0].length) == pair_keywords[t][0]) {
                foundToken = true;
                closePair.push([ret.tokens.length, pair_keywords[t][1]]);
                ret.tokens.push([KEY.PAIR_KEYWORD + t, -1, -1]);
                ret.tokens_position.push(line);
                i += pair_keywords[t][0].length - 1;
                break;
            }
        }
        if (foundToken) {
            continue;
        }
        error(Errors.UNKNOWN_TOKEN, line);
    }
    if (closePair.length > 0) {
        error(Errors.MISSING_PAIR, line);
    }

    return ret;
}
