CODIGO SEGMENT
ASSUME CS : CODIGO

    ORG 100h

    START: JMP COMPROBADOR

    MENSAJE_FINAL DB 100 DUP(0)

    INSTALADO DB "INSTALACION COMPLETADA",13,10,'$'
    DESINSTALADO DB "DESINSTALACION COMPLETADA",13,10,'$'
    FORMATO_ERRONEO DB "Introduzca tras un espacio /I para instalar o /D para desinstalar",13,10,'$'


    CESAR PROC FAR

            CMP AH, 11h
            JE CODIFICAR

            CMP AH, 22h
            JE DECODIFICAR

        CODIFICAR:
            CALL CODIFICADOR
            JMP FIN_CES
        DECODIFICAR:
            CALL DECODIFICADOR
            JMP FIN_CES

        FIN_CES:
            IRET
    CESAR ENDP

    CODIFICADOR PROC
        PUSH AX BX CX
        
        MOV SI, DX
        MOV DI, OFFSET MENSAJE_FINAL

        MOV BL, AL

        ;Guardamos el numero de elementos del Abecedario en CL
        MOV CL, 'Z'
        SUB CL, 'A'
        INC CL

        JMP BUC

        COD:
            MOV BH, DS:[SI] ;Guardamos la letra en BH
            ADD BH, BL ;Le sumamos la rotación
            MOV AH, 0 
            MOV AL, BH ;Movemos a AL la letra con la rotación sumada, con el fin de usar div
            SUB AL, 'A';Le restamos a AL la letra A para que nos quede la posición de nuestra nueva letra (entre BL y 25+BL)
            DIV CL ;Dividimos para poder usar las letras como una cola circular entre 0 y 25

            MOV BH, 'A' ;Movemos a BH A
            ADD BH, AH ;Le sumamos la posición de la letra en la cola circular para que sea la letra A + posición
                       ;Esta posición es el resto de la división anterior y se guarda automáticamente en AH

            MOV CS:[DI], BH
            INC SI
            INC DI
        BUC:
            CMP BYTE PTR DS:[SI], '$'
            JNE COD

        MOV BH, DS:[SI]
        MOV CS:[DI], BH

        MOV AX, CS
        MOV DS, AX    

        MOV DX, OFFSET MENSAJE_FINAL

        POP CX BX AX
        RET
    CODIFICADOR ENDP

    DECODIFICADOR PROC
        #TO DO

        RET
    DECODIFICADOR ENDP

    COMPROBADOR PROC

            PUSH AX CX BX

            MOV AL, 3
            MOV BX, 80h
            MOV CL, [BX]
            CMP AL, CL
            JNE ERROR_FORMATO

            ADD BX, 02h
            MOV CL, '/'
            MOV AL, [BX]
            CMP AL, CL
            JNE ERROR_FORMATO

            INC BX
            MOV AL, [BX]
            MOV CL, 'I'
            CMP AL, CL
            JE INSTALAR

            MOV CL, 'D'
            CMP AL, CL
            JE DESINSTALAR

            JMP ERROR_FORMATO

        INSTALAR:
            CALL INSTALADOR
            JMP FIN
        DESINSTALAR:
            CALL DESINSTALADOR
            JMP FIN

        ERROR_FORMATO:
            MOV AH, 09h
            MOV DX, OFFSET FORMATO_ERRONEO
            INT 21h
        FIN:
            POP BX CX AX
            RET

    COMPROBADOR ENDP

    DESINSTALADOR PROC

        XOR AX, AX
        MOV ES, AX

        CLI
            MOV ES:[55h*4], AX
            MOV ES:[55h*4+2], AX
        STI

        MOV ES, CS:[2Ch]
        MOV AH, 49h
        INT 21h

        MOV AX, CS
        MOV ES, AX
        MOV AH, 49h
        INT 21h

        MOV DX, OFFSET DESINSTALADO
        MOV AH, 09h
        INT 21h

        RET

    DESINSTALADOR ENDP

    INSTALADOR PROC
        MOV AX, 0
        MOV ES, AX
        MOV AX, OFFSET CESAR
        MOV BX, CS

        CLI
            MOV ES:[55h*4],AX
            MOV ES:[55h*4+2], BX
        STI

        MOV DX, OFFSET INSTALADO
        MOV AH, 09h
        INT 21h

        MOV DX, OFFSET COMPROBADOR
        INT 27h

        RET
    INSTALADOR ENDP

CODIGO ENDS
END START
