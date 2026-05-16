#include <iostream>
#include <string>
#include <iomanip>
#include <regex>
#include <chrono>
#include "bst.h"
#include "avl.h"
#include "bfs.h"
using namespace std;

void printMenu() {
    cout << "\n------------------------------\n";
    cout << "Flight Management System\n";
    cout << "------------------------------\n";
    cout << "1. Add New Flight (BST & AVL)\n";
    cout << "2. Search Flight (BST)\n";
    cout << "3. Delete Flight (BST & AVL)\n";
    cout << "4. Display All Flights (BST)\n";
    cout << "5. Display Flights (AVL) + Rotations\n";
    cout << "6. Compare Search Time (BST vs AVL)\n";
    cout << "7. Plan Trip from Airport A to B (BFS)\n";
    cout << "8. Exit\n";
    cout << "9. BST visualizer\n";
    cout << "10.AVL visualizer\n";


    cout << "Enter your choice: ";
}
// Visualizer 
void visualizeTree(BSTNode* node, string indent = "", bool isRight = true) {
    if (!node) return;

    cout << indent;

    // --- Print L or R ---
    if (isRight)
        cout << "R─";
    else
        cout << "L─";

    cout << node->data.id << " (" << node->data.status << ")\n";

    
    string leftIndent = indent + (isRight ? "   " : "|  ");
    string rightIndent = indent + (isRight ? "   " : "|  ");

    
    visualizeTree(node->left, leftIndent, false);

    
    visualizeTree(node->right, rightIndent, true);
}
void visualizeAVLTree(AVLNode* node, string indent = "", bool isRight = true) {
    if (!node) return;

    cout << indent;

    if (isRight)
        cout << "R─";
    else
        cout << "L─";

    cout << node->data.id << " (" << node->data.status << ")\n";

    string leftIndent = indent + (isRight ? "   " : "|  ");
    string rightIndent = indent + (isRight ? "   " : "|  ");

    visualizeAVLTree(node->left, leftIndent, false);
    visualizeAVLTree(node->right, rightIndent, true);
}






