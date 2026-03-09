# HƯỚNG DẪN TEST CODE

## YÊU CẦU HỆ THỐNG

### Cài đặt Node.js
```powershell
# Kiểm tra Node.js đã cài chưa
node --version

# Nếu chưa có, tải tại: https://nodejs.org/
```

### Cài đặt Python
```powershell
# Kiểm tra Python đã cài chưa
python --version

# Nếu chưa có, tải tại: https://www.python.org/
```

---

## CÁC BƯỚC TEST CHUNG

### 1. Mở Terminal
- Trong VS Code: `Ctrl + `` hoặc Terminal → New Terminal
- Hoặc dùng PowerShell/CMD riêng

### 2. Di chuyển đến thư mục dự án
```powershell
cd "e:\OS\Lab 2"
```

### 3. Test theo từng bài

---

## BÀI 1: GIẢI PHƯƠNG TRÌNH BẬC 2

### Cách test TCP

**Bước 1: Mở 2 terminal**

**Terminal 1 - Chạy Server:**
```powershell
node 1_tcp_server.js
```
Kết quả mong đợi:
```
Server chạy tại cổng 5000
```

**Terminal 2 - Chạy Client:**
```powershell
node 1_tcp_client.js
```
Kết quả mong đợi:
```
Kết nối thành công!
Nhập 3 số a b c (cách nhau bởi dấu cách)

Nhập a b c:
```

**Bước 2: Test các trường hợp**

Test case 1 - Hai nghiệm phân biệt:
```
Nhập a b c: 1 -5 6
→ x1 = 3.00, x2 = 2.00
```

Test case 2 - Nghiệm kép:
```
Nhập a b c: 1 -4 4
→ Nghiệm kép: x = 2.00
```

Test case 3 - Vô nghiệm:
```
Nhập a b c: 1 2 5
→ Vô nghiệm
```

**Bước 3: Thoát**
- Client: `Ctrl + C`
- Server: `Ctrl + C`

---

### Cách test UDP

**Terminal 1 - Server:**
```powershell
node 1_udp_server.js
```

**Terminal 2 - Client:**
```powershell
node 1_udp_client.js
```

Test tương tự như TCP với các input giống nhau.

---

## BÀI 2: NHÂN HAI MA TRẬN

### Test TCP

**Terminal 1:**
```powershell
node 2_tcp_server.js
```

**Terminal 2:**
```powershell
node 2_tcp_client.js
```

**Test case 1 - Ma trận 2x2:**
```
Nhập số hàng của ma trận A: 2
Nhập số cột của ma trận A (= số hàng B): 2
Nhập số cột của ma trận B: 2

Nhập ma trận A (2x2):
Hàng 1: 1 2
Hàng 2: 3 4

Nhập ma trận B (2x2):
Hàng 1: 5 6
Hàng 2: 7 8

→ Kết quả:
19 22
43 50
```

**Test case 2 - Ma trận 2x3 và 3x2:**
```
Nhập số hàng của ma trận A: 2
Nhập số cột của ma trận A (= số hàng B): 3
Nhập số cột của ma trận B: 2

Nhập ma trận A (2x3):
Hàng 1: 1 2 3
Hàng 2: 4 5 6

Nhập ma trận B (3x2):
Hàng 1: 7 8
Hàng 2: 9 10
Hàng 3: 11 12

→ Kết quả:
58 64
139 154
```

---

## BÀI 3: THAY THẾ CHUỖI TRONG FILE

### Test TCP

**Bước 1: Chuẩn bị file test**
```powershell
# Tạo file test.txt nếu chưa có
"Hello world. This is a test file. Hello again." | Out-File -FilePath test.txt -Encoding utf8
```

**Bước 2: Chạy server và client**

**Terminal 1:**
```powershell
node 3_tcp_server.js
```

**Terminal 2:**
```powershell
node 3_tcp_client.js
```

**Test case:**
```
Nhập tên file: test.txt
Nhập chuỗi cần tìm: Hello
Nhập chuỗi thay thế: Hi

→ Đã thay thế 2 lần
Hi world. This is a test file. Hi again.
```

**Kiểm tra file:**
```powershell
cat test.txt
```

---

## BÀI 4: XỬ LÝ CHUỖI

### Test TCP

**Terminal 1:**
```powershell
node 4_tcp_server.js
```

**Terminal 2:**
```powershell
node 4_tcp_client.js
```

**Test case 1:**
```
Nhập chuỗi: hello world

