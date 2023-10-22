#include "preprocessador.h"
#include "utils.h"

using namespace std;

string replace_data_section(string file_name) {
    string text, aux = get_file_name(file_name), new_file_name;
    int path_size = file_name.size()-aux.size();
    new_file_name = file_name.substr(0, path_size) + "processed_" + aux.substr(5);
    
    ifstream file(file_name);
    ofstream newFile(new_file_name);

    vector<string> secao_data;
    bool lendo_data_section = false;
    while(getline(file, text)) {
        if(text.find("SECAO DATA") != string::npos) {
            lendo_data_section = true;
        }
        if(text.find("SECAO TEXT") != string::npos) {
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

    return new_file_name;
}

string remove_macros(string file_name) {
    string text, new_file_name = get_file_name(file_name);
    int path_size = file_name.size()-new_file_name.size();
    new_file_name = file_name.substr(0, path_size) + new_file_name.substr(0, new_file_name.size()-4) + ".pre";
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
        for(auto i = 0u; i<linha.size(); i++) {
            string &token = linha[i];
            
            if(lendoMacro) {
                // troca os simbolos dos argumentos para ficar indexado #1, #2, #...
                for(auto j=0u; j<param.size(); j++)
                    if(token == param[j])
                        token = "#" + to_string(j+1);
                
                if(token == "ENDMACRO") {
                    lendoMacro = false;
                    macroSendoLida = "";
                    param.clear();
                }

                if(i == (unsigned) linha.size()-1 && lendoMacro)
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
                    for(auto j=0u; j<MDT[token].size(); j++) {    // percorre linhas do corpo
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
        for(auto i=0u; i<linha.size(); i++) {
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

string preprocessador(char *argv[]) {
    
    string file_name = argv[1], text, temp;
    bool macro = (file_name.substr(file_name.size()-4) == ".mcr");
    string aux = get_file_name(file_name);
    temp = file_name.substr(0, file_name.size()-aux.size()) + "temp_" + aux;
    
    // abre arquivos
    ifstream file(argv[1]); // abre o arquivo .asm ou .mcr
    ofstream newFile(temp); // cria novo arquivo .asm ou .mcr
    
    /* Remove espacos, tabulacoes, linhas desnecessarias,
       comentarios e alinha rotulos com instrucoes */
    bool creatingToken = false;
    while(getline(file, text)) {
        string token = "", newLine = "";
        for(auto i=0u; i<text.size(); i++) {
            if(text[i] == ';') {    // ignora comentarios
                break;
            }

            if(text[i] != ' ' && text[i] != '\t') {
                creatingToken = true;
                token += toupper(text[i]);
                //cout << (char)toupper(text[i]);
            }
            else {
                if(creatingToken) {
                    newLine += token;
                    //cout << token << ",";
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
        file_name = remove_macros(temp);
        remove(temp.c_str());   // apaga temp_file.mcr
    }
    else {
        file_name = temp;
    }
    string final_file_name = replace_data_section(file_name);
    remove(file_name.c_str()); // apaga arquivo temp_file.pre
    return final_file_name;
}
