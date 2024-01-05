#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <list>
#include <fstream>
#include <conio.h>
#include <memory>
#include <regex>

using namespace std;

// Abstract base class representing data elements
struct data_elem {
    virtual data_elem* create() = 0;               // Factory method for creating instances
    virtual ostream& print(ostream&) const = 0;    // Output data to ostream
    virtual istream& read(istream&) = 0;           // Read data from istream
    virtual ~data_elem() = default;                // Virtual destructor for polymorphic behavior
};

// Data creator class template
template <typename elem_name, typename data_type>
struct data_creator : public data_elem {
    data_type data;  // Actual data stored in the instance
    // Factory method implementation
    data_elem* create() override { return new elem_name; }
    // Output data to ostream
    ostream& print(ostream& o) const override {
        return o << data;
    }
    // Read data from istream
    istream& read(istream& i) override {
        return i >> data;
    }
};

// Data class for string data
struct string_data : public data_creator<string_data, string> {
    // Read data from istream with special handling for newline characters
    istream& read(istream& i) override {
        getline(i, data, '\n');
        return i;
    }
};

// Template class for histograms
template <typename Key, typename Value>
class hist {
public:
    map<Key, Value> data;  // Map to store histogram data

    // Overloaded [] operator for convenient access to histogram entries
    Value& operator[](const Key& key) {
        return data[key];
    }
};

// Data class for integer data
struct int_data : public data_creator<int_data, int> {};

// Factory class for creating data elements
struct fabric : public map<string, data_elem*> {
    // Constructor initializes default data element types
    fabric() {
        (*this)["Study Results"] = new string_data;
        (*this)["Age"] = new string_data;
    }

    // Factory method to create data elements based on name
    data_elem* create(string& name) {
        data_elem*& d = (*this)[name];
        if (!d) {
            erase(find(name)->first);
            return nullptr;
        }
        else {
            return d->create();
        }
    }

    // Destructor to clean up allocated memory
    ~fabric() {
        for (auto& p : *this) {
            if (p.second)
                delete p.second;
        }
    }
};

// Database class to store data elements
struct my_data_base : public list<data_elem*> {
    // Destructor to clean up allocated memory
    ~my_data_base() {
        for (auto& d : *this) {
            if (d)
                delete d;
        }
    }
};

fabric f;  // Global instance of the factory class

// Input operator to read data elements from istream
istream& operator>>(istream& i, data_elem*& d) {
    string name;
    // Read name and use the factory to create the appropriate data element
    if (getline(i, name, '=') && (d = f.create(name)) && d->read(i)) {
        return i;
    }
    else {
        cerr << "error at:" << name << endl;
        d = nullptr;
        return i;
    }
}

// Output operator to print data elements to ostream
ostream& operator<<(ostream& o, const data_elem* d) {
    if (d) {
        d->print(o);
        return o;
    }
    else {
        return o;
    }
}

// Class representing the header of data columns
struct DataHeader {
    vector<string> columns;  // Vector to store column names

    // Constructor to parse header string and extract column names
    DataHeader(const string& headers) {
        istringstream headersStream(headers);
        string headerRow;
        getline(headersStream, headerRow);

        istringstream headerRowStream(headerRow);

        // Parse header row and remove quotes, storing names in the vector
        while (getline(headerRowStream, headerRow, ',')) {
            headerRow.erase(remove(headerRow.begin(), headerRow.end(), '\"'), headerRow.end());
            columns.push_back(headerRow);
        }
    }

    // Output specific header at index 5
    void printSpecificHeaders5() const {
        if (columns.size() >= 5) {
            cout << columns[5];
        }
    }

    // Output specific header at index 11
    void printSpecificHeaders11() const {
        if (columns.size() >= 11) {
            cout << columns[11];
        }
    }

    // Get header name by index
    string getHeaderByIndex(size_t index) const {
        if (index < columns.size()) {
            return columns[index];
        }
        return "";
    }

    // Output operator to print header to ostream
    friend ostream& operator<<(ostream& os, const DataHeader& header) {
        for (const auto& col : header.columns) {
            os << col << ' ';
        }
        return os;
    }
};

struct data_column : public string {};
struct data_2column : public string {};

// Struct representing individual data records
struct my_data {
    int num;           // Record number
    data_column c[26];  // Array to store 26 data columns
};

// Input operator to read data_column from istream
istream& operator>>(istream& is, data_column& f) {
    char c;
    int num = 0;
    f.clear();
    // Read characters and handle quotes for comma-separated values
    while (is.get(c)) {
        switch (c) {
        case '"':
            num++;
            break;
        case ',':
            if (!(num & 1))
                return is;
        default:
            f += c;
        }
    }
    return is;
}

// Input operator to read my_data from istream
istream& operator>>(istream& is, my_data& d) {
    string l;
    getline(is, l);
    istringstream tmpi(l);
    // Read record number and the first data column
    tmpi >> d.num >> d.c[0];
    // Read the remaining 25 data columns
    for (int i = 0; i < 26; i++) {
        tmpi >> d.c[i];
    }

    return is;
}

