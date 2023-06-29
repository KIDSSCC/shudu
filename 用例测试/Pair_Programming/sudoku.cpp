#include <iostream>
#include<math.h>//pow
#include <time.h>//随机种子
#include <string.h>//strtok
#include <fstream>
#include<array>
#include<random>
#include<memory>
#include<algorithm>

//通过宏定义，来访问单一模式中的变量。
#define fin FileOperate::getInstance().finobject
#define fout FileOperate::getInstance().foutobject

#define i_arr Globalvarible::getInstance().G_i_arr
#define arr Globalvarible::getInstance().G_arr
#define out Globalvarible::getInstance().G_out
#define in Globalvarible::getInstance().G_in
#define sol_arr Globalvarible::getInstance().G_sol_arr
#define sol_out Globalvarible::getInstance().G_sol_out
#define sol_num Globalvarible::getInstance().G_sol_num
using namespace std;
//规范代码编写，采用类来封装输入输出行为
class FileOperate
{
public:
	//成员变量
	ifstream finobject;
	ofstream foutobject;

	static FileOperate& getInstance()
	{
		//单例模式，获取唯一实例
		static FileOperate instance;
		return instance;
	}
private:
	//设置默认构造函数，同时禁用拷贝构造与赋值构造
	FileOperate() = default;
	FileOperate(const FileOperate&) = delete;
	FileOperate& operator=(const FileOperate&) = delete;
	
};

class Globalvarible
{
public:
	//一维数组，代表1-9共9个数字，后续会被随机打乱顺序并填充至数独中。
	//std::array<int, 9>G_i_arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };//控制最中间九宫格的内容
	std::vector<int> G_i_arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::array<std::array<int, 9>, 9> G_arr{};//生成的终盘数独
	std::array<std::array<int, 9>, 9> G_out{};//向文件中输出的内容，比如生成终盘、生成初盘、求解终盘
	std::array<std::array<int, 9>, 9> G_in{};//从文件中读出了此轮棋盘
	std::array<std::array<int, 9>, 9> G_sol_arr{};
	std::array<std::array<int, 9>, 9> G_sol_out{};
	int G_sol_num = 0;//数独求解结果个数
	static Globalvarible& getInstance()
	{
		//单例模式，获取唯一实例
		static Globalvarible instance;
		return instance;
	}
private:
	//设置默认构造函数，同时禁用拷贝构造与赋值构造
	Globalvarible() = default;
	Globalvarible(const Globalvarible&) = delete;
	Globalvarible& operator=(const Globalvarible&) = delete;
};

class ParameterList
{
public:
	int c = 0;//生成的终盘数目
	int n = 0;//生成的游戏数目
	bool en = false;//是否有-n参数
	int m = -1;//难度为-1级（等价于1级）
	int r1 = 0;
	int r2 = 0;
	int u = 0;//默认无需生成唯一解游戏
	ParameterList() = default;
	void paraPrint() const
	{
		//打印程序当前执行时的命令行参数
		cout << "c is " << c << endl;
		cout << "n is " << n << endl;
		cout << "m is " << -m << endl;
		cout << "range is " << r1 << "--"<< r2 << endl;
		cout << "u is " << u << endl;
	}
	void clear()
	{
		c = 0;
		n = 0;
		en = false;
		m = -1;
		r1 = 0;
		r2 = 0;
		u = 0;
	}
};


void clearState()
{
	fin.close();
	fout.close();
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			arr[i][j] = 0;
			in[i][j] = 0;
			out[i][j] = 0;
			sol_arr[i][j] = 0;
			sol_out[i][j] = 0;
		}
	}
}

int parameterAnalysis_cn(ParameterList& curr_parameter, int argc, char** argv)
{
	int result = 0;
	//解析命令行参数c和n
	for (int i = 0; i < argc; i++)
	{
		if (argv[i][1] == 'c') {
			curr_parameter.c = atoi(argv[i + 1]);
			fout.open("final.txt");
			result++;
		}
		else if (argv[i][1] == 'n')
		{
			curr_parameter.en = true;
			curr_parameter.n = atoi(argv[i + 1]);
			fout.open("game.txt");
			result++;
		}
	}
	return result;
}

