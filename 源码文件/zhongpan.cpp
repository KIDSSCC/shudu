#include <iostream>
#include<math.h>//pow
#include <time.h>//随机种子
#include <fstream>

using namespace std;


ofstream fout;
int out[9][9];//store the output
int arr[9][9];//store the finished sudoku
int i_arr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};//used to generate the initial sudoku



//start at x-th row,y-th column
void row_exd(int x, int y) {
	for (int i = x; i < x + 3; i++) {
		for (int j = y; j < y + 3; j++) {
			arr[(i + 1) % 3 + x][(j + 3) % 9] = arr[i][j];
		}
	}
}

//start at x-th row,y-th column
void col_exd(int x, int y) {
	for (int i = x; i < x + 3; i++) {
		for (int j = y; j < y + 3; j++) {
			arr[(i + 3) % 9][(j + 1) % 3 + y] = arr[i][j];
		}
	}
}

void prttofl() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			fout << out[i][j] << " ";
		}
		fout << "\n";
	}
	fout << "\n";
}

void init_gen(int c) {
	for (int i = 0; i < 15; i++) {
		int idx1 = rand() % 9;
		int idx2 = rand() % 9;
		int t = i_arr[idx1];
		i_arr[idx1] = i_arr[idx2];
		i_arr[idx2] = t;
	}

	int k = 0;
	for (int i = 3; i < 6; i++) {
		for (int j = 3; j < 6; j++) {
			arr[i][j] = i_arr[k++];
		}
	}

	row_exd(3, 3);
	row_exd(3, 6);
	col_exd(3, 0);
	col_exd(6, 0);
	col_exd(3, 3);
	col_exd(6, 3);
	col_exd(3, 6);
	col_exd(6, 6);

	memcpy(out, arr, sizeof(arr));

	prttofl();
}


void gen(int c) {
	srand(time(0));//设置随机种子
    for (int i = 0; i < c; i++)
        init_gen(c);
}

int main(int argc, char **argv) {

	int c = 0;//生成的终盘数目

	for (int i = 0; i < argc; i++) {//遍历命令行参数，默认-n在-m或-r或-u的前面
		if (argv[i][1] == 'c') {
			c = stoi(argv[i + 1]);
			fout.open("final.txt");
		} 
	}

	if(c!=0)
		gen(c);

	cout << "c:" << c << "\n";
	
	return 0;
}