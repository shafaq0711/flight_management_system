#pragma once
#pragma once
#pragma once
#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;

class FlightAVL {
public:
    string id;
    string departure;
    string duration;
    string status;

    FlightAVL() {}
    FlightAVL(const string& _id, const string& _dep, const string& _dur, const string& _st)
        : id(_id), departure(_dep), duration(_dur), status(_st) {
    }
};

class AVLNode {
public:
    FlightAVL data;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const FlightAVL& f) : data(f), left(nullptr), right(nullptr), height(1) {}
};

class AVL {
private:
    AVLNode* root;
    long long rotationCount; 

    int height(AVLNode* n) { return n ? n->height : 0; }
    int balanceFactor(AVLNode* n) { return n ? height(n->left) - height(n->right) : 0; }
    void updateHeight(AVLNode* n) { if (n) n->height = 1 + max(height(n->left), height(n->right)); }

    
    AVLNode* rotateRight(AVLNode* y) {
        
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        rotationCount++;
        return x;
    }

    AVLNode* rotateLeft(AVLNode* x) {
       
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        rotationCount++;
        return y;
    }

    AVLNode* insertRec(AVLNode* node, const FlightAVL& f) {
        if (!node) return new AVLNode(f);
        if (f.id == node->data.id) return node; 
        if (f.id < node->data.id) node->left = insertRec(node->left, f);
        else node->right = insertRec(node->right, f);

        updateHeight(node);
        int bf = balanceFactor(node);

        // LL
        if (bf > 1 && f.id < node->left->data.id) {
            return rotateRight(node); 
        }

        // RR
        if (bf < -1 && f.id > node->right->data.id) {
            return rotateLeft(node);
        }

        // LR
        if (bf > 1 && f.id > node->left->data.id) {
            node->left = rotateLeft(node->left); 
            return rotateRight(node);            
        }

        // RL
        if (bf < -1 && f.id < node->right->data.id) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLNode* minNode(AVLNode* n) {
        while (n->left) n = n->left;
        return n;
    }

    AVLNode* deleteRec(AVLNode* node, const string& id) {
        if (!node) return nullptr;
        if (id < node->data.id) node->left = deleteRec(node->left, id);
        else if (id > node->data.id) node->right = deleteRec(node->right, id);
        else {
            
            if (!node->left || !node->right) {
                AVLNode* tmp = node->left ? node->left : node->right;
                if (!tmp) { delete node; return nullptr; }
                else { AVLNode* toReturn = tmp; delete node; node = toReturn; }
            }
            else {
                AVLNode* succ = minNode(node->right);
                node->data = succ->data;
                node->right = deleteRec(node->right, succ->data.id);
            }
        }

        if (!node) return node;

        updateHeight(node);
        int bf = balanceFactor(node);

        // LL
        if (bf > 1 && balanceFactor(node->left) >= 0) return rotateRight(node);
        // LR
        if (bf > 1 && balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        // RR
        if (bf < -1 && balanceFactor(node->right) <= 0) return rotateLeft(node);
        // RL
        if (bf < -1 && balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLNode* searchRec(AVLNode* node, const string& id) {
        if (!node) return nullptr;
        if (node->data.id == id) return node;
        if (id < node->data.id) return searchRec(node->left, id);
        return searchRec(node->right, id);
    }

    void inorderRec(AVLNode* node) {
        if (!node) return;
        inorderRec(node->left);
        cout << node->data.id << " | " << node->data.departure << " | "
            << node->data.duration << " | " << node->data.status << "\n";
        inorderRec(node->right);
    }

    void saveRec(AVLNode* node, ofstream& fout) {
        if (!node) return;
        saveRec(node->left, fout);
        fout << node->data.id << '|' << node->data.departure << '|' << node->data.duration
            << '|' << node->data.status << '\n';
        saveRec(node->right, fout);
    }

    void freeRec(AVLNode* node) {
        if (!node) return;
        freeRec(node->left);
        freeRec(node->right);
        delete node;
    }

public:
    AVLNode* Root() { return root; }
    AVL() : root(nullptr), rotationCount(0) {}
    ~AVL() { freeRec(root); }

    bool insert(const FlightAVL& f) {
        if (searchRec(root, f.id) != nullptr) return false;
        root = insertRec(root, f);
        return true;
    }

    bool remove(const string& id) {
        if (searchRec(root, id) == nullptr) return false;
        root = deleteRec(root, id);
        return true;
    }

    AVLNode* search(const string& id) {
        return searchRec(root, id);
    }

    void displayInorder() {
        inorderRec(root);
    }

    bool loadFromFile(const string& filename = "flights.txt") {
        ifstream fin(filename.c_str());
        if (!fin.is_open()) return false;
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            string parts[4];
            int p = 0;
            string cur;
            for (size_t i = 0; i <= line.size() && p < 4; ++i) {
                if (i == line.size() || line[i] == '|') {
                    parts[p++] = cur;
                    cur.clear();
                }
                else cur.push_back(line[i]);
            }
            if (p == 4) {
                FlightAVL f(parts[0], parts[1], parts[2], parts[3]);
                insert(f);
            }
        }
        fin.close();
        return true;
    }

    bool saveToFile(const string& filename = "flights.txt") {
        ofstream fout(filename.c_str());
        if (!fout.is_open()) return false;
        saveRec(root, fout);
        fout.close();
        return true;
    }

    long long getRotationCount() const { return rotationCount; }
    void resetRotationCount() { rotationCount = 0; }

    double measureSearchTimeMicrosec(const string& id, int trials = 1000) {
        if (trials <= 0) trials = 1;
        using clock = chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < trials; ++i) {
            (void)searchRec(root, id);
        }
        auto t2 = clock::now();
        double total_us = chrono::duration_cast<chrono::duration<double, std::micro>>(t2 - t1).count();
        return total_us / trials;
    }
};

#endif

