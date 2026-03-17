#include <iostream>
using namespace std;

int main() {
    static int block_arr[10], file_arr[10];
    int fragments[10], blocks[10], files[10];
    int m, n, number_of_blocks, number_of_files, temp;

    cout << "\nEnter the Total Number of Blocks:\t";
    cin >> number_of_blocks;

    cout << "Enter the Total Number of Files:\t";
    cin >> number_of_files;

    cout << "\nEnter the Size of the Blocks:\n";
    for (m = 0; m < number_of_blocks; m++) {
        cout << "Block No.[" << m + 1 << "]:\t";
        cin >> blocks[m];
    }

    cout << "Enter the Size of the Files:\n";
    for (m = 0; m < number_of_files; m++) {
        cout << "File No.[" << m + 1 << "]:\t";
        cin >> files[m];
    }

    for (m = 0; m < number_of_files; m++) {
        for (n = 0; n < number_of_blocks; n++) {
            if (block_arr[n] != 1) {
                temp = blocks[n] - files[m];
                if (temp >= 0) {
                    file_arr[m] = n;
                    break;
                }
            }
        }
        fragments[m] = temp;
        block_arr[file_arr[m]] = 1;
    }

    cout << "\nFile Number\tBlock Number\tFile Size\tBlock Size\tFragment";
    for (m = 0; m < number_of_files; m++) {
        cout << "\n" << m
             << "\t\t" << file_arr[m]
             << "\t\t" << files[m]
             << "\t\t" << blocks[file_arr[m]]
             << "\t\t" << fragments[m];
    }

    cout << "\n";
    return 0;
}