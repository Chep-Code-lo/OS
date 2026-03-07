#include "CpuSchedulingAlgorithms.h"
#include <fstream>

void print_menu() {
    cout << "\n"
         << "Chon thuat toan de chay:\n\n"
         << "  1. FCFS (First Come First Served)\n"
         << "  2. SJF - Non-preemptive\n"
         << "  3. SJF - Preemptive (SRT)\n"
         << "  4. Round Robin\n"
         << "  5. Priority - Non-preemptive\n"
         << "  6. Priority - Preemptive\n"
         << "  7. Priority with Round Robin\n"
         << "  8. Multilevel Feedback Queue\n"
         << "  9. Chay tat ca cac thuat toan\n"
         << "  0. Thoat\n\n"
         << "Nhap lua chon cua ban: ";
}
void run_one(int c, const Processes &original, Quantum quantum) {
    Processes p = original;
    cout << "\n";
    switch (c) {
        case 1: FCFS(p);              break;
        case 2: SJF(p);               break;
        case 3: SRT(p);               break;
        case 4: RR(p, quantum);       break;
        case 5: NPPS(p);              break;
        case 6: PPS(p);               break;
        case 7: PRR(p, quantum);      break;
        case 8: MLFQ(p);              break;
        default: cout << "\nLua chon khong hop le!\n"; return;
    }
}
void run_algorithm(int choice, const Processes &original, Quantum quantum) {
    if (choice == 9) {
        cout << "\n=== CHAY TAT CA CAC THUAT TOAN ===\n";
        for (int i = 1; i <= 8; i++)
            run_one(i, original, quantum);
    } else {
        run_one(choice, original, quantum);
    }
}
int main() {
    ifstream fin("process.txt");
    if (!fin) {
        cout << "FILE OPEN ERROR!\nKhong tim thay file process.txt\n";
        system("pause");
        return 1;
    }
    int n;
    fin >> n;
    Processes process(n);
    for (auto &p : process)
        fin >> p.id >> p.arrive_time >> p.burst >> p.priority;
    Quantum quantum;
    fin >> quantum;
    fin.close();
    int choice;
    while (true) {
        print_menu();
        cin >> choice;
        if (choice == 0) {
            cout << "Cam on ban da su dung chuong trinh!\n";
            break;
        }
        run_algorithm(choice, process, quantum);
        cout << "\nNhan Enter de tiep tuc...";
        cin.ignore();
        cin.get();
    }
    return 0;
}
