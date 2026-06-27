#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <filesystem>
#include <chrono>
#include "Text_Processor.hpp"
#include "AVLtree.hpp"
#include "RedBlackTree.hpp"
#include "HashChaining.hpp"
#include "HashOpenAdr.hpp"

using namespace std;


void help(){
    cout << "Comandos válidos:" << "\n";
    cout << "--help" << "\n";
    cout << "dicionario_avl" << "\n";
    cout << "dicionario_redblack" << "\n";
    cout << "dicionario_chaining" << "\n";
    cout << "dicionario_openAdr" << "\n";
    return;

};

int main(int arg, char* args[]){

    if(arg < 3){
        help();
        return 1;
    }

    map<string, int> opc = {
        {"--help", 0},
        {"dicionario_avl", 1},
        {"dicionario_redblack", 2},
        {"dicionario_chaining", 3},
        {"dicionario_openadr", 4}
    };

    string command = args[1];
    string file = args[2];

    
    vector<string> words = Text_Processor::readFile(file);
    Dictionary<string, int>* dict = nullptr;

    switch (opc[command])
    {
    case 0:
        help();
        return 0;

    case 1:{

        dict = new AVLtree<string, int>();
        break;

    }case 2:
        dict = new RedBlackTree<string, int>();
        break;
    case 3:
        dict = new HashChaining<string, int>();
        break;
    case 4:
        dict = new HashOpenAdr<string, int>();
        break;

    default:
        cout << "Comando inválido" << "\n";
        help();
        return 1;
}

auto start = chrono::high_resolution_clock::now();

for(string word: words){
    try {
        int aux = dict->get(word);
        dict->update(word, aux + 1);
    } catch(const out_of_range&) {
        dict->insert(word, 1);
    }
}

auto end = chrono::high_resolution_clock::now();
auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);


//Guarda o número de comparações necessárias para montar a estrutura
size_t comparisons = dict->getComparison_Counter();


// Nome do arquivo de métricas 
string bookName = file.substr(file.find_last_of("/\\") + 1);
bookName = bookName.substr(0, bookName.find_last_of('.'));
string csvMetrics = "result/" + bookName + "_Metricas.csv";


filesystem::create_directory("result/");


Text_Processor::writeCSV("result/" + bookName + "_result.csv", dict);


map<string, pair<size_t, long long>> metrics;

//Cópia os dados do CSV de métricas para o map
ifstream fin2(csvMetrics);
if(fin2.is_open()){
    string line;
    getline(fin2, line); // pula cabeçalho
    while(getline(fin2, line)){
        size_t c1 = line.find(',');
        size_t c2 = line.find(',', c1 + 1);
        if(c1 != string::npos && c2 != string::npos){
            string est = line.substr(0, c1);
            size_t comp = stoull(line.substr(c1 + 1, c2 - c1 - 1));
            long long tempo = stoll(line.substr(c2 + 1));
            metrics[est] = {comp, tempo};
        }
    }
    fin2.close();
}

// Atualiza a estrutura atual
metrics[command] = {comparisons, duration.count()};

// Escreve o CSV
ofstream fout(csvMetrics);
fout << "estrutura,comparacoes,tempo_ms\n";
for(auto& [est, data] : metrics){
    fout << est << "," << data.first << "," << data.second << "\n";
}

delete dict;
return 0;

};

