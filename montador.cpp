#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <map>
#include <string>

using namespace std;

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

void remove_macros(string file_name) {
    ifstream file(file_name);
    ofstream newFile("processed_" + file_name.substr(0, file_name.size()-3) + "pre");
    string text;
    map<string, pair<bool, int>> MNT;   // MNT => <nome da macro, <macro definida, qnt parametros>>
    map<string, vector<vector<string>>> MDT;    // MDT => <nome da macro, corpo da macro>
    /* Cada posicao do vetor corresponde a uma linha do corpo da macro 
       e cada linha eh um vetor em que cada posicao eh um token */

    bool lendoMacro = false, expandiu = false;
    string macroSendoLida = "";
    vector<string> param;
    while(getline(file, text)) {
        vector<string> linha = split(text);  // tokens de cada linha
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
                        if(args.size() != MNT[token].second) {
                            //////// Apontar linha do erro ///////////
                            cout << "MACRO " << token << " esperava " << MNT[token].second <<
                            " argumentos, mas recebeu " << args.size() << ".\n";
                            exit(1);
                        }
                    }
                    for(int j=0; j<MDT[token].size(); j++) {    // percorre linhas do corpo
                        for(string token2 : MDT[token][j]) {    // percorre  tokens da linha
                            if(token2[0] == '#') {                  // faz o binding
                                int argIdx = stoi(token2.substr(1));// pega index do arg
                                newFile << args[argIdx-1];
                                break;
                            }
                            else {
                                newFile << token2;
                            }
                            newFile << ' ';
                        }
                        newFile << '\n';
                    }
                }
            }
        }

        for(int i=0; i<linha.size(); i++)
            if(!expandiu)   // efetivamente passa a linha para o novo arquivo
                newFile << linha[i] << ' ';
        newFile << '\n';
    }

    file.close();
    newFile.close();
    
    for(auto [key, value] : MDT) {
        if(!MNT[key].first) continue;
        cout << "Macro " << key << ":\n";
        for(auto vetor1 : value) {
            for(auto token : vetor1) {
                cout << token << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }
}

void pre_processing(char *argv[]) {
    
    string file_name = argv[1], text, preffix;
    bool macro = (file_name.substr(file_name.size()-4) == ".mcr");
    preffix = (macro ? "temp_" : "processed_");
    file_name = preffix + file_name;
    
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
            if(newLine[newLine.size()-1] != ':') {
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
        remove_macros(file_name);
        // apaga arquivo temp
    }
}

int main(int arg, char *argv[]) {

    if(arg < 2) {
        cout << "Voce deve incluir o nome do programa .asm para compilacao!\n";
        return 0;
    }

    pre_processing(argv);

    cout << "O arquivo " << argv[1] << " foi preprocessado com sucesso!\n";

    return 0;
}
