#include <iostream>
#include<math.h>//pow
#include <time.h>//随机种子
#include <string.h>//strtok
#include <fstream>

using namespace std;

ifstream fin;
ofstream fout;

int i_arr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};//控制最中间九宫格的内容
int arr[9][9];//生成的终盘数独
int out[9][9];//向文件中输出的内容，比如生成终盘、生成初盘、求解终盘

int in[9][9]; //从文件中读出了此轮棋盘
int sol_arr[9][9];//待定
int sol_out[9][9];//待定

int sol_num = 0;//数独求解结果个数

typedef struct array_stack {
	int t_sta[9][9];
	struct array_stack *next;

	array_stack() {
	}
	array_stack(struct array_stack *p) {
		memcpy(t_sta, p->t_sta, sizeof(p->t_sta));
	}
} as;
as *sol_root;//求解树的根节点

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

bool do_solve(int u);
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
        

        //如果不要求唯一解则直接输出即可；否则看目前被挖空的out是否有唯一解，若无，重新挖，直到有唯一解为止
        if (u) {
			memcpy(in, out, sizeof(out));//把当前挖空的out赋给in作为输入，进行求解do_solve(u)
			while (!do_solve(u)) {
				memcpy(out, arr, sizeof(arr));//out变为终盘，然后重新挖空
				if(r1==0 && r2==0)
					set_lev(m);
				else if (r1 != r2)
					set_lev(rand() % (r2 - r1) + r1);
				else
					set_lev(r1);
				memcpy(in, out, sizeof(out));//将重新挖空的out赋给in作为输入，继续求解
			}
            //退出while说明已经找到了具有唯一解的棋盘out
			prttofl();
		} 
        else
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

int readsudoku() {
	char t;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			do {
				fin.get(t);
				if (fin.eof())
					return 1;//文件读完了
			} while (!((t >= '1' && t <= '9') || t == '$'));//空格、换行则继续读

			if (t != '$') 
				in[i][j] = (int)t - '0';
            else 
				in[i][j] = 0;
		}
	}
	memcpy(out, in, sizeof(in));
	return 0;
}

bool sgln(int n) {
	if (n == 1 || n == 2 || n == 4 || n == 8 || n == 16 || n == 32 || n == 64 || n == 128 || n == 256) {//内容确定
		return true;
	}
	return false;//排除内容不确定的格子
}

int as_r_xor(int x, int y, as *p) {
	int r = 0;
	for (int i = x, j = 0; j < 9; j++) {
		if ( sgln(p->t_sta[i][j]) && (j != y))//该位置内容已经确定，第二个条件存疑
			r ^= p->t_sta[i][j];
	}
	return 511 ^ r;//这一行还能填哪些，比如001100000，表示还能填6或7在这里
}


int as_c_xor(int x, int y, as *p) {
	int c = 0;
	for (int i = 0, j = y; i < 9; i++) {
		if ( sgln(p->t_sta[i][j]) && (i != x))
			c ^= p->t_sta[i][j];
	}
	return 511 ^ c;//这一列还能填哪些
}


int as_b_xor(int x, int y, as *p) {
	int b = 0;
	for (int i = (x / 3) * 3; i < x / 3 + 3; i++) {
		for (int j = (y / 3) * 3; j < y / 3 + 3; j++) {
			if ( sgln(p->t_sta[i][j]) && ((i != x) && (j != y)))
				b ^= p->t_sta[i][j];
		}
	}
	return 511 ^ b;//这一九宫格还能填哪些
}

int mtplx(int n) {
	for (int i = 0; i < 9; i++) {
		if ((n & (1 << i)) != 0)
			return n & (1 << i);
	}
	return 0;
}

int recursion(as *p) {//深搜
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (!sgln(p->t_sta[i][j])) {
				int n = mtplx(p->t_sta[i][j] & as_r_xor(i, j, p) & as_c_xor(i, j, p) & as_b_xor(i, j, p));//取所有可能取值中的最小数进行尝试
				while (n) {
					p->next = new as(p);
					p->next->t_sta[i][j] = n;//建立下一步棋盘
					recursion(p->next);
					p->t_sta[i][j] -= n;//刚才尝试过的就不再尝试了
					n = mtplx(p->t_sta[i][j] & as_r_xor(i, j, p) & as_c_xor(i, j, p) & as_b_xor(i, j, p));
				}
				return -1;//所有可能取值都遍历完了，内部节点/根节点的return
			} 
			else {
				sol_arr[i][j] = log2(p->t_sta[i][j]) + 1;
			}
		}
	}
    //对于9*9每次都走else，叶节点棋盘局面，每个数都能确定了，成功求解处一种结果
	memcpy(sol_out, sol_arr, sizeof(sol_arr));//本次结果存入sol_out
	sol_num++;
	return 0;//叶节点终盘的return
}

