#include "utils.h"

vector<string> split(string linha, char sep) {
    /*  Funcao que recebe uma string e separa ela em tokens.
        Semelhante ao split do Python */
    vector<string> res;
    string token = "";
    for(int i=0; i<linha.size(); i++) {
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