→ Kết quả:
Chữ hoa: HELLO WORLD
Chữ thường: hello world
Đảo ngược: dlrow olleh
Số từ: 2
```

**Test case 2:**
```
Nhập chuỗi: JavaScript Programming

→ Kết quả:
Chữ hoa: JAVASCRIPT PROGRAMMING
Chữ thường: javascript programming
Đảo ngược: gnimmargorP tpircSavaJ
Số từ: 2
```

---

## BÀI 5: CHAT ROOM

### Test TCP - Chat Đa Client

**Bước 1: Mở 3-4 terminal**

**Terminal 1 - Server:**
```powershell
node 5_tcp_server.js
```
Kết quả:
```
Chat Server chạy cổng 5000
```

**Terminal 2 - Client 1:**
```powershell
node 5_tcp_client.js
```
```
Nhập tên của bạn: Alice
Chào Alice! Gõ tin nhắn để chat.

Alice: Xin chào mọi người!
```

**Terminal 3 - Client 2:**
```powershell
node 5_tcp_client.js
```
```
Nhập tên của bạn: Bob
[SERVER] Alice đã tham gia chat!
Chào Bob! Gõ tin nhắn để chat.

Bob: Chào Alice!
```

**Terminal 4 - Client 3:**
```powershell
node 5_tcp_client.js
```
```
Nhập tên của bạn: Charlie
[SERVER] Alice đã tham gia chat!
[SERVER] Bob đã tham gia chat!
Chào Charlie! Gõ tin nhắn để chat.

Charlie: Hello!
```

**Kết quả khi chat:**
- Mỗi tin nhắn xuất hiện ở tất cả client khác
- Server log hiển thị tất cả hoạt động
- Khi client thoát (Ctrl+C), các client khác nhận thông báo

---

## BÀI 6: STATISTICS (PYTHON)

### Test

```powershell
python 6_statistics.py 90 81 78 95 79 72 85
```

**Kết quả mong đợi:**
```
The average value is: 82.86
The minimum value is: 72
The maximum value is: 95
```

**Test case khác:**
```powershell
python 6_statistics.py 10 20 30 40 50
```

**Kết quả:**
```
The average value is: 30.00
The minimum value is: 10
The maximum value is: 50
```

**Test lỗi - Không có tham số:**
```powershell
python 6_statistics.py
```
```
Usage: python 6_statistics.py <number1> <number2> ... <numberN>
Example: python 6_statistics.py 90 81 78 95 79 72 85
```

---

## BÀI 7: FIBONACCI (PYTHON)

### Test

```powershell
python 7_fibonacci.py 10
```

**Kết quả:**
```
Fibonacci sequence up to 10:
0 1 1 2 3 5 8 13 21 34
```

**Test case khác:**
```powershell
python 7_fibonacci.py 5
```
```
Fibonacci sequence up to 5:
0 1 1 2 3 5
```

```powershell
python 7_fibonacci.py 20
```
```
Fibonacci sequence up to 20:
0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610 987 1597 2584 4181 6765 10946 17711
```

---

## BÀI 8: PRIME NUMBERS (PYTHON)

### Test

```powershell
python 8_primes.py 20
```

**Kết quả:**
```
Prime numbers up to 20:
2 3 5 7 11 13 17 19
```

**Test case khác:**
```powershell
python 8_primes.py 50
```
```
Prime numbers up to 50:
2 3 5 7 11 13 17 19 23 29 31 37 41 43 47
```

```powershell
python 8_primes.py 100
```
```
Prime numbers up to 100:
2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97
```

---

## BÀI 9: TCP SERVER MULTI-THREADED (PYTHON)

### Test - Server Đa Luồng

**Terminal 1 - Server:**
```powershell
python 9_tcp_server_threaded.py
```
hoặc chỉ định port:
```powershell
python 9_tcp_server_threaded.py 8080
```

**Kết quả:**
```
Server listening on 127.0.0.1:8080
Waiting for connections...
```

**Terminal 2 - Client 1:**
```powershell
python 9_tcp_client_test.py
```

**Kết quả:**
```
Connected to server at 127.0.0.1:8080
Server Date/Time: 2026-03-07 10:30:45
Client ID: 1
Connection closed.
```

**Terminal 3 - Client 2 (chạy song song):**
```powershell
python 9_tcp_client_test.py
```

**Kết quả:**
```
Connected to server at 127.0.0.1:8080
Server Date/Time: 2026-03-07 10:30:47
Client ID: 2
Connection closed.
```

**Server sẽ hiển thị:**
```
[Thread 1] Connected to client: ('127.0.0.1', 54321)
[Thread 1] Sent date/time to ('127.0.0.1', 54321)
[Thread 2] Connected to client: ('127.0.0.1', 54322)
[Thread 2] Sent date/time to ('127.0.0.1', 54322)
[Thread 1] Connection closed with ('127.0.0.1', 54321)
[Thread 2] Connection closed with ('127.0.0.1', 54322)
```

---

## TROUBLESHOOTING - XỬ LÝ LỖI THƯỜNG GẶP

### Lỗi 1: Port đã được sử dụng

**Lỗi:**
```
Error: listen EADDRINUSE: address already in use :::5000
```

**Nguyên nhân:** Server đang chạy ở terminal khác

**Giải pháp:**
```powershell
# Tìm process đang dùng port 5000
netstat -ano | findstr :5000

