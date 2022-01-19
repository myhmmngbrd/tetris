

class GameScreen {
    colors = ['cyan', 'orange', 'blue', 'yellow', 'lime', 'purple', 'red', 'gray'];
    size = 18;
    space = 2;
    boardRows = 20;
    boardCols = 10;
    sideRows = 3;
    sideCols = 5;
    frameColor = '#000';
    plateColor = '#191919';
    fontSize = 18;
    fontFamily = 'ariel';
    fontColor = '#ddd';
    constructor(root = document.body) {
        this.root = root;
        root.innerHTML = '';
        this.init();
    }
    init() {
        let canvas = this.canvas = document.createElement('canvas');
        let ctx = this.ctx = canvas.getContext('2d');
        this.root.append(canvas);
        
        this.interval = this.size + this.space;
        // canvas.style.imageRendering = 'pixelated';
        ctx.imageSmoothingEnabled = false;
        
        this.x = 0;
        this.y = 0;
        this.h = this.boardRows * this.interval + this.space;
        this.sideWidth = this.sideCols * this.interval + this.space;
        this.boardX = this.x + this.sideWidth;
        this.boardWidth = this.boardCols * this.interval + this.space;
        this.rightSideX = this.boardX + this.boardWidth;

        this.textWidth = this.fontSize * 2.5
        this.textHeight = this.fontSize + 2;
        this.textX = this.sideWidth / 2 - this.textWidth / 2
        this.textY = this.fontSize + 2;
        
        canvas.style.backgroundColor = '#ddd';
        canvas.width = this.x + this.rightSideX + this.sideWidth;
        canvas.height = this.y + this.h;

        this.clear()
    }
    // 내부 함수
    clearField(x = this.boardX, y = this.y, w = this.boardWidth, h = this.h) {
        let { ctx, space, interval, boardCols, boardRows, plateColor, frameColor } = this;
        let [lengthFix, offsetFix, lineFix] = [~~(this.space/2), Math.ceil(this.space / 2), this.space % 2 / 2];
        
        ctx.translate(x, y);

        ctx.fillStyle = plateColor;
        ctx.fillRect(0, 0, w, h);

        ctx.lineWidth = space;
        ctx.strokeStyle = frameColor;
        ctx.beginPath();
        for (let col = 0; col < boardCols + 1; col++) {
            ctx.moveTo(col * interval + offsetFix + lineFix, 0);
            ctx.lineTo(col * interval + offsetFix + lineFix, boardRows * interval + offsetFix + lengthFix);
        }
        for (let row = 0; row < boardRows + 1; row++) {
            ctx.moveTo(0, row * interval + offsetFix + lineFix);
            ctx.lineTo(boardCols * interval + offsetFix + lengthFix, row * interval + offsetFix + lineFix);
        }
        ctx.closePath();
        ctx.stroke();

        ctx.translate(-x, -y);
    }
    clearSide(x, y = this.y, w = this.sideWidth, h = this.h) {
        let { ctx, plateColor } = this;
        ctx.fillStyle = plateColor;
        ctx.fillRect(x, y, w, h);
    }
    renderMino(type, x = 0, y = 0) {
        let { ctx, interval, space, size, sideCols, sideRows } = this;
        switch(type) {
            case 'I':
                x += (sideCols - 4) / 2 * interval;
                y += (sideRows - 1) / 2 * interval;
                break;
            case 'O':
                x += (sideCols - 2) / 2 * interval;
                y += (sideRows - 2) / 2 * interval;
                break;
            default:
                x += (sideCols - 3) / 2 * interval;
                y += (sideRows - 2) / 2 * interval;
                break;
        }
        ctx.translate(x, y);
        switch(type) {
            case 'I':
                ctx.fillStyle = 'cyan';
                ctx.fillRect(space, space, size, size);
                ctx.fillRect(space + interval, space, size, size);
                ctx.fillRect(space + interval * 2, space, size, size);
                ctx.fillRect(space + interval * 3, space, size, size);
                break;
            case 'J':
                ctx.fillStyle = 'orange';
                ctx.fillRect(space, space, size, size)
                ctx.fillRect(space, space + interval, size, size)
                ctx.fillRect(space + interval, space + interval, size, size)
                ctx.fillRect(space + interval * 2, space + interval, size, size)
                break;
            case 'L':
                ctx.fillStyle = 'blue';
                ctx.fillRect(space, space + interval, size, size);
                ctx.fillRect(space + interval, space + interval, size, size);
                ctx.fillRect(space + interval * 2, space + interval, size, size);
                ctx.fillRect(space + interval * 2, space, size, size);
                break;
            case 'O':
                ctx.fillStyle = 'yellow';
                ctx.fillRect(space, space, size, size);
                ctx.fillRect(space + interval, space, size, size);
                ctx.fillRect(space, space + interval, size, size);
                ctx.fillRect(space + interval, space + interval, size, size);
                break;
            case 'S':
                ctx.fillStyle = 'lime';
                ctx.fillRect(space + interval, space, size, size);
                ctx.fillRect(space + interval * 2, space, size, size);
                ctx.fillRect(space, space + interval, size, size);
                ctx.fillRect(space + interval, space + interval, size, size);
                break;
            case 'T':
                ctx.fillStyle = 'purple';
                ctx.fillRect(space + interval, space, size, size);
                ctx.fillRect(space, space + interval, size, size);
                ctx.fillRect(space + interval, space + interval, size, size);
                ctx.fillRect(space + interval * 2, space + interval, size, size);
                break;
            case 'Z':
                ctx.fillStyle = 'red';
                ctx.fillRect(space, space, size, size);
                ctx.fillRect(space + interval, space, size, size);
                ctx.fillRect(space + interval, space + interval, size, size);
                ctx.fillRect(space + interval * 2, space + interval, size, size);
                break;
        }
        ctx.translate(-x, -y);
    }
    // 인터페이스
    renderBlock(difference, x = this.boardX, y = this.y) {
        let { ctx, colors, size, space, interval, plateColor } = this;
        ctx.translate(x, y);

        ctx.lineWidth = 2;
        for (let [i, j, colorIndex] of difference) {
            i = i - 2;
            if (i < 0) continue;
            if (colorIndex == 0) {
                ctx.fillStyle = plateColor;
                ctx.fillRect(space + j * interval, space + i * interval, size, size);
            } else if (colorIndex > 0) {
                ctx.fillStyle = colors[colorIndex - 1];
                ctx.fillRect(space + j * interval, space + i * interval, size, size);
            } else if (colorIndex < 0) {
                ctx.fillStyle = plateColor;
                ctx.fillRect(space + j * interval, space + i * interval, size, size);
                ctx.strokeStyle = colors[-colorIndex - 1];
                ctx.strokeRect(space + 2 + j * interval - 1, space + 2 + i * interval - 1, size-2, size-2);
            }
        }

        ctx.translate(-x, -y);
    }  
    renderHold(type) {
        this.clearSide(this.x, this.y + this.textHeight, this.sideWidth, this.h - this.textHeight);
        this.renderMino(type, this.x, this.y + this.textHeight);
    }
    renderNext(types) {
        this.clearSide(this.rightSideX, this.y + this.textHeight, this.sideWidth, this.h - this.textHeight);
        for (let i = 0; i < types.length & i < 5; i++) {
            this.renderMino(types[i], this.rightSideX, this.y + this.textHeight + i * (this.sideRows * this.interval + this.space));
        }
    }
    clear() {
        this.clearSide(this.x, this.y);
        this.clearField();
        this.clearSide(this.rightSideX, this.y);

        this.ctx.fillStyle = this.fontColor;
        this.ctx.font = `${this.fontSize}px ${this.fontFamily}`;
        this.ctx.fillText('HOLD', this.x + this.textX, this.y + this.textY);
        this.ctx.fillText('NEXT', this.rightSideX + this.textX, this.y + this.textY);
    }
}