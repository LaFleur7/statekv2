#include "stdafx.h"//
#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <string>
#include <cstdlib>
#include <list>
using namespace std;
int width, height;
char map[50][50];
class Obiekt
{
public:
	int X, Y;
	char pole;
	void settings(int a, int b, char c)
	{
		X = a;
		Y = b;
		pole = c;
	}
	int posX()
	{
		return X;
	}
	int posY()
	{
		return Y;
	}
	void setPos(int x, int y)
	{
		X = x;
		Y = y;
	}
	char getPole()
	{
		return pole;
	}
};
class Pocisk
	:public Obiekt
{
public:
	void fly()
	{
		if (Y != 1) Y--;
		return;
	}
	~Pocisk()
	{
		map[X][Y] = 'e';
	}
};
class Ufo
	:public Obiekt
{
public:
	int kierunek;//1-prawo 0-lewo
	void poziomFly()
	{
		if (kierunek == 1)
		{
			if (X == width - 1) { kierunek = 0; Y++;}
			else { X++; }
		}
		else if (kierunek == 0)
		{
			if (X == 1) { kierunek = 1; Y++; }
			else { X--; }
		}
	}
	void setKierunek(int x)
	{
		kierunek = x;
	}
	~Ufo()
	{
		map[X][Y] = 'e';
	}
};
class Laser 
	:public Obiekt
{
public:
	int startCzas=NULL;
	void startLaser()
	{
		for (int i = 0; i < width; i++)
		{
			map[i][Y] = 'l';
			if (i==0||i==width-1)map[i][Y] = '!';
		}


	}
	void uwaga()
	{
		map[0][Y] = '!';
		map[width-1][Y] = '!';

		startCzas = time(0);

	}
	~Laser()
	{
		for (int i = 0; i < width; i++)
		{
			map[i][Y] = 'e';
		}
	}
};


list <Pocisk> shots;
list <Ufo> enemy;
list <Laser> promien;
Obiekt player;

void textColor(int x)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
}
void gotoxy(int x, int y)
{
	HANDLE hCon;
	COORD dwPos;

	dwPos.X = x;
	dwPos.Y = y;

	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hCon, dwPos);
}
void status(int co,int ilemax,int kolor)
{
	for (int i=0;i < ilemax;i++)
	{ 
		if (co > i)
		{
			textColor(kolor);
		}
		else{ textColor(119); }
		cout << "#";
		textColor(7);
		cout << "|";
		
	//119 szare 
	//204 czerwone
	//238 zolte
	}
}
void line()
{
	for (int i = 0; i <= width; i++)
	{
		cout << "--";
	}
}
void clearScreen() //czyszczenie ekranu
{
	HANDLE hOut;
	COORD Position;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}
void hidecursor() //fix for bliking
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
void round1()
{
	enemy.push_back(*new Ufo); //dodanie do listy
	enemy.back().settings(width / 2, height/2, 'o'); //ustawienie poczatkowej pozycji
	enemy.back().setKierunek(1);
	map[enemy.back().posX()][enemy.back().posY()] = 'i'; //dane do narysowania
}
void round2()
{
	enemy.push_back(*new Ufo); 
	enemy.back().settings(width / 2 - 5, 4, 'o'); 
	enemy.back().setKierunek(1);
	map[enemy.back().posX()][enemy.back().posY()] = 'i'; 
	enemy.push_back(*new Ufo);
	enemy.back().settings(width / 2 + 5, 4, 'o'); 
	enemy.back().setKierunek(1);
	map[enemy.back().posX()][enemy.back().posY()] = 'i';
}
void round3()
{
	promien.push_back(*new Laser);
	promien.back().Y = player.Y;
}
void round4()
{
	promien.push_back(*new Laser);
	promien.back().Y = 7;
	promien.push_back(*new Laser);
	promien.back().Y = 20;
	promien.push_back(*new Laser);
	promien.back().Y = 5;
	enemy.push_back(*new Ufo);
	enemy.back().settings(width / 2, 10, 'o');
	enemy.back().setKierunek(1);
	map[enemy.back().posX()][enemy.back().posY()] = 'i';
}
void round5()
{
	enemy.push_back(*new Ufo);
	enemy.back().settings(rand()%width, 13, 'o');
	enemy.back().setKierunek(0);
	map[enemy.back().posX()][enemy.back().posY()] = 'i';
	enemy.push_back(*new Ufo);
	enemy.back().settings(width / 2 + 2, 8, 'o');
	enemy.back().setKierunek(1);
	map[enemy.back().posX()][enemy.back().posY()] = 'i';
	enemy.push_back(*new Ufo);
	enemy.back().settings(width / 2 - 9, 5, 'o');
	enemy.back().setKierunek(1);
	map[enemy.back().posX()][enemy.back().posY()] = 'i';
	promien.push_back(*new Laser);
	promien.back().Y = player.Y;
	promien.push_back(*new Laser);
	promien.back().Y = player.Y+1;
	promien.push_back(*new Laser);
	promien.back().Y = player.Y-1;
	promien.push_back(*new Laser);
	promien.back().Y = player.Y+2;
	promien.push_back(*new Laser);
	promien.back().Y = player.Y-2;
}
int Czekaj6 = NULL;
void round6()
{
	if (Czekaj6 == NULL)
	{
		Czekaj6 = time(0);
	}
	else if (Czekaj6%6==0)
	{
		promien.push_back(*new Laser);
		promien.back().Y = player.Y;
	}
}
int HP,runda;
int points, speed;
int button; //button used to move
int animation = 0;
int energy;
int nowTime,startTime;

