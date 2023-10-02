# Trabalho 1 - Software Básico

O trabalho consiste em implementar em C/C++ um método de tradução de uma linguagem de montagem simples para uma representação de código objeto. O tradutor a ser implementado será um Assembler da linguagem hipotética vista em sala de aula.

# Lista de tarefas
## Montador
- [x] Leitura do arquivo de entrada para usar no código
- [x] Tratamento da entrada separando intruções, operandos e rótulos, ignorando espaçamentos e comentários
- [] Idenficar todos os tipos de erros do programa
    - [] Declarações e rótulos ausentes
    - [] Declarações e rótulos repetidos
    - [] Instruções e diretivas inválidas
    - [] Instruções e diretivas com a quantidade de operandos errada
- [] Passagem 0
- [] Primeira passagem
    - [] Tabela de instruções
    - [] Tabela de símbolos
    - [] Tabela de diretivas
- [] Segunda passagem
## Simulador
- [] Leitura do arquivo de entrada
- [] Identificar e indicar segmentation fault