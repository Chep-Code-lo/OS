# CPU Scheduling Simulator

## 1. Process.h — Định nghĩa tiến trình

```cpp
struct Process {
    string id;              // Tên tiến trình: "P1", "P2"...
    int arrive_time = 0;    // Thời điểm đến
    int burst = 0;          // Thời gian CPU cần thực thi
    int priority = 0;       // Độ ưu tiên (số nhỏ = ưu tiên cao hơn)
    int waiting_time = 0;   // Thời gian chờ = completion - arrive - burst
    int completion_time = 0;// Thời điểm hoàn thành
    bool completed = false; // Đã hoàn thành chưa
};
```

**Công thức quan trọng:**
| Chỉ số | Công thức |
|--------|-----------|
| Waiting Time (WT) | `completion_time - arrive_time - burst` |
| Turnaround Time (TAT) | `completion_time - arrive_time` |
| Response Time | Thời điểm được CPU lần đầu − `arrive_time` |

---

## 2. Scheduler.h — Hàm dùng chung

### 2.1 GanttSeg & print_gantt() — Vẽ Gantt Chart

Mỗi đoạn trên Gantt Chart được lưu bằng `GanttSeg`:
```cpp
struct GanttSeg { int idx, start, end; };
// idx  = chỉ số process trong mảng
// start/end = thời điểm bắt đầu/kết thúc đoạn đó
```

`print_gantt()` vẽ biểu đồ từ danh sách các `GanttSeg`:

**Bước 1 — Tính column width (`cw`):**
```
cw = max(độ dài tên process dài nhất, độ dài số thời gian lớn nhất) + 2
cw tối thiểu = 6
```

**Bước 2 — In 3 dòng:**
```
Dòng 1 (đường kẻ trên):   ------ ------ ------ ...
Dòng 2 (tên process):    |  P1  |  P2  |  P4  | ...   (căn giữa trong cw)
Dòng 3 (mốc thời gian):  0      1      5     10  ...   (start của mỗi seg + end cuối)
```

**Ví dụ đầu ra:**
```
 ------ ------ ----------- ------- ---------
|  P1  |  P2  |    P4     |   P1  |   P3    |
 ------ ------ ----------- ------- ---------
0      1      5           10      17        26
```

---

### 2.2 preemptive_sim() — Mô phỏng Preemptive

Hàm **template** dùng chung cho SRT và PPS. Thay vì viết lại vòng lặp tick-by-tick cho mỗi thuật toán, chỉ cần truyền vào một hàm `pick` khác nhau.

```cpp
template <typename Picker>
vector<GanttSeg> preemptive_sim(Processes &p, Picker pick)
//  Picker: lambda nhận (p, remain[], t) → trả về index process nên chạy (-1 nếu idle)
```

**Ý tưởng để viết ra hàm này:**

**Bước 1 — Nhận ra điểm chung giữa SRT và PPS**

SRT và PPS đều làm một việc: *mỗi đơn vị thời gian, hỏi "process nào nên chạy bây giờ?" rồi chạy nó 1 tick*. Sự khác nhau duy nhất là tiêu chí chọn (remaining hay priority). → Tách tiêu chí ra thành tham số `pick`.

**Bước 2 — Mô hình hoá thời gian theo tick**

Thay vì nhảy theo từng đoạn như FCFS/SJF, chạy từng đơn vị thời gian 1 (`t++` mỗi vòng). Điều này cho phép phát hiện preempt ngay khi process mới đến.

**Bước 3 — Theo dõi tiến độ bằng `remain[]`**

Không sửa `burst` gốc (cần để tính WT sau này). Dùng mảng `remain[i]` riêng để đếm "còn bao nhiêu tick cần chạy".

**Bước 4 — Điều kiện dừng vòng lặp**

Không dùng "đếm số process xong" vì dễ nhầm khi có idle time. Thay vào đó: `done` đếm **tổng tick CPU đã xử lý**, dừng khi `done == Σburst` (toàn bộ burst đã được phục vụ).

**Bước 5 — Xây dựng Gantt Chart tại chỗ**

Mỗi tick kiểm tra: process này có giống process tick trước không?
- Có → nối đuôi segment hiện tại (`segs.back().end++`) — tránh tạo hàng nghìn segment lẻ.
- Không → tạo segment mới `{k, t, t+1}` — đây chính là lúc preempt xảy ra.

**Bước 6 — Ghi kết quả ngay khi process xong**

