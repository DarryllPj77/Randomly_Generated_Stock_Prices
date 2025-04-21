#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <limits>  // Added for input validation
 
using namespace std;
 
 
// Function to generate random stock prices with or without duplicates
void generateStockPrices(int arr[], int N, bool allowDuplicates, int minPrice = 100, int maxPrice = 200) {
    // Added: Check for valid range when duplicates aren't allowed
    if (!allowDuplicates) {
        int possibleValues = maxPrice - minPrice + 1;
        if (possibleValues < N) {
            throw invalid_argument("Cannot generate unique prices. Range too small.");
        }
    }
 
    srand(time(0));
    for (int i = 0; i < N; i++) {
        if (allowDuplicates) {
            arr[i] = minPrice + rand() % (maxPrice - minPrice + 1);
        } else {
            bool unique;
            do {
                unique = true;
                int newPrice = minPrice + rand() % (maxPrice - minPrice + 1);
                for (int j = 0; j < i; j++) {
                    if (arr[j] == newPrice) {
                        unique = false;
                        break;
                    }
                }
                if (unique) arr[i] = newPrice;
            } while (!unique);
        }
    }
 
    // Ensure target price has duplicates if duplicates are allowed
    if (allowDuplicates) {
        int duplicateIndex = rand() % N;
        int duplicateValue = arr[duplicateIndex];
        for (int i = 0; i < N / 4; i++) {
            int randomIndex = rand() % N;
            arr[randomIndex] = duplicateValue;
        }
    }
}
 
// Quick Sort for Sorting by Price (Ascending Order)
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        int pi = i + 1;
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
 
// Function to count duplicates in the array
void countDuplicates(int arr[], int N) {
    bool hasDuplicates = false;
    cout << "\n=== Duplicate Analysis ===\n";  // Added UI improvement
    cout << "Duplicate stock prices and their occurrences: \n";
    int i = 0;
    while (i < N) {
        int count = 1;
        while (i + 1 < N && arr[i] == arr[i + 1]) {
            count++;
            i++;
        }
        if (count > 1) {
            hasDuplicates = true;
            cout << "  - $" << arr[i] << "\t: " << count << " occurrences\n";  // Improved formatting
        }
        i++;
    }
    if (!hasDuplicates) {
        cout << "No duplicate stock prices found.\n";
    }
    cout << "=========================\n\n";  // Added UI improvement
}
 
// Function to perform binary search with step-by-step simulation
int binarySearch(int arr[], int N, int target) {
    int left = 0, right = N - 1;
    cout << "Search steps:\n";  // Added UI improvement
    while (left <= right) {
        int mid = left + (right - left) / 2;
        cout << "  Left: " << left << ", Right: " << right 
<< " ? Mid: " << mid << " (Value: $" << arr[mid] << ")\n";  // Improved formatting
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}
 
// Function to find all occurrences of a target price
void findAllOccurrences(int arr[], int N, int target) {
    int index = binarySearch(arr, N, target);
    if (index == -1) {
        cout << "\nStock price $" << target << " not found.\n";  // Improved formatting
        return;
    }
 
    cout << "\nFound $" << target << " at index: " << index << "\n";
    cout << "Complete list of indices: ";
    int left = index;
    while (left >= 0 && arr[left] == target) {
        cout << left << " ";
        left--;
    }
    int right = index + 1;
    while (right < N && arr[right] == target) {
        cout << right << " ";
        right++;
    }
    cout << "\n\n";
}
 
//  UI Improvements & Error Handling Additions
void printHeader(const string& title) {
    cout << "\n======================================\n";
    cout << "  " << title << "\n";
    cout << "======================================\n";
}
 
int main() {
	char cont;
    do {
    try {
        printHeader("Stock Price Analyzer");
        int N;
        cout << "Enter number of stock prices to generate (1-10000): ";
        if (!(cin >> N)) throw invalid_argument("Invalid input. Please enter an integer.");
        if (N <= 0) throw invalid_argument("Number must be positive.");
        if (N > 10000) throw invalid_argument("Maximum limit is 10000.");  // Added safety check
 
        char choice;
        do {  // Added input validation loop
            cout << "Allow duplicates? (y/n): ";
            cin >> choice;
            if (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N') {
                cout << "Invalid input! Please enter 'y' or 'n'.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');
 
        bool allowDuplicates = (choice == 'y' || choice == 'Y');
        int stockPrices[N];
 
        printHeader("Generating Prices");
        generateStockPrices(stockPrices, N, allowDuplicates);
 
        printHeader("Generated Prices (Unsorted)");
        if (N > 50) {  // Added truncation for large datasets
            cout << "First 50 prices:\n";
            for (int i = 0; i < 50; i++) cout << stockPrices[i] << " ";
            cout << "\n... (" << (N - 50) << " more prices)\n";
        } else {
            for (int i = 0; i < N; i++) cout << stockPrices[i] << " ";
            cout << "\n";
        }
 
        quickSort(stockPrices, 0, N - 1);
 
        printHeader("Sorted Prices");
        if (N > 50) {  // Added truncation for large datasets
            cout << "First 50 sorted prices:\n";
            for (int i = 0; i < 50; i++) cout << stockPrices[i] << " ";
            cout << "\n... (" << (N - 50) << " more prices)\n";
        } else {
            for (int i = 0; i < N; i++) cout << stockPrices[i] << " ";
            cout << "\n";
        }
 
        countDuplicates(stockPrices, N);
 
        printHeader("Price Search");
        int target = stockPrices[rand() % N];
        cout << "Randomly selected target price: $" << target << "\n";
        findAllOccurrences(stockPrices, N, target);
 
        printHeader("Analysis Complete");
    }
    catch (const exception& e) {
        cerr << "\n!!! Error: " << e.what() << " !!!\n";
        return 1;
    }

    do {
        cout << "\nDo you want to run another session? (y/n): ";
        cin >> cont;
        if (cont != 'y' && cont != 'Y' && cont != 'n' && cont != 'N') {
                cout << "Invalid input! Please enter 'y' or 'n'.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (cont != 'y' && cont != 'Y' && cont != 'n' && cont != 'N');
 
    } while (cont == 'y' || cont == 'Y');
 
    cout << "\nThank you for using Stock Price Analyzer. Goodbye!\n";
    return 0;
}