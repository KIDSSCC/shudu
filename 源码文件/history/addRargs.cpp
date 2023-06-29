#include <iostream>
#include<math.h>//pow
#include <time.h>//随机种子
#include <fstream>

using namespace std;


ofstream fout;
int arr[9][9];//生成的终盘数独
int out[9][9];//解出来的终盘数独；如无需求解，则等价于arr
int i_arr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};//控制最中间九宫格的内容



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
			if (out[i][j] != 0)
				fout << out[i][j] << " ";
			else
				fout << '$' << " ";
		}
		fout << "\n";
	}
	fout << "\n";
}

void dig(int n) {
	for (int i = 0; i < n; i++) {
		int x, y;
		do {
			x = rand() % 9;
			y = rand() % 9;
		} while (out[x][y] == 0);
		out[x][y] = 0;//挖够n个坑
	}
}

void set_lev(int level) {//要么是难度等级，要么是挖空数
	switch (level) {
		case -1:
			dig(27);//默认一级难度，挖27个坑
			break;
		case -2:
			dig(36);
			break;
		case -3:
			dig(45);
			break;
		default:    
			dig(level);
			break;
	}
}

void init_gen(int c, int n, int m, int r1, int r2, int u) {
	for (int i = 0; i < 15; i++) {//15是交换次数，控制随机性
		int idx1 = rand() % 9;
		int idx2 = rand() % 9;
		int t = i_arr[idx1];
		i_arr[idx1] = i_arr[idx2];
		i_arr[idx2] = t;
	}

	int k = 0;
	for (int i = 3; i < 6; i++) {
		for (int j = 3; j < 6; j++) {
			arr[i][j] = i_arr[k++];//先填满最中间的九宫格
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

	memcpy(out, arr, sizeof(arr));//out之后还需要进一步挖空

    if(c!=0)        //输出终盘到final.txt
	    prttofl();
    else{           //先挖空，然后输出初盘到game.txt
        if(r1==0 && r2==0)  //没有-r参数，则按照难度等级来挖
            set_lev(m);
        else if (r1 != r2)  //有-r参数，按照范围随机选挖空数来挖
			set_lev(rand() % (r2 - r1) + r1);
		else
			set_lev(r1);
        

        //-u参数待实现
        
        prttofl();
    }
}


void gen(int c, int n , int m, int r1, int r2, int u) {
	srand(time(0));//设置随机种子
    if(c!=0){//需要生成终盘
        for (int i = 0; i < c; i++)
            init_gen(c, n, m, r1, r2, u);
    }
    else{//需要生成游戏初盘
        for (int i = 0; i < n; i++)
			init_gen(c, n, m, r1, r2, u);
    }
}

int main(int argc, char **argv) {

	int c = 0;//生成的终盘数目
    int n = 0;//生成的游戏数目
    bool en = false;//是否有-n参数
    //以下为只输入-n，其他参数的默认值
    int m = -1;//难度为-1级（等价于1级）
    int r1 = 0;
	int r2 = 0;
    int u = 0;//为1说明有唯一解


	for (int i = 0; i < argc; i++) {//遍历命令行参数，默认-n在-m或-r或-u的前面
		if (argv[i][1] == 'c') {
			c = stoi(argv[i + 1]);
			fout.open("final.txt");
		} 
        else if (argv[i][1] == 'n') {
			en = true;
			n = stoi(argv[i + 1]);
			fout.open("game.txt");
		} 
        else if (argv[i][1] == 'm') {
			if (en != true) {
				cout << "ERROR: -m needs -n parameter for shudu.exe\n";
				return 0;
			}
			m = -(stoi(argv[i + 1]));//1，2，3转为-1，-2，-3来处理级别
		} 
        else if (argv[i][1] == 'r') {
			if (en != true) {
				cout << "ERROR: -r needs -n parameter for shudu.exe\n";
				return 0;
			}
           
            //上限20、下限55，两位数
			char rge1[3];
			char rge2[3];
			for (int j = 0; j < 2; j++) {
				rge1[j] = argv[i+1][j];
				rge2[j] = argv[i+1][j+3];
			}
            rge1[2]='\0';
            rge2[2]='\0';

			r1 = stoi(rge1);
			r2 = stoi(rge2);
            // cout<<"++++++++++"<<r1<<endl;
            // cout<<"----------"<<r2<<endl;

            if (!( 20<=r1 && r1<=r2 && r2<=55 )) {
				cout << "ERROR: r1~r2 should in 20~55 for shudu.exe\n";
				return 0;
			}
		} 
        else if (argv[i][1] == 'u') {
			if (en != true) {
				cout << "ERROR: -u needs -n parameter for shudu.exe\n";
				return 0;
			}
			u = 1;
		}
	}

	
	gen(c, n, m, r1, r2, u);

	cout << "c:" << c << "\n";
    cout << "n:" << n << "\n";
    cout << "m:" << -m << "\n";
    cout << "r1~r2:" << r1 << "~" << r2 << "\n";
	cout << "u:" << u << "\n";
	
	return 0;
}