/*
Author: Giannfranco Crovetto
Date: 9/22/23
Assignment 1: FCFS non-preemptive scheduling implementation of CPU Scheduler
With I/O blocking and arrival times
*/
#include<iostream>
#include<vector>
#include<queue>
#include<iomanip>
#include<algorithm>
using namespace std;

struct BurstSequence
{
    int processID;
    vector<int> cpuBursts;
    vector<int> ioBursts;
    int currentBurstIndex;
    int arrivalTime;
    int firstResponseTime;
    int completionTime;
    int totalWaitingTime;
    bool hasStarted;
    bool isComplete;
};

struct Event
{
    int time;
    int processID;
    
    bool operator>(const Event& other) const
    {
        if (time == other.time)
            return processID > other.processID;
        return time > other.time;
    }
};

struct BurstExecution
{
    int processID;
    int burstNumber;
    int startTime;
    int endTime;
    int duration;
};

void simulateFCFS(vector<BurstSequence>& processes, vector<BurstExecution>& executions)
{
    priority_queue<Event, vector<Event>, greater<Event>> readyQueue;
    int currentTime = 0;
    
    // Add all processes at time 0 (initial arrival)
    for (int i = 0; i < processes.size(); i++)
    {
        readyQueue.push({0, i});
    }
    
    cout << "\n========================================\n";
    cout << "Execution Timeline:\n";
    cout << "========================================\n";
    
    while (!readyQueue.empty())
    {
        Event nextEvent = readyQueue.top();
        readyQueue.pop();
        
        int processIdx = nextEvent.processID;
        BurstSequence& p = processes[processIdx];
        
        // If process arrives in the future, advance time (CPU idle)
        if (nextEvent.time > currentTime)
        {
            int idleTime = nextEvent.time - currentTime;
            cout << "Time " << currentTime << "-" << nextEvent.time 
                 << ": CPU IDLE (" << idleTime << " units)\n";
            currentTime = nextEvent.time;
        }
        
        // Calculate waiting time for this burst
        int waitTime = currentTime - nextEvent.time;
        p.totalWaitingTime += waitTime;
        
        // Record first response time
        if (!p.hasStarted)
        {
            p.firstResponseTime = currentTime;
            p.hasStarted = true;
        }
        
        // Execute CPU burst
        int cpuBurst = p.cpuBursts[p.currentBurstIndex];
        int startTime = currentTime;
        int endTime = currentTime + cpuBurst;
        
        // Record this execution
        executions.push_back({p.processID, p.currentBurstIndex + 1, startTime, endTime, cpuBurst});
        
        cout << "Time " << startTime << "-" << endTime
             << ": P" << p.processID << " executing CPU burst #" 
             << (p.currentBurstIndex + 1) << " (" << cpuBurst << " units)\n";
        
        currentTime += cpuBurst;
        
        // Check if process has more bursts
        if (p.currentBurstIndex < p.ioBursts.size())
        {
            // Process goes to I/O
            int ioBurst = p.ioBursts[p.currentBurstIndex];
            int returnTime = currentTime + ioBurst;
            
            cout << "       P" << p.processID << " goes to I/O for " 
                 << ioBurst << " units (returns at time " << returnTime << ")\n";
            
            p.currentBurstIndex++;
            readyQueue.push({returnTime, processIdx});
        }
        else
        {
            // Process completed
            p.completionTime = currentTime;
            p.isComplete = true;
            cout << "       P" << p.processID << " COMPLETED at time " 
                 << currentTime << "\n";
        }
    }
    
    cout << "\nTotal time: " << currentTime << "\n";
}

void printBurstByBurstTable(const vector<BurstExecution>& executions)
{
    cout << "\n========================================\n";
    cout << "Detailed Burst-by-Burst Execution Table\n";
    cout << "========================================\n";
    cout << "Process\tBurst#\tStart\tEnd\tDuration\n";
    cout << "-----------------------------------------------\n";
    
    for (const auto& exec : executions)
    {
        cout << "P" << exec.processID << "\t"
             << exec.burstNumber << "\t"
             << exec.startTime << "\t"
             << exec.endTime << "\t"
             << exec.duration << "\n";
    }
    
    cout << "\nTotal CPU bursts executed: " << executions.size() << "\n";
}

void printDetailedResults(const vector<BurstSequence>& processes)
{
    cout << "\n========================================\n";
    cout << "Process Details\n";
    cout << "========================================\n";
    
    for (const auto& p : processes)
    {
        int totalCPU = 0;
        for (int burst : p.cpuBursts)
            totalCPU += burst;
        
        cout << "\nP" << p.processID << ":\n";
        cout << "  CPU Bursts: " << p.cpuBursts.size() << " (Total: " << totalCPU << " units)\n";
        cout << "  Response Time: " << p.firstResponseTime << "\n";
        cout << "  Waiting Time: " << p.totalWaitingTime << "\n";
        cout << "  Turnaround Time: " << p.completionTime << "\n";
        cout << "  Completion Time: " << p.completionTime << "\n";
    }
}

