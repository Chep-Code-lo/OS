#ifndef __SHORTEST__REMAINING__TIME
#define __SHORTEST__REMAINING__TIME

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"

/*
    INDENT: thụt 4 spaces (KHÔNG dùng \t vì tab độ rộng không cố định -> dễ lệch cột)
    TICK_W: 1 đơn vị thời gian (1 tick) biểu diễn bằng 2 ký tự "--" trên Gantt
*/
#define INDENT "    "
#define TICK_W 2

/*
    GSeg = 1 "đoạn" chạy liên tục của 1 process (hoặc IDLE)
    - label: chữ in trong block (vd: "P1", "P2", hoặc "ID")
    - s: thời điểm bắt đầu đoạn
    - e: thời điểm kết thúc đoạn (exclusive)
*/
typedef struct {
    const char *label; // p[i].id hoặc "ID"
    int s;             // start time
    int e;             // end time (exclusive)
} GSeg;

/* ===================== GANTT PRINT (ĐẸP, KHÔNG LỆCH) ===================== */

/*
    Đặt số x vào buffer sao cho CHỮ SỐ CUỐI của x nằm đúng cột 'col'
    -> giúp số 1 chữ số (9) hay 2 chữ số (14) vẫn thẳng hàng dưới dấu '|'
*/
static void put_number_right(char *buf, int buflen, int col, int x)
{
    char tmp[16];
    snprintf(tmp, sizeof(tmp), "%d", x);
    int L = (int)strlen(tmp);

    // start = vị trí đặt chữ số đầu tiên của tmp (canh phải tại col)
    int start = col - (L - 1);
    if (start < 0) start = 0;
    if (start + L > buflen) return;

    for (int i = 0; i < L; i++) buf[start + i] = tmp[i];
}

/*
    In Gantt 4 dòng giống mẫu:
    1) bar trên  : ----- ---- --
    2) dòng giữa : | P1 | P2 |...
    3) bar dưới  : ----- ---- --
    4) timeline  : 0    7   9  10 14 (đặt đúng dưới dấu |)
*/
static void print_gantt_pretty(const GSeg *seg, int seg_cnt)
{
    /*
        body_len = độ dài phần "thân" dòng giữa tính theo ký tự
        Format dòng giữa: | [w] | [w] | ...
        - có 1 dấu '|' đầu tiên
        - mỗi segment đóng góp: w ký tự nội dung + 1 dấu '|'
        - w = (e-s) * TICK_W  (mỗi tick 2 ký tự)
    */
    int body_len = 1;
    for (int i = 0; i < seg_cnt; i++)
    {
        int w = (seg[i].e - seg[i].s) * TICK_W;
        body_len += w + 1;
    }

    /* ---- line 1: bar top ---- */
    printf(INDENT);
    for (int i = 0; i < seg_cnt; i++)
    {
        int w = (seg[i].e - seg[i].s) * TICK_W;

        // in '-' w lần
        for (int j = 0; j < w; j++) putchar('-');

        // in space giữa các segment để nhìn tách block rõ hơn
        if (i != seg_cnt - 1) putchar(' ');
    }
    printf("\n");

    /*
        line 2: in | P1 | P2 | ...
        Đồng thời lưu vị trí cột của từng dấu '|'
        bar_col[i] = cột của dấu '|' thứ i (tính từ đầu phần sau INDENT)
        -> dùng để đặt số timeline xuống đúng dưới dấu '|'
    */
    int *bar_col = (int *)malloc(sizeof(int) * (seg_cnt + 1));
    int col = 0; // cột hiện tại (sau INDENT)

    printf(INDENT);
    putchar('|');
    bar_col[0] = col; // dấu | đầu ở cột 0
    col += 1;

    for (int i = 0; i < seg_cnt; i++)
    {
        int w = (seg[i].e - seg[i].s) * TICK_W;
        const char *label = seg[i].label ? seg[i].label : "??";

        /*
            Canh giữa label 2 ký tự (vd "P1") trong block rộng w
            left  = số spaces bên trái
            right = số spaces bên phải
        */
        int left  = (w - 2) / 2;
        int right = w - 2 - left;

        for (int j = 0; j < left; j++) putchar(' ');
        printf("%-2.2s", label); // lấy đúng 2 ký tự đầu để đều cột
        for (int j = 0; j < right; j++) putchar(' ');

        putchar('|');

        // dấu | kết thúc segment i nằm tại (col + w)
        bar_col[i + 1] = col + w;

        // cập nhật col: đã in w nội dung + 1 dấu |
        col += w + 1;
    }
    printf("\n");

    /* ---- line 3: bar bottom (y hệt bar top) ---- */
    printf(INDENT);
    for (int i = 0; i < seg_cnt; i++)
    {
        int w = (seg[i].e - seg[i].s) * TICK_W;
        for (int j = 0; j < w; j++) putchar('-');
        if (i != seg_cnt - 1) putchar(' ');
    }
    printf("\n");

    /*
        line 4: timeline
        - tạo 1 buffer toàn dấu cách
        - đặt số tại đúng các cột bar_col[]
        - mốc đầu: seg[0].s (thường 0)
        - mốc sau: seg[i-1].e (end time của segment trước)
    */
    int buf_len = body_len + 32; // chừa thêm để chứa số 2-3 chữ số
    char *timebuf = (char *)malloc(buf_len);
    memset(timebuf, ' ', buf_len);
    timebuf[buf_len - 1] = '\0';

    // mốc đầu (thường là 0) đặt dưới dấu | đầu
    put_number_right(timebuf, buf_len, bar_col[0], seg[0].s);

    // các mốc sau đặt dưới các dấu | còn lại
    for (int i = 1; i <= seg_cnt; i++)
        put_number_right(timebuf, buf_len, bar_col[i], seg[i - 1].e);

    // cắt bớt spaces cuối cho gọn
    int end = buf_len - 2;
    while (end > 0 && timebuf[end] == ' ') end--;
    timebuf[end + 1] = '\0';

    printf(INDENT "%s\n", timebuf);

    free(timebuf);
    free(bar_col);
}

