# GIẢI THÍCH CODE - CPU Scheduling Simulator

## Cấu trúc dự án

```
Lab cpu/
├── main.cpp                        ← Chương trình chính (menu, đọc file)
├── process.txt                     ← Dữ liệu đầu vào
├── CpuSchedulingAlgorithms.h       ← Include tất cả module
└── CpuSchedulingAlgorithmsModule/
    ├── Process.h                   ← Định nghĩa struct Process
    ├── Scheduler.h                 ← Hàm dùng chung (Gantt, bảng trace, mô phỏng)
    ├── FCFS.h                      ← First Come First Served
    ├── SJF.h                       ← Shortest Job First (Non-preemptive)
    ├── SRT.h                       ← Shortest Remaining Time (Preemptive SJF)
    ├── RR.h                        ← Round Robin
    ├── NPPS.h                      ← Non-preemptive Priority
    ├── PPS.h                       ← Preemptive Priority
    ├── PRR.h                       ← Priority + Round Robin
    └── MLFQ.h                      ← Multilevel Feedback Queue
```

---

## 1. Process.h — Định nghĩa tiến trình

```cpp
struct Process {
    string id;                      // Tên tiến trình: "P1", "P2"...
    int arrive_time = 0;            // Thời điểm đến
    int burst = 0;                  // Thời gian CPU cần thực thi
    int priority = 0;               // Độ ưu tiên (số nhỏ = ưu tiên cao)
    int waiting_time = 0;           // Thời gian chờ = completion - arrive - burst
    int return_time = 0;            // Thời gian quay vòng (chưa dùng trực tiếp)
    int turnaround_time = 0;        // Thời gian hoàn thành tổng (chưa dùng trực tiếp)
    int response_time = 0;          // Thời gian phản hồi (chưa dùng trực tiếp)
    int completion_time = 0;        // Thời điểm hoàn thành
    bool completed = false;         // Đã hoàn thành chưa
};

using Processes = vector<Process>;  // Alias cho vector<Process>
using Quantum   = int;              // Alias cho quantum
```

**Giải thích:**
- Mỗi `Process` lưu thông tin đầu vào (`arrive_time`, `burst`, `priority`) và kết quả sau khi chạy (`waiting_time`, `completion_time`)
- `reset()` xóa tất cả kết quả (`waiting_time`, `return_time`, `turnaround_time`, `response_time`, `completion_time`) và đặt `completed = false` để có thể chạy lại thuật toán khác

---

## 2. Scheduler.h — Các hàm dùng chung

### 2.1 Hàm tiện ích

```cpp
reset_all(p)        → Reset tất cả process về trạng thái ban đầu
sort_by_arrive(p)   → Sắp xếp theo thời gian đến (arrive_time tăng dần)
```

### 2.2 GanttSeg — Một đoạn trong Gantt Chart

```cpp
struct GanttSeg {
    int idx;    // Chỉ số process trong mảng
    int start;  // Thời điểm bắt đầu chạy đoạn này
    int end;    // Thời điểm kết thúc đoạn này
};
```

**Ví dụ:** SRTF với P1(0,8), P2(1,4), P3(2,9), P4(3,5) tạo ra:
```
Segment 0: {idx=0, start=0, end=1}   → P1 chạy từ 0 đến 1
Segment 1: {idx=1, start=1, end=5}   → P2 chạy từ 1 đến 5
Segment 2: {idx=3, start=5, end=10}  → P4 chạy từ 5 đến 10
Segment 3: {idx=0, start=10, end=17} → P1 chạy từ 10 đến 17
Segment 4: {idx=2, start=17, end=26} → P3 chạy từ 17 đến 26
```

### 2.3 print_gantt() — In Gantt Chart

Vẽ biểu đồ Gantt theo dạng:
```
 ------ ------ ------ ------ ------
|  P1  |  P2  |  P4  |  P1  |  P3  |
 ------ ------ ------ ------ ------
0      1      5      10     17     26
```

**Cách hoạt động:**
- Tính `cw` (column width): chiều rộng mỗi cột, được tính dựa trên chiều dài tên process và chiều dài số thời gian (lấy giá trị lớn nhất + 2, tối thiểu 6)
- `bar()`: In đường kẻ `------` với chiều rộng = `cw` cho mỗi segment
- Tên process được căn giữa trong mỗi đoạn
- Dòng cuối in các mốc thời gian `start` của mỗi segment, kèm `end` của segment cuối

