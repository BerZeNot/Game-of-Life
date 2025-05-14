# !/bin/bash
# compile.sh
#
# Compila os arquivos do game of life
#
# Versão 1: Compila o os arquivos do jogo e remove os arquivos objeto
#
# Paulo Giovany, Fevereiro de 2024

NOME_SAIDA=gameOfLife.app

g++ -c game_of_life.cpp

if test $? = 0
then
	g++ -o $NOME_SAIDA game_of_life.o
	if test $? = 0
	then
		rm *.o
	fi
fi