Khi `remain[k] == 0` sau khi trừ: `completion_time = t` (t đã được tăng lên), `waiting_time = t - arrive_time - burst`.

**Kết quả — Vòng lặp hoàn chỉnh:**
```
Khởi tạo: remain[i] = burst[i], total = Σburst, done = 0, t = 0

while (done < total):
    k = pick(p, remain, t)        ← hỏi "ai chạy tại t?"

    if k == -1: t++; continue     ← CPU rảnh (chưa có process nào đến)

    if k == process_trước:
        segs.back().end++         ← nối tiếp segment hiện tại
    else:
        segs.push_back({k, t, t+1})  ← tạo segment mới (có thể preempt)

    remain[k]--; done++; t++

    if remain[k] == 0:
        completion_time[k] = t
        waiting_time[k] = t - arrive_time[k] - burst[k]
```

**Điểm mấu chốt:** `done` đếm tổng số **đơn vị thời gian CPU** đã xử lý (= `Σburst`), không phải số process xong. Vòng lặp kết thúc khi tất cả burst đã được phục vụ.

**SRT dùng picker:**
```cpp
pick = process có remain[i] nhỏ nhất & arrive_time[i] <= t & remain[i] > 0
```

**PPS dùng picker:**
```cpp
pick = process có priority[i] nhỏ nhất & arrive_time[i] <= t & remain[i] > 0
```

**Ý tưởng triển khai `pick`:**

Mọi picker đều theo cùng một khung sườn — chỉ thay tiêu chí so sánh:

```cpp
// Khung sườn chung
auto pick = [](auto &p, auto &rem, int t) {
    int best = -1;
    for (int i = 0; i < (int)p.size(); i++) {
        if (p[i].arrive_time > t || rem[i] <= 0) continue;  // chưa đến hoặc đã xong
        if (best == -1 || /* tiêu chí */ )
            best = i;
    }
    return best;  // -1 nếu không có process nào sẵn sàng (CPU idle)
};
```

Thay phần `/* tiêu chí */` theo từng thuật toán:

| Thuật toán | Tiêu chí thay vào |
|---|---|
| SRT | `rem[i] < rem[best]` |
| PPS | `p[i].priority < p[best].priority` |

**Xử lý tie-break** (khi bằng nhau): ưu tiên process đến sớm hơn, rồi đến index nhỏ hơn:
```cpp
if (best == -1
    || rem[i] < rem[best]
    || (rem[i] == rem[best] && p[i].arrive_time < p[best].arrive_time)
    || (rem[i] == rem[best] && p[i].arrive_time == p[best].arrive_time && i < best))
    best = i;
```

---

### 2.3 print_results() — In kết quả tổng hợp

```cpp
void print_results(title, p, segs)
    → In tiêu đề thuật toán
    → Gọi print_gantt()    (biểu đồ Gantt)
    → Gọi print_averages() (công thức Average Waiting Time)
    → Gọi print_table()    (bảng trace: remaining time tại từng mốc)
```

Tất cả thuật toán đều gọi `print_results()` ở cuối để hiển thị kết quả.

---

## 3. Các thuật toán lập lịch

### 3.1 FCFS — First Come First Served

> **Nguyên tắc:** Ai đến trước thì được phục vụ trước. Không preemptive.

**Pseudocode:**
```
Sắp xếp process theo arrive_time
for mỗi process i:
    if t < arrive_time[i]: t = arrive_time[i]   ← CPU nghỉ chờ
    waiting_time = t - arrive_time[i]
    chạy từ t đến t + burst[i]
    t += burst[i]
```

**Ví dụ:** P1(arrive=0, burst=8), P2(arrive=1, burst=4), P3(arrive=2, burst=9), P4(arrive=3, burst=5)
```
 -------- -------- -------- --------
|   P1   |   P2   |   P3   |   P4   |
 -------- -------- -------- --------
0        8       12       21       26
```
| Process | WT | TAT |
|---------|----|-----|
| P1 | 0 | 8 |
| P2 | 7 | 11 |
| P3 | 10 | 19 |
| P4 | 18 | 23 |
| **TB** | **8.75** | **15.25** |

**Nhận xét:** Đơn giản, dễ cài đặt. Có thể gây **Convoy Effect** (process ngắn bị chặn sau process dài).

