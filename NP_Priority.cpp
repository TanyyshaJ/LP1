#include <iostream>
#include <iomanip>
#include <climits>

using namespace std;

class Process {
public:
    int id;
    int burst;
    int arrival;
    int priority; // Lower Number -> Higher Priority
};

void npPriority(Process processes[], int n, int wt[], int ct[], int tat[]) {
    int* remainingTime = new int[n];
    
    for (int i = 0; i < n; i++)
        remainingTime[i] = processes[i].burst;
    
    int currentTime = 0, completedTasks = 0, HPT = 0, maxPriority = INT_MAX;
    bool taskActive = false;
    
    while (completedTasks != n) {
        for (int j = 0; j < n; j++) {
            if (processes[j].arrival <= currentTime && remainingTime[j] > 0 && processes[j].priority < maxPriority) {
                HPT = j;
                maxPriority = processes[j].priority;
                taskActive = true;
            }
        }
        
        if (!taskActive) {
            currentTime++;
            continue;
        }
        
        currentTime += processes[HPT].burst;
        remainingTime[HPT] = 0;
        maxPriority = INT_MAX;
        taskActive = false;
        completedTasks++;
        wt[HPT] = currentTime - processes[HPT].arrival - processes[HPT].burst;
        ct[HPT] = currentTime;
        tat[HPT] = ct[HPT] - processes[HPT].arrival;
    }
    
    delete[] remainingTime;
}

void displayResults(Process proc[], int n, int ct[], int tat[], int wt[]) {
    cout << "\nProcess Schedule Results:" << endl;
    cout << setw(5) << "ID" << setw(5) << "AT" << setw(5) << "BT" << setw(5) << "PT"
         << setw(5) << "CT" << setw(5) << "TAT" << setw(5) << "WT" << endl;
    
    for (int i = 0; i < n; i++) {
        cout << setw(5) << proc[i].id;
        cout << setw(5) << proc[i].arrival;
        cout << setw(5) << proc[i].burst;
        cout << setw(5) << proc[i].priority;
        cout << setw(5) << ct[i];
        cout << setw(5) << tat[i];
        cout << setw(5) << wt[i] << endl;
    }
    
    double total_wt = 0, total_tat = 0;
    for(int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
    }
    double avg_wt = total_wt / n;
    double avg_tat = total_tat / n;
    
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avg_wt << endl;
    cout << "Average Turnaround Time: " << avg_tat << endl;
}

void useDefaultValues(Process proc[], int& n) {
    Process defaultProc[] = {
        {1, 11, 0, 2},
        {2, 28, 5, 0},
        {3, 2, 12, 3},
        {4, 10, 2, 1},
        {5, 16, 9, 4}
    };
    n = 5;
    for(int i = 0; i < n; i++) {
        proc[i] = defaultProc[i];
    }
}

void getUserInput(Process proc[], int& n) {
    cout << "Enter number of processes (max 10): ";
    cin >> n;
    while(n <= 0 || n > 10) {
        cout << "Please enter a valid number (1-10): ";
        cin >> n;
    }
    
    for(int i = 0; i < n; i++) {
        proc[i].id = i + 1;
        cout << "\nFor Process " << proc[i].id << ":" << endl;
        cout << "Enter Arrival Time: ";
        cin >> proc[i].arrival;
        cout << "Enter Burst Time: ";
        cin >> proc[i].burst;
        cout << "Enter Priority (0 is highest): ";
        cin >> proc[i].priority;
    }
}

int main() {
    Process proc[10];  // Maximum 10 processes
    int n;
    int choice;
    
    do {
        cout << "\nPriority Non-preemptive Scheduling" << endl;
        cout << "1. Use Default Values" << endl;
        cout << "2. Enter Custom Values" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
            case 2: {
                if(choice == 1) {
                    useDefaultValues(proc, n);
                } else {
                    getUserInput(proc, n);
                }
                
                int* wt = new int[n];
                int* ct = new int[n];
                int* tat = new int[n];
                
                npPriority(proc, n, wt, ct, tat);
                displayResults(proc, n, ct, tat, wt);
                
                delete[] wt;
                delete[] ct;
                delete[] tat;
                break;
            }
            case 3:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while(choice != 3);
    
    return 0;
}