// Output histograms to input.txt file
void processHistograms(const vector<my_data>& dd, const DataHeader& header) {
    hist<string, int> h1;
    // Populate histogram for column 5
    for (const auto& data : dd) {
        h1[data.c[5]]++;
    }

    hist<string, int> h2;
    // Populate histogram for column 10
    for (const auto& data : dd) {
        h2[data.c[10]]++;
    }

    // Output to input.txt file
    ofstream outputFileInput("input.txt");

    streambuf* coutbufInput = cout.rdbuf();

    // Redirect output to file
    cout.rdbuf(outputFileInput.rdbuf());

    // Output histogram for column 5
    for (const auto& entry : h1.data) {
        if (!entry.first.empty()) {
            header.printSpecificHeaders5();
            cout << "=" << entry.first << endl;
        }
    }

    // Output histogram for column 10
    for (const auto& entry : h2.data) {
        if (!entry.first.empty()) {
            header.printSpecificHeaders11();
            cout << "=" << entry.first << endl;
        }
    }

    // Restore cout buffering
    cout.rdbuf(coutbufInput);

    // Close the file
    outputFileInput.close();
}

// Process data from output.txt file
void processFile(const string& filename) {
    // Open file for reading
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    // Create histogram for text strings
    hist<string, int> textHistogram;

    // Create histogram for numeric strings
    hist<string, int> numericHistogram;

    // Regular expression for age
    regex ageRegex(R"(\b(?:up to\s*)?(\d+)\s*(?:Days?|Months?|Years?|Weeks?)\s*(?:to\s*(\d+)\s*(?:Days?|Months?|Years?|Weeks?)|\sand\solder)?\b|\b(?:up to\s*)?(\d+)\s*Years?\s*and\s*older\b)");

    // Read data from file and count occurrences of each value
    string line;
    while (getline(inputFile, line)) {
        // Check if the line contains only numbers
        bool isNumeric = all_of(line.begin(), line.end(), ::isdigit);

        // Check if the line matches the age regular expression
        if (regex_search(line, ageRegex)) {
            numericHistogram[line]++;
        }
        else if (isNumeric) {
            numericHistogram[line]++;
        }
        else {
            textHistogram[line]++;
        }
    }

    // Output text string histogram
    cout << "Text Histogram:" << endl;
    for (const auto& entry : textHistogram.data) {
        cout << "Study Results=" << entry.first << " " << entry.second << endl;
    }

    // Output numeric string histogram
    cout << "\nNumeric Histogram:" << endl;
    for (const auto& entry : numericHistogram.data) {
        cout << "Age=" << entry.first << " " << entry.second << endl;
    }

    // Output combined histogram data to the console
    auto it1 = textHistogram.data.begin();
    auto it2 = numericHistogram.data.begin();
    cout << "\nCombination of study results and age histogram:" << endl;
    while (it1 != textHistogram.data.end() && it2 != numericHistogram.data.end()) {
        cout << "Combination of study results and age = " << it1->first << " " << it1->second << " | " << it2->first << " " << it2->second << endl;
        ++it1;
        ++it2;
    }

    // Close the file
    inputFile.close();
}

int main(int argc, char** argv) {
    // Define the data header with column names
    DataHeader header("\"Rank\",\"NCT Number\",\"Title\",\"Acronym\",\"Status\",\"Study Results\",\"Conditions\",\"Interventions\",\"Outcome Measures\",\"Sponsor/Collaborators\",\"Gender\",\"Age\",\"Phases\",\"Enrollment\",\"Funded Bys\",\"Study Type\",\"Study Designs\",\"Other IDs\",\"Start Date\",\"Primary Completion Date\",\"Completion Date\",\"First Posted\",\"Results First Posted\",\"Last Update Posted\",\"Locations\",\"Study Documents\",\"URL\"");

    // Vector to store my_data records
    vector<my_data> dd;

    // Database to store data elements
    my_data_base b;

    // Read my_data records from cin and store in vector dd
    copy(istream_iterator<my_data>(cin), istream_iterator<my_data>(), back_inserter<vector<my_data>>(dd));

    // Output histograms to input.txt
    processHistograms(dd, header);

    // Open input.txt and output.txt files
    ifstream inputFile("input.txt");
    ofstream outputFileOutput("output.txt");
    streambuf* coutbufOutput = cout.rdbuf();
    cout.rdbuf(outputFileOutput.rdbuf());

    // Output data elements to output.txt
    copy_if(istream_iterator<data_elem*>(inputFile), istream_iterator<data_elem*>(), back_inserter(b),
        [](data_elem* d) { return d != nullptr; });

    // Output data elements from the database to console
    copy(begin(b), end(b), ostream_iterator<data_elem*>(cout, "\n"));

    // Restore cout buffering
    cout.rdbuf(coutbufOutput);

    // Output histograms from output.txt to console
    processFile("output.txt");

    // Output completion message
    cout << "Procedure completed";
    _getch();
    return 0;
}