### 2.4 Các hàm tính toán cho Trace Table

```cpp
executed_before(p, segs, i, t)  → Tổng thời gian process i đã chạy trước thời điểm t
remaining_at(p, segs, i, t)     → burst - executed_before = thời gian CPU còn lại
running_at(segs, t)             → Trả về idx process đang chạy tại thời điểm t (-1 nếu idle)
collect_times(p, segs)          → Thu thập tất cả mốc thời gian (start, end, arrive_time) → sắp xếp, loại trùng
```

### 2.5 print_table() — In bảng Trace (Analysis)

Tạo bảng hiển thị **remaining time** (thời gian CPU còn lại) của mỗi process tại từng mốc thời gian:

```
+------+-----+-----+-----+-----+-----+-----+-----+-----+
| time |    0|    1|    2|    3|    5|   10|   17|   26|
+------+-----+-----+-----+-----+-----+-----+-----+-----+
| P1   |    8|    7|    7|    7|    7|    7|    0|    0|
+------+-----+-----+-----+-----+-----+-----+-----+-----+
```

**Cách hoạt động:**

1. **Thu thập mốc thời gian:** Gọi `collect_times()` lấy tất cả `start`, `end` từ Gantt segments + `arrive_time` → sắp xếp, loại bỏ trùng

2. **Tính remaining time:** Với mỗi ô `(process i, thời điểm t)`:
   - Nếu `t < arrive_time` → ô trống (process chưa đến)
   - Ngược lại → gọi `remaining_at(p, segs, i, t)` = `burst - executed_before(i, t)`

3. **Tô màu:**
   - **Đỏ (RED)**: Process đang chạy tại thời điểm đó (`running_at(segs, t) == i`)
   - **Xanh dương (BLU)**: Process vừa hoàn thành tại thời điểm đó (`remaining = 0` và `completion_time == t`)
   - Không màu: Các ô bình thường

### 2.6 print_averages() — In công thức Waiting Time

```
Average waiting time = [(17-0-8)+(5-1-4)+(26-2-9)+(10-3-5)]/4 = 26/4 = 6.50ms
```

**Công thức:** `waiting_time = completion_time - arrive_time - burst`

### 2.7 print_results() — In kết quả tổng hợp

```cpp
void print_results(title, p, segs)
    → In tiêu đề thuật toán
    → Gọi print_gantt()    (biểu đồ Gantt)
    → Gọi print_averages() (công thức trung bình)
    → Gọi print_table()    (bảng trace)
```

Tất cả các thuật toán đều gọi `print_results()` ở cuối để hiển thị kết quả.

### 2.8 preemptive_sim() — Mô phỏng thuật toán Preemptive

Đây là hàm **template** dùng chung cho SRT và PPS (cả hai đều là preemptive).

```cpp
template <typename Picker>
vector<GanttSeg> preemptive_sim(Processes &p, Picker pick)
```

**Tham số `Picker`:** Hàm lambda quyết định chọn process nào chạy tại thời điểm `t`

**Cách hoạt động:**
```
Khởi tạo: remain[i] = burst[i], total = tổng burst, done = 0, t = 0

while (done < total):          ← done đếm tổng số đơn vị thời gian đã thực thi
    k = pick(p, remain, t)     ← Gọi picker để chọn process
    
    nếu k == -1 (không ai sẵn sàng):
        t++, bỏ qua
    
    nếu k == process trước đó → nối segment (segs.back().end++)
    nếu k != process trước đó → tạo segment mới {k, t, t+1}
    
    remain[k]--, done++, t++
    
    nếu remain[k] == 0:
        p[k].completed = true
        p[k].completion_time = t
        p[k].waiting_time = t - burst - arrive_time
```

**Lưu ý:** `done` đếm tổng số đơn vị thời gian CPU đã xử lý (= tổng burst), KHÔNG PHẢI số process hoàn thành.

---

## 3. Các thuật toán lập lịch

### 3.1 FCFS.h — First Come First Served

**Nguyên tắc:** Process nào đến trước được chạy trước, chạy xong mới đến process tiếp theo.

