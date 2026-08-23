#########################
## Tiago Antonio Rizzetti
## Compilação do programa de um programa usando opengl
## a compilação direta (sem gerar os objetos primeiros acarreta erros de ligação)
## por isso são gerados todos os arquivos objetos e depois feita a linkagem...
#########################
all: 
	#gera todos os arquivos objetos
	g++ -O0 -g3 -Wall -c -fmessage-length=0 *.cpp
	
	g++ -O0 -g3 -Wall -c -fmessage-length=0 tools/*.cpp 
	#move os arquivos objetos gerados na raiz para a pasta tools onde encontra-se
	#os fontes relativos aos objetos abaixo
	mv GLFont.o tools

	#faz a linkagem dos arquivos objetos com as bibliotecas do opengl.
	g++ -oprograma *.o tools/*.o -lglut -lGLU -lGL -lm
clear:
	#apaga todos os arquivos objetos gerados
	rm *.o
	rm tools/*.o
	rm programa
