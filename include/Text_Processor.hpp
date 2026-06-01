#ifndef TEXT_PROCESSOR
#define TEXT_PROCESSOR

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Dictionary.hpp"
#include "Iterator.hpp"
using namespace std;

/**
 * @brief  Classe reponsável por processar, ler e escrever arquivos
 * 
 */
class Text_Processor{
public: 

static vector<string> readFile(string path){
    ifstream file(path);

    if(!file.is_open()){
        throw runtime_error("error: Invalid File");
    }

    vector<string> words;
    string word;

    while(file >> word){
        words.push_back(word);
    }

    return words;
}

static void writeCSV(string name, Dictionary<string, int>* dict){
    ofstream file(name);

    if(!file.is_open()){
        throw runtime_error("error: ");
    }

    Iterator<string, int>* it = dict->getIterator();

    while(it->hasNext()){
        auto next = it->next();
        file << next.first << ", " << next.second << "\n";
    }
}

    

};

#endif