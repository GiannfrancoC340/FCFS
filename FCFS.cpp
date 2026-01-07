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
    int processID;
    int arrivalTime;
    int CPUburst;
    int waitingTime;
    int turnaroundTime;
    int responseTime;
    int completionTime;
};

void calculateFCFS(vector<Process>& processes)
{
    int currentTime = 0;
    
    for (int i = 0; i < processes.size(); i++)
    {
        // If CPU is idle, jump to next process arrival time
        if (currentTime < processes[i].arrivalTime)
        {
            int idleTime = processes[i].arrivalTime - currentTime;
            cout << "IDLE for " << idleTime << " units at time " << currentTime << "\n";
            currentTime = processes[i].arrivalTime;
        }
        
        // Response time = time when process first gets CPU - arrival time
        processes[i].responseTime = currentTime - processes[i].arrivalTime;
        
        // Waiting time = time spent waiting before execution
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        
        // Execute the process
        currentTime += processes[i].CPUburst;
        
        // Completion time = when process finishes
        processes[i].completionTime = currentTime;
        
        // Turnaround time = completion time - arrival time
        processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
    }
}

void printResults(const vector<Process>& processes)
{
    cout << "\n========================================\n";
    cout << "FCFS Scheduling Results\n";
    cout << "========================================\n";
    
    cout << "\nProcess\tArrival\tBurst\tWaiting\tTurnaround\tResponse\tCompletion\n";
    cout << "-----------------------------------------------------------------------\n";
    
    for (const auto& p : processes)
    {
        cout << "P" << p.processID << "\t"
             << p.arrivalTime << "\t"
             << p.CPUburst << "\t"
             << p.waitingTime << "\t"
             << p.turnaroundTime << "\t\t"
             << p.responseTime << "\t\t"
             << p.completionTime << "\n";
    }
}

void calculateAndPrintAverages(const vector<Process>& processes)
{
    double totalWaiting = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;
    int totalCPUBurstTime = 0;
    int lastCompletionTime = 0;
    
    for (const auto& p : processes)
    {
        totalWaiting += p.waitingTime;
        totalTurnaround += p.turnaroundTime;
        totalResponse += p.responseTime;
        totalCPUBurstTime += p.CPUburst;
        
        if (p.completionTime > lastCompletionTime)
            lastCompletionTime = p.completionTime;
    }
    
    int n = processes.size();
    double avgWaiting = totalWaiting / n;
    double avgTurnaround = totalTurnaround / n;
    double avgResponse = totalResponse / n;
    double cpuUtilization = (totalCPUBurstTime / (double)lastCompletionTime) * 100.0;
    
    cout << fixed << setprecision(2);
    cout << "\n========================================\n";
    cout << "Performance Metrics\n";
    cout << "========================================\n";
    cout << "Average Waiting Time: " << avgWaiting << "\n";
    cout << "Average Turnaround Time: " << avgTurnaround << "\n";
    cout << "Average Response Time: " << avgResponse << "\n";
    cout << "CPU Utilization: " << cpuUtilization << "%\n";
    cout << "Total Time: " << lastCompletionTime << "\n";
}

int main()
{
    // Example: 8 processes with arrival times and CPU bursts
    // Format: {processID, arrivalTime, CPUburst, 0, 0, 0, 0}
    vector<Process> processes = {
        {1, 0, 5, 0, 0, 0, 0},
        {2, 5, 4, 0, 0, 0, 0},
        {3, 9, 8, 0, 0, 0, 0},
        {4, 17, 3, 0, 0, 0, 0},
        {5, 20, 16, 0, 0, 0, 0},
        {6, 36, 11, 0, 0, 0, 0},
        {7, 47, 14, 0, 0, 0, 0},
        {8, 61, 4, 0, 0, 0, 0}
    };
    
    cout << "FCFS (First Come First Serve) CPU Scheduling\n";
    cout << "Number of processes: " << processes.size() << "\n";
    
    // Calculate all times using FCFS algorithm
    calculateFCFS(processes);
    
    // Print detailed results
    printResults(processes);
    
    // Print averages and statistics
    calculateAndPrintAverages(processes);
    
    return 0;
}