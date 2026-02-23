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
    string id;                  // Tên tiến trình: "P1", "P2"...
    int arrive_time = 0;        // Thời điểm đến
    int burst = 0;              // Thời gian CPU cần thực thi
    int priority = 0;           // Độ ưu tiên (số nhỏ = ưu tiên cao)
    int waiting_time = 0;       // Thời gian chờ = completion - arrive - burst
    int completion_time = 0;    // Thời điểm hoàn thành
    bool completed = false;     // Đã hoàn thành chưa
};
```

**Giải thích:**
- Mỗi `Process` lưu thông tin đầu vào (`arrive_time`, `burst`, `priority`) và kết quả sau khi chạy (`waiting_time`, `completion_time`)
- `reset()` dùng để xóa kết quả cũ khi chạy lại thuật toán khác

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
 -- -------- ---------- -------------- ------------------
|P1|   P2   |    P4    |      P1      |        P3        |
 -- -------- ---------- -------------- ------------------
0  1        5          10             17                 26
```

**Cách hoạt động:**
- `bar()`: In đường kẻ `--` với chiều rộng = `(end - start) * 2`
- Tên process được căn giữa trong mỗi đoạn
- Dòng cuối in các mốc thời gian

### 2.4 print_table() — In bảng Trace (Analysis)

Tạo bảng hiển thị **remaining time** (thời gian CPU còn lại) của mỗi process tại từng mốc thời gian:

```
+------+-----+-----+-----+-----+-----+-----+-----+-----+
| time |    0|    1|    2|    3|    5|   10|   17|   26|
+------+-----+-----+-----+-----+-----+-----+-----+-----+
| P1   |    8|    7|    7|    7|    7|    7|    0|    0|  ← 8 đỏ, 7 đỏ (t=10), 0 xanh (t=17)
+------+-----+-----+-----+-----+-----+-----+-----+-----+
```

**Cách hoạt động:**

1. **Thu thập mốc thời gian:** Lấy tất cả `start`, `end` từ Gantt segments + `arrive_time` của mỗi process → sắp xếp, loại bỏ trùng

2. **Tính remaining time:**
   - `executed_before(i, t)`: Tổng thời gian process `i` đã chạy trước thời điểm `t`
   - `remaining_at(i, t)` = `burst - executed_before(i, t)`

3. **Tô màu:**
   - **Đỏ (RED)**: Process đang chạy tại mốc thời gian đó (segment bắt đầu tại `t`)
   - **Xanh dương (BLU)**: Process vừa hoàn thành tại mốc thời gian đó (`remaining = 0` và `completion_time = t`)
   - Không màu: Các ô bình thường

4. **Ô trống:** Nếu `t < arrive_time` thì ô để trống (process chưa đến)

### 2.5 print_averages() — In công thức Waiting Time

```
Average waiting time = [(17-0-8)+(5-1-4)+(26-2-9)+(10-3-5)]/4 = 26/4 = 6.50ms
```

**Công thức:** `waiting_time = completion_time - arrive_time - burst`

### 2.6 preemptive_sim() — Mô phỏng thuật toán Preemptive

Đây là hàm **template** dùng chung cho SRT và PPS (cả hai đều là preemptive).

```cpp
template <typename Picker>
vector<GanttSeg> preemptive_sim(Processes &p, Picker pick)
```

**Tham số `Picker`:** Hàm lambda quyết định chọn process nào chạy tại thời điểm `t`

**Cách hoạt động:**
```
t = 0, remain = [8, 4, 9, 5]
  → pick() chọn process có remain nhỏ nhất & đã arrive
  → Tạo/nối GanttSeg
  → remain[k]--, done++, t++
  → Nếu remain[k] == 0 → process hoàn thành, tính waiting_time
```

**Mỗi vòng lặp (1 đơn vị thời gian):**
1. Gọi `pick()` để chọn process chạy
2. Nếu cùng process đang chạy → nối segment (`segs.back().end++`)
3. Nếu khác process → tạo segment mới
4. Giảm remain, tăng done và t
5. Nếu process xong → ghi `completion_time` và `waiting_time`

---

## 3. Các thuật toán lập lịch

### 3.1 FCFS.h — First Come First Served

**Nguyên tắc:** Process nào đến trước được chạy trước, chạy xong mới đến process tiếp theo.