**Ý tưởng triển khai:**
- Sắp xếp mảng theo `arrive_time` một lần duy nhất trước khi bắt đầu.
- Dùng biến `t` (đồng hồ) để theo dõi thời điểm CPU rảnh. Nếu `t < arrive_time[i]` thì nhảy `t` lên `arrive_time[i]` (CPU idle).
- Vì đã sắp xếp sẵn, chỉ cần duyệt `for` một lần — không cần vòng lặp lồng nhau.

---

### 3.2 SJF — Shortest Job First (Non-preemptive)

> **Nguyên tắc:** Tại mỗi thời điểm CPU rảnh, chọn process đã đến có **burst nhỏ nhất**. Chạy đến hết mới chọn tiếp.

**Pseudocode:**
```
while còn process chưa xong:
    k = process có burst nhỏ nhất trong {arrive_time <= t, chưa xong}
    if k == -1: t++   ← CPU rảnh, không ai đến
    else:
        waiting_time = t - arrive_time[k]
        chạy từ t đến t + burst[k]
        t += burst[k]
```

**Ưu điểm:** Tối ưu Average Waiting Time trong số các thuật toán non-preemptive.  
**Nhược điểm:** Có thể gây **Starvation** (process burst lớn chờ mãi).

**Ý tưởng triển khai:**
- Dùng vòng `while` — mỗi vòng chọn 1 process.
- Hàm `pick`: quét toàn bộ mảng, lọc `arrive_time <= t && !completed`, lấy `burst` nhỏ nhất.
- Nếu `pick` trả về `-1` (chưa có process nào đến) → tăng `t` lên 1 để "nhảy thời gian" đến process tiếp theo.
- Sau khi chọn xong: ghi `waiting_time = t - arrive_time`, cộng `t += burst`, đánh dấu `completed = true`.

---

### 3.3 SRT — Shortest Remaining Time (Preemptive SJF)

> **Nguyên tắc:** Mỗi đơn vị thời gian, chọn process đã đến có **remaining time nhỏ nhất**. Nếu process mới đến có remaining time nhỏ hơn process đang chạy → preempt ngay.

**Dùng `preemptive_sim()` với picker:**
```
pick = process có remain[i] nhỏ nhất & arrive_time <= t & remain > 0
```

**Ví dụ theo dõi từng tick** (cùng dữ liệu như FCFS):
```
t=0: chỉ P1 sẵn → chạy P1, remain=[7,_,_,_]
t=1: P2 đến (remain=4 < 7) → preempt P1, chạy P2
t=4: P2 xong → P4 có remain=5 < P1(7) → chạy P4
t=9: P4 xong → chạy P1 (remain=7 < P3=9)
t=16: P1 xong → chạy P3 đến t=26
```
```
 -- -------- ---------- ------- ---------
|P1|   P2   |    P4    |   P1  |   P3    |
 -- -------- ---------- ------- ---------
0  1        5         10      17        26
```

**Nhận xét:** Tối ưu Average Waiting Time trong số tất cả các thuật toán (kể cả preemptive).

**Ý tưởng triển khai:**
- Không cần viết vòng lặp riêng — dùng lại `preemptive_sim()` có sẵn.
- Chỉ cần định nghĩa hàm `pick` (lambda): duyệt mảng, trả về index của process có `remain[i]` nhỏ nhất thoả `arrive_time <= t`.
- Mọi logic preempt, tạo/nối segment, tính `completion_time` đều đã nằm trong `preemptive_sim()`.
```cpp
auto segs = preemptive_sim(p, [](auto &p, auto &rem, int t) {
    int k = -1;
    for (int i = 0; i < p.size(); i++)
        if (p[i].arrive_time <= t && rem[i] > 0)
            if (k == -1 || rem[i] < rem[k])
                k = i;
    return k;
});
```

---

### 3.4 RR — Round Robin

> **Nguyên tắc:** Mỗi process được chạy tối đa `quantum` đơn vị thời gian, sau đó nhường CPU cho process tiếp theo. Công bằng nhất.

**Pseudocode:**
```
remain[i] = burst[i]
while còn process chưa xong:
    for mỗi process i (theo arrive_time):
        if remain[i] <= 0: bỏ qua
        exec = min(remain[i], quantum)
        chạy từ t đến t + exec
        t += exec
        remain[i] -= exec
        if remain[i] == 0:
            waiting_time = (t - arrive_time[i]) - burst[i]
```

