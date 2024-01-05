#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <regex>
#include <conio.h>

using namespace std;

struct data_column : public string {};

struct my_data {
    int num;
    data_column c[26];
};

istream& operator>>(istream& is, data_column& f) {
    char c;
    int num = 0;
    f.clear();
    while (is.get(c)) {
        switch (c) {
        case ',':
            if (!(num & 1))
                return is;
        default: f += c;
        }
    }
    return is;
}

istream& operator>>(istream& is, my_data& d) {
    string l;
    getline(is, l);
    istringstream tmpi(l);

    tmpi >> d.num >> d.c[0];
    for (int i = 0; i < 26; i++) {
        tmpi >> d.c[i];
    }

    return is;
}

int main() {
    vector<my_data> dd;
    copy(istream_iterator<my_data>(cin), istream_iterator<my_data>(), back_inserter<vector<my_data>>(dd));

    ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        cerr << "Error opening output file!" << endl;
        return 1;
    }

    map<string, int> resultsHistogram;
    map<string, int> ageHistogram;
    map<string, int> resultsAgeCombinationHistogram;

    regex ageRegex(R"(\b(\d+)\s*Years?\s*to\s*(\d+)\s*Years?\b)");

    for_each(dd.begin(), dd.end(), [&](my_data& d) {
        string result = d.c[5];
        smatch match;
        string ageStr = d.c[11];
        if (regex_search(ageStr, match, ageRegex)) {
            string minAgeStr = match[1];
            string maxAgeStr = match[2];
            string ageCombination = result + " " + minAgeStr + "-" + maxAgeStr;

            resultsHistogram[result]++;
            ageHistogram[minAgeStr + "-" + maxAgeStr]++;
            resultsAgeCombinationHistogram[ageCombination]++;
        }
        });

    auto printHistogram = [&](const auto& histogram, const string& title) {
        outputFile << title << ":" << endl;
        for_each(histogram.begin(), histogram.end(), [&](const auto& entry) {
            outputFile << entry.first << ": " << entry.second << " patients" << endl;
            });
        outputFile << endl;
        };

    printHistogram(resultsHistogram, "Results Histogram");
    printHistogram(ageHistogram, "Age Histogram");
    printHistogram(resultsAgeCombinationHistogram, "Results and Age Combination Histogram");

    outputFile.close();
    cout << "Results written to 'output.txt'" << endl;

    _getch();

    return 0;
}
