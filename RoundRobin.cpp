#include <iostream>
#include <queue>
#include <iomanip>
using namespace std;

class Process {
public:
    int id;
    int burst;
    int arrival;
};

void RoundRobin(Process processes[], int n, int ts, int wt[], int ct[], int tat[]) {
    queue<int> RQ;
    int currentTime = 0, completedTasks = 0;
    // Dynamically allocate remainingTime array
    int* remainingTime = new int[n];
    int curr;
    
    for (int j = 0; j < n; j++) {
        if (processes[j].arrival <= currentTime) {
            RQ.push(j);
        }
    }
    
    for (int i = 0; i < n; i++)
        remainingTime[i] = processes[i].burst;
        
    while (completedTasks != n) {
        curr = RQ.front();
        RQ.pop();
        int cntr = 0;
        
        while (cntr != ts && cntr != remainingTime[curr]) {
            currentTime++;
            cntr++;
            for (int j = 0; j < n; j++) {
                if (processes[j].arrival == currentTime)
                    RQ.push(j);
            }
        }
        
        remainingTime[curr] -= cntr;
        
        if (remainingTime[curr] == 0) {
            ct[curr] = currentTime;
            tat[curr] = ct[curr] - processes[curr].arrival;
            completedTasks++;
            wt[curr] = ct[curr] - processes[curr].arrival - processes[curr].burst;
        } else {
            RQ.push(curr);
        }
    }
    
    // Free the dynamically allocated memory
    delete[] remainingTime;
}

void displayResults(Process proc[], int n, int wt[], int ct[], int tat[]) {
    float avgWaitingTime = 0, avgTurnaroundTime = 0;
    
    cout << "\nProcess Details:\n";
    cout << setw(5) << "ID" 
         << setw(5) << "AT" 
         << setw(5) << "BT" 
         << setw(5) << "CT" 
         << setw(5) << "TAT" 
         << setw(5) << "WT" << endl;
    
    for (int i = 0; i < n; i++) {
        cout << setw(5) << proc[i].id
             << setw(5) << proc[i].arrival
             << setw(5) << proc[i].burst
             << setw(5) << ct[i]
             << setw(5) << tat[i]
             << setw(5) << wt[i] << endl;
             
        avgWaitingTime += wt[i];
        avgTurnaroundTime += tat[i];
    }
    
    avgWaitingTime /= n;
    avgTurnaroundTime /= n;
    
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avgWaitingTime << endl;
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << avgTurnaroundTime << endl;
}

void inputProcesses(Process proc[], int n) {
    cout << "\nEnter process details:\n";
    for(int i = 0; i < n; i++) {
        cout << "\nProcess " << i+1 << ":\n";
        proc[i].id = i+1;
        cout << "Enter Arrival Time: ";
        cin >> proc[i].arrival;
        cout << "Enter Burst Time: ";
        cin >> proc[i].burst;
    }
}

int main() {
    int choice;
    const int DEFAULT_SIZE = 5;  // Define constant for default example size
    
    do {
        cout << "\nRound Robin CPU Scheduler\n";
        cout << "1. Run with user input\n";
        cout << "2. Run with default example\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1: {
                int n, timeSlice;
                cout << "\nEnter number of processes: ";
                cin >> n;
                cout << "Enter time quantum: ";
                cin >> timeSlice;
                
                Process *proc = new Process[n];
                int *wt = new int[n]();
                int *ct = new int[n];
                int *tat = new int[n];
                
                inputProcesses(proc, n);
                RoundRobin(proc, n, timeSlice, wt, ct, tat);
                displayResults(proc, n, wt, ct, tat);
                
                delete[] proc;
                delete[] wt;
                delete[] ct;
                delete[] tat;
                break;
            }
            case 2: {
                Process *proc = new Process[DEFAULT_SIZE];
                int *wt = new int[DEFAULT_SIZE]();
                int *ct = new int[DEFAULT_SIZE];
                int *tat = new int[DEFAULT_SIZE];
                
                // Initialize default example
                proc[0] = {1, 8, 0};
                proc[1] = {2, 2, 5};
                proc[2] = {3, 7, 1};
                proc[3] = {4, 3, 6};
                proc[4] = {5, 5, 8};
                
                cout << "\nRunning with default example (Time Quantum = 3):\n";
                RoundRobin(proc, DEFAULT_SIZE, 3, wt, ct, tat);
                displayResults(proc, DEFAULT_SIZE, wt, ct, tat);
                
                delete[] proc;
                delete[] wt;
                delete[] ct;
                delete[] tat;
                break;
            }
            case 3:
                cout << "\nExiting program...\n";
                break;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
    } while(choice != 3);
    
    return 0;
}