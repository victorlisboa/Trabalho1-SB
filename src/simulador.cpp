#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <map>
#include <string>
#include <array>
#include <algorithm>
#include "utils.h"

using namespace std;

int data_section(vector <int> mem){
    int data = mem.size();
    int contador_pos = 0;
    while (contador_pos < mem.size() && contador_pos < data){
        int instrucao = mem[contador_pos];
        int tamanho = TI[instrucao - 1].second;
        if (instrucao < 5 || instrucao > 8){
            for (int i=1;i<tamanho;i++){
                int op = mem[contador_pos+i];
                if(op < data) data = op;
            }
        }
        // cout << "INSTRUÇÃO: " << instrucao << endl;
        // cout << "Quantidade de operandos: " << tamanho << endl;
        // cout << "Data: " << data << endl; 
        contador_pos += tamanho;
    }
    return data;
}

void simulador(vector <int> mem, int data){
    int contador_pos = 0, contador_linha = 0;
    bool sair = false;
    int acc = 0;
    int pc_skip;

    while(!sair){
        string saida = "";
        bool skip = false;
        int instrucao = mem[contador_pos];
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

        if(instrucao >= 5 && instrucao <= 8){
            if(op1 >= data){
                cout << "Segmentation2 fault (core dumped)" << endl;
                break;
            }
        }
        
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
    transform(slots.begin(), slots.end(), back_inserter(mem),
            [](const string& str) { return stoi(str); });

    int data = data_section(mem);
    simulador(mem, data);
    return 0;
}