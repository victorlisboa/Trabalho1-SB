#include "utils.h"

vector<string> split(string linha, char sep) {
    /*  Funcao que recebe uma string e separa ela em tokens.
        Semelhante ao split do Python */
    vector<string> res;
    string token = "";
    for(auto i=0u; i<linha.size(); i++) {
        if(linha[i] != sep) {
            token += linha[i];
        }
        else {
            res.push_back(token);
            token = "";
        }
    }
    if(token != "") res.push_back(token);
    return res;
}

string get_file_name(string raw_file_name) {
    string file_name = "";
    vector<string> path = split(raw_file_name, '/');
    string last = path[path.size()-1];
    for(auto i=0u; i<last.size(); i++) {
        file_name += last[i];
    }
    return file_name;
}

// tabela de instrucoes -> <instrucao, tamanho> onde o opcode eh o proprio index
vector<pair<string, int>> TI = {
    {"ADD", 2},
    {"SUB", 2},
    {"MUL", 2},
    {"DIV", 2},
    {"JMP", 2},
    {"JMPN", 2},
    {"JMPP", 2},
    {"JMPZ", 2},
    {"COPY", 3},
    {"LOAD", 2},
    {"STORE", 2},
    {"INPUT", 2},
    {"OUTPUT", 2},
    {"STOP", 1}
};