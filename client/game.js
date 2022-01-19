
class GameClient {
    static CLOSED = 0;
    static WAIT = 1;
    static CONNECTED = 2;
    static READY = 3;
    static PLAY = 4;
    static GAMEOVER = 5;
    player = null;
    other = null;
    constructor(playerRoot, otherRoot, interfaceRoot) {
        this.sendKey = this.sendKey.bind(this);

        let ws = this.ws = new WebSocket('ws://192.168.0.4:3000');

        this.playerRoot = playerRoot;
        this.otherRoot = otherRoot;
        this.interfaceRoot = interfaceRoot;

        this.readybtn = document.createElement('button');
        this.readybtn.textContent = 'READY';
        this.readybtn.disabled = true;
        this.readybtn.onclick = () => {
            this.ws.send(GameClient.READY);
            this.readybtn.disabled = true;
            // TODO 준비 상태를 취소하는 기능도 만들까?
        }
        this.interfaceRoot.append(this.readybtn);


        ws.addEventListener('open', () => { console.log('서버에 연결 성공...'); });
        ws.addEventListener('message', this.receive);

        // window.addEventListener('keydown', (e) => {
        //     const code = e.code;
        //     ws.send(code);
        // });
        // window.addEventListener('keyup', (e) => {
        // });
    }
    receive = (e) => {
        const data = JSON.parse(e.data);
        console.log(data);
        switch(data.state) {
            case GameClient.CLOSED:
                //TODO 레디 버튼 제거
                window.removeEventListener('keydown', this.sendKey);
                this.readybtn.disabled = true;
                this.other.root.innerHTML = '대기중';
                this.other = null;
                break;
            case GameClient.WAIT:
                if (this.player == null) {
                    let window = new GameScreen(this.playerRoot);
                    this.player = {
                        id: data.id,
                        root: this.playerRoot,
                        window,
                    }
                }
                break;
            case GameClient.CONNECTED: // 두번 옴
                this.player.window.clear();
                let window = new GameScreen(this.otherRoot);
                this.other = {
                    id: data.other,
                    root: this.otherRoot,
                    window,
                }
                this.readybtn.disabled = false;
                break;
            case GameClient.PLAY:
                let user = data.id == this.player.id ? this.player : this.other;
                if (user == this.player & data.init) {
                    // TODO readybtn 삭제
                    addEventListener('keydown', this.sendKey);
                }
                if (data.gameover) {
                    // TODO 리스너 제거
                }
                if ('differ' in data) {
                    user.window.renderBlock(data.differ);
                }
                if ('next' in data) {
                    user.window.renderNext(data.next);
                }
                if ('saved' in data) {
                    user.window.renderHold(data.saved);
                }
                break;
        }
    }
    sendKey(e) {
        this.ws.send(e.code);
    }
}
new GameClient(document.querySelector('#player'), document.querySelector('#other'), document.querySelector('#interface'));