**Ví dụ** (quantum = 4):
```
 -------- -------- -------- ---- -------- ---- ----
|   P1   |   P2   |   P3   | P4 |   P1   | P3 | P3 |
 -------- -------- -------- ---- -------- ---- ----
0        4        8       12   16       20   24  26  ...
```

**Nhận xét:** Quantum nhỏ → response time tốt nhưng overhead lớn. Quantum lớn → gần giống FCFS.

**Ý tưởng triển khai:**
- Dùng `remain[i] = burst[i]` để theo dõi thời gian còn lại của mỗi process.
- Vòng lặp ngoài `while`: kiểm tra còn process nào chưa xong không.
- Vòng lặp trong `for i`: duyệt lần lượt từng process theo thứ tự đến. Mỗi lượt chạy `exec = min(remain[i], quantum)` ticks.
- **Tối ưu Gantt:** Trước khi tạo segment mới, kiểm tra nếu segment cuối cũng là process `i` → nối `segs.back().end += exec` thay vì thêm đoạn mới.
- Lặp lại cho đến khi tất cả `remain[i] == 0`.

---

### 3.5 Priority Scheduling (NPPS & PPS)

> **Nguyên tắc:** Mỗi process có một giá trị `priority`. **Số nhỏ = ưu tiên cao hơn.** Luôn chọn process có priority nhỏ nhất.

#### Non-preemptive (NPPS)
Giống SJF nhưng thay tiêu chí chọn từ `burst nhỏ nhất` thành `priority nhỏ nhất`. Khi CPU rảnh, chọn xong thì chạy đến hết.

```
k = process có priority nhỏ nhất trong {arrive_time <= t, chưa xong}
```

#### Preemptive (PPS)
Dùng `preemptive_sim()`. Mỗi tick kiểm tra lại — nếu process mới đến có priority cao hơn (số nhỏ hơn) process đang chạy → preempt ngay.

```
pick = process có priority nhỏ nhất trong {arrive_time <= t, remain > 0}
```

**Nhược điểm chung:** Có thể gây **Starvation** — process priority thấp có thể không bao giờ được chạy. Giải pháp: **Aging** (tăng dần priority theo thời gian chờ).

**Ý tưởng triển khai:**
- **NPPS:** Giống SJF hoàn toàn, chỉ thay tiêu chí `burst nhỏ nhất` → `priority nhỏ nhất` trong hàm `pick`.
- **PPS:** Giống SRT hoàn toàn, chỉ thay tiêu chí `remain nhỏ nhất` → `priority nhỏ nhất` trong lambda truyền vào `preemptive_sim()`.
```cpp
// PPS picker:
auto segs = preemptive_sim(p, [](auto &p, auto &rem, int t) {
    int k = -1;
    for (int i = 0; i < p.size(); i++)
        if (p[i].arrive_time <= t && rem[i] > 0)
            if (k == -1 || p[i].priority < p[k].priority)
                k = i;
    return k;
});
```

---

### 3.6 MLFQ — Multilevel Feedback Queue

> **Nguyên tắc:** 3 hàng đợi với mức ưu tiên giảm dần. Process bắt đầu ở queue cao nhất; nếu dùng hết quantum mà chưa xong thì bị **hạ xuống queue thấp hơn**.

**Cấu hình hàng đợi:**
| Queue | Quantum | Ghi chú |
|-------|---------|---------|
| Q0 | 8 | Ưu tiên cao nhất |
| Q1 | 16 | Ưu tiên trung bình |
| Q2 | ∞ (FCFS) | Ưu tiên thấp nhất |

**Quy tắc vận hành (mỗi tick):**
```
1. Đưa process vừa đến vào Q0
2. Tìm queue không rỗng có mức ưu tiên cao nhất (bestLv)
3. Nếu bestLv < queue của process đang chạy → PREEMPT
4. Chạy 1 tick cho process hiện tại
5. Nếu process xong → ghi nhận kết quả
6. Nếu hết quantum → hạ xuống queue thấp hơn (push_back)
7. Nếu bị preempt → giữ nguyên queue, đặt lại đầu hàng (push_front)
```

**Ví dụ minh họa** (P1 burst=20, P2 burst=5, P3 burst=30 — đều đến t=0):
```
Q0: P1 chạy 8 tick → hết quantum → xuống Q1
    P2 chạy 5 tick → xong (còn trong Q0)
Q1: P1 chạy 16 tick → hết quantum → xuống Q2 (còn 20-8-(?))...
Q2: P1 và P3 theo FCFS cho đến khi xong
```

