#include <cstdio>
#include <Windows.h>
#include <conio.h>
#include <cstdlib>
#include <vector>
using namespace std;
#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80

void gotoxy(int x, int y)
{
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hCon, dwPos);
}

void OcultarCursor()
{
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;

	cci.dwSize = 2;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hCon, &cci);
}

class Nave
{
public:
	Nave(int x_, int y_);
	~Nave();
	void pintar();
	void borrar();
	void mover();
	void pintar_corazones();
	void morir();
	int X() { return x; }
	int Y() { return y; }
	void cor() { corazones--; }
	int vid() { return vidas; }
private:
	int x, y, corazones=3, vidas=3;
};

Nave::Nave(int x_, int y_): x(x_), y(y_) {}

Nave::~Nave()
{
}

void Nave::pintar()
{
	gotoxy(x, y);
	printf("  %c", 30);
	gotoxy(x, y+1);
	printf(" %c%c%c", 40, 207, 41);
	gotoxy(x, y+2);
	printf("%c%c %c%c", 30, 190, 190, 30);
}

void pintar_limites()
{
	int i;
	for (i = 2; i < 78; i++)
	{
		gotoxy(i, 3);
		printf("%c", 205);
		gotoxy(i, 33);
		printf("%c", 205);
	}
	for (i = 4; i < 33; i++)
	{
		gotoxy(2, i);
		printf("%c", 186);
		gotoxy(77, i);
		printf("%c", 186);
	}
	gotoxy(2, 3); printf("%c", 201);
	gotoxy(2, 33); printf("%c", 200);
	gotoxy(77, 3); printf("%c", 187);
	gotoxy(77, 33); printf("%c", 188);
}

void Nave::borrar()
{
	gotoxy(x, y);
	printf("       ");
	gotoxy(x, y+1);
	printf("       "); 
	gotoxy(x, y+2);
	printf("       ");
	pintar_limites();
}

void Nave::mover()
{
	char tecla;
	if (_kbhit())
	{
		tecla = _getch();
		borrar();
		if (tecla == IZQUIERDA && x>3)
			x--;
		if (tecla == DERECHA && x+8 <78)
			x++;
		if (tecla == ABAJO && y+3<33)
			y++;
		if (tecla == ARRIBA && y>4)
			y--;
		pintar();
		pintar_corazones();
	}
}

void Nave::pintar_corazones()
{
	int i;
	gotoxy(50, 2);
	printf("Vidas: %i", vidas);
	gotoxy(64, 2);
	printf("Salud:");
	gotoxy(70, 2);
	printf("      ");
	for ( i = 0; i < corazones; i++)
	{
		gotoxy(70 + i, 2);
		printf("%c", 3);
	}
}

void Nave::morir()
{
	if (corazones == 0)
	{
		borrar();
		gotoxy(x, y);   printf("   **   ");
		gotoxy(x, y+1); printf("  ****  ");
		gotoxy(x, y+2); printf("   **   ");
		Sleep(200);
		borrar();
		gotoxy(x, y);   printf(" * ** *");
		gotoxy(x, y + 1); printf("  ****  ");
		gotoxy(x, y + 2); printf(" * ** *");
		Sleep(200);
		borrar();
		vidas--;
		corazones = 3;
		pintar_corazones();
		pintar();
	}
}



class ast
{
public:
	ast(int p, int q) :x(p), y(q) {}
	~ast();
	void pintar();
	void mover();
	void choque(Nave &n);
	int X() { return x; }
	int Y() { return y; }
private:
	int x, y;
};

ast::~ast()
{
}

void ast::pintar()
{
	gotoxy(x, y);
	printf("%c", 184);
}

void ast::mover()
{
	gotoxy(x, y);
	printf(" ");
	y++;
	if (y > 32)
	{
		x = rand() % 71 + 4;
		y = 4;
	}
	pintar();
}

void ast::choque(Nave & n)
{
	if (x >= n.X() && x < n.X()+6 && y >= n.Y() && y < n.Y()+2)
	{
		n.cor();
		n.borrar();
		n.pintar();
		n.pintar_corazones();
		x = rand() % 71 + 4;
		y = 4;
	}
}

class bala
{
public:
	bala(int _x, int _y):x(_x), y(_y) {}
	~bala();
	void mover();
	bool fuera();
	int X() { return x; }
	int Y() { return y; }
private:
	int x, y;
};

bala::~bala()
{
}

void bala::mover()
{
	gotoxy(x, y);
	printf(" ");
	if(y>4)
		y--;
	gotoxy(x, y);
	printf("*");
}

bool bala::fuera()
{
	if (y == 4)
		return true;
	else
		return false;
}

int main()
{
	OcultarCursor();
	pintar_limites();
	Nave n(37, 30);
	n.pintar();
	n.pintar_corazones();
	vector<ast*> a;
	vector<ast*>::iterator ita;
	for (int i = 0; i < 5; i++)
		a.push_back(new ast(rand() % 75 + 3, rand() % 5 + 4));
	vector<bala*> b;
	vector<bala*>::iterator it;
	bool game_over = false;
	char tecla;
	int puntos = 0;
	while (!game_over)
	{
		gotoxy(4, 2);
		printf("Puntos: %i", puntos);
		try
		{
			if (_kbhit())
			{
				tecla = _getch();
				if (tecla == 'a')
				{
					b.push_back(new bala(n.X() + 2, n.Y() - 1));
				}
			}
		}
		catch (bad_alloc& e)
		{
			printf("Insuficiente espacio de memoria." );
			exit(-1);
		}

		for (it = b.begin(); it != b.end();)
		{
			(*it)->mover();
			if ((*it)->fuera()) {
				gotoxy((*it) -> X(), (*it) -> Y());	printf(" ");
				delete (*it);
				it = b.erase(it);
			}
			else	it++;
		}

		for ( ita = a.begin(); ita != a.end(); ita++)
		{
			(*ita)->mover();
			(*ita)->choque(n);
		}

		for (ita = a.begin(); ita != a.end();ita++)
		{
			for (it = b.begin(); it != b.end();)
			{
				if ((*ita)->X() == (*it)->X() && ((*ita)->Y() + 1 == (*it)->Y() || (*ita)->Y() == (*it)->Y()))
				{
					gotoxy((*it)->X(), (*it)->Y());	printf(" ");
					delete (*it);
					it = b.erase(it);

					a.push_back(new ast(rand() % 74 + 3, 4));
					gotoxy((*ita)->X(), (*ita)->Y());	printf(" ");
					delete (*ita);
					ita = a.erase(ita);
					puntos++;
				}
				else
					it++;
			}
		}
		n.morir();
		n.mover();
		Sleep(30);
		if (n.vid()==0)
			game_over = true;
	}
	gotoxy(14, 15);
	printf("Fin del juego.");
	system("pause");
	return 0;
}