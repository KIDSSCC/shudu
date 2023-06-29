#include "pch.h"
#include "../Pair_Programming/sudoku.cpp"
#include "../Pair_Programming/check.cpp"
#include <string>
#include<iostream>

using namespace std;
TEST(sudoku, A_EndGame) {
	//创建终盘测试
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
	//创建默认初盘测试
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
	//创建难度等级初盘测试
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

	//难度2
	argv[3] = "2";
	result = begin(argc, argv);
	checkresult = checkStartGame(2);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(checkresult, 1);

	//难度3
	argv[3] = "3";
	result = begin(argc, argv);
	checkresult = checkStartGame(3);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(checkresult, 1);
}

TEST(sudoku, D_StartGame_Range) {
	//创建指定范围初盘测试
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
	//创建指定范围初盘测试
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
	//求解数独
	int argc = 2;
	auto argv = new char* [argc];
	argv[0] = "-s";
	argv[1] = "game.txt";
	int result = begin(argc, argv);
	int checkresult = ifAllZhongpan("sudoku.txt");
	EXPECT_EQ(result, 0);
	EXPECT_EQ(checkresult, 1);
}

//异常情况处理
TEST(sudoku, G_WithoutArgs) {
	//参数过少
	int argc = 1;
	auto argv = new char* [argc];
	argv[0] = "-n";
	int result = begin(argc, argv);
	EXPECT_EQ(result, 1);
}

TEST(sudoku, H_SlovePathError) {
	//求解路径不存在
	int argc = 2;
	auto argv = new char* [argc];
	argv[0] = "-s";
	argv[1] = "something.txt";
	int result = begin(argc, argv);
	EXPECT_EQ(result, 2);
}

TEST(sudoku, I_M_levelError) {
	//M参数数值错误
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
	//R参数范围错误
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
	//相关参数缺少依赖
	int argc = 2;
	auto argv = new char* [argc];
	argv[0] = "-m";
	argv[1] = "1";
	int result = begin(argc, argv);
	EXPECT_EQ(result, 5);

	//r缺少依赖
	argv[0] = "-r";
	argv[1] = "25~40";
	result = begin(argc, argv);
	EXPECT_EQ(result, 5);

	//u缺少依赖
	argv[0] = "-u";
	argv[1] = " ";
	result = begin(argc, argv);
	EXPECT_EQ(result, 5);
}

TEST(sudoku, L_NecessaryArgsError) {
	//相关参数缺少依赖
	int argc = 4;
	auto argv = new char* [argc];
	argv[0] = "-a";
	argv[1] = "1";
	argv[2] = "-b";
	argv[3] = "2";
	int result = begin(argc, argv);
	EXPECT_EQ(result, 6);
}