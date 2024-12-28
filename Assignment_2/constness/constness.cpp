#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// holds entries with int key and string values
class Table {
    // you'll probably want use to use std::pair
    // you will need to make some methods const since they'll be used in processSentence which requires a const Table
    // --- Your code here
    public:
    std::pair<int, std::string> get(int key) const{
        for (int i=0;i<d.size();i++){
            if(d[i].first==key){
                return d[i];
            }
        }
        return {-1, " "};
    }
    void add(int key, std::string value){
        d.push_back({key,value});
    }
    Table()=default;
    Table(std::initializer_list<std::pair<int,std::string>> list){
        d=list;
    }
    Table(std::vector<std::pair<int,std::string>> list){
        d=list;
    }

    std::vector<std::pair<int,std::string>> d;
    // --- 
};

std::string processSentence(const Table& t, const std::vector<int>& keySequence) {
    std::string ret = "";
    for (int key : keySequence) {
        // should return " " if it's not in the table, otherwise it should return the value
        ret += t.get(key).second;
    }
    return ret;
}

// --- Your code here 
Table filterOutOddKeys(const Table & t){
    Table tf;
    for (int i=0;i<t.d.size();i++)
    {
        if(t.d[i].first %2 == 0)
        {
            tf.add(t.d[i].first, t.d[i].second);
        }
    }
    return tf;
}

Table updateTable(const Table& t1, const Table& t2) {
    // make a copy of t1 with overwriting entries from t2
    // replace entries of t1 with entries of t2, where they overlap
    // --- Your code here
    Table t3;
    for (int i=0; i<t1.d.size();i++){
        std::pair<int, std::string> entryt1 = t1.d[i];
        std::pair<int, std::string> entryt2= t2.get(entryt1.first);

        if(entryt2.second != " "){
            t3.add(entryt2.first,entryt2.second);
        }
        else{
            t3.add(entryt1.first,entryt1.second);
        }

    }
    return t3;
    }


// ---

int main(int argc, char* argv[]) {
    // figure out what constructor allows for this initialization
    // since this table is const, the functions you implement that use it must also take in a const table
    const Table t1{{
        {1, "alpha"},
        {2, "beta"},
        {3, "gamma"},
        {4, "delta"},
        }};
    Table t2;

    std::ifstream input{"input.txt"};
    std::ofstream output{"output.txt"};

    // should output "alphagamma delta" (without parenthesis)
    output << processSentence(t1, {1, 3, 5, 4}) << std::endl;

    while (input.peek() != std::ifstream::traits_type::eof()) {
        int key;
        std::string value;
        input >> key >> value;

        t2.add(key, value);
    }
    output << processSentence(t2, {2, 1, 5, 6}) << std::endl;


    // create a constant table that is t1 updated with t2
    // you should use the updateTable function here
    // note that for it to be constant, its value has to be finalized at definition time
    // --- Your code here
    const Table t3 = updateTable(t1, t2);

    // ---

    output << std::is_const<decltype(t3)>::value << std::endl;

    // should not change the original tables
    output << processSentence(t1, {1, 3, 5, 4}) << std::endl;
    output << processSentence(t2, {2, 1, 5, 6}) << std::endl;

    // should have entries of t1 with overriding values from t2
    output << processSentence(t3, {1, 3, 5, 4}) << std::endl;
    // you need to implement this function to create a copy of the Table with only even keys
    output << processSentence(filterOutOddKeys(t3), {2, 1, 3, 5, 4}) << std::endl;
}