const Bundle = require('./bundle');
const Kicks = require('./kicks');
const Block = require('./block');

class Board {
    _field = Array.from(Array(22), () => Array(10).fill(0));
    _oldField = Array.from(Array(22), () => Array(10).fill(0));
    _log = [];

    clear = 0;

    get shape() {
        this._shape = Block.get(this.type, this.rotation);
        return this._shape;
    }

    constructor() {
        this.bundle = new Bundle;
    }

    stackField() {
        let { _field, _oldField, _log, shape, x, y, maxY, } = this;

        // let i = 0;
        // for (let row of _field) {
        //     _oldField[i++] = [...row];
        // }

        for (let [row, col] of _log) {
            _field[row][col] = 0;
        }
        while (_log.shift()) { }

        for (let i = 0; i < shape.length; i++) {
            if (i + maxY > 21 | i + maxY < 0) continue;
            for (let j = 0; j < shape.length; j++) {
                if (j + x > 9 | j + x < 0) continue;
                if (shape[i][j]) {
                    _log.push([i + maxY, j + x, -shape[i][j]]);
                    _field[i + maxY][j + x] = -shape[i][j];
                }
            }
        }
        for (let i = 0; i < shape.length; i++) {
            if (i + y > 21 | i + y < 0) continue;
            for(let j = 0; j < shape.length; j++) {
                if (j + x > 9 | j + x < 0) continue;
                if (shape[i][j]) {
                    _log.push([i + y, j + x, shape[i][j]]);
                    _field[i + y][j + x] = shape[i][j];
                }
            }
        }

        
        let differ = [];
        for (let i = 0; i < _oldField.length; i++) {
            for (let j = 0; j < _oldField[0].length; j++) {
                if (_field[i][j] != _oldField[i][j]) {
                    differ.push([i, j, _field[i][j]]);
                    _oldField[i][j] = _field[i][j];
                }
            }
        }

        return differ;
    }

    stackWall() {
        let { _field, _oldField, _log, shape, x, y, } = this;

        // let i = 0;
        // for (let row of _field) {
        //     _oldField[i++] = [...row];
        // }

        for (let [row, col] of _log) {
            _field[row][col] = 0;
        }

        while (_log.shift()) { }
        for (let i = 0; i < shape.length; i++) {
            if (i + y > 21 | i + y < 0) continue;
            for (let j = 0; j < shape.length; j++) {
                if (j + x > 9 | j + x < 0) continue;
                if (shape[i][j]) {
                    _field[i + y][j + x] = shape[i][j];
                }
            }
            for (let j = 0; j < 10; j++) {
                if (!_field[i + y][j]) break;
                if (j == 9) {
                    _field.splice(i + y, 1);
                    _field.unshift(Array(10).fill(0));
                    this.clear++;
                }
            }
        }

        
        let differ = [];
        for (let i = 0; i < _oldField.length; i++) {
            for (let j = 0; j < _oldField[0].length; j++) {
                if (_field[i][j] != _oldField[i][j]) {
                    differ.push([i, j, _field[i][j]]);
                    _oldField[i][j] = _field[i][j];
                }
            }
        }

        return differ;
    }
    
    test(dx, dy) {
        let { x, y, shape, _field, _log } = this;
        for (let i = 0; i < shape.length; i++) {
            if (!(shape[i].join('')*1)) continue; // 행에 값이 있는 경우에만 충돌 테스트
            let row = i + y + dy
            if (row > 21 || row < 0) return false; // 바닥|천장
            for (let j = 0; j < shape.length; j++) {
                if (!shape[i][j]) continue; // 열에 값이 있는 경우에만 충돌 테스트
                let col = j + x + dx;
                if (col < 0 | col > 9) return false; // 벽
                if (_field[row][col] && _log.findIndex(([e1, e2]) => e1 == row & e2 == col) == -1) return false; // 블럭
            }
        }
        return true;
    }

    createBlock() {
        this.type = this.bundle.get();
        this.next = [this.bundle.next(1), this.bundle.next(2), this.bundle.next(3), this.bundle.next(4), this.bundle.next(5)];
        [this.x, this.y] = this.type == 'I' ? [2, -1] : [3, 0];
        this.rotation = 0;
        if (this.test(0, 0)) return true;
        else return false;
    }

    moveBlock(dx, dy) {
        if (this.test(dx, dy)) {
            this.x += dx;
            this.y += dy;
            return true;
        }
        return false;
    }

    rotateBlock(direction) {
        let before = this.rotation;
        this.rotation += direction;
        if (this.rotation < 0) this.rotation = 3;
        if (this.rotation > 3) this.rotation = 0;
        let kicks = Kicks.get(this.type, before, this.rotation)
        for (let [dx, dy] of kicks) {
            if (this.test(dx, dy)) {
                this.moveBlock(dx, dy);
                return true;
            }
        }
        this.rotation = before;
        return false;
    }
    
    holdBlock() {
        if (!this._saved) {
            this._saved = this.type;
            this.createBlock();
        } else {
            [this.type, this._saved] = [this._saved, this.type];
            [this.x, this.y] = this.type == 'I' ? [2, -1] : [3, 0];
            this.rotation = 0;
        }
        return true;
    }

    creatGarbageLine(n = 1) {
        let { _log, _field } = this;
        for (let [row, col] of _log) {
            _field[row][col] = 0;
        }
        while (_log.shift()) { }

        for (let i = 0; i < n; i++) {
            this._field.shift();
            this._field.push(Array(10).fill(8));
            this._field[21][~~(Math.random() * 10)] = 0;
            if (!this.test(0, 0)) {
                if (this.test(0, -1)) {
                    this.moveBlock(0, -1);
                } else {
                    return false;
                }
            }
        }
        return true;
    }

    gameOver() {
        let differ = [];
        for (let i = 0; i < this._field.length; i++) {
            for (let j = 0; j < this._field[0].length; j++) {
                if (this._field[i][j]) {
                    differ.push([i, j, 8]);
                    this._field[i][j] = 8;
                }
            }
        }
        return differ;
    }

    get saved() { return this._saved }
    get maxY() {
        let dy = 0;
        while(this.test(0, ++dy)) { }
        this._maxY = this.y + --dy;
        return this._maxY;
    }

    render() {
        let { _field } = this;
        let result = '';
        for (let row of _field) {
            for (let col of row) {
                result += !col ? '□' : col > 0 ? '■' : '▦';
            }
            result += '\n';
        }
        console.log(result);
    }
}

module.exports = Board;