const net = require('net');
const readline = require('readline');
const fs = require('fs');

const client = new net.Socket();
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

client.connect(5002, '127.0.0.1', () => {
    console.log('✓ Kết nối OK\n');
    
    rl.question('Đường dẫn file: ', (filePath) => {
        // Đọc file
        let fileContent;
        try {
            fileContent = fs.readFileSync(filePath, 'utf8');
            console.log(`Đã đọc file (${fileContent.length} ký tự)\n`);
        } catch (err) {
            console.log('Lỗi đọc file!');
            client.destroy();
            rl.close();
            return;
        }
        
        rl.question('Chuỗi cần thay thế (s1): ', (s1) => {
            rl.question('Chuỗi thay thế (s2): ', (s2) => {
                
                console.log('\n→ Gửi đến server...\n');
                
                // Gửi file + 2 chuỗi
                const request = JSON.stringify({
                    fileContent: fileContent,
                    s1: s1,
                    s2: s2
                });
                
                client.write(request);
            });
        });
    });
});

client.on('data', (data) => {
    const response = JSON.parse(data.toString());
    
    console.log(`✓ Đã thay thế ${response.count} lần\n`);
    
    rl.question('Lưu kết quả vào file (đường dẫn): ', (outputPath) => {
        fs.writeFileSync(outputPath, response.result, 'utf8');
        console.log(`✓ Đã lưu: ${outputPath}\n`);
        
        rl.question('Tiếp tục? (y/n): ', (answer) => {
            if (answer === 'y') {
                console.log('Khởi động lại...\n');
                client.destroy();
                rl.close();
            } else {
                client.destroy();
                rl.close();
            }
        });
    });
});

client.on('close', () => {
    console.log('Đã đóng');
    rl.close();
});
