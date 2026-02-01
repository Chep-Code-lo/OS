const net = require('net');
const readline = require('readline');

const clients = [];
const server = net.createServer((socket) => {
    
    socket.write('Nhập tên của bạn: ');
    let username = '';
    let isNamed = false;
    
    socket.on('data', (data) => {
        const msg = data.toString().trim();
        
        if (!isNamed) {
            username = msg;
            clients.push({ socket, username });
            console.log(`[+] ${username} tham gia (${clients.length} online)\n`);
            
            broadcast(`[SERVER] ${username} đã tham gia chat!`, socket);
            socket.write(`Chào ${username}! Gõ tin nhắn để chat.\n\n`);
            isNamed = true;
            return;
        }
        
        const message = `[${username}]: ${msg}`;
        console.log(message);
        broadcast(message, socket);
        
        socket.write('ok - Đã gửi\n');
    });
    
    socket.on('close', () => {
        const index = clients.findIndex(c => c.socket === socket);
        if (index !== -1) {
            clients.splice(index, 1);
            console.log(`[-] ${username} rời khỏi (${clients.length} online)\n`);
            broadcast(`[SERVER] ${username} đã rời khỏi!`);
        }
    });
});

function broadcast(message, exclude = null) {
    clients.forEach(client => {
        if (client.socket !== exclude) {
            client.socket.write(message + '\n');
        }
    });
}

server.listen(5004, () => {
    console.log('='.repeat(40));
    console.log('  CHAT ROOM SERVER - TCP');
    console.log('  Port: 5004');
    console.log('='.repeat(40) + '\n');
});
