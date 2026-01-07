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
    vector<int> cpuBursts;
    int currentBurstIndex;
    int totalWaitingTime;
    int totalResponseTime;
    int completionTime;
    int firstResponseTime;
    bool hasStarted;
};

void simulateFCFS(vector<Process>& processes)
{
    int currentTime = 0;
    vector<int> readyQueue; // Indices of processes ready to execute
    
    // Initialize all processes in ready queue
    for (int i = 0; i < processes.size(); i++)
    {
        readyQueue.push_back(i);
    }
    
    cout << "\n========================================\n";
    cout << "Execution Timeline:\n";
    cout << "========================================\n";
    
    while (!readyQueue.empty())
    {
        int currentProcessIdx = readyQueue.front();
        readyQueue.erase(readyQueue.begin());
        
        Process& p = processes[currentProcessIdx];
        int burstTime = p.cpuBursts[p.currentBurstIndex];
        
        // Record first response time
        if (!p.hasStarted)
        {
            p.firstResponseTime = currentTime;
            p.hasStarted = true;
        }
        
        cout << "Time " << currentTime << "-" << (currentTime + burstTime) 
             << ": P" << p.processID << " (Burst #" << (p.currentBurstIndex + 1) << ")\n";
        
        currentTime += burstTime;
        p.currentBurstIndex++;
        
        // If process has more bursts, add it back to the end of queue
        if (p.currentBurstIndex < p.cpuBursts.size())
        {
            readyQueue.push_back(currentProcessIdx);
        }
        else
        {
            // Process completed all bursts
            p.completionTime = currentTime;
        }
    }
}

void calculateMetrics(vector<Process>& processes)
{
    // Calculate waiting and response times
    int currentTime = 0;
    vector<int> readyQueue;
    vector<int> lastExecutionTime(processes.size(), 0);
    
    for (int i = 0; i < processes.size(); i++)
    {
        readyQueue.push_back(i);
        processes[i].totalWaitingTime = 0;
    }
    
    while (!readyQueue.empty())
    {
        int currentProcessIdx = readyQueue.front();
        readyQueue.erase(readyQueue.begin());
        
        Process& p = processes[currentProcessIdx];
        int burstIdx = 0;
        
        // Count how many bursts this process has executed
        for (int i = 0; i < processes.size(); i++)
        {
            if (i == currentProcessIdx)
            {
                for (int j = 0; j < readyQueue.size(); j++)
                {
                    if (readyQueue[j] == currentProcessIdx)
                        burstIdx++;
                }
                break;
            }
        }
        
        // Waiting time is current time minus last execution time
        int waitingTime = currentTime - lastExecutionTime[currentProcessIdx];
        p.totalWaitingTime += waitingTime;
        
        int burstTime = p.cpuBursts[burstIdx];
        currentTime += burstTime;
        lastExecutionTime[currentProcessIdx] = currentTime;
        
        // Check if process has more bursts
        if (burstIdx + 1 < p.cpuBursts.size())
        {
            readyQueue.push_back(currentProcessIdx);
        }
    }
}

void printResults(const vector<Process>& processes)
{
    cout << "\n========================================\n";
    cout << "Final Results for All 8 Processes\n";
    cout << "========================================\n";
    cout << "Process\tBursts\tWaiting\tTurnaround\tResponse\tCompletion\n";
    cout << "-------------------------------------------------------------------\n";
    
    for (const auto& p : processes)
    {
        int totalBurstTime = 0;
        for (int burst : p.cpuBursts)
        {
            totalBurstTime += burst;
        }
        
        int turnaroundTime = p.completionTime;
        
        cout << "P" << p.processID << "\t"
             << p.cpuBursts.size() << "\t"
             << p.totalWaitingTime << "\t"
             << turnaroundTime << "\t\t"
             << p.firstResponseTime << "\t\t"
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
        totalWaiting += p.totalWaitingTime;
        totalTurnaround += p.completionTime;
        totalResponse += p.firstResponseTime;
        
        for (int burst : p.cpuBursts)
        {
            totalCPUBurstTime += burst;
        }
        
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
    // Initialize 8 processes with their CPU bursts
    vector<Process> processes(8);
    
    // P1
    processes[0] = {1, {5, 3, 17, 4, 9, 12, 7, 6, 3, 54}, 0, 0, 0, 0, 0, false};
    
    // P2
    processes[1] = {2, {4, 12, 5, 11, 14, 4, 15, 6, 7, 3}, 0, 0, 0, 0, 0, false};
    
    // P3
    processes[2] = {3, {8, 4, 18, 7, 15, 3, 11, 9, 11}, 0, 0, 0, 0, 0, false};
    
    // P4
    processes[3] = {4, {3, 5, 5, 16, 7, 13, 4, 10, 6}, 0, 0, 0, 0, 0, false};
    
    // P5
    processes[4] = {5, {16, 17, 5, 3, 4, 16, 8, 3, 6}, 0, 0, 0, 0, 0, false};
    
    // P6
    processes[5] = {6, {11, 4, 7, 6, 4, 15, 16, 5, 5}, 0, 0, 0, 0, 0, false};
    
    // P7
    processes[6] = {7, {14, 5, 6, 12, 4, 4, 14, 6, 15}, 0, 0, 0, 0, 0, false};
    
    // P8
    processes[7] = {8, {4, 5, 6, 14, 9, 5, 6, 4, 8}, 0, 0, 0, 0, 0, false};
    
    cout << "FCFS Multi-Burst CPU Scheduling\n";
    cout << "8 Processes with multiple CPU bursts each\n";
    
    // Simulate FCFS execution
    simulateFCFS(processes);
    
    // Reset for metric calculation
    for (auto& p : processes)
    {
        p.currentBurstIndex = 0;
        p.hasStarted = false;
    }
    
    calculateMetrics(processes);
    
    // Print results
    printResults(processes);
    
    // Print averages
    calculateAndPrintAverages(processes);
    
    return 0;
}