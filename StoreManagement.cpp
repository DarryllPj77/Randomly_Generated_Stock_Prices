#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
#include <limits>
 
using namespace std;
 
struct Items {
    int id;
    int quantity;
};
 
vector<Items> generateItems(int N) {
    vector<Items> items;
    for (int i = 0; i < N; ++i) {
        Items item;
        item.id = rand() % 100 + 1;
        item.quantity = rand() % 50 + 1;
        items.push_back(item);
    }
    return items;
}
 
void displayItems(const vector<Items>& items) {
    cout << "\n=====================================" << endl;
    cout << "           INVENTORY LIST           " << endl;
    cout << "=====================================" << endl;
    cout << left << setw(10) << "ID" << setw(10) << "Quantity" << endl;
    cout << "-------------------------------------" << endl;
    for (const auto& item : items) {
        cout << left << setw(10) << item.id << setw(10) << item.quantity << endl;
    }
    cout << "=====================================" << endl;
}
 
void insertionSort(vector<Items>& items) {
    int n = items.size();
    for (int i = 1; i < n; ++i) {
        Items key = items[i];
        int j = i - 1;
        while (j >= 0 && items[j].id > key.id) {
            items[j + 1] = items[j];
            j--;
        }
        items[j + 1] = key;
    }
}
 
string getTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", ltm);
    return string(buffer);
}
 
void writeToFile(const vector<Items>& items, const string& filename) {
    ofstream outfile(filename, ios::app);
    if (!outfile) {
        cerr << "\n[ERROR] Could not open file to save data." << endl;
        return;
    }
    outfile << "Session: " << getTimestamp() << "\n";
    outfile << "ID,Quantity\n";
    for (const auto& item : items) {
        outfile << item.id << "," << item.quantity << "\n";
    }
    outfile << "-------------------------------------\n";
    outfile.close();
    cout << "\n[INFO] Data successfully saved in " << filename << endl;
}
 
void viewPreviousSessions(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "\n[INFO] No previous sessions found." << endl;
        return;
    }
    cout << "\n=====================================" << endl;
    cout << "        PREVIOUS SESSIONS          " << endl;
    cout << "=====================================" << endl;
    string line;
    while (getline(infile, line)) {
        cout << line << endl;
    }
    infile.close();
    cout << "=====================================" << endl;
}
 
void clearInputStream() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
 
int getValidatedInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && cin.peek() == '\n') {
            return value;
        }
        cerr << "\n[ERROR] Invalid input. Please enter a valid integer.\n";
        clearInputStream();
    }
}
 
int main() {
    srand(time(nullptr));
    const string filename = "itemsession.txt";
    int choice;
    do {
        cout << "\n=====================================" << endl;
        cout << "         INVENTORY MANAGER          " << endl;
        cout << "=====================================" << endl;
        cout << "  1. Create New Inventory" << endl;
        cout << "  2. View Previous Sessions" << endl;
        cout << "  3. Exit" << endl;
        cout << "=====================================" << endl;
        choice = getValidatedInt("Enter your choice: ");
        switch (choice) {
            case 1: {
                int N;
                do {
                    N = getValidatedInt("\nEnter the number of items to generate: ");
                    if (N <= 0) 
                        cerr << "[ERROR] Please enter a positive integer.\n";
                } while (N <= 0);
                vector<Items> items = generateItems(N);
                displayItems(items);
                insertionSort(items);
                cout << "\n[INFO] Inventory sorted." << endl;
                displayItems(items);
                writeToFile(items, filename);
                break;
            }
            case 2:
                viewPreviousSessions(filename);
                break;
            case 3:
                cout << "\n[INFO] Exiting program. Goodbye!" << endl;
                return 0;
            default:
                cerr << "\n[ERROR] Invalid choice. Please try again." << endl;
        }
    } while (true);
    return 0;
}