#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <map>
#include <string>
#include <array>
#include <algorithm>
using namespace std;

vector<pair<string, int>> TI =          // tabela de instrucoes <instrucao, tamanho> 
{                                       // opcode eh o proprio index
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

vector<string> split(string linha, char sep=' ') {
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
int data_section(vector <int> mem){
    return 0;
}

void simulador(vector <int> mem, int data){
    int contador_pos = 0, contador_linha = 0;
    bool sair = false;
    int acc = 0;
    int pc_skip;

    while(!sair){
        string saida = "";
        bool skip = false;
        int instrucao = (mem[contador_pos]);
        int op1, op2;
        if(contador_pos + 1 < mem.size()) op1 = mem[contador_pos+1];
        if(contador_pos + 2 < mem.size()) op2 = mem[contador_pos+2];
        switch(instrucao){
            case 1: acc += mem[op1];
            break;
            case 2: acc -= mem[op1];
            break;
            case 3: acc *= mem[op1];
            break;
            case 4: acc /= mem[op1];
            break;
            case 5: pc_skip = mem[op1]; skip = true;
            break;
            case 6: if(acc < 0) {
                pc_skip = mem[op1];
                skip = true;
            }
            break;
            case 7: if(acc > 0) {
                pc_skip = mem[op1];
                skip = true;    
            }
            break;
            case 8: if(acc == 0) {
                pc_skip = op1;
                skip = true;    
            }
            break;
            case 9: mem[op2] = mem[op1]; 
            break;
            case 10: acc = mem[op1];
            break;
            case 11: mem[op1] = acc;
            break;
            case 12: scanf("%i", &mem[op1]);
            break;
            case 13: saida = to_string(mem[op1]);
            break;
            case 14: sair = true;
            break;
        }
        getchar();
        cout<<"PC <- " << contador_pos << " ACC <- " << acc;
        if(saida != ""){
            cout << " SAIDA: " << saida;
        }
        if(!skip) contador_pos += TI[instrucao-1].second;
        else contador_pos = pc_skip;
        cout << endl;
        
    }  
}

int main(int arg, char *argv[]){

    if(arg < 2) {
        cout << "Voce deve incluir o nome do programa .asm para compilacao!\n";
        exit(1);
    }

    string file_name = argv[1], text;
    ifstream file(argv[1]);
    getline(file, text);
    vector <string> slots = split(text);
    vector <int> mem;
    std::transform(slots.begin(), slots.end(), std::back_inserter(mem),
            [](const std::string& str) { return std::stoi(str); });

    int data = data_section(mem);
    simulador(mem, data);
    return 0;
}