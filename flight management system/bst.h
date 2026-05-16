#pragma once
#ifndef BST_H
#define BST_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <chrono>
using namespace std;


class Flight {
public:
    string id;
    string departure;
    string duration;
    string status;

    Flight() {}
    Flight(const string& _id, const string& _dep, const string& _dur, const string& _st)
        : id(_id), departure(_dep), duration(_dur), status(_st) {
    }
};


class BSTNode {
public:
    Flight data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const Flight& f) : data(f), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;

    BSTNode* insertRec(BSTNode* node, const Flight& f) {
        if (!node) return new BSTNode(f);
        if (f.id == node->data.id) return node; // ignore duplicates
        if (f.id < node->data.id)
            node->left = insertRec(node->left, f);
        else
            node->right = insertRec(node->right, f);
        return node;
    }

    BSTNode* minNode(BSTNode* node) {
        if (!node) return nullptr;
        while (node->left) node = node->left;
        return node;
    }

    BSTNode* deleteRec(BSTNode* node, const string& id) {
        if (!node) return nullptr;
        if (id < node->data.id)
            node->left = deleteRec(node->left, id);
        else if (id > node->data.id)
            node->right = deleteRec(node->right, id);
        else {
            
            if (!node->left) {
                BSTNode* r = node->right;
                delete node;
                return r;
            }
            else if (!node->right) {
                BSTNode* l = node->left;
                delete node;
                return l;
            }
            else {
                BSTNode* succ = minNode(node->right);
                node->data = succ->data;
                node->right = deleteRec(node->right, succ->data.id);
            }
        }
        return node;
    }

    BSTNode* searchRec(BSTNode* node, const string& id) {
        if (!node) return nullptr;
        if (node->data.id == id) return node;
        if (id < node->data.id) return searchRec(node->left, id);
        return searchRec(node->right, id);
    }

    void inorderRec(BSTNode* node) {
        if (!node) return;
        inorderRec(node->left);
        cout << left << setw(12) << node->data.id
            << setw(10) << node->data.departure
            << setw(12) << node->data.duration
            << setw(12) << node->data.status << "\n";
        inorderRec(node->right);
    }

    void saveRec(BSTNode* node, ofstream& fout) {
        if (!node) return;
        saveRec(node->left, fout);
        fout << node->data.id << '|'
            << node->data.departure << '|'
            << node->data.duration << '|'
            << node->data.status << '\n';
        saveRec(node->right, fout);
    }

    void freeRec(BSTNode* node) {
        if (!node) return;
        freeRec(node->left);
        freeRec(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}
    ~BST() { freeRec(root); }

    BSTNode* Root() { return root; }

    bool insertFlight(const Flight& f) {
        if (searchRec(root, f.id)) return false;
        root = insertRec(root, f);
        return true;
    }

    bool deleteFlight(const string& id) {
        if (!searchRec(root, id)) return false;
        root = deleteRec(root, id);
        return true;
    }

    BSTNode* searchFlight(const string& id) {
        return searchRec(root, id);
    }

    void displayInorder() {
        inorderRec(root);
    }

    bool loadFromFile(const string& filename = "flights.txt") {
        ifstream fin(filename);
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
                Flight f(parts[0], parts[1], parts[2], parts[3]);
                insertFlight(f);
            }
        }
        fin.close();
        return true;
    }

    bool saveToFile(const string& filename = "flights.txt") {
        ofstream fout(filename);
        if (!fout.is_open()) return false;
        saveRec(root, fout);
        fout.close();
        return true;
    }

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


