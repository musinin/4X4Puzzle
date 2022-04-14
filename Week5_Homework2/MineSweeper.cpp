#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <memory.h>
#include <time.h>
#include <ctype.h>

enum LabelType { Empty = 0, Bomb = 9 };
enum MaskType { Hide = 0, Open, Flag };
static int MineMapMask[40][80];				// �ִ� ����x���� 80x40
static int MineMapLabel[40][80];
static int nx, ny;
static int nBomb;

inline int& mask(int x, int y) { return MineMapMask[y][x]; }
inline int& label(int x, int y) { return MineMapLabel[y][x]; }
inline bool isValid(int x, int y) { return (x >= 0 && x < nx&& y >= 0 && y < ny); }
inline bool isBomb(int x, int y) { return isValid(x, y) && label(x, y) == Bomb; }
inline bool isEmpty(int x, int y) { return isValid(x, y) && label(x, y) == Empty; }

static void dig(int x, int y)							// (x,y)�� �Ĵ�(����) �Լ�
{
	if (isValid(x, y) && mask(x, y) != Open)
	{
		mask(x, y) = Open;
		if (label(x, y) == 0)
		{
			dig(x - 1, y - 1);
			dig(x - 1, y);
			dig(x - 1, y + 1);
			dig(x, y - 1);
			dig(x, y + 1);
			dig(x + 1, y - 1);
			dig(x + 1, y);
			dig(x + 1, y + 1);
		}
	}
}

static void mark(int x, int y)							// (x,y)�� ����� �ȴ� �Լ�
{
	if (isValid(x, y) && mask(x, y) == Hide) 
		mask(x, y) = Flag;
}

static int getBombCount()								// ����� ���� ����ϴ� �Լ�
{
	int count = 0;
	for (int y = 0; y < ny; y++)
		for (int x = 0; x < nx; x++)
			if (mask(x, y) == Flag) count++;
	return count;
}

static void print()											// ���� ���� ȭ�� ��� �Լ�
{
	system("cls");
	printf("  �߰�:%2d  ��ü:%2d\n", getBombCount(), nBomb);
	printf("  ");
	for (int i = 0; i < nx; i++) printf("%2d", i + 1);
	printf("\n");

	for (int y = 0; y < ny; y++)
	{
		printf("%2d", y + 1);
		for (int x = 0; x < nx; x++)
		{
			if (mask(x, y) == Hide) printf("��");			// ���� ���� ��
			else if (mask(x, y) == Flag) printf("��");		// ���� ���� ��ġ
			else											// �� �ڸ�
			{
				if (isBomb(x, y)) printf("��");				// 9: ����
				else if (isEmpty(x, y)) printf("  ");		// 0: ��ĭ ǥ��
				else printf("%2d", label(x, y));			// 1~8: ���� ǥ��
			}
		}
		printf("\n");
	}
}

static int countNbrBombs(int x, int y)						// ������ ������ �� ��� �Լ�
{
	int count = 0;
	for (int yy = y - 1; yy <= y + 1; yy++)
		for (int xx = x - 1; xx <= x + 1; xx++)
			if (isValid(xx, yy) && label(xx, yy) == Bomb) 
				count++;
	return count;
}

static void init(int w, int h, int total = 9)
{
	nx = w;
	ny = h;
	nBomb = total;

	srand((unsigned int)time(NULL));

	for (int y = 0; y < ny; y++)
	for (int x = 0; x < nx; x++)
	{
		mask(x, y) = Hide;
		label(x, y) = 0;
	}
	for (int i = 0; i < nBomb; i++)
	{
		int x, y;
		do
		{
			x = rand() % nx;
			y = rand() % ny;
		} while (label(x, y) != Empty);
		label(x, y) = Bomb;
	}
	for (int y = 0; y < ny; y++)
	for (int x = 0; x < nx; x++)
		if (label(x, y) == Empty) 
			label(x, y) = countNbrBombs(x, y);
}

static bool getPos(int& x, int& y)
{
	char str[80], opt[3];
	printf("\n ��, �� (����:p) enter --> ");
	gets_s(str);

	int ret = sscanf_s(str, "%d%d%s", &y, &x, opt);
	x--;
	y--;
	return (ret == 3);
}

static int checkDone()										// ���� ���� �˻� �Լ�
{
	int count = 0;
	for (int y = 0; y < ny; y++)
		for (int x = 0; x < nx; x++)
		{
			if (mask(x, y) != Open) count++;
			else if (isBomb(x, y)) return -1;
		}
	return (count == nBomb) ? 1 : 0;
}

void playMineSweeper(int width, int height, int total)			// ���� ã�� �� �Լ�
{
	int x, y, status;
	init(width, height, total);
	do
	{
		print();
		bool isBomb = getPos(x, y);
		if (isBomb) mark(x, y);
		else dig(x, y);
		status = checkDone();
	} while (status == 0);

	print();
	if (status < 0) printf("\n���� : ���� ����!!!\n\n");
	else printf("\n���� : Ž�� ����!!!\n\n");
}

void main()
{
	int width, height, total;
	printf(" <Mine Sweeper>\n");
	printf("���ڸ��� ũ�� �Է� (���� ����) : ");
	scanf_s("%d%d", &width, &height);
	printf("�ż��� �� ������ ���� �Է� : ");
	scanf_s("%d", &total);

	playMineSweeper(width, height, total);
}