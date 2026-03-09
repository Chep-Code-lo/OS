const dgram = require('dgram');
const server = dgram.createSocket('udp4');

function multiply(A, B) {
    const result = [];
    for (let i = 0; i < A.length; i++) {
        result[i] = [];
        for (let j = 0; j < B[0].length; j++) {
            result[i][j] = 0;
            for (let k = 0; k < B.length; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

server.on('message', (msg, rinfo) => {
    const { m1, m2 } = JSON.parse(msg.toString());
    
    console.log('Nhận:', m1, 'x', m2);
    
    const result = multiply(m1, m2);
    console.log('→', result, '\n');
    
    const response = Buffer.from(JSON.stringify(result));
    server.send(response, rinfo.port, rinfo.address);
});

server.bind(5001, () => {
    console.log('UDP Server chạy cổng 5001\n');
});
