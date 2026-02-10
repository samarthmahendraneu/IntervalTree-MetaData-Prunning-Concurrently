
#include <iostream>
#include <algorithm>


// (file path, min, max)
// "2024-01-01/0001.parquet", "aaa", "app"
// "2024-01-01/0002.parquet", "baa", "cat"
// "2024-01-01/0003.parquet", "bob", "dad"
// "2024-01-01/0004.parquet", "eel", "fit"
// "2024-01-01/0005.parquet", "goo", "hop"
// "2024-01-01/0006.parquet", "ink", "lit"
// "2024-01-01/0007.parquet", "pop", "sit"
// "2024-01-01/0008.parquet", "run", "zoo"
// "2024-01-02/0001.parquet", "aaa", "app"
// "2024-01-02/0002.parquet", "baa", "cat"
// "2024-01-03/0001.parquet", "www", "yaa"
// "2024-01-04/0001.parquet", "ybb", "zzz"
//
// Lookup
// Input: <"2024-01-01", "website">, <"2024-01-04", "youth">, <"2024-01-01", "buzz">
// Output:
// <"2024-01-01", "website">: ["2024-01-01/0008.parquet"]
// <"2024-01-04", "youth">: ["2024-01-04/0001.parquet"]

// lookup [date] - > []

// iterval (a, b)

 // Tree -> itervals - >

// """ million files -> iterate  """
//



// two -> [itervals]
// 3 -> split -> add


#include <unordered_map>
#include <map>
#include <shared_mutex>
#include <string>
#include <utility>
#include <thread>
using namespace std;

class Interval {
public:
    string start, end;
    string max_value;
    string file_name;
    bool leaf = true;

    Interval(string start, string end, string max_value):start(std::move(start)), end(std::move(end)), max_value(std::move(max_value)){}
};

class Node {

Interval data;
Node* left;
Node* right;
shared_mutex m;

public:
    Node(): data(Interval("", "", "")) {
        left = nullptr;
        right = nullptr;
    }

    void insert(const string& start, const string& end, string file_name) {
        unique_lock lock(m);
        if (this->data.leaf) {
            this->data.start = start;
            this->data.end = end;
            this->data.max_value = end;
            this->data.file_name = file_name;
            this->data.leaf = false;
            this->left = new Node();
            this->right = new Node();
        }
        else {
            if (start <= this->data.start) {
                lock.unlock();
                left->insert(start, end, file_name);
                lock.lock();
            }
            else {
                lock.unlock();
                right->insert(start, end, file_name);
                lock.lock();
            }

        }

        this->data.max_value = max(this->data.max_value, end);
    }

    vector<string> find(string search_key) {
        shared_lock  lock(m);
        vector<string> res;

        if (this->data.start <= search_key && search_key <= this->data.end) {
            res.push_back(this->data.file_name);
        }

        if (!this->data.leaf) {
            if (search_key <= this->data.max_value) {
                vector<string> temp = left->find(search_key);
                res.insert(res.begin(), temp.begin(), temp.end());
            }
            if (search_key >= data.start){
                vector<string> temp = right->find(search_key);
                res.insert(res.begin(), temp.begin(), temp.end());
            }
        }

        return res;
    }

    ~Node() = default;

};



int main() {

    // "2024-01-01/0001.parquet", "aaa", "app"
    // "2024-01-01/0002.parquet", "baa", "cat"
    // "2024-01-01/0003.parquet", "bob", "dad"

    Node* root = new Node();
    root->insert("aaa", "app", "2024-01-01/0001.parquet");
    root->insert("baa", "cat", "2024-01-01/0002.parquet");
    root->insert("bob", "dad", "2024-01-01/0003.parquet");
    thread t1([&]() {
        while (true) {
            cout<<"daa"<<endl;
                for (auto x: root->find("daa")) {
        cout<<x<<endl;
        }
        }

    });


    thread t2([&]() {
        while (true) {
            cout<<"aba"<<endl;
for (auto x: root->find("aba")) {
    cout<<x<<endl;
        }
        }
    });
    t1.join();
    t2.join();

}