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

void first_pass(string file_name) {
    ifstream file(file_name);
    string text;
    
    int contador_pos = 0, contador_linha = 0;
    while(getline(file, text)) {
        contador_linha++;
        if(text.find("SECAO") != string::npos) continue;
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
        for(auto i = 0u; i<TI.size(); i++) {
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
int getOp(string expression){
    vector<string> values = split(expression,'+');
    int op = 0;
    if (TS.find(values[0]) != TS.end()){
        op += TS[values[0]][1];
        if(values.size() > 1) op += stoi(values[1]);
    }
    else{
        return -1;
    }
    return op;
}
void second_pass(string file_name) {
    ifstream file(file_name);
    string text, raw_name = get_file_name(file_name), path = file_name.substr(0, file_name.size()-raw_name.size());
    // size_t last_index = file_name.find_last_of(".");
    // string raw_name = file_name.substr(0, last_index);
    ofstream newFile(path + raw_name.substr(10,raw_name.size()-14) + ".obj");
    string secao = "text";

    int contador_pos = 0, contador_linha = 0;
    while(getline(file, text)) {
        bool operandos_corretos = true;
        bool rotulo_ausente = false;
        contador_linha++;
        if(text.find("SECAO") != string::npos) {
            if (text.find("SECAO DATA") != string::npos) {
                secao = "data";
            }
            continue;
        }
        vector<string> linha = split(text);
        string instrucao = linha[0];                // nao necessariamente vai ser uma instrucao
        if(linha[0][linha[0].size()-1] == ':') {    // tem rotulo na linha
            linha.erase(linha.begin());
        }

        instrucao = linha[0];
        string op1 = "", op2 = "";
        if (secao == "text"){
            if (instrucao == "STOP"){
                if(linha.size() != 1){
                    operandos_corretos = false;
                }
                else{
                    newFile << "14 ";
                }
            }
            else if(instrucao == "COPY"){
                if(linha.size() != 2){
                    operandos_corretos = false;
                }
                else{
                    vector<string> ops = split(linha[1],',');
                    if (ops.size() != 2){
                        operandos_corretos = false;
                    }
                    else{
                        newFile << "9 ";
                        op1 = ops[0];
                        op2 = ops[1];
                    }
                }
            }
            else{
                bool achou_instrucao = false;
                for(auto i = 0u; i<TI.size(); i++) {
                    if(instrucao == TI[i].first) {
                        achou_instrucao = true;
                        string opcode = to_string(i+1);
                        contador_pos += TI[i].second;
                        newFile << opcode << " ";
                        if(linha.size() != 2 || split(linha[1], ',').size() != 1){
                            operandos_corretos = false;
                        }
                        else{
                            op1 = linha[1];
                        }
                        break;
                    }
                }
                if(!achou_instrucao) {
                    cout << "ERRO LEXCIO na linha " << contador_linha << ". Operacao " << instrucao << " invalida." << endl;
                }
            }
            int op;
            if(op1 != ""){
                op = getOp(op1); 
                if(op != -1){
                    newFile << to_string(op) << " ";
                }
                else {
                    rotulo_ausente = true;
                }
            }
            if(op2 != ""){
                op = getOp(op2); 
                if(op != -1){
                    newFile << to_string(op) << " ";
                }
                else {
                    rotulo_ausente = true;
                }
            }      
        }
        if(secao == "data"){
            string diretiva = linha[0];
            if(linha.size() > 2){
                operandos_corretos = false;
            }
            else{
                if(diretiva == "SPACE"){
                    int repeticoes = 1;
                    if(linha[linha.size()-1] != "SPACE") {
                        repeticoes = stoi(linha[1]);
                    }
                    for (int i = 0; i < repeticoes; i++){
                        newFile << "0 ";
                    }
                }
                else if(diretiva == "CONST"){
                    newFile << linha[1] << " ";
                }
            }
        }
        if(rotulo_ausente){
            cout << "Erro semantico na linha " << contador_linha << ". Rotulo nao definido." << endl;
        }
        if(!operandos_corretos){
            cout << "Erro sintatico na linha " << contador_linha << ". Quantidade de operandos invalida. " << endl; 
        }
    }
    file.close();
    newFile.close();
}

int main(int arg, char *argv[]) {

    if(arg < 2) {
        cout << "Voce deve incluir o nome do programa .asm para compilacao!\n";
        exit(1);
    }
    
    string FILE_NAME = preprocessador(argv);
    first_pass(FILE_NAME);
    second_pass(FILE_NAME);

    // cout << "Tabela de Simbolos:\nRotulo\t\tEndereco\t\tLinha\n";
    // for(auto [rotulo, arr] : TS) {
    //     if(TS[rotulo][0]) {
    //         cout << rotulo << "\t\t" << TS[rotulo][1] << "\t\t" << TS[rotulo][2] << '\n';
    //     }
    // }

    return 0;
}