```
Duyệt process theo thứ tự arrive_time:
  nếu t < arrive_time → chờ (t = arrive_time)
  waiting_time = t - arrive_time
  chạy hết burst → t += burst
```

**Ví dụ:** P1(0,8), P2(1,4), P3(2,9), P4(3,5)
```
P1 chạy 0→8, P2 chạy 8→12, P3 chạy 12→21, P4 chạy 21→26
```

### 3.2 SJF.h — Shortest Job First (Non-preemptive)

**Nguyên tắc:** Tại mỗi thời điểm, chọn process có `burst` nhỏ nhất trong các process đã đến. Chạy đến xong rồi mới chọn tiếp.

```
while (còn process chưa xong):
  k = process có burst nhỏ nhất & arrive_time <= t & chưa completed
  chạy k hết burst → t += burst
```

### 3.3 SRT.h — Shortest Remaining Time (Preemptive SJF)

**Nguyên tắc:** Giống SJF nhưng **preemptive** — tại MỖI đơn vị thời gian, kiểm tra lại xem process nào có **remaining time** nhỏ nhất.

Dùng `preemptive_sim()` với picker:
```
Chọn process có remain[i] nhỏ nhất & arrive_time <= t
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

**Nguyên tắc:** Mỗi process được chạy tối đa `quantum` đơn vị thời gian, rồi nhường cho process tiếp theo.

```
while (còn process chưa xong):
  for mỗi process i (theo thứ tự):
    exec = min(remain[i], quantum)
    chạy i trong exec đơn vị
    remain[i] -= exec
```

### 3.5 NPPS.h — Non-preemptive Priority Scheduling

**Nguyên tắc:** Chọn process có `priority` nhỏ nhất (ưu tiên cao nhất) trong các process đã đến. Chạy đến xong.

```
k = process có priority nhỏ nhất & arrive_time <= t & chưa completed
chạy k hết burst
```

### 3.6 PPS.h — Preemptive Priority Scheduling

**Nguyên tắc:** Giống NPPS nhưng **preemptive** — kiểm tra priority mỗi đơn vị thời gian.

Dùng `preemptive_sim()` với picker:
```
Chọn process có priority nhỏ nhất & arrive_time <= t & remain > 0
```

### 3.7 PRR.h — Priority with Round Robin

**Nguyên tắc:** Chọn process có priority cao nhất, nhưng chạy theo quantum (không chạy hết burst).

```
while (còn process chưa xong):
  idx = process có priority nhỏ nhất & remain > 0
  exec = min(remain[idx], quantum)
  chạy idx trong exec đơn vị
```

### 3.8 MLFQ.h — Multilevel Feedback Queue

**Nguyên tắc:** 3 hàng đợi với quantum tăng dần. Process bắt đầu ở Queue 0, nếu chạy hết quantum mà chưa xong thì bị đẩy xuống queue thấp hơn.

```
Queue 0: quantum = 2 (ưu tiên cao nhất)
Queue 1: quantum = 4
Queue 2: FCFS (chạy hết burst)
```

```
while (còn process chưa xong):
  Tìm process ở queue thấp nhất (ưu tiên cao nhất)
  exec = quantum của queue đó (Queue 2: chạy hết)
  Nếu chưa xong & đã dùng hết quantum → level++
```

---

## 4. main.cpp — Chương trình chính

**Luồng hoạt động:**

1. Đọc file `process.txt`:
   ```
   4                    ← Số lượng process
   P1 0 8 1             ← id arrive_time burst priority
   P2 1 4 1
   P3 2 9 3
   P4 3 5 2
   2                    ← Quantum (dùng cho RR, PRR)
   ```

2. Hiện menu cho người dùng chọn thuật toán (1-8), chạy tất cả (9), hoặc thoát (0)

3. Với mỗi lựa chọn:
   - Copy mảng process gốc (để không ảnh hưởng lần chạy sau)
   - Gọi hàm thuật toán tương ứng
   - Thuật toán tự in Gantt chart, bảng trace, và average waiting time

---

## 5. Cách biên dịch và chạy

```bash
g++ -std=c++17 -o main.exe main.cpp
./main.exe
```

Hoặc dùng file `run.bat` có sẵn.