array<int, 2> particularProcess(const char* para)
{
	int num = 0;
	array<int, 2> result{};
	for (int i = 0; i < 5; i++)
	{
		if (para[i] != '~')
		{
			num = num * 10+(para[i]-'0');
		}
		else
		{
			result[0] = num;
			num = 0;
		}
	}
	result[1] = num;
	return result;
}

int parameterAnalysis_mru(ParameterList& curr_parameter, int argc, char** argv)
{
	//m,r,u参数都需要在n参数设置的情况下生效
	bool match = false;
	for (int i = 0; i < argc; i++)
	{
		//解析参数m，
		if (argv[i][1] == 'm')
		{
			match = true;
			int temp = atoi(argv[i + 1]);
			if (!(temp == 1 || temp == 2 || temp == 3)) {
				cout << "ERROR: level should in 1~3! \n";
				return 1;
			}
			curr_parameter.m = -temp;//1，2，3转为-1，-2，-3来处理级别
		}
		//解析参数r
		else if (argv[i][1] == 'r')
		{
			match = true;
			//上限20、下限55

			auto range = particularProcess(argv[i + 1]);
			curr_parameter.r1 = range[0];
			curr_parameter.r2 = range[1];
			
			if (!(20 <= curr_parameter.r1 && curr_parameter.r1 <= curr_parameter.r2 && curr_parameter.r2 <= 55)) {
				cout << "ERROR: r1~r2 should in 20~55!\n";
				return 2;
			}
			
		}
		//解析参数u
		else if (argv[i][1] == 'u')
		{
			match = true;
			curr_parameter.u = 1;
		}
	}
	if (match && !curr_parameter.en)
	{
		cout << "ERROR: argv needs -n parameter!\n";
		return 3;
	}
	return 0;
}
int parameterAnalysis_s(int argc, char** argv)
{
	//解析参数s
	for (int i = 0; i < argc; i++)
	{
		if (argv[i][1] == 's') {
			fin.open(argv[i + 1]);
			fout.open("sudoku.txt");
			if (!fin.is_open())
			{
				cout << "path not exists\n";
				return 1;
			}
			return 0;//这里一定不要忘记！！！！不然会执行gen函数
		}
	}
	return -1;
}


using as = struct array_stack {
	//求解数独时用于记录当前求解状态的结构体
	std::array<std::array<int, 9>, 9> t_sta{};
	std::shared_ptr<array_stack> next;

	//默认构造函数与拷贝构造函数
	array_stack() = default;
	explicit array_stack(std::shared_ptr<array_stack> p):t_sta(p->t_sta) {}
};

int getRandom()
{
	//通过随机数引擎来生成随机数，随机数范围在[1,32767];
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(1, 32767);
	return distribution(gen);
}

void shuffleArray()
{
	//打乱数组
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(i_arr.begin(), i_arr.end(), gen);
}

//start at x-th row,y-th column
void row_exd(int x, int y) {
	//将一片3*3区域内的顺序打乱，复制到另一片3*3的区域
	for (int i = x; i < x + 3; i++) {
		for (int j = y; j < y + 3; j++) {
			//为了满足对数组索引的要求，预先将计算结果转为整型
			int x_index = (i + 1) % 3 + x;
			int y_index = (j + 3) % 9;
			arr[x_index][y_index] = arr[i][j];
		}
	}
}

//start at x-th row,y-th column
void col_exd(int x, int y) {
	//将一片3*3区域内的顺序打乱，复制到另一片3*3的区域
	for (int i = x; i < x + 3; i++) {
		for (int j = y; j < y + 3; j++) {
			//为了满足对数组索引的要求，预先将计算结果转为整型
			int x_index = (i + 3) % 9;
			int y_index = (j + 1) % 3 + y;
			arr[x_index][y_index] = arr[i][j];
		}
	}
}

void prttofl() {
	//将当前的若干盘数独输出到文件中
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			// 0代表空格，输出时以$符号代替
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
	//用于在9*9的全局数独中挖出若干空位
	for (int i = 0; i < n; i++) {
		int x = 0;
		int y = 0;
		do {
			x = getRandom() % 9;
			y = getRandom() % 9;
		} while (out[x][y] == 0);
		//挖够n个坑
		if (x >= 0 && x < 9 && y >= 0 && y < 9)
		{
			out[x][y] = 0;
		}
	}
}

