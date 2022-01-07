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
    _colors: ['cyan', 'orange', 'blue', 'yello', 'lime', 'purple', 'red'],
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
    get color() {
        if (this._out.length) return this._colors[this._out[0]];
    },
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

function getKick(direction, rotation, type) {
    if (type != 'I') {
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
    } else {
        if (direction == 1) {
            if (rotation == 1) return [[0, 0], [-2, 0], [1, 0], [-2, 1], [1, -2]];
            else if (rotation == 2) return [[0, 0], [-1, 0], [2, 0], [-1, -2], [2, 1]];
            else if (rotation == 3) return [[0, 0], [2, 0], [-1, 0], [2, -1], [-1, 2]];
            else if (rotation == 0) return [[0, 0], [1, 0], [-2, 0], [1, 2], [-2, -1]];
        } else if (direction == -1) {
            if (rotation == 0) return [[0, 0], [2, 0], [-1, 0], [2, -1], [-1, 2]];
            else if (rotation == 1) return [[0, 0], [1, 0], [-2, 0], [1, 2], [-2, -1]];
            else if (rotation == 2) return [[0, 0], [-2, 0], [1, 0], [-2, 1], [1, -2]];
            else if (rotation == 3) return [[0, 0], [-1, 0], [2, 0], [-1, -2], [2, 1]];
        }
    }
}

class Block {
    /** x: 0~3 y: 0~3
     * □□□□
     * □□□□
     * □□□□
     * □□□□
     */
    //function
    constructor(block) {
        this.init(block)
    }
    init(block) {
        if (!block) {
            this.type = Bundle.new;
            this.color = Bundle.color;
            this.x = 3;
            this.y = 0;
            this.rotation = 0;
        } else {
            this.type = block.type;
            this.color = block.color;
            this.x = block.x;
            this.y = block.y;
            this.rotation = block.rotation;
        }
    }
    move(dx, dy) {
        this.x += dx;
        this.y += dy;
    }
    rorate(direction) {
        this.rotation += direction;
        if (this.rotation < 0) this.rotation = 3;
        if (this.rotation > 3) this.rotation = 0;
    }
    //interface
    type = null;
    x = 0;
    y = 0;
    rotation = 0;
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
                if (j + block.x < 0 | j + block.x > 9) continue; // 벽
                this.field[i + block.y][j + block.x] += block.field[i][j];
            }
        }
    }
    clear(block) {
        let cleared = 0;
        for (let i = 0; i < 4; i++) {
            if (i + block.y > 21) continue;
            for (let j = 0; j < 10; j++) {
                if (!this.field[i + block.y][j]) break;
                else if (j == 9) {
                    this.field.splice(i + block.y, 1);
                    this.field.unshift(Array(10).fill(0));
                    cleared++;    
                }
            }
        }
        console.log(cleared + '점');
        return cleared;
    }
}

class Game {
    constructor() {
        this.block = new Block;
        this.ill = new Block(this.block);
        this.board = new Board;
        this.hardDrop(this.ill);
        this.render();

        document.addEventListener('keydown', (e) => {
            if (e.code == 'ArrowDown') {
                this.softDrop(this.block, 1);        
                this.render();
            } else if (e.code == 'Space') {
                this.hardDrop(this.block, 1);
                this.render();
            } else if (e.code == 'ArrowLeft') {
                this.move(this.block, -1, 0);
                this.ill.init(this.block);
                this.hardDrop(this.ill);
                this.render();
            } else if (e.code == 'ArrowRight') {
                this.move(this.block, 1, 0);
                this.ill.init(this.block);
                this.hardDrop(this.ill);
                this.render();
            } else if (e.code == 'KeyX') {
                this.rotate(1);
                this.ill.init(this.block);
                this.hardDrop(this.ill);
                this.render();
            } else if (e.code == 'KeyZ') {
                this.rotate(-1);
                this.ill.init(this.block);
                this.hardDrop(this.ill);
                this.render();
            }
        })
    }
    state = null;
    test(block, dx, dy) {
        let { board } = this;
        for (let i = 0; i < 4; i++) {
            if (!(block.field[i].join('')*1)) continue;
            if (i + block.y + dy > 21 | i + block.y + dy < 0) return false; // 바닥/천장
            for (let j = 0; j < 4; j++) {
                if (!block.field[i][j]) continue;
                if (board.field[i + block.y + dy][j + block.x + dx]) return false; // 블럭
                if (j + block.x + dx < 0 | j + block.x + dx > 9) return false; // 벽
            }
        }
        return true;
    }
    hold() {}
    move(block, dx, dy) {
        if (this.test(block, dx, dy)) {
            block.move(dx, dy);
            return true;
        } else return false;
    }
    softDrop(block, stack) {
        if (this.test(block, 0, 1)) {
            block.move(0, 1);
            return true;
        } else {
            if (stack) {
                this.board.stack(block);
                this.board.clear(block);
                block.init();
                this.ill.init(block);
                this.hardDrop(this.ill);
            }
            return false;
        }
    }
    hardDrop(block, stack) {
        while (this.softDrop(block, stack)) {}
    }
    rotate(direction) {
        let { block } = this;
        if (block.type == 'O') return false;
        block.rorate(direction)
        if (this.kick(direction, block.rotation, block.type)) {
            return true;
        } else {
            block.rorate(-1 * direction);
            return false;
        }
    }
    kick(direction, rotation, type) {
        let tests = getKick(direction, rotation, type);
        for (let [x, y] of tests) {
            if (this.test(this.block, x, y)) {
                this.move(this.block, x, y);
                return true;
            }
        }
        return false;
    }
    render() {
        let { board, block, ill } = this;
        let result = '';
        for (let i = 0; i < 22; i++) {
            let row = '│';
            for (let j = 0; j < 10; j++) {
                if (i - block.y < 4 & i - block.y >= 0 && block.field[i - block.y][j - block.x]) row += '■';
                else if (i - ill.y < 4 & i - ill.y >= 0 && ill.field[i - ill.y][j - ill.x]) row += '□';
                else if (board.field[i][j]) row += '▣';
                else row += '　';
            }
            row += '│'
            result += row + '<br>';
        }
        result += '└──────────┘';
        document.body.innerHTML = result;
    }
}

g = new Game;

