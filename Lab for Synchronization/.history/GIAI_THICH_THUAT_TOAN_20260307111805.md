# Synchronization

---

## 1. BOUNDED BUFFER (Producer – Consumer)

### Ý tưởng triển khai

Bài toán Producer–Consumer (Sản xuất – Tiêu thụ) mô phỏng tình huống:
- **Producer** (nhà sản xuất) liên tục tạo ra dữ liệu và đưa vào một **bộ đệm vòng** (circular buffer) có kích thước giới hạn (`BUF_SIZE = 5`).
- **Consumer** (người tiêu dùng) lấy dữ liệu ra khỏi bộ đệm để xử lý.
- Vấn đề cần giải quyết:
  - Producer không được ghi khi bộ đệm **đầy**.
  - Consumer không được đọc khi bộ đệm **rỗng**.
  - Nhiều thread không được truy cập bộ đệm **cùng lúc** (race condition).

**Giải pháp:** Dùng **2 semaphore đếm** (`empty_slots`, `full_slots`) để kiểm soát luồng dữ liệu và **1 mutex** để bảo vệ vùng tới hạn.

---

### Các hàm thuật toán quan trọng

#### `put(int x)` — Ghi vào bộ đệm
```cpp
static void put(int x) {
    buf[in_idx] = x;
    in_idx = (in_idx + 1) % BUF_SIZE;
}
```
- Ghi giá trị `x` vào vị trí `in` của mảng vòng.
- Tăng `in` theo modulo để quay vòng: khi `in` đạt `BUF_SIZE`, nó trở về 0.
- **Chỉ được gọi khi đã giữ mutex** — không thread-safe nếu gọi một mình.

#### `get_item()` — Đọc từ bộ đệm
```cpp
static int get_item() {
    int x = buf[out_idx];
    out_idx = (out_idx + 1) % BUF_SIZE;
    return x;
}
```
- Đọc giá trị tại vị trí `out` và tăng `out` theo modulo.
- **Chỉ được gọi khi đã giữ mutex**.

#### `producer()` — Luồng sản xuất
```cpp
static void* producer(void*) {
    while (true) {
        sleep(1);
        sem_wait(&empty_slots);      // Chờ có ô trống
        pthread_mutex_lock(&mtx);    // Vào vùng tới hạn
        int x = next_num++;
        put(x);
        std::cout << "P " << pthread_self() << " -> " << x << "\n";
        pthread_mutex_unlock(&mtx);  // Ra vùng tới hạn
        sem_post(&full_slots);       // Báo có ô đầy
    }
}
```
**Trình tự:**
1. `sem_wait(&empty_slots)` — Giảm semaphore "ô trống". Nếu = 0 (bộ đệm đầy) → **chặn** tại đây.
2. `pthread_mutex_lock` — Khóa mutex để độc quyền truy cập bộ đệm.
3. Ghi số và tăng `next_num`.
4. `pthread_mutex_unlock` — Trả khóa.
5. `sem_post(&full_slots)` — Tăng semaphore "ô đầy", thông báo cho consumer.

#### `consumer()` — Luồng tiêu thụ
```cpp
static void* consumer(void*) {
    while (true) {
        sleep(1);
        sem_wait(&full_slots);       // Chờ có ô đầy
        pthread_mutex_lock(&mtx);    // Vào vùng tới hạn
        int x = get_item();
        std::cout << "C " << pthread_self() << " <- " << x << "\n";
        pthread_mutex_unlock(&mtx);  // Ra vùng tới hạn
        sem_post(&empty_slots);      // Báo có ô trống
    }
}
```
**Trình tự đối xứng với producer:**
1. `sem_wait(&full_slots)` — Chờ có dữ liệu.
2. Lấy dữ liệu.
3. `sem_post(&empty_slots)` — Thông báo bộ đệm có chỗ trống.

---

### Giải thích Output

