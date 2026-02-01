# Multithreading Lab Programs

This folder contains Python implementations of classic multithreading problems for an Operating Systems course.

## Programs Overview

### 6. Statistical Calculator (6_statistics.py)
**Problem**: Calculate average, minimum, and maximum values using separate threads.

**Description**: 
Program nhận một dãy số từ command line và tạo 3 worker threads riêng biệt:
- Thread 1: Tính giá trị trung bình (average)
- Thread 2: Tìm giá trị nhỏ nhất (minimum) 
- Thread 3: Tìm giá trị lớn nhất (maximum)

Các biến `average`, `minimum`, `maximum` được lưu global. Worker threads sẽ set các giá trị này, và parent thread sẽ xuất kết quả sau khi tất cả workers đã kết thúc.

**Usage**:
```bash
python 6_statistics.py 90 81 78 95 79 72 85
```

**Expected Output**:
```
Input numbers: [90, 81, 78, 95, 79, 72, 85]
Calculating statistics using multiple threads...

The average value is 82
The minimum value is 72
The maximum value is 95
```

**Key Functions**:
- `calculate_average(numbers)`: Thread tính trung bình
- `calculate_minimum(numbers)`: Thread tìm giá trị min
- `calculate_maximum(numbers)`: Thread tìm giá trị max

**Concepts**: Thread creation, global variables, thread synchronization with join()

---

### 7. Fibonacci Sequence Generator (7_fibonacci.py)
**Problem**: Generate Fibonacci sequence in a separate thread while parent waits.

**Description**:
Dãy Fibonacci là dãy số: 0, 1, 1, 2, 3, 5, 8, ...
Công thức: fib(0)=0, fib(1)=1, fib(n)=fib(n-1)+fib(n-2)

Program hoạt động như sau:
1. User nhập số lượng Fibonacci numbers cần generate qua command line
2. Program tạo một thread riêng để generate dãy Fibonacci
3. Dãy được lưu trong shared data structure (array/list)
4. Parent thread phải đợi child thread hoàn thành (sử dụng `join()`)
5. Sau khi child thread kết thúc, parent thread mới xuất dãy số

**Usage**:
```bash
python 7_fibonacci.py 10
```

**Expected Output**:
```
Generating first 10 Fibonacci numbers...

Fibonacci sequence (10 numbers):
[0, 1, 1, 2, 3, 5, 8, 13, 21, 34]

Formatted output:
fib(0) = 0
fib(1) = 1
fib(2) = 1
...
fib(9) = 34
```

**Key Functions**:
- `generate_fibonacci(n)`: Thread function sinh dãy Fibonacci với n phần tử
- Sử dụng list `fibonacci_sequence` làm shared data

**Concepts**: Thread synchronization, parent-child thread coordination, shared data structures

---

### 8. Prime Numbers Finder (8_primes.py)
**Problem**: Find all prime numbers ≤ N using T threads (distributed computing).

**Description**:
Program tìm tất cả số nguyên tố nhỏ hơn hoặc bằng số N do user nhập, sử dụng T threads để chia task.

**Extension (Phần mở rộng)**:
- N: Số cần tìm primes (tìm prime < N)
- T: Số lượng threads
- Mỗi thread: Xử lý N/T numbers
- Kết quả: T threads trả về => parent thread gom lại và output

**Cách hoạt động**:
1. Chia range [2, N] thành T phần
2. Mỗi thread kiểm tra một range riêng: Prime(x, y)
3. Mỗi thread lưu kết quả vào local list
4. Sử dụng lock để add kết quả vào global list (thread-safe)
5. Parent thread đợi tất cả threads xong, sort và xuất kết quả

**Usage**:
```bash
python 8_primes.py 100 4
```

**Parameters**:
- N: Upper limit for prime search
- T: Number of threads (default: 4)

**Expected Output**:
```
Finding all prime numbers <= 100 using 4 threads...

Thread 1: Checking range [2, 26]
Thread 2: Checking range [27, 51]
Thread 3: Checking range [52, 76]
Thread 4: Checking range [77, 100]

Found 25 prime numbers:
[2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]

Formatted output (10 per row):
[2, 3, 5, 7, 11, 13, 17, 19, 23, 29]
[31, 37, 41, 43, 47, 53, 59, 61, 67, 71]
[73, 79, 83, 89, 97]
```

