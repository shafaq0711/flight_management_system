#pragma once
#pragma once
#pragma once
#ifndef BFS_H
#define BFS_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX_AIRPORTS = 100;

class AirportClass {
public:
    int id;
    string name;
    string city;
    string country;
    bool emergency;

    AirportClass() : id(-1), name(""), city(""), country(""), emergency(false) {}
};

class Graph {
private:
    AirportClass airports[MAX_AIRPORTS];
    int adj[MAX_AIRPORTS][MAX_AIRPORTS];
    int n; 

    int indexById(int id) {
        for (int i = 0; i < n; ++i) if (airports[i].id == id) return i;
        return -1;
    }

public:
    Graph() : n(0) {
        for (int i = 0; i < MAX_AIRPORTS; ++i)
            for (int j = 0; j < MAX_AIRPORTS; ++j)
                adj[i][j] = 0;
    }

    bool loadAirports(const string& filename = "airports.txt") {
        ifstream fin(filename.c_str());
        if (!fin.is_open()) return false;
        string line;
        n = 0;
        while (getline(fin, line) && n < MAX_AIRPORTS) {
            if (line.empty()) continue;
            
            string parts[5];
            int p = 0;
            string cur;
            for (size_t i = 0; i <= line.size() && p < 5; ++i) {
                if (i == line.size() || line[i] == '|') {
                    parts[p++] = cur;
                    cur.clear();
                }
                else cur.push_back(line[i]);
            }
            if (p == 5) {
                airports[n].id = atoi(parts[0].c_str());
                airports[n].name = parts[1];
                airports[n].city = parts[2];
                airports[n].country = parts[3];
                airports[n].emergency = (parts[4] == "1");
                ++n;
            }
        }
        fin.close();
        return true;
    }

    bool loadRoutes(const string& filename = "routes.txt") {
        ifstream fin(filename.c_str());
        if (!fin.is_open()) return false;
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            
            string parts[2];
            int p = 0;
            string cur;
            for (size_t i = 0; i <= line.size() && p < 2; ++i) {
                if (i == line.size() || line[i] == '|') {
                    parts[p++] = cur;
                    cur.clear();
                }
                else cur.push_back(line[i]);
            }
            if (p == 2) {
                int aId = atoi(parts[0].c_str());
                int bId = atoi(parts[1].c_str());
                int ai = indexById(aId);
                int bi = indexById(bId);
                if (ai >= 0 && bi >= 0) {
                    adj[ai][bi] = 1;
                    adj[bi][ai] = 1; 
                }
            }
        }
        fin.close();
        return true;
    }

    int size() const { return n; }

    AirportClass getAirport(int idx) const {
        if (idx >= 0 && idx < n) return airports[idx];
        return AirportClass();
    }

    
    void bfsByIndex(int src, int dist[], int parent[]) {
        for (int i = 0; i < n; ++i) { dist[i] = -1; parent[i] = -1; }
        if (src < 0 || src >= n) return;
        int queue[MAX_AIRPORTS];
        int front = 0, rear = 0;
        dist[src] = 0;
        queue[rear++] = src;
        while (front < rear) {
            int u = queue[front++];
            for (int v = 0; v < n; ++v) {
                if (adj[u][v] && dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    queue[rear++] = v;
                }
            }
        }
    }

    void printBFSTable(const int dist[], const int parent[]) {
        cout << "Airport\tDistance\tParent\n";
        for (int i = 0; i < n; ++i) {
            cout << airports[i].name << '\t';
            if (dist[i] == -1) cout << "INF\t";
            else cout << dist[i] << '\t';
            if (parent[i] == -1) cout << "None\n";
            else cout << airports[parent[i]].name << '\n';
        }
    }

    
    int reconstructRoute(int src, int dest, const int parent[], int routeIndices[], int maxLen) {
        if (dest < 0 || dest >= n || src < 0 || src >= n) return 0;
        if (parent[dest] == -1 && dest != src) return 0; 
        int tmp[MAX_AIRPORTS];
        int cnt = 0;
        int cur = dest;
        while (cur != -1 && cnt < MAX_AIRPORTS) {
            tmp[cnt++] = cur;
            if (cur == src) break;
            cur = parent[cur];
        }
        
        int len = 0;
        for (int i = cnt - 1; i >= 0 && len < maxLen; --i) {
            routeIndices[len++] = tmp[i];
        }
        return len;
    }
};

#endif