void menu()
{
	//manu
}

int main()
{

	SetConsoleTitle(TEXT("kruci gang"));
	srand(time(NULL));
	menu();//starts a menu
	HP = 3;
	width = 30;
	height = 24;
	speed = 15;
	runda = 1; //dac zero w celach diagnosycznych
	energy = 6;
	startTime = time(0);
	player.settings(width / 2, height - 1, 'x');
	char graczChar = player.getPole();
	map[player.posX()][player.posY()] = graczChar; //do przyznania pola obecnemu miejscu
	char shotChar = 'i';
	char enemyChar = 'o';


	

	system("cls");
	hidecursor();
	bool work = true;
	while (work) //drawing board
	{
		gotoxy(0, 0);

		if (runda==1 && enemy.empty())
		{
			round1();
		}
		else if (runda == 2 && enemy.empty()&& promien.empty())
		{
			round2();
		}
		else if (runda == 3 && enemy.empty() && promien.empty())
		{
			round3();
		}
		else if (runda == 4 && enemy.empty() && promien.empty())
		{
			round4();
		}
		else if (runda == 5 && enemy.empty() && promien.empty())
		{
			round5();
		}
		else if (runda == 6 && enemy.empty() && promien.empty())
		{
			
			round6();
		}

		textColor(255);
		line(); //upper line
		cout << endl;
		for (int i = 0; i <= height; i++) //left and right walls 
		{
			cout << "|";
			for (int j = 0; j<width; j++)
			{
				if (map[j][i] == graczChar) //darwing player's positon
				{
					textColor(255);
					cout << "xx";
					textColor(7);
					map[j][i + 1] = 'v';
				}
				else if (map[j][i]==enemyChar) //darwing enemy's positon
				{
					textColor(12);
					cout << "oo";
					textColor(7);
				}
				else if (map[j][i] == 'l') //darwing laser's positon
				{
					textColor(170);
					cout << "##";
					textColor(7);
				}
				else if (map[j][i] == '!') //darwing laser's positon
				{
					textColor(252);
					cout << "!!";
					textColor(7);
				}
				else if (map[j][i] == shotChar) //darwing shot's positon
				{
					textColor(12);
					cout << "ii";
					textColor(255);
				}
				else if (map[j][i] == 'v') //darwing shot's positon
				{
					if (animation == 0)
					{
						textColor(12);
					}
					else
					{
						textColor(10);
					}
					cout << "vv";
					textColor(255);
				}
				else
				{
					textColor(7);
					cout << "  ";
					textColor(255);
				}
			}
			textColor(255);
			cout << "|" << endl;

		}
		line();//lower line
		textColor(7);
		//GUI
		cout << endl << "Punkty ";
		textColor(10);
		cout << points;
		textColor(7);
		cout << "|";
		status(HP,3,204);
		cout << "HP|";
		status(energy, 6, 238);
		textColor(7);
		cout << "Energy| " << speed << "fps ";
		cout <<"|Czas:" <<nowTime;
		

		if (_kbhit())
		{
			button = _getch();
			switch (button)
			{
			case 97: //moving left 
			{
				if (player.posX() > 0)
				{
					map[player.posX()][player.posY()] = 'e';
					map[player.posX()][player.posY() + 1] = 'e';
					player.setPos(player.posX() - 1, player.posY());
					map[player.posX()][player.posY()] = graczChar;
				}
			}
			break;
			case 100: //moving right
			{
				if (player.posX() < (width - 1))
				{
					map[player.posX()][player.posY()] = 'e';
					map[player.posX()][player.posY() + 1] = 'e';
					player.setPos(player.posX() + 1, player.posY());
					map[player.posX()][player.posY()] = graczChar;
				}
			}
			break;
			case 119: // going up
			{
				if (player.posY() > 1)
				{
					map[player.posX()][player.posY()] = 'e';
					map[player.posX()][player.posY() + 1] = 'e';
					player.setPos(player.posX(), player.posY() - 1);
					map[player.posX()][player.posY()] = graczChar;
			
				}
			}
			break;
			case 115: // going down
			{
				if (player.posY() < (height - 1))
				{
					map[player.posX()][player.posY()] = 'e';
					map[player.posX()][player.posY() + 1] = 'e';
					player.setPos(player.posX(), player.posY() + 1);
					map[player.posX()][player.posY()] = graczChar;
				}
			}
			break;
			case 32: // shot
			{
				if (energy!=0)
				{
					shots.push_back(*new Pocisk); //dodanie do listy
					shots.back().settings(player.posX(), player.posY() - 1, 'i'); //ustawienie poczatkowej pozycji
					map[shots.back().posX()][shots.back().posY()] = shotChar; //dane do narysowania
					energy--;
				}
			}break;
			}
		}
		if (!shots.empty()) //kontrola strzelania
		{
			for (list < Pocisk >::iterator i = shots.begin(); i != shots.end(); i++)
			{
				map[i->posX()][i->posY()] = 'e';

				i->fly();

				if (i->Y == 1)
				{
					i = shots.erase(i);
					if (i == shots.end()) //bo sie bagowalo
					{
						break;
					}
				
				}
				else if (!shots.empty())
				{
					map[i->posX()][i->posY()] = shotChar;

				}

			}

		}
		if (!enemy.empty())
		{
			for (list < Ufo >::iterator i = enemy.begin(); i != enemy.end(); i++)
			{
				map[i->posX()][i->posY()] = 'e';

				i->poziomFly();

				if (map[i->posX()][i->posY()+1] == shotChar ||i->Y==height-1)
				{
					if (i->Y == height - 1) { HP--; }
					else { points++; }
					i = enemy.erase(i);
					if (i == enemy.end()) //bo sie bagowalo
					{
						break;
					}
				}
				else if (!enemy.empty())
				{
					map[i->posX()][i->posY()] = enemyChar;
				}
			}
		}
		if (!promien.empty())
		{
			for (list < Laser >::iterator i = promien.begin(); i != promien.end(); i++)
			{
				
				if (i->startCzas == NULL)
				{
					i->uwaga();
				}
				else if (time(0) - i->startCzas == 8)
				{
					i = promien.erase(i); 
					points++;
					break;
				}
				else if (time(0) - i->startCzas >= 3)
				{
					i->startLaser();
					if (i->Y == player.Y)
					{
						HP--;
						player.settings(width / 2, height - 1, 'x');
						map[player.X][player.Y] = 'x';
					}
				}
			}
		}
		switch (points)
		{
			case 1:
			{
				runda = 2;
				break;
			}
			case 3:
			{
				runda = 3;
				break;
			}
			case 4:
			{
				runda = 4;
				break;
			}
			case 8:
			{
				runda = 5;
				break;
			}
			case 16:
			{
				runda = 6;
				break;
			}
			/*case NULL:
			{
				runda = 7;
				break;
			}*/
		}

		if (animation == 0)
		{
			animation = 1;
		}
		else
		{
			animation = 0;
		}
		nowTime = time(0) - startTime;
		if (energy != 6 && nowTime % 5 == 0)
		{
			energy++;
		}
		if (HP == 0)
		{
			break;
		}
		Sleep(1000 / speed);
	}
	system("cls");
	cout << "GAME OVER" << endl;
	return 0;
}