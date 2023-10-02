#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

using namespace std;

void pre_processing(ifstream &file, string file_name) {
    string text;
    ofstream newFile("processed_" + file_name);

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
    newFile.close();
}

int main(int arg, char *argv[]) {

    if(arg < 2) {
        cout << "Voce deve incluir o nome do programa .asm para compilacao!";
        return 0;
    }
    
    ifstream file(argv[1]);
    pre_processing(file, argv[1]);
    file.close();

    cout << "O arquivo " << argv[1] << " foi preprocessado com sucesso!\n";

    return 0;
}