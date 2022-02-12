export default function exitMessage(
    code: number,
    data: string,
    pos: [number, number] | []
): [number, string] {
    const posMsg = `[${pos.join(":")}] `;
    switch (code) {
        case 1:
            return [code, posMsg + data + " 가 잘못된 위치에 있습니다.\n"];
        case 2:
            return [code, posMsg + data + " 가 필요합니다\n"];
        case 3:
            return [
                code,
                posMsg +
                    data +
                    " 는 하나 이상의 인자가 필요합니다. (또는 변수가 선언되지 않았습니다.)\n",
            ];
        case 4:
            return [code, posMsg + data + " 인식할 수 없는 토큰입니다.\n"];
        case 5:
            return [
                code,
                posMsg +
                    data +
                    " 반복 가능 횟수를 초과했습니다. (무한 반복 감지)\n",
            ];
        default:
            return [code, posMsg + data + " 알 수 없는 오류가 발생했습니다.\n"];
    }
}
