#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <map>
#include <string>
#include <array>
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
    string text;
    size_t last_index = file_name.find_last_of("."); 
    string raw_name = file_name.substr(0, last_index); 
    ofstream newFile(raw_name.substr(10,raw_name.size()) + ".obj");
    string secao = "text";

    int contador_pos = 0, contador_linha = 0;
    while(getline(file, text)) {
        bool operandos_corretos = true;
        contador_linha++;
        if(text.find("SECAO") != -1) {
            if (text.find("SECAO DATA") != -1) {
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
                if(linha.size() > 1){
                    operandos_corretos = false;
                    cout << "Erro sintatico na linha " << contador_linha << ". Operandos invalidos." << endl;
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
                        newFile << "09 ";
                        op1 = ops[0];
                        op2 = ops[1];
                    }
                }
            }
            else{
                bool achou_instrucao = false;
                for(int i=0; i<TI.size(); i++) {
                    if(instrucao == TI[i].first) {
                        achou_instrucao = true;
                        string opcode = "";
                        if(i < 9){
                            opcode = "0" + to_string(i+1);
                        }
                        else{
                            opcode = to_string(i+1);
                        }
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
            }
            bool operando_valido = true;
            int op;
            if(op1 != ""){
                op = getOp(op1); 
                if(op != -1){
                    newFile << to_string(op) << " ";
                }
            }
            if(op2 != ""){
                op = getOp(op2); 
                if(op != -1){
                    newFile << to_string(op) << " ";
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
    }
    file.close();
    newFile.close();
}

void replace_data_section(string file_name) {
    string text, new_file_name = "processed_" + file_name.substr(5);
    ifstream file(file_name);
    ofstream newFile(new_file_name);

    vector<string> secao_data;
    bool lendo_data_section = false;
    while(getline(file, text)) {
        if(text.find("SECAO DATA") != -1) {
            lendo_data_section = true;
        }
        if(text.find("SECAO TEXT") != -1) {
            lendo_data_section = false;
        }

        if(lendo_data_section) {
            secao_data.push_back(text);
        }
        else {
            newFile << text << '\n';
        }
    }
    for(auto linha : secao_data) {
        newFile << linha << '\n';
    }
}

string remove_macros(string file_name) {
    string text, new_file_name = "temp_" + file_name.substr(5, file_name.size()-8) + "pre";
    ifstream file(file_name);
    ofstream newFile(new_file_name);
    map<string, pair<bool, int>> MNT;   // MNT => <nome da macro, <macro definida, qnt parametros>>
    map<string, vector<vector<string>>> MDT;    // MDT => <nome da macro, corpo da macro>
    /* Cada posicao do vetor corresponde a uma linha do corpo da macro 
       e cada linha eh um vetor em que cada posicao eh um token */

    bool lendoMacro = false;
    string macroSendoLida = "";
    vector<string> param;
    while(getline(file, text)) {
        vector<string> linha = split(text);  // tokens de cada linha
        bool expandiu = false;
        for(int i=0; i<linha.size(); i++) {
            string &token = linha[i];
            int tokenSize = token.size();
            
            if(lendoMacro) {
                // troca os simbolos dos argumentos para ficar indexado #1, #2, #...
                for(int j=0; j<param.size(); j++)
                    if(token == param[j])
                        token = "#" + to_string(j+1);
                
                if(token == "ENDMACRO") {
                    lendoMacro = false;
                    macroSendoLida = "";
                    param.clear();
                }

                if(i == linha.size()-1 && lendoMacro)
                    MDT[macroSendoLida].push_back(linha);
            }
            else {  // nao ta lendo macro
                if(token == "MACRO") {       // linha de declaracao da macro
                    lendoMacro = true;
                    macroSendoLida = linha[0].substr(0, linha[0].size()-1);
                    if(i != linha.size()-1) param = split(linha[i+1], ',');
                    int qnt_param = param.size();
                    MNT[macroSendoLida] = {true, qnt_param}; // declara macro na MNT
                    break;
                }
                else if(MNT[token].first) {     // eh uma macro
                    expandiu = true;    // vai substituir essa linha pela expansao da macro
                    vector<string> args;
                    if(MNT[token].second > 0) {
                        args = split(linha[i+1], ',');
                    }
                    for(int j=0; j<MDT[token].size(); j++) {    // percorre linhas do corpo
                        for(string token2 : MDT[token][j]) {    // percorre  tokens da linha
                            if(token2[0] == '#') {                  // faz o binding
                                int argIdx = stoi(token2.substr(1));// pega index do arg
                                newFile << args[argIdx-1];
                            }
                            else {
                                newFile << token2;
                            }
                            newFile << ' ';
                        }
                        newFile << '\n';
                    }
                    break;
                }
            }
        }

        bool escreveuLinha = false;
        for(int i=0; i<linha.size(); i++) {
            if(!expandiu && !lendoMacro && linha[i] != "ENDMACRO") {  // efetivamente passa a linha para o novo arquivo
                escreveuLinha = true;
                newFile << linha[i] << ' ';
            }
        }
        if(escreveuLinha)
            newFile << '\n';
    }

    file.close();
    newFile.close();
    remove(file_name.c_str());  // apaga o arquivo temp_X.mcr
    
    cout << "Macros removidas do codigo:\n";
    for(auto [key, value] : MDT) {
        if(!MNT[key].first) continue;
        cout << key << ":\n";
        for(auto vetor1 : value) {
            for(auto token : vetor1) {
                cout << token << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }
    return new_file_name;
}

void pre_processing(char *argv[]) {
    
    string file_name = argv[1], text;
    file_name = "temp_" + file_name;
    bool macro = (file_name.substr(file_name.size()-4) == ".mcr");
    
    // abre arquivos
    ifstream file(argv[1]); // abre o arquivo .asm ou .mcr
    ofstream newFile(file_name); // cria novo arquivo .asm ou .mcr

    /* Remove espacos, tabulacoes, linhas desnecessarias,
       comentarios e alinha rotulos com instrucoes */
    bool creatingToken = false;
    while(getline(file, text)) {
        string token = "", newLine = "";
        for(int i=0; i<text.size(); i++) {
            if(text[i] == ';') {    // ignora comentarios
                break;
            }

            if(text[i] != ' ' && text[i] != '\t') {
                creatingToken = true;
                token += toupper(text[i]);
            }
            else {
                if(creatingToken) {
                    newLine += token;
                    if(text[i-1] != ',') newLine += ' ';
                    creatingToken = false;
                    token = "";
                }
            }
        }
        if(token.size() > 0) {
            newLine += token;
        }
        creatingToken = false;

        if(newLine.size() > 0) {
            newFile << newLine;
            if(newLine[newLine.size()-1] != ':') { // deixar rotulo na mesma linha que instrucao
                newFile << '\n';
            }
            else {
                newFile << ' ';
            }
        }
    }
    file.close();
    newFile.close();
    if(macro) {
        file_name = remove_macros(file_name);
    }
    replace_data_section(file_name);
    remove(file_name.c_str()); // apaga arquivo temp
}

int main(int arg, char *argv[]) {

    if(arg < 2) {
        cout << "Voce deve incluir o nome do programa .asm para compilacao!\n";
        exit(1);
    }
    
    pre_processing(argv);
    //cout << "O arquivo " << argv[1] << " foi preprocessado com sucesso!\n";

    string FILE_NAME = "processed_" + (string) argv[1];
    first_pass(FILE_NAME);
    second_pass(FILE_NAME);

    cout << "Tabela de Simbolos:\nRotulo\t\tEndereco\tLinha\n";
    for(auto [rotulo, arr] : TS) {
        if(TS[rotulo][0]) {
            cout << rotulo << "\t\t" << TS[rotulo][1] << "\t\t" << TS[rotulo][2] << '\n';
        }
    }

    return 0;
}
