#include <iostream>
#include <map>
using namespace std;


void help(){
    cout << "Comando inválido" << endl;
    //TODO
    return;

};

int main(int arg, char* args[]){

    if(arg < 1){
        help();
    }

    map<string, int> opc = {
        {"--help", 0},
        {"dicionario_avl", 1},
        {"dicionario_redblack", 2},
        {"dicionario_chaining", 3},
        {"dicionario_openadr", 4}
    };

    string command = args[3];

    switch (opc[command])
    {
    case 0:
        help();
        break;

    case 1: 
        //TODO  
        break;
    case 2:
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