/* ===================== SRT CORE (ĐÚNG + CÓ IDLE) ===================== */

/*
    pick_srt: chọn process theo SRT tại thời điểm t
    Rule:
    - chỉ chọn process chưa completed và đã arrive (arrive_time <= t)
    - ưu tiên remaining nhỏ nhất
    - tie-break: arrive_time nhỏ hơn, nếu vẫn bằng thì index nhỏ hơn
    Return:
    - index process k
    - hoặc -1 nếu tại thời điểm t chưa có process nào arrive => CPU IDLE
*/
static int pick_srt(Process *p, int *remain, int len, int t)
{
    int k = -1;
    int best = INT_MAX;

    for (int i = 0; i < len; i++)
    {
        if (!p[i].completed && p[i].arrive_time <= t)
        {
            if (remain[i] < best)
            {
                best = remain[i];
                k = i;
            }
            else if (remain[i] == best && k != -1)
            {
                if (p[i].arrive_time < p[k].arrive_time) k = i;
                else if (p[i].arrive_time == p[k].arrive_time && i < k) k = i;
            }
        }
    }
    return k; // -1 nếu CPU idle
}

/*
    srt_calculate_time: mô phỏng SRT theo từng tick (1 đơn vị thời gian)
    Tính ra:
    - response_time: (lần đầu được CPU) - arrive_time
    - return_time: thời điểm hoàn thành
    - waiting_time: return - arrive - burst
*/
void srt_calculate_time(Process *p, int len)
{
    int *remain = (int *)malloc(sizeof(int) * len);     // remaining burst của mỗi process
    bool *started = (bool *)malloc(sizeof(bool) * len); // đánh dấu process đã chạy lần đầu chưa

    for (int i = 0; i < len; i++)
    {
        remain[i] = p[i].burst;
        started[i] = false;
    }

    int t = 0;              // thời gian hiện tại
    int completed_cnt = 0;  // số process đã xong

    while (completed_cnt < len)
    {
        int k = pick_srt(p, remain, len, t);

        // Nếu chưa process nào đến -> CPU rảnh 1 tick
        if (k == -1)
        {
            t++;
            continue;
        }

        // Lần đầu process k được chạy -> tính response_time chuẩn
        if (!started[k])
        {
            started[k] = true;
            p[k].response_time = t - p[k].arrive_time;
        }

        // Chạy process k đúng 1 tick
        remain[k]--;
        t++;

        // Nếu chạy xong
        if (remain[k] == 0)
        {
            p[k].completed = true;
            completed_cnt++;

            p[k].return_time = t; // finish time
            p[k].waiting_time = p[k].return_time - p[k].arrive_time - p[k].burst;
            if (p[k].waiting_time < 0) p[k].waiting_time = 0;
        }
    }

    free(started);
    free(remain);
}

