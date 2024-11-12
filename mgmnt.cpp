#include <iostream>
#include <map>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

class Memory {
    int *blocks;
    int *process;
    int *flag;
    int bsize;
    int psize;

public:
    Memory() {
        blocks = nullptr;
        process = nullptr;
        flag = nullptr;
        bsize = 0;
        psize = 0;
    }
    
    ~Memory() {
        cleanup();
    }
    
    void cleanup() {
        delete[] blocks;
        delete[] process;
        delete[] flag;
        blocks = nullptr;
        process = nullptr;
        flag = nullptr;
    }

    void useDefaultValues() {
        cleanup();
        bsize = 5;
        psize = 4;
        
        blocks = new int[bsize];
        process = new int[psize];
        flag = new int[bsize];
        
        // Default block sizes
        blocks[0] = 100;
        blocks[1] = 500;
        blocks[2] = 200;
        blocks[3] = 300;
        blocks[4] = 600;
        
        // Default process sizes
        process[0] = 212;
        process[1] = 417;
        process[2] = 112;
        process[3] = 426;
        
        cout << "\nUsing default values:" << endl;
        displayValues();
    }

    void input() {
        cleanup();
        
        do {
            cout << "Enter the number of blocks (1-10): ";
            cin >> bsize;
        } while (bsize <= 0 || bsize > 10);
        
        do {
            cout << "Enter the number of processes (1-10): ";
            cin >> psize;
        } while (psize <= 0 || psize > 10);
        
        blocks = new int[bsize];
        process = new int[psize];
        flag = new int[bsize];
        
        cout << "\nEnter the size of each memory block:" << endl;
        for (int i = 0; i < bsize; i++) {
            cout << "Block " << i << ": ";
            while (!(cin >> blocks[i]) || blocks[i] <= 0) {
                cout << "Please enter a valid positive number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        
        cout << "\nEnter the size of each process:" << endl;
        for (int i = 0; i < psize; i++) {
            cout << "Process " << i << ": ";
            while (!(cin >> process[i]) || process[i] <= 0) {
                cout << "Please enter a valid positive number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        
        displayValues();
    }

    void displayValues() {
        cout << "\nMemory Blocks:" << endl;
        for (int i = 0; i < bsize; i++) {
            cout << "Block " << i << ": " << blocks[i] << " KB" << endl;
        }
        
        cout << "\nProcesses:" << endl;
        for (int i = 0; i < psize; i++) {
            cout << "Process " << i << ": " << process[i] << " KB" << endl;
        }
        cout << endl;
    }

    void firstfit() {
        if (!validateData()) return;
        
        cout << "\nFirst Fit Allocation:\n" << endl;
        
        fill(flag, flag + bsize, 0);
        map<int, int> mp;
        
        for (int i = 0; i < psize; i++) {
            bool allocated = false;
            for (int j = 0; j < bsize; j++) {
                if (process[i] <= blocks[j] && flag[j] == 0) {
                    mp[j] = process[i];
                    flag[j] = 1;
                    allocated = true;
                    break;
                }
            }
            if (!allocated) {
                cout << "Process " << i << " (" << process[i] << " KB) could not be allocated" << endl;
            }
        }
        
        displayAllocation(mp);
    }

    void bestfit() {
        if (!validateData()) return;
        
        cout << "\nBest Fit Allocation:\n" << endl;
        
        fill(flag, flag + bsize, 0);
        map<int, int> mp;
        
        for (int i = 0; i < psize; i++) {
            int bestFit = -1;
            int minWastage = numeric_limits<int>::max();
            
            for (int j = 0; j < bsize; j++) {
                if (process[i] <= blocks[j] && flag[j] == 0) {
                    int wastage = blocks[j] - process[i];
                    if (wastage < minWastage) {
                        minWastage = wastage;
                        bestFit = j;
                    }
                }
            }
            
            if (bestFit != -1) {
                mp[bestFit] = process[i];
                flag[bestFit] = 1;
            } else {
                cout << "Process " << i << " (" << process[i] << " KB) could not be allocated" << endl;
            }
        }
        
        displayAllocation(mp);
    }

    void worstfit() {
        if (!validateData()) return;
        
        cout << "\nWorst Fit Allocation:\n" << endl;
        
        fill(flag, flag + bsize, 0);
        map<int, int> mp;
        
        for (int i = 0; i < psize; i++) {
            int worstFit = -1;
            int maxWastage = -1;
            
            for (int j = 0; j < bsize; j++) {
                if (process[i] <= blocks[j] && flag[j] == 0) {
                    int wastage = blocks[j] - process[i];
                    if (wastage > maxWastage) {
                        maxWastage = wastage;
                        worstFit = j;
                    }
                }
            }
            
            if (worstFit != -1) {
                mp[worstFit] = process[i];
                flag[worstFit] = 1;
            } else {
                cout << "Process " << i << " (" << process[i] << " KB) could not be allocated" << endl;
            }
        }
        
        displayAllocation(mp);
    }

private:
    bool validateData() {
        if (blocks == nullptr || process == nullptr) {
            cout << "Error: No data available. Please input values first." << endl;
            return false;
        }
        return true;
    }

    void displayAllocation(const map<int, int>& mp) {
        cout << "\nAllocation Results:" << endl;
        cout << setw(10) << "Block No" 
             << setw(20) << "Block Size (KB)"
             << setw(30) << "Process Size Allocated (KB)"
             << setw(20) << "Internal Fragmentation (KB)" << endl;
        cout << string(80, '-') << endl;
        
        for (const auto& it : mp) {
            cout << setw(10) << it.first
                 << setw(20) << blocks[it.first]
                 << setw(30) << it.second
                 << setw(20) << blocks[it.first] - it.second << endl;
        }
        
        // Display unallocated blocks
        for (int i = 0; i < bsize; i++) {
            if (flag[i] == 0) {
                cout << setw(10) << i
                     << setw(20) << blocks[i]
                     << setw(30) << "Not allocated"
                     << setw(20) << "-" << endl;
            }
        }
    }
};

int main() {
    Memory m;
    int choice;
    
    do {
        cout << "\nMemory Allocation Simulator" << endl;
        cout << "1. Use Default Values" << endl;
        cout << "2. Input Custom Values" << endl;
        cout << "3. First Fit Algorithm" << endl;
        cout << "4. Best Fit Algorithm" << endl;
        cout << "5. Worst Fit Algorithm" << endl;
        cout << "6. Display Current Values" << endl;
        cout << "7. Exit" << endl;
        cout << "\nEnter your choice: ";
        
        while (!(cin >> choice)) {
            cout << "Please enter a valid number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        switch (choice) {
            case 1:
                m.useDefaultValues();
                break;
            case 2:
                m.input();
                break;
            case 3:
                m.firstfit();
                break;
            case 4:
                m.bestfit();
                break;
            case 5:
                m.worstfit();
                break;
            case 6:
                m.displayValues();
                break;
            case 7:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 7);
    
    return 0;
}