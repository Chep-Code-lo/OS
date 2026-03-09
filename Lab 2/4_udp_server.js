const dgram = require('dgram');
const server = dgram.createSocket('udp4');

function processCommand(cmd, text, ...params) {
    switch(cmd) {
        case 'UPPER':
            return text.toUpperCase();
        case 'LOWER':
            return text.toLowerCase();
        case 'DELETE':
            const [start, end] = params.map(Number);
            return text.slice(0, start) + text.slice(end);
        case 'INSERT':
            const pos = Number(params[0]);
            const substr = params[1];
            return text.slice(0, pos) + substr + text.slice(pos);
        default:
            return 'Lệnh không hợp lệ';
    }
}

server.on('message', (msg, rinfo) => {
    const parts = msg.toString().split('|');
    const [cmd, text, ...params] = parts;
    
    console.log(`Lệnh: ${cmd}, Text: ${text}`);
    
    const result = processCommand(cmd, text, ...params);
    console.log(`→ ${result}\n`);
    
    server.send(result, rinfo.port, rinfo.address);
});

server.bind(5003, () => {
    console.log('UDP Server chạy cổng 5003\n');
});