```
P 140234567890 -> 0        ← Producer (thread id) đã đặt số 0 vào buffer
P 140234111222 -> 1        ← Producer khác đặt số 1
C 140235000001 <- 0        ← Consumer lấy số 0 ra
C 140235000002 <- 1        ← Consumer khác lấy số 1 ra
```

| Ký hiệu | Ý nghĩa |
|---------|---------|
| `P <tid> -> <N>` | Producer với thread-id `<tid>` đã **sản xuất** số `<N>` vào buffer |
| `C <tid> <- <N>` | Consumer với thread-id `<tid>` đã **tiêu thụ** số `<N>` từ buffer |

---

## 2. DINING PHILOSOPHERS (Triết gia ăn tối)

### Ý tưởng triển khai

Bài toán 5 triết gia ngồi quanh bàn tròn, mỗi người cần **2 chiếc nĩa** (trái và phải) để ăn. Vấn đề:
- **Deadlock**: Mọi người đều cầm nĩa bên trái và chờ nĩa bên phải → khóa chết.
- **Starvation**: Có người không bao giờ được ăn.

**Giải pháp dùng trong code:** Dùng **Condition Variable** + **Mutex** (thay vì semaphore đơn giản):
- Mỗi triết gia có **1 condition variable** riêng.
- Triết gia chỉ được ăn khi **cả hai hàng xóm đều không ăn**.
- Khi một triết gia xong việc, anh ta **báo hiệu (signal)** cho cả hai hàng xóm để họ kiểm tra lại.

---

### Các hàm thuật toán quan trọng

#### `pickup_forks(int id)` — Cầm nĩa
```cpp
void pickup_forks(int id) {
    int left  = LEFT;   // (id-1+NUMPHIL) % NUMPHIL
    int right = RIGHT;  // (id+1) % NUMPHIL

    pthread_mutex_lock(&lock);
    state[id] = HUNGRY;

    while (state[id] == HUNGRY &&
           (state[left] == EATING || state[right] == EATING)) {
        std::cout << "Philosopher " << id << " is hungry and waiting...\n";
        pthread_cond_wait(&cond[id], &lock);  // Ngủ chờ + nhả lock
    }

    state[id] = EATING;
    std::cout << "Philosopher " << id << " is allowed to eat now\n";
    pthread_mutex_unlock(&lock);
}
```
**Giải thích:**
1. Đặt trạng thái thành `HUNGRY`.
2. Nếu hàng xóm trái **hoặc** phải đang `EATING` → gọi `pthread_cond_wait`:
   - Thread **ngủ** và **nhả mutex** nguyên tử.
   - Khi được đánh thức, **lấy lại mutex** và kiểm tra điều kiện `while` lại (tránh spurious wakeup).
3. Khi thoát vòng `while` → không còn hàng xóm ăn → chuyển sang `EATING`.

> **Vòng `while` thay vì `if`**: Quan trọng! Dùng `while` để kiểm tra lại điều kiện sau khi bị đánh thức, vì có thể bị đánh thức giả (spurious wakeup) hoặc điều kiện thay đổi trước khi giành lại lock.

#### `return_forks(int id)` — Trả nĩa
```cpp
void return_forks(int id) {
    int left  = LEFT;
    int right = RIGHT;

    pthread_mutex_lock(&lock);
    state[id] = THINKING;
    std::cout << "Philosopher " << id << " has put down forks\n";

    pthread_cond_signal(&cond[left]);   // Đánh thức hàng xóm trái
    std::cout << "Philosopher " << id << " signaled philosopher " << left  << "...\n";

    pthread_cond_signal(&cond[right]);  // Đánh thức hàng xóm phải
    std::cout << "Philosopher " << id << " signaled philosopher " << right << "...\n";

    pthread_mutex_unlock(&lock);
}
```
**Giải thích:**
1. Đặt trạng thái về `THINKING` (trả nĩa).
2. `pthread_cond_signal(&cond[left])` — Đánh thức triết gia bên trái (nếu đang chờ) để kiểm tra xem có thể ăn chưa.
3. Tương tự cho triết gia bên phải.
4. Nhả mutex.

