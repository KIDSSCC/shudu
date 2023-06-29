#include "pch.h"
#include "../Pair_Programming/sudoku.cpp"
#include "../Pair_Programming/check.cpp"
#include <string>
#include<iostream>

using namespace std;
TEST(sudoku, A_EndGame) {
	//�������̲���
	int argc = 2;
	auto argv = new char* [argc];
	argv[0] = "-c";
	argv[1] = "1000";
	int result = begin(argc, argv);
	int checkresult = ifAllZhongpan("final.txt");
	EXPECT_EQ(result, 0);
	EXPECT_EQ(checkresult, 1);
}


TEST(sudoku, B_DefaultStartGame) {
	//����Ĭ�ϳ��̲���
	int argc = 2;
	auto argv = new char* [argc];
	argv[0] = "-n";
	argv[1] = "1000";
	int result = begin(argc, argv);
	int checkresult = checkStartGame(1);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(checkresult, 1);
}



TEST(sudoku, C_StartGame_Level) {
	//�����Ѷȵȼ����̲���
	int argc = 4;
	auto argv = new char* [argc];
	argv[0] = "-n";
	argv[1] = "1000";
	argv[2] = "-m";
	argv[3] = "1";
	int result = begin(argc, argv);
	int checkresult = checkStartGame(1);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(checkresult, 1);

	//�Ѷ�2
	argv[3] = "2";
	result = begin(argc, argv);
	checkresult = checkStartGame(2);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(checkresult, 1);

	//�Ѷ�3
	argv[3] = "3";
	result = begin(argc, argv);
	checkresult = checkStartGame(3);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(checkresult, 1);
}

TEST(sudoku, D_StartGame_Range) {
	//����ָ����Χ���̲���
	int argc = 4;
	auto argv = new char* [argc];
	argv[0] = "-n";
	argv[1] = "1000";
	argv[2] = "-r";
	argv[3] = "25~40 ";
	int result = begin(argc, argv);
	int checkresult = checkStartGame(-1);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(checkresult, 1);
}


TEST(sudoku, E_StartGame_Unique) {
	//����ָ����Χ���̲���
	int argc = 3;
	auto argv = new char* [argc];
	argv[0] = "-n";
	argv[1] = "1000";
	argv[2] = "-u";
	int result = begin(argc, argv);
	int checkresult = checkStartGame(1);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(checkresult, 1);
}


TEST(sudoku, F_Solve_Game) {
	//�������
	int argc = 2;
	auto argv = new char* [argc];
	argv[0] = "-s";
	argv[1] = "game.txt";
	int result = begin(argc, argv);
	int checkresult = ifAllZhongpan("sudoku.txt");
	EXPECT_EQ(result, 0);
	EXPECT_EQ(checkresult, 1);
}

//�쳣�������
TEST(sudoku, G_WithoutArgs) {
	//��������
	int argc = 1;
	auto argv = new char* [argc];
	argv[0] = "-n";
	int result = begin(argc, argv);
	EXPECT_EQ(result, 1);
}

TEST(sudoku, H_SlovePathError) {
	//���·��������
	int argc = 2;
	auto argv = new char* [argc];
	argv[0] = "-s";
	argv[1] = "something.txt";
	int result = begin(argc, argv);
	EXPECT_EQ(result, 2);
}

TEST(sudoku, I_M_levelError) {
	//M������ֵ����
	int argc = 4;
	auto argv = new char* [argc];
	argv[0] = "-n";
	argv[1] = "1000";
	argv[2] = "-m";
	argv[3] = "5";
	int result = begin(argc, argv);
	EXPECT_EQ(result, 3);
}

TEST(sudoku, J_R_levelError) {
	//R������Χ����
	int argc = 4;
	auto argv = new char* [argc];
	argv[0] = "-n";
	argv[1] = "1000";
	argv[2] = "-r";
	argv[3] = "10~60";
	int result = begin(argc, argv);
	EXPECT_EQ(result, 4);
}

TEST(sudoku, K_ArgsDependencyError) {
	//��ز���ȱ������
	int argc = 2;
	auto argv = new char* [argc];
	argv[0] = "-m";
	argv[1] = "1";
	int result = begin(argc, argv);
	EXPECT_EQ(result, 5);

	//rȱ������
	argv[0] = "-r";
	argv[1] = "25~40";
	result = begin(argc, argv);
	EXPECT_EQ(result, 5);

	//uȱ������
	argv[0] = "-u";
	argv[1] = " ";
	result = begin(argc, argv);
	EXPECT_EQ(result, 5);
}

TEST(sudoku, L_NecessaryArgsError) {
	//��ز���ȱ������
	int argc = 4;
	auto argv = new char* [argc];
	argv[0] = "-a";
	argv[1] = "1";
	argv[2] = "-b";
	argv[3] = "2";
	int result = begin(argc, argv);
	EXPECT_EQ(result, 6);
}