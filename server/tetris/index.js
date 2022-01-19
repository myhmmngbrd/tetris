const Control = require('./control');

class Game {
    id = 1;
    user1 = null;
    user2 = null;
    connect(ws) {
        const user = new User;
        user.id = this.id++;
        ws.send(JSON.stringify({id: user.id}));
        if (this.user1) {
            this.user2 = {
                ws,
                user,
            }
            this.user1.ws.send(JSON.stringify({id: user.id}));
            ws.send(JSON.stringify({id: this.user1.user.id}));
            // 게임 시작
            this.startGame();
        } else {
            this.user1 = {
                ws,
                user,
            }
            // 대기
        }
        // TODO user2도 있으면 연결 거부
        // FIXME 게임 종료 후에도 result를 전송하려다가 오류 뜸 게임 종료하면 리턴
    }
    unconnect() {

    }
    startGame() {
        let { user1, user2 } = this;

        let result = user1.user.init();
        result.id = user1.user.id;
        user1.ws.send(JSON.stringify(result));
        user2.ws.send(JSON.stringify(result));
        
        result = user2.user.init();
        result.id = user2.user.id;
        user1.ws.send(JSON.stringify(result));
        user2.ws.send(JSON.stringify(result));

        
        user1.ws.on('message', data => {
            const code =  data.toString();
            const result = user1.user.key(code);
            result.id = user1.user.id;
            if (result) {
                //booadcast
                user1.ws.send(JSON.stringify(result));
                user2.ws.send(JSON.stringify(result));
            }
        });
        user2.ws.on('message', data => {
            const code =  data.toString();
            const result = user2.user.key(code);
            result.id = user2.user.id;
            if (result) {
                //booadcast
                user1.ws.send(JSON.stringify(result));
                user2.ws.send(JSON.stringify(result));
            }
        });
    }
}

class User {
    static ID = 0;

    ws = null; // FIXME 지금은 참조밖에 안됨. 세션 만들고 재접속시 변경하는 알고리즘 넣어야 함.
    controller = null;
    other = null;
    _id = null;
    _state = 0;
    
    constructor(ws) {
        this._id = User.ID++;
        this.ws = ws;

        this.keyMessage = this.keyMessage.bind(this);
        this.readyMessage = this.readyMessage.bind(this);
        this._closed = this._closed.bind(this);
        this.drop = this.drop.bind(this);

        this._wait();
    }

    get id() { return this._id; }

    // state setter funcs
    /**
     * 상태 0. closed : 세션 연결 종료, 모든 리스너와 타이머가 종료되고 상대 세션이 종료되기 전이라면 이를 알려야 함
     * 상태 1. wait : 세션이 연결되어 있으나 상대가 없는 상태. 
     * 상태 2. connected : 상대방을 찾아서 연결되었으나 게임 시작을 대기중인 상태 ready 버튼을 활성화하는 메세지를 보내야 함
     * 상태 3. ready : 게임을 시작할 준비가 된 상태. 상대방도 ready일 시 play 상태로
     * 상태 4. play : 게임 중인 상태
     * 상태 5. gameover : 게임오버 이후 상대를 기다리는 상태
     */
    static CLOSED = 0;
    static WAIT = 1;
    static CONNECTED = 2;
    static READY = 3;
    static PLAY = 4;
    static GAMEOVER = 5;
    get state() { return this._state; }
    set state(val) { this._state = val; }
    send(result) {
        result.id = this._id;
        result.state = this._state;
        this.ws.send(JSON.stringify(result));
        if (this.other) this.other.ws.send(JSON.stringify(result));
    }
    match(user) {
        if (this.state != User.WAIT) return console.error('플레이어가 준비 상태가 아닙니다!!');
        if (user.state != User.WAIT) return console.error('상대방이 준비 상태가 아닙니다!!');

        this._connected(user);
        this.other._connected(this);
    }
    _closed() {
        this.state = User.CLOSED;
        this.other.ws.send(JSON.stringify({ id: this._id, state: 0 })); // 상대한테만 보내야 하므로 정적으로 구현
        this.other._wait();
        this.other = null;
        clearInterval(this.dropTimer);
        delete this.ws;
        delete this.controller;
    }
    _wait() {
        this.state = User.WAIT;
        this.other = null;
        this.controller = null;
        this.ws.off('message', this.keyMessage);
        this.ws.off('close', this._closed); // 이 상태에선 굳이
        clearInterval(this.dropTimer);
        this.send({});
    }
    _connected(other) {
        this.state = User.CONNECTED;
        this.send({ other: other.id });
        this.other = other; // other을 먼저 설정하고 send하면 메세지가 쓸데없이 둘 다한테 감
        this.ws.once('message', this.readyMessage);
        // TODO 종료 리스너도 여기에
        this.ws.on('close', this._closed);
    }
    readyMessage(data) {
        data = data.toString();
        if (data == User.READY) {
            this._ready();
        }
        // TODO 모든 메세지 함수 통합?
    }
    _ready() {
        this.state = User.READY;
        if (this.other.state == User.READY) {
            this._play();
            this.other._play();
        }
    }
    _play() {
        this.state = User.PLAY;
        this.controller = new Control;
        const result = this.controller.init();
        result.init = true;
        this.send(result);
        this.ws.on('message', this.keyMessage);
        this.dropTimer = setInterval(this.drop, 500);
    }
    keyMessage(data) {
        const code = data.toString();
        const result = this.controller.key(code);
        if (!result) return;
        this.send(result);
        if (result.clear) {
            const num = this.calDamage(result.clear, result.combo);
            const otherResult = this.other.controller.garbage(num);
            this.other.send(otherResult);
            if (otherResult.gameover) {
                this.other._gameover();
            }
        }
        if (result.gameover) {
            this._gameover();
        }
    }
    calDamage(clear, combo) {
        let result = 0;
        if (clear == 2) result += 1;
        else if (clear == 3) result += 2;
        else if (clear == 4) result += 4;
        if (combo > 2) result += 1;
        else if (combo > 4) result += 2;
        else if (combo > 6) result += 3;
        else if (combo > 8) result += 4;
        else if (combo > 11) result += 5;
        return result;
    }
    drop() {
        const result = this.controller.key('ArrowDown');
        this.send(result);
        if (result.clear) {
            const num = this.calDamage(result.clear, result.combo);
            const otherResult = this.other.controller.garbage(num);
            this.other.send(otherResult);
            if (otherResult.gameover) {
                this.other._gameover();
            }
        }
        if (result.gameover) {
            this._gameover();
        }
    }
    _gameover() {
        this.state = User.GAMEOVER;
        this.ws.off('message', this.keyMessage);
        clearInterval(this.dropTimer);
    }
}

class Match {
    static ID = 0;
    users = [];
    connect(ws) {
        let { users } = this;
        let user = new User(ws);
        users.push(user);
        this.match(user);
        ws.on('close', () => {
            const index = users.findIndex(e => e.id == user.id);
            users.splice(index, 1);
        })
    }
    match(user) {
        let { users } = this;
        for (let other of users) {
            if (other == user) continue;
            if (other.state == User.WAIT) {
                user.match(other);
                break;
            }
        }
    }
}


module.exports = Match;