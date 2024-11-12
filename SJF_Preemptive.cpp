#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
using namespace std;

class Process {
public:
    int id;
    int arrival;
    int burst;
    int completion;
    int turnaround;
    int waiting;
    int remaining;
    bool completed;

    // Default constructor
    Process() {
        id = 0;
        arrival = 0;
        burst = 0;
        completion = 0;
        turnaround = 0;
        waiting = 0;
        remaining = 0;
        completed = false;
    }

    // Parameterized constructor
    Process(int pid, int at, int bt) {
        id = pid;
        arrival = at;
        burst = bt;
        completion = 0;
        turnaround = 0;
        waiting = 0;
        remaining = bt;
        completed = false;
    }
};

void SJFPreemptive(Process processes[], int n) {
    int currentTime = 0;
    int completedProcesses = 0;
    
    // Initialize remaining time for each process
    for(int i = 0; i < n; i++) {
        processes[i].remaining = processes[i].burst;
    }
    
    while(completedProcesses != n) {
        int minRemaining = numeric_limits<int>::max();
        int selectedProcess = -1;
        
        for(int i = 0; i < n; i++) {
            if(processes[i].arrival <= currentTime && !processes[i].completed) {
                if(processes[i].remaining < minRemaining) {
                    minRemaining = processes[i].remaining;
                    selectedProcess = i;
                }
                else if(processes[i].remaining == minRemaining) {
                    if(processes[i].arrival < processes[selectedProcess].arrival) {
                        selectedProcess = i;
                    }
                }
            }
        }
        
        if(selectedProcess == -1) {
            currentTime++;
            continue;
        }
        
        processes[selectedProcess].remaining--;
        currentTime++;
        
        if(processes[selectedProcess].remaining == 0) {
            processes[selectedProcess].completed = true;
            processes[selectedProcess].completion = currentTime;
            processes[selectedProcess].turnaround = processes[selectedProcess].completion - 
                                                  processes[selectedProcess].arrival;
            processes[selectedProcess].waiting = processes[selectedProcess].turnaround - 
                                               processes[selectedProcess].burst;
            completedProcesses++;
        }
    }
}

void displayResults(Process processes[], int n) {
    float avgWaiting = 0, avgTurnaround = 0;
    
    cout << "\nProcess Details:\n";
    cout << setw(5) << "ID" 
         << setw(5) << "AT" 
         << setw(5) << "BT" 
         << setw(5) << "CT" 
         << setw(5) << "TAT" 
         << setw(5) << "WT" << endl;
         
    for(int i = 0; i < n; i++) {
        cout << setw(5) << processes[i].id
             << setw(5) << processes[i].arrival
             << setw(5) << processes[i].burst
             << setw(5) << processes[i].completion
             << setw(5) << processes[i].turnaround
             << setw(5) << processes[i].waiting << endl;
             
        avgWaiting += processes[i].waiting;
        avgTurnaround += processes[i].turnaround;
    }
    
    avgWaiting /= n;
    avgTurnaround /= n;
    
    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avgWaiting << endl;
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << avgTurnaround << endl;
}

void inputProcesses(Process processes[], int n) {
    cout << "\nEnter process details:\n";
    for(int i = 0; i < n; i++) {
        int at, bt;
        cout << "\nProcess " << i + 1 << ":\n";
        cout << "Enter Arrival Time: ";
        cin >> at;
        cout << "Enter Burst Time: ";
        cin >> bt;
        processes[i] = Process(i + 1, at, bt);
    }
}

void setDefaultProcesses(Process processes[], int n) {
    // Initialize default processes using the constructor
    Process defaultProc[] = {
        Process(1, 0, 7),  // id, arrival, burst
        Process(2, 2, 4),
        Process(3, 4, 1),
        Process(4, 5, 4),
        Process(5, 6, 2)
    };
    
    for(int i = 0; i < n; i++) {
        processes[i] = defaultProc[i];
    }
}

int main() {
    int choice;
    const int DEFAULT_SIZE = 5;
    
    do {
        cout << "\nSJF Preemptive CPU Scheduler\n";
        cout << "1. Run with user input\n";
        cout << "2. Run with default example\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1: {
                int n;
                cout << "\nEnter number of processes: ";
                cin >> n;
                
                Process* processes = new Process[n];
                
                inputProcesses(processes, n);
                SJFPreemptive(processes, n);
                displayResults(processes, n);
                
                delete[] processes;
                break;
            }
            case 2: {
                Process* processes = new Process[DEFAULT_SIZE];
                
                setDefaultProcesses(processes, DEFAULT_SIZE);
                cout << "\nRunning default example:\n";
                SJFPreemptive(processes, DEFAULT_SIZE);
                displayResults(processes, DEFAULT_SIZE);
                
                delete[] processes;
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