#include <iostream>
#include<math.h>//pow
#include <time.h>//�������
#include <string.h>//strtok
#include <fstream>
#include<array>
#include<random>
#include<memory>
#include<algorithm>

//ͨ���궨�壬�����ʵ�һģʽ�еı�����
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
//�淶�����д������������װ���������Ϊ
class FileOperate
{
public:
	//��Ա����
	ifstream finobject;
	ofstream foutobject;

	static FileOperate& getInstance()
	{
		//����ģʽ����ȡΨһʵ��
		static FileOperate instance;
		return instance;
	}
private:
	//����Ĭ�Ϲ��캯����ͬʱ���ÿ��������븳ֵ����
	FileOperate() = default;
	FileOperate(const FileOperate&) = delete;
	FileOperate& operator=(const FileOperate&) = delete;
	
};

class Globalvarible
{
public:
	//һά���飬����1-9��9�����֣������ᱻ�������˳������������С�
	//std::array<int, 9>G_i_arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };//�������м�Ź��������
	std::vector<int> G_i_arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::array<std::array<int, 9>, 9> G_arr{};//���ɵ���������
	std::array<std::array<int, 9>, 9> G_out{};//���ļ�����������ݣ������������̡����ɳ��̡��������
	std::array<std::array<int, 9>, 9> G_in{};//���ļ��ж����˴�������
	std::array<std::array<int, 9>, 9> G_sol_arr{};
	std::array<std::array<int, 9>, 9> G_sol_out{};
	int G_sol_num = 0;//�������������
	static Globalvarible& getInstance()
	{
		//����ģʽ����ȡΨһʵ��
		static Globalvarible instance;
		return instance;
	}
private:
	//����Ĭ�Ϲ��캯����ͬʱ���ÿ��������븳ֵ����
	Globalvarible() = default;
	Globalvarible(const Globalvarible&) = delete;
	Globalvarible& operator=(const Globalvarible&) = delete;
};

class ParameterList
{
public:
	int c = 0;//���ɵ�������Ŀ
	int n = 0;//���ɵ���Ϸ��Ŀ
	bool en = false;//�Ƿ���-n����
	int m = -1;//�Ѷ�Ϊ-1�����ȼ���1����
	int r1 = 0;
	int r2 = 0;
	int u = 0;//Ĭ����������Ψһ����Ϸ
	ParameterList() = default;
	void paraPrint() const
	{
		//��ӡ����ǰִ��ʱ�������в���
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
	//���������в���c��n
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
	//m,r,u��������Ҫ��n�������õ��������Ч
	bool match = false;
	for (int i = 0; i < argc; i++)
	{
		//��������m��
		if (argv[i][1] == 'm')
		{
			match = true;
			int temp = atoi(argv[i + 1]);
			if (!(temp == 1 || temp == 2 || temp == 3)) {
				cout << "ERROR: level should in 1~3! \n";
				return 1;
			}
			curr_parameter.m = -temp;//1��2��3תΪ-1��-2��-3��������
		}
		//��������r
		else if (argv[i][1] == 'r')
		{
			match = true;
			//����20������55

			auto range = particularProcess(argv[i + 1]);
			curr_parameter.r1 = range[0];
			curr_parameter.r2 = range[1];
			
			if (!(20 <= curr_parameter.r1 && curr_parameter.r1 <= curr_parameter.r2 && curr_parameter.r2 <= 55)) {
				cout << "ERROR: r1~r2 should in 20~55!\n";
				return 2;
			}
			
		}
		//��������u
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
	//��������s
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
			return 0;//����һ����Ҫ���ǣ���������Ȼ��ִ��gen����
		}
	}
	return -1;
}


using as = struct array_stack {
	//�������ʱ���ڼ�¼��ǰ���״̬�Ľṹ��
	std::array<std::array<int, 9>, 9> t_sta{};
	std::shared_ptr<array_stack> next;

	//Ĭ�Ϲ��캯���뿽�����캯��
	array_stack() = default;
	explicit array_stack(std::shared_ptr<array_stack> p):t_sta(p->t_sta) {}
};

int getRandom()
{
	//ͨ�������������������������������Χ��[1,32767];
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(1, 32767);
	return distribution(gen);
}

void shuffleArray()
{
	//��������
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(i_arr.begin(), i_arr.end(), gen);
}

//start at x-th row,y-th column
void row_exd(int x, int y) {
	//��һƬ3*3�����ڵ�˳����ң����Ƶ���һƬ3*3������
	for (int i = x; i < x + 3; i++) {
		for (int j = y; j < y + 3; j++) {
			//Ϊ�����������������Ҫ��Ԥ�Ƚ�������תΪ����
			int x_index = (i + 1) % 3 + x;
			int y_index = (j + 3) % 9;
			arr[x_index][y_index] = arr[i][j];
		}
	}
}

