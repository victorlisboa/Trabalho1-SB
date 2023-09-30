#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int arg, char *argv[]) {

    if(arg < 2) {
        cout << "Voce deve incluir o nome do programa .asm para compilacao!";
        return 0;
    }
    
    cout << "Conteudo do programa " << argv[1] << ":\n";
    
    string text;
    ifstream file(argv[1]);

    while(getline(file, text)) {
        cout << text << '\n';
    }

    file.close();

    return 0;
}