# Kill process (thay PID bằng số thực tế)
taskkill /PID <PID> /F
```

### Lỗi 2: Không kết nối được

**Lỗi:**
```
Error: connect ECONNREFUSED 127.0.0.1:5000
```

**Nguyên nhân:** Server chưa chạy

**Giải pháp:**
- Đảm bảo server đang chạy trước
- Kiểm tra port đúng
- Kiểm tra IP address (127.0.0.1)

### Lỗi 3: Module không tìm thấy

**Lỗi Node.js:**
```
Error: Cannot find module 'net'
```

**Giải pháp:**
- Module 'net', 'dgram', 'readline' là built-in, không cần cài
- Đảm bảo Node.js version >= 12

**Lỗi Python:**
```
ModuleNotFoundError: No module named 'socket'
```

**Giải pháp:**
- Module 'socket', 'threading', 'sys' là built-in
- Đảm bảo Python version >= 3.6

### Lỗi 4: File không tìm thấy (Bài 3)

**Lỗi:**
```
ENOENT: no such file or directory
```

**Giải pháp:**
```powershell
# Tạo file test.txt
echo "Hello world" > test.txt

# Hoặc
New-Item -Path test.txt -ItemType File -Value "Hello world"
```

### Lỗi 5: Encoding tiếng Việt

**Vấn đề:** Hiển thị sai ký tự tiếng Việt

**Giải pháp:**
```powershell
# PowerShell: Set encoding UTF-8
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
chcp 65001
```

---

## TIPS - MẸO VẶT

### 1. Chạy nhiều test nhanh
```powershell
# Tạo alias để chạy nhanh
function tcp1s { node 1_tcp_server.js }
function tcp1c { node 1_tcp_client.js }

# Sử dụng
tcp1s
tcp1c
```

### 2. Xem log chi tiết

Thêm vào code để debug:
```javascript
console.log('DEBUG:', variable);
```

### 3. Auto-restart server

Cài nodemon:
```powershell
npm install -g nodemon

# Dùng thay vì node
nodemon 1_tcp_server.js
```

### 4. Test nhiều client cùng lúc

**Windows Terminal:** Mở split pane (Alt+Shift+D)

**VS Code:** Click icon "+" để tạo terminal mới

### 5. Kiểm tra kết nối mạng

```powershell
# Test localhost
ping 127.0.0.1

# Xem các port đang mở
netstat -an | findstr LISTENING
```

---

## QUY TRÌNH TEST CHUẨN

### Checklist cho mỗi bài:

- [ ] **Bước 1:** Đọc yêu cầu trong README.md
- [ ] **Bước 2:** Chạy server trước
- [ ] **Bước 3:** Đợi server báo "đang chạy"
- [ ] **Bước 4:** Chạy client
- [ ] **Bước 5:** Test ít nhất 3 test case khác nhau
- [ ] **Bước 6:** Test edge case (số âm, số 0, chuỗi rỗng...)
- [ ] **Bước 7:** Kiểm tra server log
- [ ] **Bước 8:** Thoát đúng cách (Ctrl+C)
- [ ] **Bước 9:** Verify không có process nào còn chạy

### Thời gian test dự kiến:

- Bài 1-4: ~5 phút/bài
- Bài 5 (Chat): ~10 phút
- Bài 6-8 (Python): ~3 phút/bài
- Bài 9 (Threading): ~7 phút

**Tổng thời gian:** ~50-60 phút để test toàn bộ

---

## TÀI LIỆU THAM KHẢO

- **Node.js Net module:** https://nodejs.org/api/net.html
- **Node.js Dgram module:** https://nodejs.org/api/dgram.html
- **Python Socket:** https://docs.python.org/3/library/socket.html
- **Python Threading:** https://docs.python.org/3/library/threading.html

---

**Chúc bạn test thành công! 🚀**
