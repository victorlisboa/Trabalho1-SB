# Trabalho 1 - Software Básico

O trabalho consiste em implementar em C/C++ um método de tradução de uma linguagem de montagem simples para uma representação de código objeto. O tradutor a ser implementado será um Assembler da linguagem hipotética vista em sala de aula.

# Lista de tarefas
## Montador
- [x] Leitura do arquivo de entrada para usar no código
- [] Idenficar todos os tipos de erros do programa
    - [] Declarações e rótulos ausentes
    - [x] Declarações e rótulos repetidos
    - [] Instruções e diretivas inválidas
    - [] Instruções e diretivas com a quantidade de operandos errada
- [x] Passagem 0
    - [x] Tratamento da entrada separando intruções, operandos e rótulos, ignorando espaçamentos e comentários
    - [x] Eliminação das macros
    - [x] Realocando seção data para final
- [x] Primeira passagem
    - [x] Tabela de instruções
    - [x] Tabela de símbolos
    - [x] Tabela de diretivas
- [] Segunda passagem
## Simulador
- [] Leitura do arquivo de entrada
- [] Identificar e indicar segmentation fault