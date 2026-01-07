/*
Author: Giannfranco Crovetto
Date: 1/6/26
FCFS non-preemptive scheduling implementation of CPU Scheduler
*/

#include<iostream>
#include<vector>
#include<iomanip>
using namespace std;

struct Process
{
    int CPUburst;
};

void calculateTimes(const vector<Process>& processes, vector<int>& waitingTime, 
                   vector<int>& turnaroundTime, vector<int>& responseTime, 
                   vector<int>& completionTime)
{
    int n = processes.size();
    
    // First process
    waitingTime[0] = 0;
    responseTime[0] = 0;
    turnaroundTime[0] = processes[0].CPUburst;
    completionTime[0] = processes[0].CPUburst;
    
    // Rest of the processes
    for (int i = 1; i < n; i++)
    {
        // Completion time = previous completion time + current burst
        completionTime[i] = completionTime[i - 1] + processes[i].CPUburst;
        
        // Waiting time = previous process completion time (when this process starts)
        waitingTime[i] = completionTime[i - 1];
        
        // Response time = waiting time (since process starts immediately when it's its turn)
        responseTime[i] = waitingTime[i];
        
        // Turnaround time = completion time (since arrival time is 0 for all)
        turnaroundTime[i] = completionTime[i];
    }
}

void printProcessBursts(const vector<Process>& processes, int numToShow)
{
    cout << "First " << numToShow << " Process CPU Bursts:\n";
    for (int i = 0; i < numToShow && i < processes.size(); i++)
    {
        cout << "P" << i + 1 << ": " << processes[i].CPUburst << "\n";
    }
}

void printResults(const vector<int>& waitingTime, const vector<int>& turnaroundTime, 
                 const vector<int>& responseTime, const vector<int>& completionTime, 
                 int numToShow)
{
    cout << "\n========================================\n";
    cout << "FCFS Scheduling Results (First " << numToShow << " processes)\n";
    cout << "========================================\n";
    cout << "Process\tWaiting\tTurnaround\tResponse\tCompletion\n";
    cout << "-------------------------------------------------------\n";
    
    for (int i = 0; i < numToShow && i < waitingTime.size(); i++)
    {
        cout << "P" << i + 1 << "\t"
             << waitingTime[i] << "\t"
             << turnaroundTime[i] << "\t\t"
             << responseTime[i] << "\t\t"
             << completionTime[i] << "\n";
    }
}

void calculateAndPrintAverages(const vector<int>& waitingTime, const vector<int>& turnaroundTime, 
                              const vector<int>& responseTime, const vector<Process>& processes,
                              int numProcesses)
{
    double totalWaiting = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;
    int totalCPUBurst = 0;
    
    for (int i = 0; i < numProcesses; i++)
    {
        totalWaiting += waitingTime[i];
        totalTurnaround += turnaroundTime[i];
        totalResponse += responseTime[i];
        totalCPUBurst += processes[i].CPUburst;
    }
    
    double avgWaiting = totalWaiting / numProcesses;
    double avgTurnaround = totalTurnaround / numProcesses;
    double avgResponse = totalResponse / numProcesses;
    
    // CPU Utilization: For FCFS with all processes at time 0, it's 100%
    // unless there are idle periods (which would be in your process list)
    int totalTime = turnaroundTime[numProcesses - 1];  // Last process completion time
    double cpuUtilization = (totalCPUBurst / (double)totalTime) * 100.0;
    
    cout << fixed << setprecision(2);
    cout << "\n========================================\n";
    cout << "Performance Metrics (First " << numProcesses << " processes)\n";
    cout << "========================================\n";
    cout << "Average Waiting Time: " << avgWaiting << "\n";
    cout << "Average Turnaround Time: " << avgTurnaround << "\n";
    cout << "Average Response Time: " << avgResponse << "\n";
    cout << "CPU Utilization: " << cpuUtilization << "%\n";
    cout << "Total Time to Complete: " << totalTime << "\n";
}

int main()
{
    vector<Process> processes = { 
        {5}, {4}, {8}, {3}, {16}, {11}, {14}, {4},
        {3}, {12}, {4}, {5}, {17}, {4}, {5}, {5}, {17}, {5},
        {18}, {5}, {5}, {7}, {6}, {6}, {4}, {11}, {7}, {16}, {3},
        {6}, {12}, {14}, {9}, {14}, {15}, {7}, {4}, {4}, {4}, {9},
        {12}, {4}, {3}, {13}, {16}, {15}, {4}, {5}, {7}, {15}, {11},
        {4}, {8}, {16}, {14}, {6}, {6}, {6}, {9}, {10}, {3}, {5},
        {6}, // IDLE process of 6
        {4},
        {3}, // IDLE process of 3
        {7},
        {11}, // IDLE process of 11
        {6},
        {6}, // IDLE process of 6
        {5},
        {15}, // IDLE process of 15
        {8},
        {54}, // IDLE process of 54
        {3} 
    };
    
    int n = processes.size();
    int numProcessesToShow = 8;  // Show only first 8 in detailed output
    
    vector<int> waitingTime(n);
    vector<int> turnaroundTime(n);
    vector<int> responseTime(n);
    vector<int> completionTime(n);
    
    cout << "FCFS (First Come First Serve) CPU Scheduling\n";
    cout << "Total processes in queue: " << n << "\n\n";
    
    // Calculate all times
    calculateTimes(processes, waitingTime, turnaroundTime, responseTime, completionTime);
    
    // Print CPU bursts for first 8
    printProcessBursts(processes, numProcessesToShow);
    
    // Print detailed results for first 8
    printResults(waitingTime, turnaroundTime, responseTime, completionTime, numProcessesToShow);
    
    // Calculate and print averages for first 8
    calculateAndPrintAverages(waitingTime, turnaroundTime, responseTime, processes, numProcessesToShow);
    
    cout << "\n\nNote: IDLE processes are included in the full process queue.\n";
    cout << "Total processes including IDLE: " << n << "\n";
    
    return 0;
}