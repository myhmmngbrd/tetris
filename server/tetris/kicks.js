const Offsets = {
    'default': [
        [[0, 0], [0, 0], [0, 0], [0, 0], [0, 0]],
        [[0, 0], [1, 0], [1, 1], [0, -2], [1, -2]],
        [[0, 0], [0, 0], [0, 0], [0, 0], [0, 0]],
        [[0, 0], [-1, 0], [-1, 1], [0, -2], [-1, -2]]
    ],
    'I': [
        [[0, 0], [-1, 0], [2, 0], [-1, 0], [2, 0]],
        [[-1, 0], [0, 0], [0, 0], [0, -1], [0, 2]],
        [[-1, -1], [1, -1], [-2, -1], [1, 0], [-2, 0]],
        [[0, -1], [0, -1], [0, -1], [0, 1], [0, -2]]
    ],
    'O': [
        [[0, 0]],
        [[0, 1]],
        [[-1, 1]],
        [[-1, 0]]
    ],
    /**
     * @param {string} type [I, J, L, O, S, T, Z]
     * @param {number} rotation_pre 0~3
     * @param {number} rotation_next 0~3
     */
    get(type, rotation_pre, rotation_next) {
        let pre, next, result = [];
        if (type != 'I' && type != 'O') type = 'default';
        pre = this[type][rotation_pre];
        next = this[type][rotation_next];
        for (let i = 0; i < pre.length; i++) {
            result[i] = [ pre[i][0] - next[i][0], pre[i][1] - next[i][1] ]
        }
        return result;
    }
}

module.exports = Offsets;