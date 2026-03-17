#include <iostream>
#include <vector>
using namespace std;

int main() {
    int nb, np;

    cout << "Memory Management Scheme - Best Fit\n\n";

    cout << "Enter number of blocks: ";
    cin >> nb;

    cout << "Enter number of files: ";
    cin >> np;

    vector<int> blocks(nb);
    vector<int> files(np);

    // nhập block
    cout << "\nEnter size of blocks:\n";
    for (int i = 0; i < nb; i++) {
        cout << "Block " << i + 1 << ": ";
        cin >> blocks[i];
    }

    // nhập file
    cout << "\nEnter size of files:\n";
    for (int i = 0; i < np; i++) {
        cout << "File " << i + 1 << ": ";
        cin >> files[i];
    }

    vector<int> allocation(np, -1); // lưu block gán cho file
    vector<int> fragment(np, 0);    // lưu phần dư
    vector<bool> used(nb, false);   // block đã dùng chưa

    // thuật toán Best Fit
    for (int i = 0; i < np; i++) {
        int bestIdx = -1;
        int minFragment = 1e9;

        for (int j = 0; j < nb; j++) {
            if (!used[j] && blocks[j] >= files[i]) {
                int diff = blocks[j] - files[i];
                if (diff < minFragment) {
                    minFragment = diff;
                    bestIdx = j;
                }
            }
        }

        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            fragment[i] = minFragment;
            used[bestIdx] = true;
        }
    }

    // in kết quả
    cout << "\nFile\tSize\tBlock\tBlockSize\tFragment\n";
    for (int i = 0; i < np; i++) {
        if (allocation[i] != -1) {
            cout << i + 1 << "\t"
                 << files[i] << "\t"
                 << allocation[i] + 1 << "\t"
                 << blocks[allocation[i]] << "\t\t"
                 << fragment[i] << "\n";
        } else {
            cout << i + 1 << "\t"
                 << files[i] << "\t"
                 << "Not Allocated\n";
        }
    }

    return 0;
}