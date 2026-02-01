const net = require('net');

// Xử lý các lệnh
function processCommand(cmd, text, ...params) {
    switch(cmd) {
        case 'UPPER':
            return text.toUpperCase();
        case 'LOWER':
            return text.toLowerCase();
        case 'DELETE':
            // DELETE text start end
            const [start, end] = params.map(Number);
            return text.slice(0, start) + text.slice(end);
        case 'INSERT':
            // INSERT text position substring
            const pos = Number(params[0]);
            const substr = params[1];
            return text.slice(0, pos) + substr + text.slice(pos);
        default:
            return 'Lệnh không hợp lệ';
    }
}

const server = net.createServer((socket) => {
    console.log('Client kết nối!\n');
    
    socket.on('data', (data) => {
        const parts = data.toString().split('|');
        const [cmd, text, ...params] = parts;
        
        console.log(`Lệnh: ${cmd}`);
        console.log(`Text: ${text}`);
        console.log(`Params: ${params.join(', ')}`);
        
        const result = processCommand(cmd, text, ...params);
        console.log(`→ ${result}\n`);
        
        socket.write(result);
    });
});

server.listen(5003, () => {
    console.log('Server chạy cổng 5003');
    console.log('Lệnh: UPPER, LOWER, DELETE, INSERT\n');
});
