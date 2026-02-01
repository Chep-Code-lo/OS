const dgram = require('dgram');
const server = dgram.createSocket('udp4');

function replaceAll(content, s1, s2) {
    return content.split(s1).join(s2);
}

server.on('message', (msg, rinfo) => {
    const request = JSON.parse(msg.toString());
    const { fileContent, s1, s2 } = request;
    
    console.log(`Nhận file (${fileContent.length} ký tự)`);
    console.log(`Thay thế: "${s1}" → "${s2}"`);
    
    const result = replaceAll(fileContent, s1, s2);
    const count = (fileContent.split(s1).length - 1);
    
    console.log(`Đã thay thế ${count} lần\n`);
    
    const response = Buffer.from(JSON.stringify({ result, count }));
    server.send(response, rinfo.port, rinfo.address);
});

server.bind(5002, () => {
    console.log('UDP Server chạy cổng 5002\n');
});
