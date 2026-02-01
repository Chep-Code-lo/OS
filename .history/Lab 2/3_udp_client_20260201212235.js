const dgram = require('dgram');
const readline = require('readline');
const fs = require('fs');

const client = dgram.createSocket('udp4');
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

console.log('UDP Client\n');

rl.question('Đường dẫn file: ', (filePath) => {
    let fileContent;
    try {
        fileContent = fs.readFileSync(filePath, 'utf8');
        console.log(`Đã đọc file (${fileContent.length} ký tự)\n`);
    } catch (err) {
        console.log('Lỗi đọc file!');
        client.close();
        rl.close();
        return;
    }
    
    rl.question('Chuỗi cần thay thế (s1): ', (s1) => {
        rl.question('Chuỗi thay thế (s2): ', (s2) => {
            
            console.log('\n→ Gửi đến server...\n');
            
            const request = JSON.stringify({
                fileContent: fileContent,
                s1: s1,
                s2: s2
            });
            
            const msg = Buffer.from(request);
            client.send(msg, 5002, '127.0.0.1');
        });
    });
});

client.on('message', (msg) => {
    const response = JSON.parse(msg.toString());
    
    console.log(`ok - Đã thay thế ${response.count} lần\n`);
    
    rl.question('Lưu kết quả vào file: ', (outputPath) => {
        fs.writeFileSync(outputPath, response.result, 'utf8');
        console.log(`ok - Đã lưu: ${outputPath}\n`);
        
        client.close();
        rl.close();
    });
});
