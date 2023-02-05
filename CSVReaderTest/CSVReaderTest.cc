﻿#include <gtest/gtest.h>
#include "../CSVReader/CSVReader.h"
#include "../CSVReader/Functions.cpp"

// Тестирование функции ReadLine() на корректном примере.
TEST(CSVReader, ReadLineCorrect) 
{
	vector<string> row_t;
	string line = "1";

	for (size_t i = 0; i < 10; i++)
	{
		row_t.push_back(to_string(i));
		line += "," + to_string(i);
	}

	string row_number;
	vector<string> row;

	ReadLine(line, row, row_number);

	EXPECT_EQ(row_number, "1");
	EXPECT_EQ(row.size(), row_t.size());
	for (size_t i = 0; i < row.size(); i++)
	{
		EXPECT_EQ(row[i], row_t[i]);
	}
}

// Тестирование функции ReadLine() на НЕкорректном примере.
TEST(CSVReader, ReadLineUncorrect)
{
	vector<string> row_t;
	string line = "A";

	for (size_t i = 0; i < 10; i++)
	{
		row_t.push_back(to_string(i));
		line += "," + to_string(i);
	}

	string row_number;
	vector<string> row;

	try 
	{
		ReadLine(line, row, row_number);
	}
	catch (runtime_error)
	{

	}
	EXPECT_EQ(row_number, "A");
	EXPECT_EQ(row.size(), row_t.size());
	for (size_t i = 0; i < row.size(); i++)
	{
		EXPECT_EQ(row[i], row_t[i]);
	}
}

// Тестирование функции Order() на корректном примере.
TEST(CSVReader, OrderCorrect)
{
	string cell_value = "=A2+B3";

	char op = '+';
	string arg1 = "2";
	string arg2 = "3";
	string op_res = Order(op, arg1, arg2, cell_value);
	EXPECT_EQ(op_res, to_string(5.));

	op = '-';
	arg1 = "2";
	arg2 = "3";
	op_res = Order(op, arg1, arg2, cell_value);
	EXPECT_EQ(op_res, to_string(-1.));

	op = '*';
	arg1 = "2";
	arg2 = "3";
	op_res = Order(op, arg1, arg2, cell_value);
	EXPECT_EQ(op_res, to_string(6.));

	op = '/';
	arg1 = "2";
	arg2 = "2";
	op_res = Order(op, arg1, arg2, cell_value);
	EXPECT_EQ(op_res, to_string(1.));
}

// Тестирование функции Order() на НЕкорректном примере.
TEST(CSVReader, OrderUncorrect)
{
	string cell_value = "=A2+B3";

	char op = '+';
	string arg1 = "2A";
	string arg2 = "3";
	string op_res = Order(op, arg1, arg2, cell_value);
	EXPECT_EQ(op_res, cell_value);

	op = '-';
	arg1 = "2";
	arg2 = "3D";
	op_res = Order(op, arg1, arg2, cell_value);
	EXPECT_EQ(op_res, cell_value);

	op = '*';
	arg1 = "2D";
	arg2 = "3A";
	op_res = Order(op, arg1, arg2, cell_value);
	EXPECT_EQ(op_res, cell_value);

	op = '/';
	arg1 = "D2A";
	arg2 = "C2e";
	op_res = Order(op, arg1, arg2, cell_value);
	EXPECT_EQ(op_res, cell_value);
}