bool do_solve(int u) {
	sol_root = new as();//当前求解棋盘的根节点
	sol_root->next = NULL;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (in[i][j] != 0) {
				sol_root->t_sta[i][j] = 1 << (in[i][j] - 1);//sta为映射状态矩阵；将1~9映射到000000001~100000000
			}
			else{
				sol_root->t_sta[i][j] = 0;//暂时先全部补0
			}			
		}
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (in[i][j] == 0) {
				int r = as_r_xor(i, j,sol_root);//这个空所在的行还可以填谁
				int c = as_c_xor(i, j, sol_root);//这个空所在的列还可以填谁
				int b = as_b_xor(i, j, sol_root);//这个空所在的九宫格还可以填谁
				sol_root->t_sta[i][j] = r & c & b;//这个空还可以填谁
			}		
		}
	}

	recursion(sol_root);

	if (!u) {   //设置了-u时这一步无需输出求解结果，重点在生成唯一解的棋盘，而不在求解
		memcpy(out, sol_out, sizeof(sol_out));//多解的话，只会输出找到的最后一个解
		prttofl();
	}
	if (sol_num > 1)
		return true;//是唯一解
	return false;//多解
}

int main(int argc, char **argv) {

	int c = 0;//生成的终盘数目
    int n = 0;//生成的游戏数目
    bool en = false;//是否有-n参数
    //以下为只输入-n，其他参数的默认值
    int m = -1;//难度为-1级（等价于1级）
    int r1 = 0;
	int r2 = 0;
    int u = 0;//默认无需生成唯一解游戏


    if (argc <= 1) {
        cout<<"please input some args...";
        return 0;
    }

	for (int i = 0; i < argc; i++) {//遍历命令行参数，默认-n在-m或-r或-u的前面
		if (argv[i][1] == 'c') {
			c = stoi(argv[i + 1]);
			fout.open("final.txt");
		} 
        else if (argv[i][1] == 's') {
			fin.open(argv[i + 1]);
			fout.open("sudoku.txt");
			while (!readsudoku()) { //每次读入一个棋盘，返回1代表文件读完了
				do_solve(u);//对当前棋盘进行求解
			}
            return 0;//这里一定不要忘记！！！！不然会执行gen函数
		} 
        else if (argv[i][1] == 'n') {
			en = true;
			n = stoi(argv[i + 1]);
			fout.open("game.txt");
		} 
        else if (argv[i][1] == 'm') {
			if (en != true) {
				cout << "ERROR: -m needs -n parameter!\n";
				return 0;
			}
            int temp = stoi(argv[i + 1]);
            if(!(temp==1 || temp==2 || temp==3)){
                cout << "ERROR: level should in 1~3! \n";
				return 0;
            }
			m = -temp;//1，2，3转为-1，-2，-3来处理级别
		} 
        else if (argv[i][1] == 'r') {
			if (en != true) {
				cout << "ERROR: -r needs -n parameter!\n";
				return 0;
			}
           
            //上限20、下限55
			char *rge1;
			char *rge2;
			rge1 = strtok(argv[i+1],  "~");
            rge2 = strtok(NULL,  "~");

			r1 = stoi(rge1);
			r2 = stoi(rge2);
            // cout<<"++++++++++"<<r1<<endl;
            // cout<<"----------"<<r2<<endl;

            if (!( 20<=r1 && r1<=r2 && r2<=55 )) {
				cout << "ERROR: r1~r2 should in 20~55!\n";
				return 0;
			}
		} 
        else if (argv[i][1] == 'u') {
			if (en != true) {
				cout << "ERROR: -u needs -n parameter!\n";
				return 0;
			}
			u = 1;//要求生成唯一解游戏
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