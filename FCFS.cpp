#include<iostream>
#include<iomanip>  
using namespace std;

class Process {
public:
    int id;
    int burst;
    int arrival;
    
    // Default constructor
    Process() {
        id = 0;
        burst = 0;
        arrival = 0;
    }
    
    // Parameterized constructor
    Process(int pid, int bt, int at) {
        id = pid;
        burst = bt;
        arrival = at;
    }
};

void sortByArrival(Process processes[], int n) {
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - i - 1; j++) {
            if(processes[j].arrival > processes[j + 1].arrival) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

void FCFS(Process processes[], int n, int wt[], int ct[], int tat[]) {
    int wait = 0;
    for(int i = 0; i < n; i++) {
        // If there's a gap between processes
        if(wait < processes[i].arrival) {
            wait = processes[i].arrival;
        }
        wt[i] = wait - processes[i].arrival;
        ct[i] = wait + processes[i].burst;
        tat[i] = ct[i] - processes[i].arrival;
        wait += processes[i].burst;
    }
}

void displayResults(Process processes[], int n, int wt[], int ct[], int tat[]) {
    cout << "\nProcess Details:\n";
    cout << setw(5) << "ID" << setw(5) << "AT" << setw(5) << "BT" << setw(5) << "CT" 
         << setw(5) << "TAT" << setw(5) << "WT" << endl;

    for(int i = 0; i < n; i++) {
        cout << setw(5) << processes[i].id
             << setw(5) << processes[i].arrival
             << setw(5) << processes[i].burst
             << setw(5) << ct[i]
             << setw(5) << tat[i]
             << setw(5) << wt[i] << endl;
    }

    // Calculate average waiting time and turnaround time
    double total_wt = 0, total_tat = 0;
    for(int i = 0; i < n; i++) {
        total_wt += wt[i];
        total_tat += tat[i];
    }
    double avg_wt = total_wt / n;
    double avg_tat = total_tat / n;

    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avg_wt << endl;
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << avg_tat << endl;
}

void inputProcesses(Process processes[], int n) {
    cout << "\nEnter process details:\n";
    for(int i = 0; i < n; i++) {
        cout << "\nProcess " << i + 1 << ":\n";
        processes[i].id = i + 1;
        cout << "Enter Arrival Time: ";
        cin >> processes[i].arrival;
        cout << "Enter Burst Time: ";
        cin >> processes[i].burst;
    }
}

void setDefaultProcesses(Process processes[], int n) {
    Process defaultProc[] = {
        Process(1, 6, 2),  // id, burst, arrival
        Process(2, 2, 5),
        Process(3, 8, 1),
        Process(4, 3, 0),
        Process(5, 4, 4)
    };
    
    for(int i = 0; i < n; i++) {
        processes[i] = defaultProc[i];
    }
}

int main() {
    int choice;
    const int DEFAULT_SIZE = 5;
    
    do {
        cout << "\nFirst Come First Serve (FCFS) CPU Scheduler\n";
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
                
                // Dynamically allocate arrays
                Process* processes = new Process[n];
                int* wt = new int[n];
                int* ct = new int[n];
                int* tat = new int[n];
                
                inputProcesses(processes, n);
                sortByArrival(processes, n);
                FCFS(processes, n, wt, ct, tat);
                displayResults(processes, n, wt, ct, tat);
                
                // Free memory
                delete[] processes;
                delete[] wt;
                delete[] ct;
                delete[] tat;
                break;
            }
            case 2: {
                // Create arrays for default example
                Process* processes = new Process[DEFAULT_SIZE];
                int* wt = new int[DEFAULT_SIZE];
                int* ct = new int[DEFAULT_SIZE];
                int* tat = new int[DEFAULT_SIZE];
                
                setDefaultProcesses(processes, DEFAULT_SIZE);
                cout << "\nRunning default example:\n";
                sortByArrival(processes, DEFAULT_SIZE);
                FCFS(processes, DEFAULT_SIZE, wt, ct, tat);
                displayResults(processes, DEFAULT_SIZE, wt, ct, tat);
                
                // Free memory
                delete[] processes;
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