# GIẢI THÍCH CODE - Lab Deadlock

---

## 1. FILE `Bankers.py` — Thuật toán Banker (Kiểm tra trạng thái an toàn)

### Ý tưởng giải thuật toán Banker

**Bài toán đặt ra:** Hệ thống có `n` tiến trình và `m` loại tài nguyên. Mỗi tiến trình khai báo trước số tài nguyên **tối đa (Max)** nó sẽ cần. Hệ thống đã **cấp phát (Allocation)** một phần. Câu hỏi: liệu hệ thống có ở **trạng thái an toàn** hay không?

**Ý tưởng cốt lõi:**
1. Tính **Need = Max - Allocation** → số tài nguyên mỗi tiến trình còn thiếu
2. Tính **Available = Total - tổng Allocation** → tài nguyên đang rảnh
3. **Giả lập quá trình chạy:** Lặp đi lặp lại việc tìm tiến trình có `Need ≤ Available` (tức là hệ thống đủ tài nguyên để cấp cho nó chạy xong). Khi tiến trình chạy xong → **thu hồi tài nguyên** (cộng Allocation của nó vào Available) → thử tìm tiến trình tiếp theo
4. Nếu **tất cả tiến trình đều chạy xong được** → hệ thống an toàn (SAFE). Nếu bị kẹt (không tìm được tiến trình nào chạy tiếp) → không an toàn (UNSAFE), có nguy cơ deadlock

**Tóm gọn:** Thử xem "nếu cấp phát theo đúng thứ tự nào đó, tất cả tiến trình có chạy xong hết không?" — nếu có thì an toàn.

---

### 1.1. Hàm `calculate_need(n, Allocation, Max)`

```python
def calculate_need(n, Allocation, Max):
    Need = []
    for i in range(n):
        row = []
        for j in range(3):
            row.append(Max[i][j] - Allocation[i][j])
        Need.append(row)
    return Need
```

**Mục đích:** Tính ma trận **Need** — số tài nguyên mỗi tiến trình còn cần thêm để hoàn thành.

**Công thức:**
```
Need[i][j] = Max[i][j] - Allocation[i][j]
```

**Giải thích từng bước:**
- Duyệt qua từng tiến trình `i` (0 → n-1)
- Với mỗi loại tài nguyên `j`, tính: **tối đa cần - đã cấp = còn cần thêm**
- Kết quả trả về là ma trận Need

**Ví dụ với input:**
| Process | Allocation | Max     | Need = Max - Alloc |
|---------|-----------|---------|-------------------|
| P0      | 0 1 0     | 7 5 3   | 7 4 3             |
| P1      | 2 0 0     | 3 2 2   | 1 2 2             |
| P2      | 3 0 2     | 9 0 2   | 6 0 0             |

---

### 1.2. Hàm `calculate_available(n, Allocation, Total)`

```python
def calculate_available(n, Allocation, Total):
    Available = [0,0,0]
    for j in range(3):
        s = 0
        for i in range(n):
            s += Allocation[i][j]
        Available[j] = Total[j] - s
    return Available
```

**Mục đích:** Tính vector **Available** — số tài nguyên hiện đang rảnh (chưa cấp cho ai).

**Công thức:**
```
Available[j] = Total[j] - Σ Allocation[i][j]  (với mọi i)
```

**Giải thích từng bước:**
- Với mỗi loại tài nguyên `j`:
  - Cộng tổng tài nguyên loại `j` đã cấp cho tất cả tiến trình → `s`
  - `Available[j]` = Tổng tài nguyên hệ thống - Tổng đã cấp

**Ví dụ:**
```
Total     = [10, 5, 7]
Tổng Alloc = [0+2+3, 1+0+0, 0+0+2] = [5, 1, 2]
Available  = [10-5, 5-1, 7-2] = [5, 4, 5]
```

---

### 1.3. Hàm `banker(n, Allocation, Need, Available)` HÀM CHÍNH

```python
def banker(n, Allocation, Need, Available):
    Work = Available[:]
    Finish = [False]*n
    Sequence = []
    for step in range(n):
        found = False
        for i in range(n):
            if not Finish[i]:
                if (Need[i][0] <= Work[0] and Need[i][1] <= Work[1] and Need[i][2] <= Work[2]):
                    for j in range(3):
                        Work[j] += Allocation[i][j]
                    Finish[i] = True
                    Sequence.append(i)
                    found = True
        if not found:
            break
    if all(Finish):
        print("SAFE STATE")
    else:
        print("NOT SAFE STATE")
```

**Mục đích:** Kiểm tra hệ thống có ở **trạng thái an toàn (safe state)** hay không, bằng thuật toán Safety Algorithm.

**Giải thích từng bước:**

