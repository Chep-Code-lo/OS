#include <iostream>

using namespace std;

#define FRAMES_NUMBER 3

int LRU(int pages[], int size)
{
    // 0) Initialize Frames With '-1'
    int frames[FRAMES_NUMBER];
    for (int i = 0; i < FRAMES_NUMBER; i++)
        frames[i] = -1;

    int paeFaults = 0; // Count The Page Faults

    // Go Through All Pages
    for (int pageIndex = 0; pageIndex < size; pageIndex++)
    {

        // 1) Find Page In The Frames
        bool isFound = false;
        for (int i = 0; i < FRAMES_NUMBER; i++)
            if (frames[i] == pages[pageIndex])
            {
                isFound = true;
                // Printing
                cout << pages[pageIndex] << endl;
                break;
            }

        // If Not Found
        if (!isFound)
        {
            // 2) Find A Free Frame
            bool hasFreeFrame = false;
            for (int i = 0; i < FRAMES_NUMBER; i++)
                if (frames[i] == -1)
                {
                    hasFreeFrame = true;
                    frames[i] = pages[pageIndex];
                    paeFaults++;

                    // Printing
                    cout << pages[pageIndex] << "\t\t";
                    for (int f = 0; f < FRAMES_NUMBER; f++)
                        cout << frames[f] << "\t";
                    cout << endl;

                    break;
                }

            // 3) Page Replacement (Not Found & No Free Frame)
            if (!hasFreeFrame)
            {
                // Array To Store The Last Index For Each Page In The Frames
                int lastUse[FRAMES_NUMBER];

                // Assign Last Index For Each Page In The Frames
                for (int i = 0; i < FRAMES_NUMBER; i++)
                    for (int p = pageIndex; p >= 0; p--)
                        if (pages[p] == frames[i])
                        {
                            lastUse[i] = p;
                            break;
                        }

                // Find The Victim Frame (With The Lowest Index)
                int victim = 0;
                for (int i = 0; i < FRAMES_NUMBER; i++)
                    if (lastUse[i] < lastUse[victim])
                        victim = i;

                frames[victim] = pages[pageIndex];
                paeFaults++;

                // Printing
                cout << pages[pageIndex] << "\t\t";
                for (int f = 0; f < FRAMES_NUMBER; f++)
                    cout << frames[f] << "\t";
                cout << endl;
            }
        }
    }
    return paeFaults;
}

int main()
{
    int pages[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};

    cout << "Number Of Page Faults = \n" << LRU(pages, 5);

    getchar();
    return 0;
}