//start at x-th row,y-th column
void col_exd(int x, int y) {
	//��һƬ3*3�����ڵ�˳����ң����Ƶ���һƬ3*3������
	for (int i = x; i < x + 3; i++) {
		for (int j = y; j < y + 3; j++) {
			//Ϊ�����������������Ҫ��Ԥ�Ƚ�������תΪ����
			int x_index = (i + 3) % 9;
			int y_index = (j + 1) % 3 + y;
			arr[x_index][y_index] = arr[i][j];
		}
	}
}

void prttofl() {
	//����ǰ������������������ļ���
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			// 0����ո����ʱ��$���Ŵ���
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
	//������9*9��ȫ���������ڳ����ɿ�λ
	for (int i = 0; i < n; i++) {
		int x = 0;
		int y = 0;
		do {
			x = getRandom() % 9;
			y = getRandom() % 9;
		} while (out[x][y] == 0);
		//�ڹ�n����
		if (x >= 0 && x < 9 && y >= 0 && y < 9)
		{
			out[x][y] = 0;
		}
	}
}

void set_lev(int m,int r1,int r2) {
	
	//Ҫô���Ѷȵȼ���Ҫô���ڿ���
	int rangeR = (r1 == r2) ? r1 : getRandom() % (r2 - r1) + r1;
	int level = (r1 == 0 && r2 == 0) ? m : rangeR;
	std::array<int, 3> blocks{ 27,36,45 };
	int index = -level - 1;
	level < 0 ? dig(blocks[index]) : dig(level);
}

bool do_solve(int u);

void solveUnique(int m,int r1,int r2,int u)
{
	//�����Ҫ��Ψһ����ֱ��������ɣ�����Ŀǰ���ڿյ�out�Ƿ���Ψһ�⣬���ޣ������ڣ�ֱ����Ψһ��Ϊֹ
	if (u) {
		//�ѵ�ǰ�ڿյ�out����in��Ϊ���룬�������do_solve(u)
		in = out;
		while (!do_solve(u)) {
			out = arr;
			set_lev(m, r1, r2);
			//�������ڿյ�out����in��Ϊ���룬�������
			in = out;
		}
		//�˳�while˵���Ѿ��ҵ��˾���Ψһ�������out
		prttofl();
	}
	else
		prttofl();
}
void init_gen(int c, int m, int r1, int r2, int u) {
	//ͨ������c���ж����ɵ������̺�������

	//�������鲢���������5������
	shuffleArray();
	int k = 0;
	for (int i = 3; i < 6; i++) {
		for (int j = 3; j < 6; j++) {
			//���������м�ľŹ���,
			arr[i][j] = i_arr[k];
			k += 1;
		}
	}
	//���4�������6������
	row_exd(3, 3);
	row_exd(3, 6);
	//���7�ţ�1������
	col_exd(3, 0);
	col_exd(6, 0);
	//���8�ţ�2������
	col_exd(3, 3);
	col_exd(6, 3);
	//���9�ţ�3������
	col_exd(3, 6);
	col_exd(6, 6);

	//out֮����Ҫ��һ���ڿ�
	out = arr;
	if (c != 0)
	{
		//������̵�final.txt
		prttofl();
		return;
	}
	
	//��û��r����������£�����m�ȼ��ڿգ�������r1��r2�ķ�Χ����
	set_lev(m, r1, r2);
	solveUnique(m, r1, r2, u);
}


void gen(int c, int n, int m, int r1, int r2, int u) {
	cout << "here\n";
	exit(0);
	if (c != 0) {//��Ҫ��������
		for (int i = 0; i < c; i++)
			init_gen(c, m, r1, r2, u);
	}
	else {
		//��Ҫ������Ϸ����
		for (int i = 0; i < n; i++)
			init_gen(c, m, r1, r2, u);
	}
}
int readsudoku() {
	char t = ' ';
	bool fileEnd = false; // ��־�ļ��Ƿ��Ѿ���ȡ���
	int row = 0;
	int col = 0;
	//��ʼ���ļ��ж�ȡ����
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
		return 1; // �ļ���ȡ���
	}
	out = in;
	return 0;
}

bool sgln(int n) {
	if (n == 1 || n == 2 || n == 4 || n == 8 || n == 16 || n == 32 || n == 64 || n == 128 || n == 256) {//����ȷ��
		return true;
	}
	return false;//�ų����ݲ�ȷ���ĸ���
}

int as_r_xor(int x, int y, std::shared_ptr<array_stack> p) {
	int r = 0;
	for (int i = x, j = 0; j < 9; j++) {
		if (sgln(p->t_sta[i][j]) && (j != y))//��λ�������Ѿ�ȷ�����ڶ�����������
			r ^= p->t_sta[i][j];
	}
	return 511 ^ r;//��һ�л�������Щ������001100000����ʾ������6��7������
}


