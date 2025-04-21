#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <random>
#include <string>

using namespace std;

// Type alias for an inventory item represented as a pair: (item_id, quantity)
typedef pair<int, int> InventoryItem;

// Function to generate random inventory data.
// Default ranges guarantee item IDs between 1 and 100 and quantities between 1 and 50.
vector<InventoryItem> generateInventory(int n, int id_min = 1, int id_max = 100, int qty_min = 1, int qty_max = 50) {
    vector<InventoryItem> inventory;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> id_dist(id_min, id_max);
    uniform_int_distribution<> qty_dist(qty_min, qty_max);

    for (int i = 0; i < n; ++i) {
        int id = id_dist(gen);
        int qty = qty_dist(gen);
        inventory.push_back(make_pair(id, qty));
    }
    return inventory;
}

// Utility function to print the inventory to the console.
void printInventory(const vector<InventoryItem>& inventory) {
    for (const auto& item : inventory) {
        cout << "(" << item.first << ", " << item.second << ") ";
    }
    cout << "\n";
}

// Insertion Sort implementation that sorts the inventory by item_id.
// It prints the intermediate state of the array after each iteration.
vector<InventoryItem> insertionSortInventory(vector<InventoryItem> inventory) {
    cout << "Starting Insertion Sort:" << endl;
    int n = inventory.size();
    for (int i = 1; i < n; ++i) {
        InventoryItem key = inventory[i];
        int j = i - 1;
        cout << "\nIteration " << i << ": key = (" << key.first << ", " << key.second << ")" << endl;
        // Shift elements that are greater than key to one position ahead
        while (j >= 0 && inventory[j].first > key.first) {
            inventory[j + 1] = inventory[j];
            --j;
        }
        inventory[j + 1] = key;
        cout << "Array after iteration " << i << ": ";
        printInventory(inventory);
    }
    return inventory;
}

// Writes the sorted inventory data to a file in the format item_id,quantity.
// Throws an exception if the file cannot be opened.
void writeInventoryToFile(const string& filename, const vector<InventoryItem>& inventory) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        throw runtime_error("Unable to open file for writing.");
    }
    for (const auto& item : inventory) {
        outFile << item.first << "," << item.second << "\n";
    }
    outFile.close();
    cout << "\nInventory successfully written to '" << filename << "'." << endl;
}

// Reads the contents of a file and prints them to the console.
void readFileAndPrint(const string& filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        throw runtime_error("Unable to open file for reading.");
    }
    string line;
    while (getline(inFile, line)) {
        cout << line << "\n";
    }
    inFile.close();
}

int main() {
    try {
        int n;
        cout << "Enter the number of items to generate: ";
        cin >> n;
        if (cin.fail()) {
            throw runtime_error("Invalid input: not an integer.");
        }

        // Generate random inventory data.
        auto inventory = generateInventory(n);
        cout << "\nGenerated Inventory Data:" << endl;
        printInventory(inventory);

        // Sort the inventory using Insertion Sort.
        auto sortedInventory = insertionSortInventory(inventory);
        cout << "\nSorted Inventory Data:" << endl;
        printInventory(sortedInventory);

        // Write sorted data to file.
        string filename = "sorted_inventory.txt";
        try {
            writeInventoryToFile(filename, sortedInventory);
        } catch (const exception &e) {
            cerr << "Error writing to file: " << e.what() << "\n";
        }

        // Read and display the contents of the file.
        cout << "\nContents of " << filename << ":" << endl;
        try {
            readFileAndPrint(filename);
        } catch (const exception &e) {
            cerr << "Error reading file: " << e.what() << "\n";
        }
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}
