#ifndef TEXT_PROCESSOR
#define TEXT_PROCESSOR

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Dictionary.hpp"
#include "Iterator.hpp"
#include <algorithm>
#include <cctype> 
#include <sstream> 
#include <boost/locale.hpp> 

using namespace std;

class Text_Processor {
private:

    static string cleanWord(string word){
        vector<string> specialChars = {"«", "»", "“", "”", "•", "‘", "’", "\xEF\xBF\xBD", "—", "–"};
        for(const string& s : specialChars){
            size_t pos;
            while((pos = word.find(s)) != string::npos){
                word.erase(pos, s.length());
            }
        }

        word.erase(remove_if(word.begin(), word.end(), [](unsigned char c){
            return (c <= 127 && ispunct(c) && c != '-') || isdigit(c);
        }), word.end());

        return word;
    }

public: 

static vector<string> readFile(string path){
    ifstream file(path);

    if(!file.is_open()){
        throw runtime_error("error: Invalid File");
    }

    boost::locale::generator gen;
    std::locale loc = gen("pt_BR.UTF-8");
    
    vector<string> words;
    string line;
    
    while(getline(file, line)){
        size_t pos;
        while ((pos = line.find("--")) != string::npos) {
            line.replace(pos, 2, " ");
        }

        stringstream ss(line);
        string word;
        
        while(ss >> word){
            word = boost::locale::to_lower(word, loc);
            
            word = cleanWord(word);

            while(!word.empty() && word.front() == '-') {
                word.erase(word.begin());
            }
            while(!word.empty() && word.back() == '-') {
                word.pop_back();
            }

            if(!word.empty())
                words.push_back(word);
        }
    }

    return words;
}

static void writeCSV(string name, Dictionary<string, int>* dict){

    ofstream file(name);

    if(!file.is_open()){
        throw runtime_error("error: Could not open output file: " + name);
    }

    vector<pair<string, int>> aux;
    Iterator<string, int>* it = dict->getIterator();

    while(it->hasNext()){
        auto next = it->next();
        aux.push_back(next);
    }
    
    delete it;

    boost::locale::generator gen;
    std::locale loc = gen("pt_BR.UTF-8");

    sort(aux.begin(), aux.end(), [&loc](const pair<string, int>& a, const pair<string, int>& b){
        return std::use_facet<std::collate<char>>(loc).compare(
            a.first.data(), a.first.data() + a.first.size(),
            b.first.data(), b.first.data() + b.first.size()
        ) < 0;
    });
    
    for(auto& entrie: aux){
        file << entrie.first << ", " << entrie.second << "\n";
    }
}

};

#endif