/*
    srt_print_gantt_chart:
    - không dùng dữ liệu p[i].completed (vì srt_calculate_time đã set)
    - mô phỏng lại SRT chỉ để build các segment liên tục (GSeg)
    - rồi in bằng print_gantt_pretty()
*/
void srt_print_gantt_chart(Process *p, int len)
{
    int *remain = (int *)malloc(sizeof(int) * len); // remaining burst cho mô phỏng in gantt
    bool *done = (bool *)malloc(sizeof(bool) * len); // trạng thái done riêng (không đụng p[i].completed)

    for (int i = 0; i < len; i++)
    {
        remain[i] = p[i].burst;
        done[i] = false;
    }

    int seg_cap = 16, seg_cnt = 0;
    GSeg *segs = (GSeg *)malloc(sizeof(GSeg) * seg_cap);

    int t = 0;
    int completed_cnt = 0;
    int last = -999; // pid chạy ở tick trước (để biết khi nào đổi segment)

    while (completed_cnt < len)
    {
        // chọn k theo done[] + arrive_time, remaining nhỏ nhất (y hệt logic SRT)
        int k = -1;
        int best = INT_MAX;

        for (int i = 0; i < len; i++)
        {
            if (!done[i] && p[i].arrive_time <= t)
            {
                if (remain[i] < best)
                {
                    best = remain[i];
                    k = i;
                }
                else if (remain[i] == best && k != -1)
                {
                    if (p[i].arrive_time < p[k].arrive_time) k = i;
                    else if (p[i].arrive_time == p[k].arrive_time && i < k) k = i;
                }
            }
        }

        /*
            Nếu pid đổi (k != last) -> mở segment mới:
            - label = "ID" nếu idle, ngược lại label = p[k].id
            - segment bắt đầu tại t
        */
        if (k != last)
        {
            if (seg_cnt == seg_cap)
            {
                seg_cap *= 2;
                segs = (GSeg *)realloc(segs, sizeof(GSeg) * seg_cap);
            }
            segs[seg_cnt].label = (k == -1) ? "ID" : p[k].id;
            segs[seg_cnt].s = t;
            segs[seg_cnt].e = t;
            seg_cnt++;
            last = k;
        }

        // tick này thuộc segment hiện tại
        segs[seg_cnt - 1].e++;

        // Nếu idle -> tăng thời gian rồi tiếp tục
        if (k == -1)
        {
            t++;
            continue;
        }

        // chạy process k 1 tick
        remain[k]--;
        t++;

        // nếu xong -> đánh dấu done
        if (remain[k] == 0)
        {
            done[k] = true;
            completed_cnt++;
        }
    }

    // in gantt theo segment
    print_gantt_pretty(segs, seg_cnt);

    free(segs);
    free(done);
    free(remain);
}

/* ===================== WRAPPER ===================== */

void SRT(Process *p, int len)
{
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    int total_response_time = 0;

    // reset trường thời gian
    process_init(p, len);

    // sort theo arrive_time để thuật toán ổn định (đặc biệt tie-break)
    quick_sort_by_arrive_time(p, len);

    // tính thời gian theo SRT
    srt_calculate_time(p, len);

    // tổng hợp + tính turnaround
    for (int i = 0; i < len; i++)
    {
        p[i].turnaround_time = p[i].return_time - p[i].arrive_time;
        total_waiting_time += p[i].waiting_time;
        total_turnaround_time += p[i].turnaround_time;
        total_response_time += p[i].response_time;
    }

    printf("\tSJF Preemptive (Shortest Remaining Time)\n\n");

    // in gantt đẹp
    srt_print_gantt_chart(p, len);

    // in averages
    printf("\n");
    printf(INDENT "Average Waiting Time     : %.2lf\n", (double)total_waiting_time / (double)len);
    printf(INDENT "Average Turnaround Time  : %.2lf\n", (double)total_turnaround_time / (double)len);
    printf(INDENT "Average Response Time    : %.2lf\n\n", (double)total_response_time / (double)len);

    // in bảng process
    print_table(p, len);
}

#endif