**Key Functions**:
- `is_prime(n)`: Kiểm tra một số có phải số nguyên tố không
- `find_primes_in_range(start, end)`: Thread function tìm primes trong range [start, end]
- Sử dụng `threading.Lock()` để đảm bảo thread-safe khi thêm vào global list

**Concepts**: Thread pool, work distribution, thread-safe data structures, locks

---

### 9. Multithreaded TCP Date Server (9_tcp_server_threaded.py)
**Problem**: TCP server that handles each client in a separate thread.

**Description**:
Đây là bài mở rộng của socket-based date server, được modify để server phục vụ mỗi client request trong một thread riêng biệt.

**Cách hoạt động**:
1. Server lắng nghe trên port được chỉ định
2. Mỗi khi có client connect, server:
   - Accept connection
   - Tăng client counter (thread-safe với lock)
   - Tạo một thread mới để handle client đó
   - Thread gửi current date/time cho client
   - Đóng connection sau khi gửi xong
3. Main thread tiếp tục lắng nghe client mới
4. Nhiều clients có thể connect đồng thời

**Usage**:
```bash
# Terminal 1 - Start server
python 9_tcp_server_threaded.py 8080

# Terminal 2, 3, 4... - Connect clients
python 9_tcp_client_test.py 8080
```

**Expected Output (Server)**:
```
========================================
Multithreaded TCP Date Server
========================================
Server listening on 127.0.0.1:8080
Press Ctrl+C to stop the server
========================================

[Main] Created thread 1 for client ('127.0.0.1', 54321)
[Main] Active threads: 2

[Thread 1] Connected to client: ('127.0.0.1', 54321)
[Thread 1] Sent date/time to ('127.0.0.1', 54321)
[Thread 1] Connection closed with ('127.0.0.1', 54321)
```

**Expected Output (Client)**:
```
Connecting to server at 127.0.0.1:8080...
Connected!

Response from server:
========================================
Server Date/Time: 2026-02-01 14:30:45
Client ID: 1
========================================

Connection closed
```

**Key Features**:
- Mỗi client connection được handle bởi dedicated thread
- Server có thể handle nhiều clients đồng thời
- Thread-safe client ID counter (sử dụng lock)
- Daemon threads để tự động cleanup khi program exits
- Proper error handling và connection cleanup

**Key Functions**:
- `handle_client(client_socket, client_address, client_id)`: Thread function xử lý mỗi client
- Sử dụng `threading.Lock()` để protect shared counter
- Thread được set `daemon=True` để không block program exit

**Files**:
- `9_tcp_server_threaded.py`: Multithreaded server
- `9_tcp_client_test.py`: Test client để kết nối và test server

**Concepts**: Network programming, concurrent server design, daemon threads, thread-safe counters

---

## Key Threading Concepts Demonstrated

1. **Thread Creation**: Using `threading.Thread(target=function, args=(...))`
2. **Thread Synchronization**: Using `thread.join()` to wait for completion
3. **Global Variables**: Sharing data between threads
4. **Thread-Safe Operations**: Using locks (`threading.Lock()`)
5. **Daemon Threads**: Background threads that don't block program exit
6. **Work Distribution**: Dividing tasks among multiple threads
7. **Concurrent I/O**: Handling multiple network connections simultaneously

## Testing Tips

### Test Program 6:
```bash
python 6_statistics.py 90 81 78 95 79 72 85
python 6_statistics.py 1 2 3 4 5
python 6_statistics.py 100
```

### Test Program 7:
```bash
python 7_fibonacci.py 5
python 7_fibonacci.py 15
python 7_fibonacci.py 20
```

### Test Program 8:
```bash
# Test with different thread counts
python 8_primes.py 100 1   # Single thread
python 8_primes.py 100 2   # Two threads
python 8_primes.py 100 4   # Four threads
python 8_primes.py 1000 8  # Eight threads, larger range
```

### Test Program 9:
```bash
# In separate terminals:
# Terminal 1:
python 9_tcp_server_threaded.py 8080

# Terminals 2-5 (simultaneously):
python 9_tcp_client_test.py 8080
```

## Requirements

- Python 3.6 or higher
- Standard library only (no external dependencies)

## Notes

- All programs use Python's `threading` module
- Error handling included for invalid inputs
- Thread-safe implementations where needed
- Clear output formatting for easy verification
