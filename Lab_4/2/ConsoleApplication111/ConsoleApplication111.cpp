#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <conio.h> 

using namespace std;

// Шаблон класса для гистограммы
template<typename Key, typename Value>
class hist {
public:
    map<Key, Value> data;

    // Оператор индексации для удобного доступа к элементам гистограммы
    Value& operator[](const Key& key) {
        return data[key];
    }
};

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
    copy(istream_iterator<my_data>(cin),
        istream_iterator<my_data>(),
        back_inserter<vector<my_data>>(dd));

    // Создание гистограммы
    hist<string, int> h1;
    for (const auto& data : dd) {
        h1[data.c[5]]++;
    }

    hist<string, int> h2;
    for (const auto& data : dd) {
        h2[data.c[10]]++;
    }

    // Вывод гистограммы
    for (const auto& entry : h1.data) {
        cout << entry.first << " " << entry.second << endl;
    }
    for (const auto& entry : h2.data) {
        cout << entry.first << " " << entry.second << endl;
    }
    auto it1 = h1.data.begin();
    auto it2 = h2.data.begin();
    while (it1 != h1.data.end() && it2 != h2.data.end()) {
        cout << it1->first << " " << it1->second << " | " << it2->first << " " << it2->second << endl;
        ++it1;
        ++it2;
    }

    _getch();
    return 0;
}