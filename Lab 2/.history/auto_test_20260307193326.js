// AUTO TEST SCRIPT - Test các bài TCP/UDP tự động
const net = require('net');
const dgram = require('dgram');

// Màu sắc cho console
const colors = {
    reset: '\x1b[0m',
    green: '\x1b[32m',
    red: '\x1b[31m',
    yellow: '\x1b[33m',
    blue: '\x1b[36m'
};

function log(message, color = 'reset') {
    console.log(colors[color] + message + colors.reset);
}

// ========== BÀI 1: GIẢI PHƯƠNG TRÌNH BẬC 2 ==========
function testBai1TCP() {
    return new Promise((resolve) => {
        log('\n╔══════════════════════════════════════╗', 'blue');
        log('║   BÀI 1: TCP - Phương trình bậc 2   ║', 'blue');
        log('╚══════════════════════════════════════╝', 'blue');
        
        const client = new net.Socket();
        const testCases = [
            { input: '1 -5 6', expected: 'x1 = 3.00, x2 = 2.00', desc: 'Hai nghiệm phân biệt' },
            { input: '1 -4 4', expected: 'Nghiệm kép: x = 2.00', desc: 'Nghiệm kép' },
            { input: '1 2 5', expected: 'Vô nghiệm', desc: 'Vô nghiệm' }
        ];
        
        let currentTest = 0;
        let passed = 0;
        
        client.connect(5000, '127.0.0.1', () => {
            log('✓ Kết nối thành công!', 'green');
            sendNextTest();
        });
        
        function sendNextTest() {
            if (currentTest < testCases.length) {
                const test = testCases[currentTest];
                log(`\nTest ${currentTest + 1}: ${test.desc}`, 'yellow');
                log(`  Input: ${test.input}`);
                client.write(test.input);
            } else {
                client.destroy();
                log(`\n✅ Hoàn thành: ${passed}/${testCases.length} test cases passed`, 
                    passed === testCases.length ? 'green' : 'red');
                resolve();
            }
        }
        
        client.on('data', (data) => {
            const result = data.toString().trim();
            const test = testCases[currentTest];
            
            if (result === test.expected) {
                log(`  ✓ PASS: ${result}`, 'green');
                passed++;
            } else {
                log(`  ✗ FAIL: Got "${result}", Expected "${test.expected}"`, 'red');
            }
            
            currentTest++;
            setTimeout(sendNextTest, 100);
        });
        
        client.on('error', (err) => {
            log(`✗ Lỗi: ${err.message}`, 'red');
            resolve();
        });
    });
}

// ========== BÀI 1: UDP ==========
function testBai1UDP() {
    return new Promise((resolve) => {
        log('\n╔══════════════════════════════════════╗', 'blue');
        log('║   BÀI 1: UDP - Phương trình bậc 2   ║', 'blue');
        log('╚══════════════════════════════════════╝', 'blue');
        
        const client = dgram.createSocket('udp4');
        const testCases = [
            { input: '1 -3 2', expected: 'x1 = 2.00, x2 = 1.00', desc: 'Hai nghiệm' },
            { input: '1 0 1', expected: 'Vô nghiệm', desc: 'Vô nghiệm' }
        ];
        
        let currentTest = 0;
        let passed = 0;
        let timeout;
        
        log('✓ Bắt đầu test UDP', 'green');
        
        client.on('message', (msg) => {
            clearTimeout(timeout);
            const result = msg.toString().trim();
            const test = testCases[currentTest];
            
            if (result === test.expected) {
                log(`  ✓ PASS: ${result}`, 'green');
                passed++;
            } else {
                log(`  ✗ FAIL: Got "${result}", Expected "${test.expected}"`, 'red');
            }
            
            currentTest++;
            if (currentTest < testCases.length) {
                setTimeout(sendNextTest, 100);
            } else {
                client.close();
                log(`\n✅ Hoàn thành: ${passed}/${testCases.length} test cases passed`, 
                    passed === testCases.length ? 'green' : 'red');
                resolve();
            }
        });
        
        function sendNextTest() {
            if (currentTest < testCases.length) {
                const test = testCases[currentTest];
                log(`\nTest ${currentTest + 1}: ${test.desc}`, 'yellow');
                log(`  Input: ${test.input}`);
                
                const msg = Buffer.from(test.input);
                client.send(msg, 5000, '127.0.0.1');
                
                timeout = setTimeout(() => {
                    log('  ✗ TIMEOUT', 'red');
                    client.close();
                    resolve();
                }, 2000);
            }
        }
        
        sendNextTest();
    });
}

// ========== BÀI 4: XỬ LÝ CHUỖI ==========
function testBai4TCP() {
    return new Promise((resolve) => {
        log('\n╔══════════════════════════════════════╗', 'blue');
        log('║   BÀI 4: TCP - Xử lý chuỗi          ║', 'blue');
        log('╚══════════════════════════════════════╝', 'blue');
        
        const client = new net.Socket();
        const testCases = [
            { 
                input: 'hello world',
                desc: 'Test chuỗi thường'
            },
            { 
                input: 'JavaScript',
                desc: 'Test một từ'
            }
        ];
        
        let currentTest = 0;
        let passed = 0;
        
        client.connect(5000, '127.0.0.1', () => {
            log('✓ Kết nối thành công!', 'green');
            sendNextTest();
        });
        
        function sendNextTest() {
            if (currentTest < testCases.length) {
                const test = testCases[currentTest];
                log(`\nTest ${currentTest + 1}: ${test.desc}`, 'yellow');
                log(`  Input: "${test.input}"`);
                client.write(test.input);
            } else {
                client.destroy();
                log(`\n✅ Hoàn thành test Bài 4`, 'green');
                resolve();
            }
        }
        
        client.on('data', (data) => {
            const result = data.toString().trim();
            log(`  ✓ Nhận kết quả:`, 'green');
            result.split('\n').forEach(line => {
                log(`    ${line}`, 'green');
            });
            
            currentTest++;
            setTimeout(sendNextTest, 100);
        });
        
        client.on('error', (err) => {
            log(`✗ Lỗi: ${err.message}`, 'red');
            resolve();
        });
    });
}

// ========== MAIN ==========
async function main() {
    console.clear();
    log('╔════════════════════════════════════════════╗', 'blue');
    log('║   AUTO TEST - TCP/UDP LAB 2                ║', 'blue');
    log('╚════════════════════════════════════════════╝', 'blue');
    
    log('\n⚠️  ĐẢM BẢO CÁC SERVER ĐÃ CHẠY:', 'yellow');
    log('   • Bài 1 TCP: node 1_tcp_server.js (port 5000)');
    log('   • Bài 1 UDP: node 1_udp_server.js (port 5000)');
    log('   • Bài 4 TCP: node 4_tcp_server.js (port 5000)\n');
    
    // Đợi 2 giây để user đọc
    await new Promise(resolve => setTimeout(resolve, 2000));
    
    // Chọn test nào chạy
    const testToRun = process.argv[2];
    
    if (!testToRun || testToRun === '1tcp') {
        await testBai1TCP();
    }
    
    if (!testToRun || testToRun === '1udp') {
        await testBai1UDP();
    }
    
    if (!testToRun || testToRun === '4tcp') {
        await testBai4TCP();
    }
    
    log('\n' + '═'.repeat(50), 'blue');
    log('✅ HOÀN TẤT TẤT CẢ TEST', 'green');
    log('═'.repeat(50) + '\n', 'blue');
}

main();
