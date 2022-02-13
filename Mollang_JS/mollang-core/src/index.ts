import compile from "./compile";
import run, { ENV } from "./runtime";
import tokenize from "./tokenizer";
import show from "./result";

/**
 * Mollang 실행
 * @param code - 실행할 Mollang 코드
 * @param options - 옵션
 * @returns [출력, 오류 코드, 파싱 결과]
 */
export default function main(
    code: string,
    options: {
        maxRecursion?: number;
        outputFn?: (msg: string) => void;
        errorFn?: (code: number) => void;
        inputFn: () => string;
    }
) {
    const { maxRecursion = 100000, outputFn, errorFn, inputFn } = options;

    code = code
        .split("\n")
        .map((line: string) => line.trim())
        .join("\n");

    const outputs: string[] = [];
    const errorCodes: number[] = [];

    const data = tokenize(code, (errorCode) => {
        errorFn && errorFn(errorCode);
        errorCodes.push(errorCode);
    });
    const compiled = compile(data, (errorCode) => {
        errorFn && errorFn(errorCode);
        errorCodes.push(errorCode);
    });

    const defaultEnv = new ENV();

    run(
        defaultEnv,
        data,
        compiled,
        maxRecursion,
        (msg) => {
            outputFn && outputFn(msg.toString());
            outputs.push(msg.toString());
        },
        (errorCode) => {
            errorFn && errorFn(errorCode);
            errorCodes.push(errorCode);
        },
        inputFn
    );
    return [outputs, errorCodes, show(data, compiled)];
}