void set_lev(int m,int r1,int r2) {
	
	//要么是难度等级，要么是挖空数
	int rangeR = (r1 == r2) ? r1 : getRandom() % (r2 - r1) + r1;
	int level = (r1 == 0 && r2 == 0) ? m : rangeR;
	std::array<int, 3> blocks{ 27,36,45 };
	int index = -level - 1;
	level < 0 ? dig(blocks[index]) : dig(level);
}

bool do_solve(int u);

void solveUnique(int m,int r1,int r2,int u)
{
	//如果不要求唯一解则直接输出即可；否则看目前被挖空的out是否有唯一解，若无，重新挖，直到有唯一解为止
	if (u) {
		//把当前挖空的out赋给in作为输入，进行求解do_solve(u)
		in = out;
		while (!do_solve(u)) {
			out = arr;
			set_lev(m, r1, r2);
			//将重新挖空的out赋给in作为输入，继续求解
			in = out;
		}
		//退出while说明已经找到了具有唯一解的棋盘out
		prttofl();
	}
	else
		prttofl();
}
void init_gen(int c, int m, int r1, int r2, int u) {
	//通过参数c来判断生成的是终盘函数初盘

	//打乱数组并将其填充至5号区域
	shuffleArray();
	int k = 0;
	for (int i = 3; i < 6; i++) {
		for (int j = 3; j < 6; j++) {
			//先填满最中间的九宫格,
			arr[i][j] = i_arr[k];
			k += 1;
		}
	}
	//填充4号区域和6号区域
	row_exd(3, 3);
	row_exd(3, 6);
	//填充7号，1号区域
	col_exd(3, 0);
	col_exd(6, 0);
	//填充8号，2号区域
	col_exd(3, 3);
	col_exd(6, 3);
	//填充9号，3号区域
	col_exd(3, 6);
	col_exd(6, 6);

	//out之后还需要进一步挖空
	out = arr;
	if (c != 0)
	{
		//输出终盘到final.txt
		prttofl();
		return;
	}
	
	//在没有r参数的情况下，按照m等级挖空，否则在r1至r2的范围中挖
	set_lev(m, r1, r2);
	solveUnique(m, r1, r2, u);
}


void gen(int c, int n, int m, int r1, int r2, int u) {
	cout << "here\n";
	exit(0);
	if (c != 0) {//需要生成终盘
		for (int i = 0; i < c; i++)
			init_gen(c, m, r1, r2, u);
	}
	else {
		//需要生成游戏初盘
		for (int i = 0; i < n; i++)
			init_gen(c, m, r1, r2, u);
	}
}
int readsudoku() {
	char t = ' ';
	bool fileEnd = false; // 标志文件是否已经读取完毕
	int row = 0;
	int col = 0;
	//开始从文件中读取数独
	while (row < 9) {
		fin.get(t);
		if (fin.eof()) {
			fileEnd = true;
			break;
		}
		if ((t >= '1' && t <= '9') || t == '$') {
			if (t != '$') {
				in[row][col] = (int)t - '0';
			}
			else {
				in[row][col] = 0;
			}
			col++;
			if (col == 9) {
				col = 0;
				row++;
			}
		}
	}
	if (fileEnd) {
		return 1; // 文件读取完毕
	}
	out = in;
	return 0;
}

bool sgln(int n) {
	if (n == 1 || n == 2 || n == 4 || n == 8 || n == 16 || n == 32 || n == 64 || n == 128 || n == 256) {//内容确定
		return true;
	}
	return false;//排除内容不确定的格子
}

int as_r_xor(int x, int y, std::shared_ptr<array_stack> p) {
	int r = 0;
	for (int i = x, j = 0; j < 9; j++) {
		if (sgln(p->t_sta[i][j]) && (j != y))//该位置内容已经确定，第二个条件存疑
			r ^= p->t_sta[i][j];
	}
	return 511 ^ r;//这一行还能填哪些，比如001100000，表示还能填6或7在这里
}


int as_c_xor(int x, int y, std::shared_ptr<array_stack> p) {
	int c = 0;
	for (int i = 0, j = y; i < 9; i++) {
		if (sgln(p->t_sta[i][j]) && (i != x))
			c ^= p->t_sta[i][j];
	}
	return 511 ^ c;//这一列还能填哪些
}


