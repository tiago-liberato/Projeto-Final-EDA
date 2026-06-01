#include <iostream>
#include <map>
#include <vector>
#include "Text_Processor.hpp"
#include "AVLtree.hpp"

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
    Dictionary<string, int>* dict;

    switch (opc[command])
    {
    case 0:
        help();
        break;

    case 1:{
        
        break;
    }case 2:
        //TODO
        break;
    case 3:
        //TODO
        break;
    case 4:
        //TODO
        break;
    default:
        break;
    }

};

