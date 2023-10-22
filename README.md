# Trabalho 1 - Software Básico

O trabalho consiste em implementar em C/C++ um método de tradução de uma linguagem de montagem simples para uma representação de código objeto. O tradutor a ser implementado será um Assembler da linguagem hipotética vista em sala de aula.

## Alunos

- Luís Henrique Araújo Martins - 221002058
- Victor Hugo França Lisboa - 200028341

## SO utilizado

Testado em Linux e Windows

## Como compilar

O projeto é compilado utilizando o Makefile, bastando digitar `make` ou `make all` para compilar tanto o montador quanto o simulador.

Para apagar os arquivos objetos e os executáveis, utilize o comando `make clean`.

## Como usar
### Montador

Para utilizar o montador, basta executar o seguinte comando: `./montador caminho/do/seu/arquivo.asm`.
O montador está funcionando completamente e aceita arquivos do tipo `.asm` e `.mcr`.
Alguns testes estão disponíveis no diretório `testes`.

### Simulador

De forma similar, para rodar o simulador utiliza-se o comando `./simulador caminho/do/seu/arquivo.obj`.
O simulador também está funcionando perfeitamente e aceita apenas arquivos `.obj`.
O funcionamento segue conforme a especificação do trabalho. Quando executa instrução de input, ele simplesmente não passa para a próxima instrução enquanto o usuário não digita um número.

## Observações

As linhas dos erros léxicos, sintáticos ou semânticos serão indicadas em relação ao arquivo pré-processado, não ao arquivo original.