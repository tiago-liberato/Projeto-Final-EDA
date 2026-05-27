#ifndef TEXT_PROCESSOR
#define TEXT_PROCESSOR

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
 * @brief 
 * 
 */
class Text_Processor{

private:

string path;
vector<string> words;


public: 

Text_Processor(string path){
    this->path = path;
}

    

};

#endif