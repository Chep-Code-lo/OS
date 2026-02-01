const net = require('net');

// Hàm replace
function replaceAll(content, s1, s2) {
    return content.split(s1).join(s2);
}

const server = net.createServer((socket) => {
    console.log('Client kết nối!\n');
    
    socket.on('data', (data) => {
        const request = JSON.parse(data.toString());
        const { fileContent, s1, s2 } = request;
        
        console.log(`Nhận file (${fileContent.length} ký tự)`);
        console.log(`Thay thế: "${s1}" → "${s2}"`);
        
        // Thay thế
        const result = replaceAll(fileContent, s1, s2);
        const count = (fileContent.split(s1).length - 1);
        
        console.log(`Đã thay thế ${count} lần\n`);
        
        // Gửi lại
        socket.write(JSON.stringify({ result, count }));
    });
});

server.listen(5002, () => {
    console.log('Server chạy tại cổng 5002\n');
});
