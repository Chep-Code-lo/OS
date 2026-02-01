const dgram = require('dgram');
const readline = require('readline');

const server = dgram.createSocket('udp4');
const clients = new Map(); // {address:port => username}

server.on('message', (msg, rinfo) => {
    const key = `${rinfo.address}:${rinfo.port}`;
    const data = msg.toString();
    
    // Nếu là đăng ký tên
    if (data.startsWith('NAME:')) {
        const username = data.slice(5);
        clients.set(key, username);
        console.log(`[+] ${username} tham gia (${clients.size} online)\n`);
        
        // Thông báo cho tất cả
        broadcast(`[SERVER] ${username} tham gia!`, rinfo);
        return;
    }
    
    // Tin nhắn chat
    const username = clients.get(key) || 'Anonymous';
    const message = `[${username}]: ${data}`;
    console.log(message);
    
    // Broadcast
    broadcast(message, rinfo);
    
    // Xác nhận
    server.send('✓', rinfo.port, rinfo.address);
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