void printSummaryTable(const vector<BurstSequence>& processes)
{
    cout << "\n========================================\n";
    cout << "Summary Table (Per Process)\n";
    cout << "========================================\n";
    cout << "Process\tResponse\tWaiting\tTurnaround\tCompletion\n";
    cout << "-------------------------------------------------------\n";
    
    for (const auto& p : processes)
    {
        cout << "P" << p.processID << "\t"
             << p.firstResponseTime << "\t\t"
             << p.totalWaitingTime << "\t"
             << p.completionTime << "\t\t"
             << p.completionTime << "\n";
    }
}

void calculateAndPrintAverages(const vector<BurstSequence>& processes)
{
    double totalWaiting = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;
    int totalCPUTime = 0;
    int maxCompletionTime = 0;
    
    for (const auto& p : processes)
    {
        totalWaiting += p.totalWaitingTime;
        totalTurnaround += p.completionTime;
        totalResponse += p.firstResponseTime;
        
        for (int burst : p.cpuBursts)
            totalCPUTime += burst;
        
        if (p.completionTime > maxCompletionTime)
            maxCompletionTime = p.completionTime;
    }
    
    int n = processes.size();
    double avgWaiting = totalWaiting / n;
    double avgTurnaround = totalTurnaround / n;
    double avgResponse = totalResponse / n;
    double cpuUtilization = (totalCPUTime / (double)maxCompletionTime) * 100.0;
    
    cout << fixed << setprecision(2);
    cout << "\n========================================\n";
    cout << "Performance Metrics\n";
    cout << "========================================\n";
    cout << "Average Waiting Time: " << avgWaiting << "\n";
    cout << "Average Turnaround Time: " << avgTurnaround << "\n";
    cout << "Average Response Time: " << avgResponse << "\n";
    cout << "CPU Utilization: " << cpuUtilization << "%\n";
    cout << "Total Execution Time: " << maxCompletionTime << "\n";
}

int main()
{
    vector<BurstSequence> processes(8);
    vector<BurstExecution> executions;
    
    // P1: CPU, I/O, CPU, I/O, ...
    processes[0] = {1, {5, 3, 5, 4, 6, 4, 3, 4}, {27, 31, 43, 18, 22, 26, 24}, 0, 0, -1, 0, 0, false, false};
    
    // P2
    processes[1] = {2, {4, 5, 7, 12, 9, 4, 9, 7, 8}, {48, 44, 42, 37, 76, 41, 31, 43}, 0, 0, -1, 0, 0, false, false};
    
    // P3
    processes[2] = {3, {8, 12, 18, 14, 4, 15, 14, 5, 6}, {33, 41, 65, 21, 61, 18, 26, 31}, 0, 0, -1, 0, 0, false, false};
    
    // P4
    processes[3] = {4, {3, 4, 5, 3, 4, 5, 6, 5, 3}, {35, 41, 45, 51, 61, 54, 82, 77}, 0, 0, -1, 0, 0, false, false};
    
    // P5
    processes[4] = {5, {16, 17, 5, 16, 7, 13, 11, 6, 3, 4}, {24, 21, 36, 26, 31, 28, 21, 13, 11}, 0, 0, -1, 0, 0, false, false};
    
    // P6
    processes[5] = {6, {11, 4, 5, 6, 7, 9, 12, 15, 8}, {22, 8, 10, 12, 14, 18, 24, 30}, 0, 0, -1, 0, 0, false, false};
    
    // P7
    processes[6] = {7, {14, 17, 11, 15, 4, 7, 16, 10}, {46, 41, 42, 21, 32, 19, 33}, 0, 0, -1, 0, 0, false, false};
    
    // P8
    processes[7] = {8, {4, 5, 6, 14, 16, 6}, {14, 33, 51, 73, 87}, 0, 0, -1, 0, 0, false, false};
    
    cout << "FCFS CPU Scheduling with I/O\n";
    cout << "8 Processes with CPU and I/O bursts\n";
    
    // Simulate FCFS
    simulateFCFS(processes, executions);
    
    // Print burst-by-burst table (NEW!)
    printBurstByBurstTable(executions);
    
    // Print results
    printDetailedResults(processes);
    printSummaryTable(processes);
    calculateAndPrintAverages(processes);
    
    return 0;
}