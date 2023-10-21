#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <map>
#include <string>
#include <array>
#include "preprocessador.h"
#include "utils.h"

using namespace std;

map<string, array<int, 3>> TS;          // tabela de simbolos <simbolo, <definido, endereco, linha>>
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

void first_pass(string file_name) {
    ifstream file(file_name);
    string text;
    
    int contador_pos = 0, contador_linha = 0;
    while(getline(file, text)) {
        contador_linha++;
        if(text.find("SECAO") != -1) continue;
        vector<string> linha = split(text);
        string instrucao = linha[0];                // nao necessariamente vai ser uma instrucao
        if(linha[0][linha[0].size()-1] == ':') {    // tem rotulo na linha
            instrucao = linha[1];
            string rotulo = linha[0].substr(0, linha[0].size()-1);
            if(TS[rotulo][0]) {                     // rotulo ja foi definido
                cout << "ERRO SEMANTICO na linha " << contador_linha << ": rotulo " <<
                rotulo << " previamente definido na linha " << TS[rotulo][2] << ".\n";
            }
            else {  // insere rotulo na tabela de simbolos
                TS[rotulo][0] = true;
                TS[rotulo][1] = contador_pos;
                TS[rotulo][2] = contador_linha;
            }
        }
        bool achou_instrucao = false;               // procura instrucao na tabela de instrucoes
        for(int i=0; i<TI.size(); i++) {
            if(instrucao == TI[i].first) {
                achou_instrucao = true;
                contador_pos += TI[i].second;
                break;
            }
        }
        bool achou_diretiva = false;
        if(!achou_instrucao) {  // se nao eh uma instrucao, pode ser uma diretiva
            if(instrucao == "CONST") {
                achou_diretiva = true;
                contador_pos += 1;
            }
            else if(instrucao == "SPACE") {
                achou_diretiva = true;
                if(linha[linha.size()-1] != "SPACE") {
                    contador_pos += stoi(linha[linha.size()-1]); // reserva X palavras (16 bits)
                }
                else {
                    contador_pos += 1;
                }
            }
        }
        if(!achou_instrucao && !achou_diretiva) {
            cout << "ERRO LEXICO na linha " << contador_linha << ": expressao " <<
            instrucao << " nao reconhecida.\n";
        }
    }
}

int main(int arg, char *argv[]) {

    if(arg < 2) {
        cout << "Voce deve incluir o nome do programa .asm para compilacao!\n";
        exit(1);
    }
    
    string FILE_NAME = preprocessador(argv);
    first_pass(FILE_NAME);

    cout << "Tabela de Simbolos:\nRotulo\t\tEndereco\tLinha\n";
    for(auto [rotulo, arr] : TS) {
        if(TS[rotulo][0]) {
            cout << rotulo << "\t\t" << TS[rotulo][1] << "\t\t" << TS[rotulo][2] << '\n';
        }
    }

    return 0;
}