#### `philosopher()` — Vòng lặp chính của mỗi triết gia
```cpp
void* philosopher(void* num) {
    int id = *static_cast<int*>(num);
    while (true) {
        think(id);        // Suy nghĩ (sleep ngẫu nhiên 1-3 giây)
        pickup_forks(id); // Cố lấy 2 nĩa (có thể chờ)
        eat(id);          // Ăn (sleep ngẫu nhiên 1-3 giây)
        return_forks(id); // Trả nĩa + báo hiệu hàng xóm
    }
}
```

---

### Giải thích Output

```
Philosopher 0 is thinking for 2 seconds
Philosopher 1 is thinking for 1 seconds
Philosopher 1 reappears from sleep from thinking
Philosopher 1 is allowed to eat now         ← Phil 1 lấy được nĩa, bắt đầu ăn
Philosopher 1 is eating for 3 seconds
Philosopher 2 is hungry and waiting...      ← Phil 2 muốn ăn nhưng Phil 1 (hàng xóm) đang ăn
Philosopher 1 reappears from sleep from eating
Philosopher 1 has put down forks            ← Phil 1 trả nĩa
Philosopher 1 signaled philosopher 0 to see if it can eat
Philosopher 1 signaled philosopher 2 to see if it can eat  ← Phil 2 được đánh thức
Philosopher 2 is allowed to eat now         ← Phil 2 kiểm tra lại → được phép ăn
```

| Thông điệp | Ý nghĩa |
|------------|---------|
| `is thinking for X seconds` | Đang suy nghĩ, ngủ X giây |
| `is hungry and waiting...` | Muốn ăn nhưng bị chặn vì hàng xóm đang ăn |
| `is allowed to eat now` | Điều kiện thoả mãn, bắt đầu ăn |
| `has put down forks` | Ăn xong, trả nĩa |
| `signaled philosopher X` | Báo hiệu để X kiểm tra có thể ăn không |

---

## 3. READERS AND WRITERS (Đọc và Ghi)

### Ý tưởng triển khai

Bài toán: Nhiều thread cùng truy cập **một tài nguyên chung** (database, file...).
- **Nhiều reader** có thể đọc **cùng lúc** (không xung đột).
- **Writer** cần **độc quyền**: không có reader hoặc writer nào khác được truy cập.

**Ưu tiên: Reader-first** (ưu tiên người đọc):
- Nếu đã có ít nhất 1 reader đang đọc, writer phải chờ.
- Reader mới đến vẫn được phục vụ ngay kể cả khi writer đang chờ → writer có thể bị **đói (starvation)**.

**Cấu trúc dữ liệu:** `ReadWriteLock` struct chứa:
- `readLock` (semaphore = 1): bảo vệ biến `readCount`.
- `writeLock` (semaphore = 1): cờ độc quyền ghi.
- `readCount`: số reader đang đọc hiện tại.

---

### Các hàm thuật toán quan trọng

#### `readLockFunc()` — Reader muốn vào đọc
```cpp
void readLockFunc(ReadWriteLock* rw) {
    sem_wait(&rw->readLock);      // Khóa để cập nhật readCount
    rw->readCount++;
    if (rw->readCount == 1)       // Reader ĐẦU TIÊN vào
        sem_wait(&rw->writeLock); // Chặn writer
    std::cout << "Thread " << pthread_self() << " is READING.\n";
    sem_post(&rw->readLock);      // Nhả khóa readCount
}
```
**Giải thích logic quan trọng:**
- `readCount++` trước, sau đó kiểm tra nếu là reader **đầu tiên** → chiếm `writeLock`.
- Các reader tiếp theo không cần chiếm `writeLock` nữa (đã được giữ).
- `readLock` (mutex) được nhả ngay sau khi cập nhật → nhiều reader có thể vào vùng đọc **đồng thời**.

