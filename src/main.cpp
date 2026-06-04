#include <iostream>
#include <map>
#include <vector>
#include "Text_Processor.hpp"
#include "AVLtree.hpp"
#include "RedBlackTree.hpp"

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
        //TODO
        break;
    case 4:
        //TODO
        break;

    default:
        cout << "Comando inválido" << "\n";
        help();
        return 1;
}


for(string word: words){
    if(dict->contains(word)){
        int aux = dict->get(word);
        aux ++;
        dict->update(word, aux);
    }else{
        dict->insert(word, 1);
    }
}

Text_Processor::writeCSV("result.csv", dict);

delete dict;
return 0;

};

