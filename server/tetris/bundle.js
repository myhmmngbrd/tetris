class Bundle {
    //var
    _types = ['I', 'J', 'L', 'O', 'S', 'T', 'Z'];
    _out = [];
    _pushBundle() {
        let tmp = [0, 1, 2, 3, 4, 5, 6];
        for (let i = 0; i < 7; i++) {
            let index = ~~(Math.random() * (7 - i))
            this._out.push(tmp[index]);
            tmp.splice(index, 1);
        }
    };
    //interface
    get() {
        this._out.shift();
        return this.next(0);
    };
    /** @param {number} i */
    next(i) {
        while (this._out.length <= i) { this._pushBundle(); }
        return this._types[this._out[i]];
    };
}

module.exports = Bundle;