1. **Khởi tạo:**
   - `Work = Available[:]` — sao chép vector Available làm tài nguyên "khả dụng hiện tại"
   - `Finish = [False]*n` — đánh dấu tất cả tiến trình chưa hoàn thành
   - `Sequence = []` — danh sách thứ tự an toàn

2. **Vòng lặp chính** (lặp tối đa `n` lần):
   - Duyệt qua tất cả tiến trình `i`:
     - Nếu `Finish[i] == False` (chưa chạy xong) **VÀ** `Need[i] <= Work` (tài nguyên cần ≤ tài nguyên rảnh):
       - **Giả lập tiến trình chạy xong:** `Work[j] += Allocation[i][j]` (thu hồi tài nguyên)
       - Đánh dấu `Finish[i] = True`
       - Thêm `i` vào `Sequence`
   - Nếu không tìm thấy tiến trình nào thỏa mãn → `break`

3. **Kết luận:**
   - Nếu `all(Finish) == True` → **SAFE STATE** (có thể cấp phát an toàn)
   - Ngược lại → **NOT SAFE STATE** (có nguy cơ deadlock)

**Trace qua ví dụ:**

| Bước | Work       | Chọn Pi | Need[i] ≤ Work? | Work mới (sau thu hồi)   |
|------|-----------|---------|------------------|--------------------------|
| 1    | [5, 4, 5] | P1      | [1,2,2] ≤ [5,4,5] ✅ | [5+2, 4+0, 5+0] = [7, 4, 5] |
| 2    | [7, 4, 5] | P2      | [6,0,0] ≤ [7,4,5] ✅ | [7+3, 4+0, 5+2] = [10, 4, 7] |
| 3    | [10, 4, 7]| P0      | [7,4,3] ≤ [10,4,7] ✅ | [10+0, 4+1, 7+0] = [10, 5, 7] |

→ Tất cả `Finish = [True, True, True]` → **SAFE STATE**
→ Safe Sequence: **P1 → P2 → P0**

### Output khi chạy `Bankers.py`

```
Number of processes = 3
Allocation
P0 : [0, 1, 0]
P1 : [2, 0, 0]
P2 : [3, 0, 2]
Max
P0 : [7, 5, 3]
P1 : [3, 2, 2]
P2 : [9, 0, 2]
Need
P0 : [7, 4, 3]
P1 : [1, 2, 2]
P2 : [6, 0, 0]

Total Resources = [10, 5, 7]
Available = [5, 4, 5]

Initial Work = [5, 4, 5]

Process P1 runs
Work becomes = [7, 4, 5]

Process P2 runs
Work becomes = [10, 4, 7]

Process P0 runs
Work becomes = [10, 5, 7]

Finish = [True, True, True]
SAFE STATE
Safe Sequence:
P1 -> P2 -> P0
```

**Giải thích output:**
- Đầu tiên in ra các ma trận Allocation, Max, Need và vector Total, Available
- `Initial Work = [5, 4, 5]` → tài nguyên rảnh ban đầu
- **Bước 1:** P1 chạy được vì `Need[P1]=[1,2,2] ≤ Work=[5,4,5]` → thu hồi tài nguyên P1 → `Work=[7,4,5]`
- **Bước 2:** P2 chạy được vì `Need[P2]=[6,0,0] ≤ Work=[7,4,5]` → thu hồi → `Work=[10,4,7]`
- **Bước 3:** P0 chạy được vì `Need[P0]=[7,4,3] ≤ Work=[10,4,7]` → thu hồi → `Work=[10,5,7]`
- Kết quả: **SAFE STATE** — hệ thống an toàn, thứ tự: **P1 → P2 → P0**

### Trường hợp 2: NOT SAFE STATE (file `input_bankers2.txt`)

Input:
```
4 tiến trình, 3 loại tài nguyên, Total = [10, 5, 7]
Allocation = [[0,1,0], [2,0,0], [3,0,2], [2,1,1]]
Max        = [[7,5,3], [3,2,2], [9,0,2], [8,5,3]]
Need       = [[7,4,3], [1,2,2], [6,0,0], [6,4,2]]
Available  = [10-7, 5-2, 7-3] = [3, 3, 4]
```

**Trace:**

| Bước | Work       | Duyệt Pi | Need[i] ≤ Work?           | Kết quả        |
|------|-----------|----------|---------------------------|----------------|
| 1    | [3, 3, 4] | P0       | [7,4,3] ≤ [3,3,4]? ❌ (7>3) | bỏ qua       |
| 1    | [3, 3, 4] | P1       | [1,2,2] ≤ [3,3,4]? ✅     | P1 chạy xong   |
|      |           |          | Work = [3+2,3+0,4+0]      | = [5, 3, 4]   |
| 2    | [5, 3, 4] | P0       | [7,4,3] ≤ [5,3,4]? ❌ (7>5) | bỏ qua       |
| 2    | [5, 3, 4] | P2       | [6,0,0] ≤ [5,3,4]? ❌ (6>5) | bỏ qua       |
| 2    | [5, 3, 4] | P3       | [6,4,2] ≤ [5,3,4]? ❌ (6>5) | bỏ qua       |