int main() {
    BST bst;
    AVL avl;
    Graph g;

    
    bst.loadFromFile("flights.txt");
    avl.loadFromFile("flights.txt");
    g.loadAirports("airports.txt");
    g.loadRoutes("routes.txt");

    while (true) {
        printMenu();
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        if (choice == 1) {

            string id, dep, dur, status;

            
            while (true) {
                cout << "Enter Flight ID (Format: AA123): ";
                cin >> ws;
                getline(cin, id);

               
                if (regex_match(id, regex("^[A-Za-z]{2}[0-9]{3}$"))) break;

                cout << "❌ Invalid Format! Example: PK303, EK521\n";
            }

            
            while (true) {
                cout << "Enter Departure (HH:MM): ";
                getline(cin, dep);

                
                if (regex_match(dep, regex("^([01]?[0-9]|2[0-3]):[0-5][0-9]$"))) break;

                cout << "❌ Invalid Time! Format must be HH:MM (Example: 09:45)\n";
            }

            
            while (true) {
                cout << "Enter Duration (Format: XhY): ";
                getline(cin, dur);

                
                if (regex_match(dur, regex("^[0-9]+h[0-9]{1,2}$"))) break;

                cout << "❌ Invalid Duration! Example: 2h30\n";
            }

            
            while (true) {
                cout << "Enter Status (On-time / Delayed / Cancelled): ";
                getline(cin, status);

                string s = status;
                for (char& c : s) c = tolower(c);

                if (s == "on-time" || s == "delayed" || s == "cancelled")
                    break;

                cout << "❌ Invalid Status! Must be On-time, Delayed, Cancelled.\n";
            }

          
            Flight f(id, dep, dur, status);
            bool b1 = bst.insertFlight(f);

            FlightAVL fa(id, dep, dur, status);
            bool b2 = avl.insert(fa);

            if (b1 && b2) cout << " Flight inserted into BST & AVL.\n";
            else if (!b1 && !b2) cout << " Flight already exists in both trees.\n";
            else cout << " Inserted (one of the structures updated).\n";
        }


        else if (choice == 2) {
            string id;
            cout << "Enter Flight ID to search (BST): ";
            cin >> ws; getline(cin, id);
            BSTNode* node = bst.searchFlight(id);
            if (node) {
                cout << "Found: " << node->data.id << " | " << node->data.departure
                    << " | " << node->data.duration << " | " << node->data.status << "\n";
            }
            else cout << "Flight not found.\n";
        }

        else if (choice == 3) {
            string id;
            cout << "Enter Flight ID to delete: ";
            cin >> ws; getline(cin, id);
            bool d1 = bst.deleteFlight(id);
            bool d2 = avl.remove(id);
            if (d1 || d2) cout << "Deleted (if existed) from BST and AVL.\n";
            else cout << "Flight not found in either structure.\n";
        }

        else if (choice == 4) {
            cout << "\nBST Inorder Display:\n";
            cout << left << setw(12) << "FlightID" << setw(10) << "Depart" << setw(12)
                << "Duration" << setw(12) << "Status" << "\n";
            bst.displayInorder();
        }

        else if (choice == 5) {
            cout << "\nAVL Inorder Display:\n";
            cout << left << setw(12) << "FlightID" << setw(10) << "Depart" << setw(12)
                << "Duration" << setw(12) << "Status" << "\n";
            avl.displayInorder();
            cout << "Total rotations (since program start): " << avl.getRotationCount() << "\n";
        }

        else if (choice == 6) {
            string id;
            cout << "Enter Flight ID to compare search time: ";
            cin >> ws; getline(cin, id);\
                int trials = 1000;
            cout << "Enter number of trials for averaging (recommended 1000): ";
            if (!(cin >> trials)) {
                cin.clear();
                cin.ignore(10000, '\n');
                trials = 1000;
            }
            double bst_us = bst.measureSearchTimeMicrosec(id, trials);
            double avl_us = avl.measureSearchTimeMicrosec(id, trials);
            cout << fixed;
            cout << "BST average search time: " << bst_us << " microseconds (" << trials << " trials)\n";
            cout << "AVL average search time: " << avl_us << " microseconds (" << trials << " trials)\n";
            cout << "AVL rotations: " << avl.getRotationCount() << "\n";
            if (bst_us < avl_us) cout << "BST slightly faster for this key (may vary).\n";
            else if (avl_us < bst_us) cout << "AVL slightly faster for this key (may vary).\n";
            else cout << "Times equal (within measurement resolution).\n";
        }

        else if (choice == 7) {
            if (g.size() == 0) {
                cout << "No airport data loaded. Make sure airports.txt and routes.txt are present.\n";
                continue;
            }
            cout << "Airports (index : name (city)):\n";
            for (int i = 0; i < g.size(); ++i) {
                auto a = g.getAirport(i);
                cout << i << " : " << a.name << " (" << a.city << ")\n";
            }
            int sidx = -1, didx = -1;
            cout << "Enter source index: ";
            if (!(cin >> sidx)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalid\n"; continue; }
            cout << "Enter destination index: ";
            if (!(cin >> didx)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Invalid\n"; continue; }
            int n = g.size();
            if (sidx < 0 || sidx >= n || didx < 0 || didx >= n) {
                cout << "Invalid indices.\n"; continue;
            }
            int dist[MAX_AIRPORTS];
            int parent[MAX_AIRPORTS];
            g.bfsByIndex(sidx, dist, parent);
            cout << "\nBFS Traversal Table:\n";
            g.printBFSTable(dist, parent);
            if (dist[didx] == -1) {
                cout << "No route from " << g.getAirport(sidx).name << " to " << g.getAirport(didx).name << "\n";
            }
            else {
                cout << "Minimum stops from " << g.getAirport(sidx).name << " to " << g.getAirport(didx).name
                    << ": " << dist[didx] << "\n";
                int routeIndices[MAX_AIRPORTS];
                int len = g.reconstructRoute(sidx, didx, parent, routeIndices, MAX_AIRPORTS);
                cout << "Route: ";
                for (int i = 0; i < len; ++i) {
                    cout << g.getAirport(routeIndices[i]).name;
                    if (i + 1 < len) cout << " -> ";
                }
                cout << "\n";
            }
        }

        else if (choice == 8) {
            
            if (!bst.saveToFile("flights.txt")) cout << "Warning: failed to save BST flights.\n";
            if (!avl.saveToFile("flights.txt")) cout << "Warning: failed to save AVL flights.\n";
            cout << "Saved. Exiting.\n";
            break;
        }


        else if (choice == 9) {
            cout << "BST Visualizer:\n";
            visualizeTree(bst.Root());
        }

        else if (choice == 10) {
            cout << "AVL Visualizer:\n";
            visualizeAVLTree(avl.Root());
        }
        else {
            cout << "Invalid choice. Try again.\n";
        }

    }

    return 0;
}
