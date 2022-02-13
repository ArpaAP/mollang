import { Tokenized } from "./tokenizer";
import { Compiled } from "./compile";

export default function show(data: Tokenized, compiled: Compiled) {
    let ret = `${"=".repeat(20)}\nTOKENS\n`;
    for (let i = 0; i < data.tokens.length; i++) {
        ret += `${i}: ${data.tokens[i][0]} ${data.tokens[i][1]}\n`;
    }
    ret += "\nLITERALS\n";
    for (let i = 0; i < data.literals.length; i++) {
        ret += `${i}: ${data.literals[i].text} ${compiled.literal_owned[i]}\n`;
    }
    ret += `${"=".repeat(20)}\n`;

    return ret;
}
