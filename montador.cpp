#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

using namespace std; 

void remove_macros(string file_name) {
    ifstream file(file_name);
    string text;
    while(getline(file, text)) {
        //cout << text << '\n';
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

    bool creatingToken = false;
    while(getline(file, text)) {
        string token = "", newLine = "";
        for(int i=0; i<text.size(); i++) {
            if(text[i] == ';') {    // ignora comentarios
                break;
            }

            if(text[i] != ' ' && text[i] != '\t') {
                //cout << text[i];
                creatingToken = true;
                token += toupper(text[i]);
            }
            else {
                //cout << " teve espaco e ele vai colocar o token " << token << " na linha\n";
                //cout << token;
                if(creatingToken) {
                    newLine += token;
                    if(text[i-1] != ',') newLine += ' ';
                    creatingToken = false;
                    token = "";
                }
                else {
                    //cout << text[i];
                }
            }
        }
        if(token.size() > 0) {
            newLine += token;
            //cout << token;
        }
        creatingToken = false;

        if(newLine.size() > 0) {
            //cout << newLine;
            newFile << newLine;
            if(newLine[newLine.size()-1] != ':') {
                //cout << '\n';
                newFile << '\n';
            }
            else {
                newFile << ' ';
            }
        }
    }
    file.close();
    newFile.close();
    if(macro) remove_macros(file_name);
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