```
Sắp xếp process theo arrive_time
Duyệt process theo thứ tự:
    nếu t < arrive_time → chờ (t = arrive_time)
    waiting_time = t - arrive_time
    tạo segment {i, t, t + burst}
    t += burst
    completion_time = t
```

**Ví dụ:** P1(0,8), P2(1,4), P3(2,9), P4(3,5)
```
P1 chạy 0→8, P2 chạy 8→12, P3 chạy 12→21, P4 chạy 21→26
```

### 3.2 SJF.h — Shortest Job First (Non-preemptive)

**Nguyên tắc:** Tại mỗi thời điểm, chọn process có `burst` nhỏ nhất trong các process đã đến. Chạy đến xong rồi mới chọn tiếp.

```
while (done < n):
    k = process có burst nhỏ nhất & arrive_time <= t & chưa completed
    nếu k == -1: t++ (chờ process đến)
    waiting_time = t - arrive_time
    tạo segment {k, t, t + burst}
    t += burst
    completion_time = t
    done++
```

### 3.3 SRT.h — Shortest Remaining Time (Preemptive SJF)

**Nguyên tắc:** Giống SJF nhưng **preemptive** — tại MỖI đơn vị thời gian, kiểm tra lại xem process nào có **remaining time** nhỏ nhất.

Dùng `preemptive_sim()` với picker:
```
Chọn process có remain[i] nhỏ nhất & arrive_time <= t & remain > 0
Tie-break: arrive_time nhỏ hơn, rồi id nhỏ hơn
```

**Ví dụ chi tiết:**
```
t=0: P1 arrive, remain=[8]. Chọn P1 (duy nhất). remain=[7,_,_,_]
t=1: P2 arrive, remain=[7,4]. Chọn P2 (4<7). remain=[7,3,_,_]
t=2: P3 arrive, remain=[7,3,9]. Chọn P2 (3 nhỏ nhất). remain=[7,2,9,_]
t=3: P4 arrive, remain=[7,2,9,5]. Chọn P2 (2 nhỏ nhất). remain=[7,1,9,5]
t=4: remain=[7,1,9,5]. Chọn P2 (1 nhỏ nhất). remain=[7,0,9,5] → P2 xong!
t=5: remain=[7,_,9,5]. Chọn P4 (5<7<9). remain=[7,_,9,4]
...
t=9: remain=[7,_,9,1]. Chọn P4. remain=[7,_,9,0] → P4 xong!
t=10: remain=[7,_,9]. Chọn P1 (7<9). remain=[6,_,9,_]
...
t=16: remain=[1,_,9]. Chọn P1. remain=[0,_,9] → P1 xong!
t=17-25: P3 chạy. t=26: P3 xong!
```

### 3.4 RR.h — Round Robin

**Nguyên tắc:** Mỗi process được chạy tối đa `quantum` đơn vị thời gian, rồi nhường cho process tiếp theo trong danh sách.

```
Khởi tạo: remain[i] = burst[i]
while (còn process chưa xong):
    for mỗi process i (theo thứ tự arrive_time):
        nếu remain[i] <= 0 → bỏ qua
        exec = min(remain[i], quantum)
        tạo hoặc nối segment
        t += exec
        remain[i] -= exec
        nếu remain[i] == 0:
            waiting_time = (t - arrive_time) - burst
            completion_time = t
```

**Lưu ý:** Thuật toán duyệt tuần tự qua tất cả process (không dùng queue), nếu segment liên tiếp cùng process thì nối lại.

### 3.5 NPPS.h — Non-preemptive Priority Scheduling

**Nguyên tắc:** Chọn process có `priority` nhỏ nhất (ưu tiên cao nhất) trong các process đã đến. Chạy đến xong.

```
while (done < n):
    k = process có priority nhỏ nhất & arrive_time <= t & chưa completed
    nếu k == -1: t++ (chờ)
    waiting_time = t - arrive_time
    tạo segment {k, t, t + burst}
    t += burst
    completion_time = t
    done++
```

### 3.6 PPS.h — Preemptive Priority Scheduling

**Nguyên tắc:** Giống NPPS nhưng **preemptive** — kiểm tra priority mỗi đơn vị thời gian.

Dùng `preemptive_sim()` với picker:
```
Chọn process có priority nhỏ nhất & arrive_time <= t & !completed & remain > 0
```