→ Không tìm được tiến trình nào chạy tiếp → `break`
→ `Finish = [False, True, False, False]` → **NOT SAFE STATE**

**Output:**
```
Number of processes = 4
Allocation
P0 : [0, 1, 0]
P1 : [2, 0, 0]
P2 : [3, 0, 2]
P3 : [2, 1, 1]
Max
P0 : [7, 5, 3]
P1 : [3, 2, 2]
P2 : [9, 0, 2]
P3 : [8, 5, 3]
Need
P0 : [7, 4, 3]
P1 : [1, 2, 2]
P2 : [6, 0, 0]
P3 : [6, 4, 2]

Total Resources = [10, 5, 7]
Available = [3, 3, 4]

Initial Work = [3, 3, 4]

Process P1 runs
Work becomes = [5, 3, 4]

Finish = [False, True, False, False]
NOT SAFE STATE
```

**Giải thích:** Chỉ có P1 chạy được. Sau khi thu hồi tài nguyên P1 → Work=[5,3,4], nhưng P0 cần [7,4,3], P2 cần [6,0,0], P3 cần [6,4,2] — tất cả đều cần tài nguyên loại A ≥ 6 mà Work chỉ có 5 → **kẹt**, hệ thống không an toàn.

---

## 2. FILE `deadlock_detection.py` — Thuật toán phát hiện Deadlock

### Ý tưởng giải thuật toán Deadlock Detection

**Bài toán đặt ra:** Hệ thống đã cấp phát tài nguyên cho các tiến trình, mỗi tiến trình đang **yêu cầu thêm (Request)** một số tài nguyên. Câu hỏi: có tiến trình nào đang bị **deadlock** (chờ nhau vô hạn) hay không?

**Ý tưởng cốt lõi:**
1. Khác với Banker dùng **Need** (Max - Alloc), thuật toán này dùng trực tiếp ma trận **Request** — tài nguyên tiến trình đang thực sự yêu cầu **ngay lúc này**
2. **Giả lập tương tự Banker:** Tìm tiến trình có `Request ≤ Available` → cho nó chạy xong → thu hồi tài nguyên → lặp lại
3. Sau khi không thể tìm thêm tiến trình nào:
   - Nếu **tất cả đã chạy xong** → không có deadlock
   - Nếu **còn tiến trình chưa chạy xong** → những tiến trình đó đang bị **deadlock** (chúng chờ tài nguyên mà không bao giờ được giải phóng)

### 2.1. Hàm `detect_deadlock(n, m, allocation, request, available)` HÀM CHÍNH

```python
def detect_deadlock(n, m, allocation, request, available):
    work = available[:]
    finish = [False] * n
    safe_sequence = []
    while True:
        found = False
        for i in range(n):
            if not finish[i] and all(request[i][j] <= work[j] for j in range(m)):
                for j in range(m):
                    work[j] += allocation[i][j]
                finish[i] = True
                safe_sequence.append(f"P{i}")
                found = True
        if not found:
            break
    if all(finish):
        print("No Deadlock Detected")
    else:
        deadlocked = [f"P{i}" for i in range(n) if not finish[i]]
        print("Deadlock Detected!")
        print("Deadlocked Processes:", ", ".join(deadlocked))
```

**Mục đích:** Phát hiện **deadlock** trong hệ thống bằng thuật toán Deadlock Detection Algorithm.

**Giải thích từng bước:**

1. **Khởi tạo:** giống thuật toán Banker
   - `work = available[:]`
   - `finish = [False] * n`

2. **Vòng lặp `while True`:**
   - Duyệt tất cả tiến trình `i`:
     - Nếu `finish[i] == False` **VÀ** `request[i] <= work` (yêu cầu hiện tại ≤ tài nguyên rảnh):
       - Thu hồi tài nguyên: `work[j] += allocation[i][j]`
       - Đánh dấu `finish[i] = True`
   - Nếu không tìm thấy tiến trình thỏa mãn → thoát vòng lặp

3. **Kết luận:**
   - `all(finish) == True` → **Không có deadlock**
   - Ngược lại → **Có deadlock**, in ra các tiến trình bị deadlock

**Trace qua ví dụ:**

Input:
```
n=3 tiến trình, m=2 loại tài nguyên
Allocation = [[1,0], [0,1], [1,1]]
Request    = [[0,1], [1,0], [1,1]]
Available  = [1,0]
```

