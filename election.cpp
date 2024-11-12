#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>
#include <string>
using namespace std;

class Process {
public:
    int id;
    string pname;
    Process() {
        id = 0;
        pname = "";
    }
    friend class Election;
};

class Election {
private:
    Process* p;
    int* flag;
    int num;

    int highest(Process p[]) {
        int pos = 0, max = 0;
        for (int i = 0; i < num; i++) {
            if (p[i].id > max) {
                max = p[i].id;
                pos = i;
            }
        }
        return pos;
    }

    int find_position(int id) {
        for (int i = 0; i < num; i++) {
            if (p[i].id == id)
                return i;
        }
        return -1;  // Return -1 if not found
    }

    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    Election() : p(nullptr), flag(nullptr), num(0) {}
    
    ~Election() {
        delete[] p;
        delete[] flag;
    }

    void input() {
        cout << "\n=== Process Input ===" << endl;
        cout << "Enter number of processes: ";
        while (!(cin >> num) || num <= 0) {
            cout << "Invalid input. Please enter a positive number: ";
            clearInputBuffer();
        }

        p = new Process[num];
        flag = new int[num];

        for (int i = 0; i < num; i++) {
            cout << "\nProcess " << i + 1 << ":" << endl;
            cout << "Enter process name: ";
            clearInputBuffer();
            getline(cin, p[i].pname);
            
            cout << "Enter process ID: ";
            while (!(cin >> p[i].id) || p[i].id < 0) {
                cout << "Invalid ID. Please enter a non-negative number: ";
                clearInputBuffer();
            }
        }

        int pos = highest(p);
        cout << "\nProcess named '" << p[pos].pname << "' has crashed. ID: " << p[pos].id << endl;
    }

    Process bully(Process coordinator) {
        if (!p) {
            cout << "Error: No processes initialized!" << endl;
            return coordinator;
        }

        for (int i = 0; i < num; i++)
            flag[i] = 0;

        int crashed = highest(p);
        flag[crashed] = 1;

        cout << "\n=== Bully Algorithm Execution ===" << endl;
        for (int i = 0; i < num; i++) {
            if (p[i].id > coordinator.id) 
                cout << "Election message sent from Process " << coordinator.pname 
                     << " (ID: " << coordinator.id << ") to Process " 
                     << p[i].pname << " (ID: " << p[i].id << ")" << endl;
            else 
                flag[i] = 1;
        }

        for (int i = 0; i < num; i++) {
            if (flag[i] != 1 && p[i].id > coordinator.id) {
                cout << "Process " << p[i].pname << " (ID: " << p[i].id 
                     << ") takes over from current coordinator " << coordinator.pname 
                     << " (ID: " << coordinator.id << ")" << endl;
                flag[find_position(coordinator.id)] = 1;
                coordinator = p[i];
            }
        }

        for (int i = 0; i < num; i++) {
            if (flag[i] == 0) 
                coordinator = bully(coordinator);
        }
        return coordinator;
    }

    void ring(Process coordinator) {
        if (!p) {
            cout << "Error: No processes initialized!" << endl;
            return;
        }

        cout << "\n=== Ring Algorithm Execution ===" << endl;
        vector<int> vec;
        int crashed = highest(p);
        int pos = find_position(coordinator.id);
        
        if (pos == -1) {
            cout << "Error: Coordinator not found in process list!" << endl;
            return;
        }

        int count = 0, i = pos;
        while (count < num) {
            if (i != crashed) 
                vec.push_back(p[i].id);
            i = (i + 1) % num;
            count++;
        }

        cout << "Message path: [";
        for (size_t i = 0; i < vec.size(); i++) {
            cout << vec[i];
            if (i < vec.size() - 1) cout << " -> ";
        }
        cout << "]" << endl;

        int coord_id = *max_element(vec.begin(), vec.end());
        int final_coord_pos = find_position(coord_id);
        cout << "Final coordinator is: " << p[final_coord_pos].pname 
             << " (ID: " << coord_id << ")" << endl;
    }
};

int main() {
    Election e;
    Process i_coord;
    Process f_coord;
    int ch = 0;
    bool initialized = false;

    while (ch != 4) {
        cout << "\n=== ELECTION ALGORITHM MENU ===" << endl;
        cout << "1. Initialize Processes" << endl;
        cout << "2. Execute Bully Algorithm" << endl;
        cout << "3. Execute Ring Algorithm" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";

        while (!(cin >> ch)) {
            cout << "Invalid input. Please enter a number (1-4): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (ch) {
            case 1:
                e.input();
                cout << "\nEnter the process which detects coordinator crash:" << endl;
                cout << "Process name: ";
                cin.ignore();
                getline(cin, i_coord.pname);
                cout << "Process ID: ";
                while (!(cin >> i_coord.id) || i_coord.id < 0) {
                    cout << "Invalid ID. Please enter a non-negative number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                initialized = true;
                break;

            case 2:
                if (!initialized) {
                    cout << "\nPlease initialize processes first (Option 1)!" << endl;
                    break;
                }
                f_coord = e.bully(i_coord);
                cout << "\nFinal coordinator is: " << f_coord.pname 
                     << " (ID: " << f_coord.id << ")" << endl;
                break;

            case 3:
                if (!initialized) {
                    cout << "\nPlease initialize processes first (Option 1)!" << endl;
                    break;
                }
                e.ring(i_coord);
                break;

            case 4:
                cout << "\nExiting program. Goodbye!" << endl;
                break;

            default:
                cout << "\nInvalid choice! Please select 1-4." << endl;
        }
    }
    return 0;
}