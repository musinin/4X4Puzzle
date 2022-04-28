#include "Hangman.h"
#include "MyDic.h"
void main()
{
	Hangman	game;
	MyDic	dic;

	srand((unsigned int)time(NULL));
	dic.load("Mywords.txt");
	int i = rand() % MAXWORDS;
	game.play(dic.getEng(i), dic.getKor(i));
}