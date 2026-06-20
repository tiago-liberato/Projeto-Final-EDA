#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <filesystem>
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


for(string word: words){
    try {
        int aux = dict->get(word);
        dict->update(word, aux + 1);
    } catch(const out_of_range&) {
        dict->insert(word, 1);
    }
}


//Guarda o número de comparações necessárias para montar a estrutura
size_t comparisons = dict->getComparison_Counter();


// Nome do arquivo de métricas 
string bookName = file.substr(file.find_last_of("/\\") + 1);
bookName = bookName.substr(0, bookName.find_last_of('.'));
string csvMetrics = "result/" + bookName + "_Metricas.csv";


filesystem::create_directory("result/");


Text_Processor::writeCSV("result/" + bookName + "_result.csv", dict);


map<string, size_t> metrics;

//Cópia os dados do CSV de métricas para o map
ifstream fin(csvMetrics);
if(fin.is_open()){
    string line;
    getline(fin, line); // pula cabeçalho
    while(getline(fin, line)){
        size_t comma = line.find(',');
        if(comma != string::npos)
            metrics[line.substr(0, comma)] = stoull(line.substr(comma + 1));
    }
    fin.close();
}


// Atualiza só a estrutura atual e salva tudo
metrics[command] = comparisons;
ofstream fout(csvMetrics);

fout << "estrutura,comparacoes\n";

for(auto& [est, comp] : metrics){
    fout << est << "," << comp << "\n";
}

//verifica se todas as métricas foram coletadas e gera os gráficos
if(metrics.size() == 4){
    string comand = "python -u generate_charts.py \"result/" + bookName + "_Metricas.csv\"";
    cout << "Comando: " << comand << endl;
    system(comand.c_str());
}



delete dict;
return 0;

};