| Bước | Work   | Duyệt Pi | Request[i] ≤ Work? | Work mới              |
|------|--------|----------|--------------------|-----------------------|
| 1    | [1, 0] | P0       | [0,1] ≤ [1,0]? ❌ (1>0) | —                |
| 1    | [1, 0] | P1       | [1,0] ≤ [1,0]? ✅ | [1+0, 0+1] = [1, 1]  |
| 2    | [1, 1] | P0       | [0,1] ≤ [1,1]? ✅ | [1+1, 1+0] = [2, 1]  |
| 3    | [2, 1] | P2       | [1,1] ≤ [2,1]? ✅ | [2+1, 1+1] = [3, 2]  |

→ `Finish = [True, True, True]` → **No Deadlock Detected**
→ Safe Sequence: **P1 → P0 → P2**

### Output khi chạy `deadlock_detection.py`

```
Processes: 3
Resources: 2

Allocation Matrix
[1, 0]
[0, 1]
[1, 1]

Request Matrix
[0, 1]
[1, 0]
[1, 1]

Available: [1, 0]

Running Deadlock Detection...

No Deadlock Detected
Safe Sequence: P1 -> P0 -> P2
```

**Giải thích output:**
- In thông tin cơ bản: 3 tiến trình, 2 loại tài nguyên
- Ma trận Allocation: tài nguyên đã cấp cho mỗi tiến trình
- Ma trận Request: tài nguyên mỗi tiến trình đang yêu cầu thêm
- Available = [1, 0]: tài nguyên đang rảnh
- **Kết quả:** Không phát hiện deadlock
  - P1 chạy trước (Request[1]=[1,0] ≤ [1,0]) → thu hồi → Work=[1,1]
  - P0 chạy tiếp (Request[0]=[0,1] ≤ [1,1]) → thu hồi → Work=[2,1]
  - P2 chạy cuối (Request[2]=[1,1] ≤ [2,1]) → thu hồi → Work=[3,2]
  - Thứ tự an toàn: **P1 → P0 → P2**

### Trường hợp 2: Deadlock Detected (file `input_deadlock2.txt`)

Input:
```
n=3 tiến trình, m=3 loại tài nguyên
Allocation = [[1,0,1], [0,1,0], [0,0,1]]
Request    = [[0,1,0], [1,0,1], [1,1,0]]
Available  = [0,0,0]
```

**Trace:**

| Bước | Work      | Duyệt Pi | Request[i] ≤ Work?          | Kết quả   |
|------|-----------|----------|-----------------------------|-----------|
| 1    | [0, 0, 0] | P0       | [0,1,0] ≤ [0,0,0]? ❌ (1>0) | bỏ qua   |
| 1    | [0, 0, 0] | P1       | [1,0,1] ≤ [0,0,0]? ❌ (1>0) | bỏ qua   |
| 1    | [0, 0, 0] | P2       | [1,1,0] ≤ [0,0,0]? ❌ (1>0) | bỏ qua   |

→ Không tìm được tiến trình nào → `break`
→ `Finish = [False, False, False]` → **Deadlock Detected!**
→ Deadlocked Processes: **P0, P1, P2**

**Output:**
```
Processes: 3
Resources: 3

Allocation Matrix
[1, 0, 1]
[0, 1, 0]
[0, 0, 1]

Request Matrix
[0, 1, 0]
[1, 0, 1]
[1, 1, 0]

Available: [0, 0, 0]

Running Deadlock Detection...

Deadlock Detected!
Deadlocked Processes: P0, P1, P2
```

**Giải thích:** Available = [0,0,0] — hệ thống không còn tài nguyên rảnh nào. Tất cả tiến trình đều đang yêu cầu thêm nhưng không ai được đáp ứng → cả 3 tiến trình bị **deadlock** (chờ nhau vô hạn).

---

## 3. SO SÁNH HAI THUẬT TOÁN

| Tiêu chí              | Banker's Algorithm              | Deadlock Detection               |
|------------------------|---------------------------------|----------------------------------|
| **Khi nào dùng**       | Trước khi cấp tài nguyên        | Sau khi đã cấp tài nguyên       |
| **Mục đích**           | Ngăn chặn deadlock (Prevention) | Phát hiện deadlock (Detection)   |
| **Dữ liệu đầu vào**   | Allocation, Max, Total          | Allocation, Request, Available   |
| **Ma trận so sánh**    | **Need** = Max - Allocation     | **Request** (yêu cầu hiện tại)  |
| **Kết quả**            | Safe / Not Safe State           | Deadlock / No Deadlock           |
| **Giống nhau**         | Cùng ý tưởng: giả lập chạy từng tiến trình, thu hồi tài nguyên, kiểm tra tất cả có chạy xong không |                                  |
