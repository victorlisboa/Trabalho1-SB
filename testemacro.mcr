SOMA: MACRO &A,&B   ;define a macro soma
LOAD &A             ;carrega primeiro valor em ACC
ADD &B              ;adiciona o segundo valor
ENDMACRO            ;termina macro

SUBTRAI: MACRO &X,&Y
LOAD &X
SUB &Y
STORE RESULTADO     ;ja guarda e imprime tudo na macro
OUTPUT RESULTADO
ENDMACRO

;;;;;;;;;;;
SECAO TEXTO
;;;;;;;;;;;

SOMA UM,DOIS        ;chama macro
STORE RESULTADO     ;guarda o resultado
OUTPUT RESULTADO    ;imprime resultado
SUBTRAI RESULTADO,MENOS3

;;;;;;;;;;
SECAO DATA
;;;;;;;;;;

UM: CONST 1
DOIS: CONST 2
TRES: CONST 3
RESULTADO: SPACE