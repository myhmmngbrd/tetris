const Minos = {
    'I': [
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0],
        [0, 1, 1, 1, 1],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0]
    ],
    'J': [
        [2, 0, 0],
        [2, 2, 2],
        [0, 0, 0]
    ],
    'L': [
        [0, 0, 3],
        [3, 3, 3],
        [0, 0, 0]
    ],
    'O': [
        [0, 4, 4],
        [0, 4, 4],
        [0, 0, 0]
    ],
    'S': [
        [0, 5, 5],
        [5, 5, 0],
        [0, 0, 0]
    ],
    'T': [
        [0, 6, 0],
        [6, 6, 6],
        [0, 0, 0]
    ],
    'Z': [
        [7, 7, 0],
        [0, 7, 7],
        [0, 0, 0]
    ],
    /**
     * @param {string} type [I, J, L, O, S, T, Z]
     * @param {number} rotation 0~3
     */
    get(type, rotation) {
        let mino = this[type];
        let result = mino;
        while(rotation) {
            result = [];
            for (let i = 0; i < mino.length; i++) {
                result.push([]);
                for (let j = 0; j < mino.length; j++) {
                    result[i][j] = mino[mino.length - 1 - j][i];
                }
            }
            mino = result;
            rotation--;
        }
        return result;
    }
}

module.exports = Minos;