### 3.7 PRR.h — Priority with Round Robin

**Nguyên tắc:** Chọn process có priority cao nhất (số nhỏ nhất), nhưng chạy theo quantum (không chạy hết burst). Sau mỗi lượt, chọn lại process có priority cao nhất.

```
Khởi tạo: remain[i] = burst[i]
while (completed < n):
    idx = process có priority nhỏ nhất & remain > 0 & arrive_time <= t
    nếu idx == -1: t++ (chờ)
    exec = min(remain[idx], quantum)
    tạo hoặc nối segment
    t += exec
    remain[idx] -= exec
    nếu remain[idx] == 0:
        waiting_time = (t - arrive_time) - burst
        completion_time = t
        completed++
```

### 3.8 MLFQ.h — Multilevel Feedback Queue

**Nguyên tắc:** 3 hàng đợi với quantum tăng dần. Process bắt đầu ở Queue 0, nếu chạy hết quantum mà chưa xong thì bị đẩy xuống queue thấp hơn.

```
Mỗi process có level[i] (mặc định 0 -> ưu tiên cao nhất)
Quantum theo level :
    Level 0: quantum = 8  (ưu tiên cao nhất)
    Level 1: quantum = 16
    Level 2: quantum = 0  → FCFS (chạy tới khi xong hoặc bị preempt bởi queue ưu tiên cao hơn)
```

**Các biến trạng thái:**
```
remain[i]   ← Thời gian CPU còn lại
level[i]    ← Level hiện tại (0, 1, 2)
queues[3]   ← 3 deque, mỗi deque là 1 hàng đợi
inQueue[i]  ← Process có đang trong queue không
curProc     ← Process đang chạy (-1 nếu idle)
curUsed     ← Số tick đã dùng cho process đang chạy (so với quantum)
```

**Hàm `enqueueArrivals(time)`:**
```
Quét toàn bộ process:
    nếu arrive_time <= time && remain > 0 && !inQueue && không phải curProc
    → đẩy vào queues[level[i]]
→ Mỗi tick cập nhật "ai đã đến hệ thống"
```

**Vòng lặp chính (mỗi tick):**
```
1. enqueueArrivals(t)                → đưa process đã tới vào queue theo level
2. Tìm bestLv = queue ưu tiên cao nhất (nhỏ nhất) đang không rỗng
3. Nếu bestLv < level[curProc]       → preempt: đẩy curProc về đầu queue, reset curProc
4. Nếu curProc == -1                 → lấy process từ queue ưu tiên cao nhất
5. Chạy curProc 1 tick               → remain--, curUsed++, tạo/nối segment
6. Nếu remain == 0                   → process xong, tính waiting/completion, reset curProc
7. Nếu curUsed >= quantum (q > 0)    → hết quantum, hạ level (nếu chưa = 2), đưa vào queue mới
```

**Đặc biệt:**
- Khi bị preempt, process được đẩy về **đầu** queue (`push_front`) → được ưu tiên chạy lại trước
- Khi hết quantum, process bị đẩy về **cuối** queue (`push_back`) → xếp sau các process khác
- Level 2 có quantum = 0, nghĩa là `q > 0` luôn sai → process ở level 2 chạy liên tục cho đến khi xong hoặc bị preempt

---

## 4. main.cpp — Chương trình chính

**Luồng hoạt động:**

1. Đọc file `process.txt`:
   ```
   3                    ← Số lượng process
   P1 0 36 1            ← id arrive_time burst priority
   P2 16 20 1
   P3 20 12 3
   8                    ← Quantum (dùng cho RR, PRR)
   ```

2. Hiện menu cho người dùng chọn thuật toán (1-8), chạy tất cả (9), hoặc thoát (0)

3. Với mỗi lựa chọn:
   - `run_one()`: Copy mảng process gốc → gọi hàm thuật toán tương ứng
   - Nếu chọn 9: Gọi `run_one()` lần lượt từ 1 đến 8
   - Thuật toán tự in kết quả qua `print_results()` (Gantt chart + average + bảng trace)

---

## 5. Cách biên dịch và chạy

```bash
g++ -std=c++17 -o main.exe main.cpp
./main.exe
```

Hoặc dùng file `run.bat` có sẵn.
