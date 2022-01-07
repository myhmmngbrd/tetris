const Minos = {
    'I': [
        [
            [0, 0, 0, 0],
            [1, 1, 1, 1],
            [0, 0, 0, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 0, 1, 0],
            [0, 0, 1, 0],
            [0, 0, 1, 0],
            [0, 0, 1, 0]
        ],
        [
            [0, 0, 0, 0],
            [0, 0, 0, 0],
            [1, 1, 1, 1],
            [0, 0, 0, 0]
        ],
        [
            [0, 1, 0, 0],
            [0, 1, 0, 0],
            [0, 1, 0, 0],
            [0, 1, 0, 0]
        ]
    ],
    'J': [
        [
            [1, 0, 0, 0],
            [1, 1, 1, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 1, 1, 0],
            [0, 1, 0, 0],
            [0, 1, 0, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 0, 0, 0],
            [1, 1, 1, 0],
            [0, 0, 1, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 1, 0, 0],
            [0, 1, 0, 0],
            [1, 1, 0, 0],
            [0, 0, 0, 0],
        ],
    ],
    'L': [
        [
            [0, 0, 1, 0],
            [1, 1, 1, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 1, 0, 0],
            [0, 1, 0, 0],
            [0, 1, 1, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 0, 0, 0],
            [1, 1, 1, 0],
            [1, 0, 0, 0],
            [0, 0, 0, 0]
        ],
        [
            [1, 1, 0, 0],
            [0, 1, 0, 0],
            [0, 1, 0, 0],
            [0, 0, 0, 0]
        ]
    ],
    'O': [
        [
            [0, 1, 1, 0],
            [0, 1, 1, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0]
        ]
    ],
    'S': [
        [
            [0, 1, 1, 0],
            [1, 1, 0, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 1, 0, 0],
            [0, 1, 1, 0],
            [0, 0, 1, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 0, 0, 0],
            [0, 1, 1, 0],
            [1, 1, 0, 0],
            [0, 0, 0, 0]
        ],
        [
            [1, 0, 0, 0],
            [1, 1, 0, 0],
            [0, 1, 0, 0],
            [0, 0, 0, 0]
        ]
    ],
    'T': [
        [
            [0, 1, 0, 0],
            [1, 1, 1, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 1, 0, 0],
            [0, 1, 1, 0],
            [0, 1, 0, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 0, 0, 0],
            [1, 1, 1, 0],
            [0, 1, 0, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 1, 0, 0],
            [1, 1, 0, 0],
            [0, 1, 0, 0],
            [0, 0, 0, 0]
        ]
    ],
    'Z': [
        [
            [1, 1, 0, 0],
            [0, 1, 1, 0],
            [0, 0, 0, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 0, 1, 0],
            [0, 1, 1, 0],
            [0, 1, 0, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 0, 0, 0],
            [1, 1, 0, 0],
            [0, 1, 1, 0],
            [0, 0, 0, 0]
        ],
        [
            [0, 1, 0, 0],
            [1, 1, 0, 0],
            [1, 0, 0, 0],
            [0, 0, 0, 0]
        ]
    ]
}

const Bundle = {
    //var
    _types: ['I', 'J', 'L', 'O', 'S', 'T', 'Z'],
    _out: [],
    _pushBundle: function() {
        let tmp = [0, 1, 2, 3, 4, 5, 6];
        for (let i = 0; i < 7; i++) {
            let index = Math.floor(Math.random() * (7 - i))
            this._out.push(tmp[index]);
            tmp.splice(index, 1);
        }
    },
    //interface
    get new() {
        this._out.shift();
        return this.next(0);
    },
    get current() {
        return this.next(0);
    },
    next: function(i) {
        while (this._out.length <= i) { this._pushBundle(); }
        return this._types[this._out[i]];
    },
}

function getKick(direction, rotation) {
    if (direction == 1) {
        if (rotation == 1) return [[0, 0], [-1, 0], [-1, -1], [0, 2], [-1, 2]];
        else if (rotation == 2) return [[0, 0], [1, 0], [1, 1], [0, -2], [1, -2]];
        else if (rotation == 3) return [[0, 0], [1, 0], [1, -1], [0, 2], [1, 2]];
        else if (rotation == 0) return [[0, 0], [-1, 0], [-1, 1], [0, -2], [-1, -2]];
    } else if (direction == -1) {
        if (rotation == 0) return [[0, 0], [1, 0], [1, 1], [0, -2], [1, -2]];
        else if (rotation == 1) return [[0, 0], [-1, 0], [-1, -1], [0, 2], [-1, 2]];
        else if (rotation == 2) return [[0, 0], [-1, 0], [-1, 1], [0, -2], [-1, -2]];
        else if (rotation == 3) return [[0, 0], [1, 0], [1, -1], [0, 2], [1, 2]];
    }
}

class Block {
    /** x: 0~3 y: 0~3
     * □□□□
     * □□□□
     * □□□□
     * □□□□
     */
    type = null;
    x = 0;
    y = 0;
    rotation = 0;
    //function
    constructor(block) {
        this.init(block)
    }
    init(block) {
        if (!block) {
            this.type = Bundle.new;
            this.x = 3;
            this.y = 0;
            this.rotation = 0;
        } else {
            this.type = block.type;
            this.x = block.x;
            this.y = block.y;
            this.rotation = block.rotation;
        }
    }
    //interface
    get field() {
        return Minos[this.type][this.rotation];
    }
}

class Board {
    /** x: 0~9 y:0~21
     * ■□□iiii□□□ (0, 0) i(3, 0)
     * □□□iiii□□□
     * □□□iiii□□□
     * □□□iiii□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     * □□□□□□□□□□
     */
    field = Array.from(Array(22), () => Array(10).fill(0));
    stack(block) {
        for (let i = 0; i < 4; i++) {
            if (i + block.y > 21) continue; // 바닥
            for (let j = 0; j < 4; j++) {
                if (j + block.x < 0 || j + block.x > 9) continue; // 벽
                this.field[i + block.y][j + block.x] += block.field[i][j];
            }
        }
    }
}

class Game {
    constructor() {
        this.block = new Block;
        this.board = new Board;
        this.board.stack(this.block);
        console.log(this.board.field);
    }
    state = null;
    test() {}
    hold() {}
    drop() {}
    hardDrop() {}
    kick() {}
}

g = new Game;