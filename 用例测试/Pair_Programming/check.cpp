#include <iostream>
#include <fstream>
#include<memory>
#include<array>
#include<random>
#include <unordered_set>
using namespace std;

ifstream checkfin;
int checkin[9][9];
int checkout[9][9];

int check_readsudoku() {
	char t = ' ';
	bool fileEnd = false; // ��־�ļ��Ƿ��Ѿ���ȡ���
	int row = 0;
	int col = 0;
	//��ʼ���ļ��ж�ȡ����
	while (row < 9) {
		checkfin.get(t);
		if (checkfin.eof()) {
			fileEnd = true;
			break;
		}
		if ((t >= '1' && t <= '9') || t == '$') {
			if (t != '$') {
				checkin[row][col] = (int)t - '0';
			}
			else {
				checkin[row][col] = 0;
			}
			col++;
			if (col == 9) {
				col = 0;
				row++;
			}
		}
	}
	if (fileEnd) {
		return 1; // �ļ���ȡ���
	}
	return 0;
}

bool ifZhongpan() {
	std::unordered_set<int> rowSet;
	std::unordered_set<int> colSet;
	std::unordered_set<int> boxSet;
	for (int i = 0; i < 9; ++i) {
		rowSet.clear();
		colSet.clear();
		boxSet.clear();

		for (int j = 0; j < 9; ++j) {
			// �����
			if (checkin[i][j] != 0 && rowSet.count(checkin[i][j]) > 0) {
				return false;
			}
			rowSet.insert(checkin[i][j]);

			// �����
			if (checkin[j][i] != 0 && colSet.count(checkin[j][i]) > 0) {
				return false;
			}
			colSet.insert(checkin[j][i]);

			// ���3x3����
			int rowIndex = 3 * (i / 3) + (j / 3);
			int colIndex = 3 * (i % 3) + (j % 3);
			if (checkin[rowIndex][colIndex] != 0 && boxSet.count(checkin[rowIndex][colIndex]) > 0) {
				return false;
			}
			boxSet.insert(checkin[rowIndex][colIndex]);
		}
	}

	return true;
}

bool ifAllZhongpan(const char* path) {
	checkfin.open(path);
	while (!check_readsudoku()) { //ÿ�ζ���һ�����̣�����1�����ļ�������
		if (ifZhongpan() == 0) {
			return false;
		}
	}
	return true;
}

bool checkStartGame(int num)
{
	checkfin.open("game.txt");
	int charcount = 0;
	int blockcount = 0;
	bool result = true;
	while (!checkfin.eof())
	{
		char t = ' ';
		checkfin.get(t);
		if (t >= '1' && t <= '9' || t == ' ' || t == '\n')
		{
			charcount++;
			continue;
		}
		else if (t == '$')
		{
			charcount++;
			blockcount++;
		}
		else
		{
			result = false;
			break;
		}
		if (charcount == 81)
		{
			charcount = 0;
			if (num == -1&& !(blockcount >= 20 && blockcount <= 45))
			{
				result = false;
				break;
			}
			if (num == 1&&blockcount!=27)
			{
				result = false;
				break;
			}
			if (num == 2 && blockcount != 36)
			{
				result = false;
				break;
			}
			if (num == 3 && blockcount!=45)
			{
				result = false;
				break;
			}
		}
	}
	return result;
	
}