#### `readUnlockFunc()` — Reader rời khỏi
```cpp
void readUnlockFunc(ReadWriteLock* rw) {
    sem_wait(&rw->readLock);      // Khóa để cập nhật readCount
    rw->readCount--;
    if (rw->readCount == 0)       // Reader CUỐI CÙNG ra
        sem_post(&rw->writeLock); // Nhả khóa, cho phép writer
    std::cout << "Thread " << pthread_self() << " is DONE READING.\n";
    sem_post(&rw->readLock);
}
```
**Giải thích:**
- `readCount--`, nếu là reader **cuối cùng** → nhả `writeLock` → writer đang chờ được vào.

#### `writeLockFunc()` — Writer muốn ghi
```cpp
void writeLockFunc(ReadWriteLock* rw) {
    sem_wait(&rw->writeLock);     // Chờ cho đến khi không còn reader/writer
    std::cout << "Thread " << pthread_self() << " is WRITING.\n";
}
```
- Writer chỉ cần chiếm **1 semaphore** (`writeLock`).
- Nếu `writeLock = 0` (reader đang giữ hoặc writer khác giữ) → **bị chặn**.

#### `writeUnlockFunc()` — Writer hoàn thành
```cpp
void writeUnlockFunc(ReadWriteLock* rw) {
    std::cout << "Thread " << pthread_self() << " is DONE WRITING.\n";
    sem_post(&rw->writeLock);     // Nhả khóa
}
```

---

### Sơ đồ hoạt động của ReadWriteLock

```
writeLock:  [1] ─── Reader đầu tiên vào ──► [0] ─── Reader cuối cùng ra ──► [1]
                                                           ▲
                                              Writer chờ ở đây nếu readCount > 0

readCount:   0 ──► 1 ──► 2 ──► ... ──► N ──► N-1 ──► ... ──► 0
```

---

### Giải thích Output

```
Thread 140234001 is READING.         ← Reader bắt đầu đọc
Thread 140234002 is READING.         ← Reader khác cũng đọc đồng thời (hợp lệ)
Thread 140235001 is READING.         ← Thêm một reader nữa
Thread 140234001 is DONE READING.    ← Reader 1 xong
Thread 140234002 is DONE READING.    ← Reader 2 xong
Thread 140235001 is DONE READING.    ← Reader cuối, writeLock được nhả
Thread 140236001 is WRITING.         ← Writer vào (độc quyền)
Thread 140236001 is DONE WRITING.    ← Writer xong, nhả writeLock
Thread 140234003 is READING.         ← Readers tiếp tục
```

| Thông điệp | Ý nghĩa |
|------------|---------|
| `is READING` | Thread (reader) đã lấy quyền đọc, đang trong vùng đọc |
| `is DONE READING` | Thread reader rời vùng đọc |
| `is WRITING` | Thread (writer) đã lấy quyền ghi độc quyền |
| `is DONE WRITING` | Thread writer nhả quyền ghi |

**Quan sát:**
- Nhiều dòng `READING` xuất hiện liên tiếp → chứng tỏ **đọc song song** hoạt động đúng.
- `WRITING` luôn xuất hiện **một mình** (không xen kẽ với READING hay WRITING khác).
- Writer có thể phải chờ lâu nếu liên tục có reader mới → **reader starvation với writer**.

---

## Bảng So Sánh 3 Thuật Toán

| Tiêu chí | Bounded Buffer | Dining Philosophers | Readers & Writers |
|----------|---------------|--------------------|--------------------|
| **Vấn đề giải quyết** | Điều phối sản xuất–tiêu thụ | Tránh deadlock khi chia sẻ tài nguyên | Tối ưu truy cập đọc/ghi |
| **Cơ chế chính** | Semaphore đếm + Mutex | Mutex + Condition Variable | Semaphore nhị phân |
| **Đồng thời** | Nhiều P/C nhưng tuần tự với buffer | Tối đa 2 triết gia không kề nhau | Nhiều reader đồng thời |
| **Nguy cơ** | Buffer đầy/rỗng | Deadlock, Starvation | Writer starvation |
| **Ưu tiên** | FIFO | Không ưu tiên | Reader-first |
