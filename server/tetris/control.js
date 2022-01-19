
const Board = require('./board');
// FIXME 홀드 할때 타입이 똑같으면 리턴?


class User {
    id = 0;
    combo = 0;
    _saved = 0;
    _gameover = 0;

    constructor() {
        this.board = new Board;
    }
    init() {
        if (this.board.createBlock()) {
            let differ = this.board.stackField();
            let next = this.board.next;
            return { differ, next };
        }
    }
    left() { 
        if (this.board.moveBlock(-1, 0)) {
            let differ = this.board.stackField();
            return { differ };
        }
    }
    right() {
        if (this.board.moveBlock(1, 0)) {
            let differ = this.board.stackField();
            return { differ };
        }
    }
    softDrop() {
        if (this.board.moveBlock(0, 1)) {
            let differ = this.board.stackField();
            return { differ };
        } else {
            let differ = this.board.stackWall();
            if(this.board.createBlock()){
                differ = differ.concat(this.board.stackField());
                let next = this.board.next;
                this._saved = 0;

                if (this.board.clear) {
                    this.combo++;
                } else {
                    this.combo = 0;
                }
                let combo = this.combo;
                let clear = this.board.clear;
                this.board.clear = 0;

                return { differ, next, clear, combo };
            } else {
                this._gameover = 1;
                differ = this.board.gameOver(); 
                return { differ, gameover: 1 };
            }
        }    
    }
    hardDrop() {
        let dy = 0;
        while(this.board.test(0, ++dy)) {}
        this.board.moveBlock(0, --dy);
        let differ = this.board.stackWall();
        if (this.board.createBlock()) {
            differ = differ.concat(this.board.stackField());
            let next = this.board.next;
            this._saved = 0;

            if (this.board.clear) {
                this.combo++;
            } else {
                this.combo = 0;
            }
            let combo = this.combo;
            let clear = this.board.clear;
            this.board.clear = 0;

            return { differ, next, clear, combo };
        } else {
            this._gameover = 1;
            differ = this.board.gameOver();
            return { differ, gameover: 1 };
        }
    }
    rotateCounterClockwise() {
        if (this.board.rotateBlock(-1)) {
            let differ = this.board.stackField();
            return { differ };
        }
    }
    rotateClockwise() {
        if (this.board.rotateBlock(1)) {
            let differ = this.board.stackField();
            return { differ };
        }
    }
    hold() {
        if (this._saved) return;
        if (this.board.holdBlock()) {
            let differ = this.board.stackField();
            let saved = this.board.saved;
            let next = this.board.next;
            this._saved = 1;
            return { differ, saved, next };
        }
    }
    garbage(n = 1) {
        if (this.board.creatGarbageLine(n)) {
            let differ = this.board.stackField();
            return { differ };
        } else {
            let differ = this.board.stackField();
            differ = differ.concat(this.board.gameOver());
            this._gameover = 1;
            return { differ, gameover: 1 };
        }
    }
    /** @param {string} code */
    key(code) {
        let { board } = this;
        let differ, saved, next;
        if (this._gameover) return;
        switch(code) {
            case 'ArrowLeft':
                return this.left();
            case 'ArrowRight':
                return this.right();
            case 'ArrowDown':
                return this.softDrop();
            case 'Space':
                return this.hardDrop();
            case 'KeyZ':
                return this.rotateCounterClockwise();
            case 'KeyX':
                return this.rotateClockwise();
            case 'KeyC':
                return this.hold();
        }
    }
}

module.exports = User;