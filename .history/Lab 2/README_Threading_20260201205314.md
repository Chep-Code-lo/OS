# Multithreading Lab Programs

This folder contains Python implementations of classic multithreading problems for an Operating Systems course.

## Programs Overview

### 6. Statistical Calculator (6_statistics.py)
**Problem**: Calculate average, minimum, and maximum values using separate threads.

**Usage**:
```bash
python 6_statistics.py 90 81 78 95 79 72 85
```

**Expected Output**:
```
The average value is 82
The minimum value is 72
The maximum value is 95
```

**Concepts**: Thread creation, global variables, thread synchronization with join()

---

### 7. Fibonacci Sequence Generator (7_fibonacci.py)
**Problem**: Generate Fibonacci sequence in a separate thread while parent waits.

**Usage**:
```bash
python 7_fibonacci.py 10
```

**Expected Output**:
```
Fibonacci sequence (10 numbers):
[0, 1, 1, 2, 3, 5, 8, 13, 21, 34]
```

**Concepts**: Thread synchronization, parent-child thread coordination, shared data structures

---

### 8. Prime Numbers Finder (8_primes.py)
**Problem**: Find all prime numbers ≤ N using T threads (distributed computing).

**Usage**:
```bash
python 8_primes.py 100 4
```

**Parameters**:
- N: Upper limit for prime search
- T: Number of threads (default: 4)

**Expected Output**:
```
Found 25 prime numbers:
[2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]
```

**Concepts**: Thread pool, work distribution, thread-safe data structures, locks

---

### 9. Multithreaded TCP Date Server (9_tcp_server_threaded.py)
**Problem**: TCP server that handles each client in a separate thread.

**Usage**:
```bash
# Terminal 1 - Start server
python 9_tcp_server_threaded.py 8080

# Terminal 2, 3, 4... - Connect clients
python 9_tcp_client_test.py 8080
```

**Features**:
- Each client connection is handled by a dedicated thread
- Server can handle multiple clients simultaneously
- Thread-safe client ID counter
- Daemon threads for automatic cleanup

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