**Đặc điểm nổi bật:**
- Process CPU-bound (burst lớn) dần bị đẩy xuống queue thấp
- Process I/O-bound (burst nhỏ) hoàn thành nhanh trong Q0, response time tốt
- Giả lập hành vi thực tế của scheduler trong hệ điều hành hiện đại

**Ý tưởng triển khai:**
- Dùng 3 `deque<int>` để lưu index process trong mỗi queue. Deque cho phép `push_front` (khi bị preempt) và `push_back` (khi hết quantum).
- Biến `inQueue[i]` tránh đẩy trùng một process vào queue nhiều lần.
- Biến `curUsed` đếm số tick đã chạy trong lượt hiện tại, so với `quantum[level[curProc]]`.
- Mỗi tick: ưu tiên check preempt trước (có queue cao hơn đang chờ không?) → rồi mới chạy tick.
- **Quy tắc push:**
  - Bị preempt → `queues[level[i]].push_front(i)` (quay lại đầu hàng, giữ level)
  - Hết quantum → `level[i]++` (nếu chưa = 2), `queues[level[i]].push_back(i)` (xếp cuối hàng mới)

---

## 4. Bảng so sánh các thuật toán

| Thuật toán | Preemptive | Tiêu chí chọn | Starvation | Độ phức tạp | Ưu điểm | Nhược điểm |
|---|:---:|---|:---:|---|---|---|
| **FCFS** | Không | Thời gian đến | Không | O(n log n) | Đơn giản, dễ cài | Convoy Effect |
| **SJF** | Không | Burst nhỏ nhất | Có | O(n²) | WT tối ưu (non-preemptive) | Cần biết trước burst |
| **SRT** | Có | Remaining nhỏ nhất | Có | O(n²) | WT tối ưu tuyệt đối | Overhead preempt cao |
| **RR** | Có | Xoay vòng (quantum) | Không | O(n) | Công bằng, RT tốt | Hiệu quả phụ thuộc quantum |
| **NPPS** | Không | Priority nhỏ nhất | Có | O(n²) | Phân biệt độ quan trọng | Starvation process thấp |
| **PPS** | Có | Priority nhỏ nhất | Có | O(n²) | Phản ứng nhanh với ưu tiên cao | Starvation + overhead |
| **PRR** | Có | Priority + quantum | Có | O(n²) | Kết hợp ưu tiên & công bằng | Phức tạp hơn |
| **MLFQ** | Có | Queue level + quantum | Không | O(n²) | Thích nghi tự động theo hành vi | Cài đặt phức tạp nhất |

### Khi nào dùng thuật toán nào?

| Tình huống | Thuật toán phù hợp |
|---|---|
| Hệ thống đơn giản, batch job | FCFS |
| Muốn WT thấp nhất, biết trước burst | SJF / SRT |
| Hệ thống tương tác, cần công bằng | RR |
| Các tiến trình có độ ưu tiên khác nhau | NPPS / PPS |
| Hệ điều hành đa người dùng thực tế | MLFQ |

### So sánh các chỉ số hiệu năng (cùng dữ liệu P1–P4)

> P1(0,8), P2(1,4), P3(2,9), P4(3,5) — quantum = 3

| Thuật toán | Avg WT | Avg TAT | Avg RT |
|---|:---:|:---:|:---:|
| FCFS | 8.75 | 14.25 | 8.75 |
| SJF | 4.50 | 10.00 | 4.50 |
| SRT | 3.00 | 8.50 | 0.75 |
| RR (q=3) | 5.75 | 11.25 | 1.50 |
| NPPS | — | — | — |
| PPS | — | — | — |

> RT (Response Time) = thời điểm được CPU lần đầu − arrive_time. SRT có RT tốt vì preempt nhanh.

---

## 5. Đọc file đầu vào & Chạy chương trình

**Định dạng `process.txt`:**
```
3                    ← Số lượng process
P1 0 36 1            ← id  arrive_time  burst  priority
P2 16 20 1
P3 20 12 3
8                    ← Quantum (dùng cho RR, PRR, MLFQ)
```

**Menu chương trình:** Chọn 1–8 để chạy từng thuật toán, chọn 9 để chạy tất cả, 0 để thoát.

**Biên dịch và chạy:**
```bash
g++ -std=c++17 -o main.exe main.cpp
./main.exe
```
Hoặc dùng file `run.bat` có sẵn.