int as_b_xor(int x, int y, std::shared_ptr<array_stack> p) {
	int b = 0;
	for (int i = (x / 3) * 3; i < x / 3 + 3; i++) {
		for (int j = (y / 3) * 3; j < y / 3 + 3; j++) {
			if (sgln(p->t_sta[i][j]) && ((i != x) && (j != y)))
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

int recursion(std::shared_ptr<array_stack> p) {//深搜
	int i = 0;
	int j = 0;
	while (i < 9 )
	{
		if (!sgln(p->t_sta[i][j]))
		{
			int n = mtplx(p->t_sta[i][j] & as_r_xor(i, j, p) & as_c_xor(i, j, p) & as_b_xor(i, j, p));//取所有可能取值中的最小数进行尝试
			while (n) {
				p->next = std::make_unique<array_stack>(p);
				p->next->t_sta[i][j] = n;//建立下一步棋盘
				recursion(p->next);
				p->t_sta[i][j] -= n;//刚才尝试过的就不再尝试了
				n = mtplx(p->t_sta[i][j] & as_r_xor(i, j, p) & as_c_xor(i, j, p) & as_b_xor(i, j, p));
			}
			return -1;//所有可能取值都遍历完了，内部节点/根节点的return
		}
		else
		{
			sol_arr[i][j] = (int)(log2(p->t_sta[i][j])) + 1;
		}
		j = j + 1;
		if (j >= 9)
		{
			j = 0;
			i = i + 1;
		}
	}
	//对于9*9每次都走else，叶节点棋盘局面，每个数都能确定了，成功求解处一种结果
	sol_out = sol_arr;
	sol_num++;
	return 0;//叶节点终盘的return
}

// 求解数独
bool do_solve(int u) {
	
	//当前求解棋盘的根节点
	auto sol_root= std::make_shared<array_stack>();
	sol_root->next = nullptr;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (in[i][j] != 0) {
				// 映射为独热编码
				sol_root->t_sta[i][j] = 1 << (in[i][j] - 1);//sta为映射状态矩阵；将1~9映射到000000001~100000000
			}
			else {
				sol_root->t_sta[i][j] = 0;//暂时先全部补0
			}
		}
	}
	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			// 对于数独盘中的每一个位置
			if (in[i][j] == 0) {
				int r = as_r_xor(i, j, sol_root);//这个空所在的行还可以填谁
				int c = as_c_xor(i, j, sol_root);//这个空所在的列还可以填谁
				int b = as_b_xor(i, j, sol_root);//这个空所在的九宫格还可以填谁
				sol_root->t_sta[i][j] = r & c & b;//这个空还可以填谁
			}
		}
	}

	recursion(sol_root);

	if (!u) {   //设置了-u时这一步无需输出求解结果，重点在生成唯一解的棋盘，而不在求解
		//memcpy(out, sol_out, sizeof(sol_out));//多解的话，只会输出找到的最后一个解
		out = sol_out;
		prttofl();
	}
	if (sol_num > 1)
		return true;//是唯一解
	return false;//多解
}

int begin(int argc, char** argv) {
	
	ParameterList curr_para;
	if (argc <= 2) {
		cout << "please input some args...";
		//参数过少
		curr_para.clear();
		clearState();
		return 1;
	}

	//判断-s参数，-1：无参数，0：有参数，1：文件打开失败
	int s_condition = parameterAnalysis_s(argc, argv);
	if (s_condition>=0)
	{
		if (s_condition == 1)
		{
			//需要求解的数独不存在
			curr_para.clear();
			clearState();
			return 2;
		}
		while (!readsudoku()) { //每次读入一个棋盘，返回1代表文件读完了
			do_solve(curr_para.u);//对当前棋盘进行求解
		}
		return 0;
	}
	
	int cn_condition=parameterAnalysis_cn(curr_para, argc, argv);
	
	int mru_condition=parameterAnalysis_mru(curr_para, argc, argv);
	if (mru_condition > 0)
	{
		//3:m的范围不对，4:r的范围不对，5:没有n参数
		curr_para.clear();
		clearState();
		return mru_condition + 2;
	}

	if (cn_condition == 0)
	{
		//缺少必要参数
		curr_para.clear();
		clearState();
		return 6;
	}

	gen(curr_para.c, curr_para.n, curr_para.m, curr_para.r1, curr_para.r2, curr_para.u);

	curr_para.paraPrint();
	curr_para.clear();
	clearState();
	return 0;
}