int as_c_xor(int x, int y, std::shared_ptr<array_stack> p) {
	int c = 0;
	for (int i = 0, j = y; i < 9; i++) {
		if (sgln(p->t_sta[i][j]) && (i != x))
			c ^= p->t_sta[i][j];
	}
	return 511 ^ c;//��һ�л�������Щ
}


int as_b_xor(int x, int y, std::shared_ptr<array_stack> p) {
	int b = 0;
	for (int i = (x / 3) * 3; i < x / 3 + 3; i++) {
		for (int j = (y / 3) * 3; j < y / 3 + 3; j++) {
			if (sgln(p->t_sta[i][j]) && ((i != x) && (j != y)))
				b ^= p->t_sta[i][j];
		}
	}
	return 511 ^ b;//��һ�Ź���������Щ
}

int mtplx(int n) {
	for (int i = 0; i < 9; i++) {
		if ((n & (1 << i)) != 0)
			return n & (1 << i);
	}
	return 0;
}

int recursion(std::shared_ptr<array_stack> p) {//����
	int i = 0;
	int j = 0;
	while (i < 9 )
	{
		if (!sgln(p->t_sta[i][j]))
		{
			int n = mtplx(p->t_sta[i][j] & as_r_xor(i, j, p) & as_c_xor(i, j, p) & as_b_xor(i, j, p));//ȡ���п���ȡֵ�е���С�����г���
			while (n) {
				p->next = std::make_unique<array_stack>(p);
				p->next->t_sta[i][j] = n;//������һ������
				recursion(p->next);
				p->t_sta[i][j] -= n;//�ղų��Թ��ľͲ��ٳ�����
				n = mtplx(p->t_sta[i][j] & as_r_xor(i, j, p) & as_c_xor(i, j, p) & as_b_xor(i, j, p));
			}
			return -1;//���п���ȡֵ���������ˣ��ڲ��ڵ�/���ڵ��return
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
	//����9*9ÿ�ζ���else��Ҷ�ڵ����̾��棬ÿ��������ȷ���ˣ��ɹ���⴦һ�ֽ��
	sol_out = sol_arr;
	sol_num++;
	return 0;//Ҷ�ڵ����̵�return
}

// �������
bool do_solve(int u) {
	
	//��ǰ������̵ĸ��ڵ�
	auto sol_root= std::make_shared<array_stack>();
	sol_root->next = nullptr;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (in[i][j] != 0) {
				// ӳ��Ϊ���ȱ���
				sol_root->t_sta[i][j] = 1 << (in[i][j] - 1);//staΪӳ��״̬���󣻽�1~9ӳ�䵽000000001~100000000
			}
			else {
				sol_root->t_sta[i][j] = 0;//��ʱ��ȫ����0
			}
		}
	}
	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			// �����������е�ÿһ��λ��
			if (in[i][j] == 0) {
				int r = as_r_xor(i, j, sol_root);//��������ڵ��л�������˭
				int c = as_c_xor(i, j, sol_root);//��������ڵ��л�������˭
				int b = as_b_xor(i, j, sol_root);//��������ڵľŹ��񻹿�����˭
				sol_root->t_sta[i][j] = r & c & b;//����ջ�������˭
			}
		}
	}

	recursion(sol_root);

	if (!u) {   //������-uʱ��һ�����������������ص�������Ψһ������̣����������
		//memcpy(out, sol_out, sizeof(sol_out));//���Ļ���ֻ������ҵ������һ����
		out = sol_out;
		prttofl();
	}
	if (sol_num > 1)
		return true;//��Ψһ��
	return false;//���
}

int begin(int argc, char** argv) {
	
	ParameterList curr_para;
	if (argc <= 2) {
		cout << "please input some args...";
		//��������
		curr_para.clear();
		clearState();
		return 1;
	}

	//�ж�-s������-1���޲�����0���в�����1���ļ���ʧ��
	int s_condition = parameterAnalysis_s(argc, argv);
	if (s_condition>=0)
	{
		if (s_condition == 1)
		{
			//��Ҫ��������������
			curr_para.clear();
			clearState();
			return 2;
		}
		while (!readsudoku()) { //ÿ�ζ���һ�����̣�����1�����ļ�������
			do_solve(curr_para.u);//�Ե�ǰ���̽������
		}
		return 0;
	}
	
	int cn_condition=parameterAnalysis_cn(curr_para, argc, argv);
	
	int mru_condition=parameterAnalysis_mru(curr_para, argc, argv);
	if (mru_condition > 0)
	{
		//3:m�ķ�Χ���ԣ�4:r�ķ�Χ���ԣ�5:û��n����
		curr_para.clear();
		clearState();
		return mru_condition + 2;
	}

	if (cn_condition == 0)
	{
		//ȱ�ٱ�Ҫ����
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
