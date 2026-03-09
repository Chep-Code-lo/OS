const dgram = require('dgram');
const readline = require('readline');

const server = dgram.createSocket('udp4');
const clients = new Map(); 

server.on('message', (msg, rinfo) => {
    const key = `${rinfo.address}:${rinfo.port}`;
    const data = msg.toString();
    if (data.startsWith('NAME:')) {
        const username = data.slice(5);
        clients.set(key, username);
        console.log(`[+] ${username} tham gia (${clients.size} online)\n`);
        broadcast(`[SERVER] ${username} tham gia!`, rinfo);
        return;
    }

    const username = clients.get(key) || 'Anonymous';
    const message = `[${username}]: ${data}`;
    console.log(message);

    broadcast(message, rinfo);
    
    server.send('ok', rinfo.port, rinfo.address);
});

function broadcast(message, exclude) {
    const msg = Buffer.from(message);
    clients.forEach((username, key) => {
        const [addr, port] = key.split(':');
        if (key !== `${exclude.address}:${exclude.port}`) {
            server.send(msg, parseInt(port), addr);
        }
    });
}

server.bind(5004, () => {
    console.log('='.repeat(40));
    console.log('  CHAT ROOM SERVER - UDP');
    console.log('  Port: 5004');
    console.log('='.repeat(40) + '\n');
});
