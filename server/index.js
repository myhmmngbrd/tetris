const http = require('http');
const fs = require('fs');
const ws = require('ws');

const Match = require('./tetris');

const home = fs.readFileSync('../client/index.html');

const app = (req, res) => {
    console.log(req.url);
    if (req.url == '/') {
        res.end(home);
    } else {
        fs.readFile('../client' + req.url, (err, data) => {
            res.end(data);
        })
    }
}

const server = http.createServer(app);
server.listen(3000);

const wsServer = new ws.Server({
    server,
})

// const game = new Game;
const match = new Match;

wsServer.on('connection', (ws, req) => {
    const ip = req.socket.remoteAddress;
    match.connect(ws);
});




// const { networkInterfaces } = require('os');

// const nets = networkInterfaces();
// const results = {}; // Or just '{}', an empty object

// for (const name of Object.keys(nets)) {
//     for (const net of nets[name]) {
//         // Skip over non-IPv4 and internal (i.e. 127.0.0.1) addresses
//         if (net.family === 'IPv4' && !net.internal) {
//             if (!results[name]) {
//                 results[name] = [];
//             }
//             results[name].push(net.address);
//         }
//     }
// }

// console.log(results);