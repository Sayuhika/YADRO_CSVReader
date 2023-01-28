#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

extern vector<string> *exceptions_collector;

// ������� ������ ������
int ReadLine(string line, vector<string>& row, string& r_n)
{
	stringstream str(line);
	string value;

	getline(str, value, ',');
	int row_number = -1;
	try
	{
		row_number = stoi(value);
	}
	catch (const exception&)
	{
		r_n = value;
	}

	while (getline(str, value, ','))
	{
		row.push_back(value);
	}

	return row_number;
}

// ������� ������ �������� ������
bool FindCellIndexes(int &arg_r, string &arg_c, vector<int> row_numbers, vector<string> column_headers, 
	size_t &index_r, size_t&index_c, string &cell_value)
{
	for (size_t i = 0; i < row_numbers.size(); i++) 
	{
		index_r = -1;

		if (arg_r == row_numbers[i]) {
			index_r = i;
			break;
		}
	}
	// �������
	for (size_t i = 0; i < column_headers.size(); i++) 
	{
		index_c = -1;

		if (arg_c == column_headers[i]) {
			index_c = i;
			break;
		}
	}
	if ((index_c == -1) || (index_r == -1)) 
	{
		string ex_mes = ("��������� ��������� ��������� (" + cell_value + ") �� �������:");
		if (index_r == -1) ex_mes += ("\n������ � ������� (" + to_string(arg_r) + ") �� �������.");
		if (index_c == -1) ex_mes += ("\n������� � ������ (" + arg_c + ") �� �������.");
		exceptions_collector->push_back(ex_mes);
		return false;
	}
	return true;
}

// ������� �������������� �������� ����� ����� �����������
string Order(char op, string arg1, string arg2, string& cell_value)
{
	double arg1_i, arg2_i;

	try {
		arg1_i = stod(arg1);
		arg2_i = stod(arg2);
	}
	catch (const exception&)
	{
		exceptions_collector->push_back(
			"��������� ��������� ��������� (" + cell_value + ") �� �������:" +
			"\n���� �� ���������� �� �������� ������." +
			"\n����� ��������:  " + arg1 +
			"\n������ ��������: " + arg2);

		return (arg1 + arg1);
	};

	switch (op) {
	case '+':
		return to_string(arg1_i + arg2_i);
	case '-':
		return to_string(arg1_i - arg2_i);
	case '*':
		return to_string(arg1_i * arg2_i);
	case '/':
	{
		if (arg2_i == 0)
			return "NaN";
		else 
			return to_string(arg1_i / arg2_i);
	}
	default:
		return (arg1 + arg1);
	}
}

// ������� ���������� �������� � ������
string GetResultAtCell(int c_n, int r_n, vector<string>& column_headers, vector<int>& row_numbers, 
	vector<vector<string>>& table, vector<vector<bool>>& map)
{
	string cell_value = table[r_n][c_n];

	if (cell_value[0] == '=')
	{
		// ����� ���������
		char op_chr = '\0';
		size_t op_pos = cell_value.find_first_of("+-*/", 1);
		if (op_pos != string::npos) op_chr = cell_value[op_pos];
		else {
			// ������ ������ ��������� "=A1"
			string arg_s;
			int arg_i, argr_i;
			size_t eos_i = cell_value.find_last_not_of(nums);
			arg_i = stoi(cell_value.substr(eos_i + 1, cell_value.size() - eos_i));
			arg_s = cell_value.substr(1, eos_i);

			size_t index_r, index_c;
			if (!FindCellIndexes(arg_i, arg_s, row_numbers, column_headers, index_r, index_c, cell_value))
			{
				map[r_n][c_n] = true;
				return table[r_n][c_n] = cell_value;
			}

			map[r_n][c_n] = true; 
			table[r_n][c_n] = cell_value = GetResultAtCell(index_c, index_r, column_headers, row_numbers, table, map);
			return cell_value;
		}

		// ������ ���� ���������� "=A1+A2"
		// ���� ��������� ���������� �� ������ // ������ // ========================================== //
		string arg1_s, arg2_s;
		int arg1_i, arg2_i;
		size_t eos_i = 0;

		// ������� ��� ������� � ����� ������ ������� ���������
		eos_i = cell_value.find_last_not_of(nums);
		arg2_i = stoi(cell_value.substr(eos_i + 1, cell_value.size() - eos_i));
		arg2_s = cell_value.substr(op_pos + 1, eos_i - op_pos);

		// ������� ��� ������� � ����� ������ ������ ���������
		eos_i = cell_value.find_last_not_of(nums, op_pos - 1);
		arg1_i = stoi(cell_value.substr(eos_i + 1, op_pos - eos_i));
		arg1_s = cell_value.substr(1, eos_i);

		// ���� ���������������� ������ ����� � �������
		// �������� 1
		size_t index1_r, index2_r, index1_c, index2_c;

		if (!FindCellIndexes(arg1_i, arg1_s, row_numbers, column_headers, index1_r, index1_c, cell_value))
		{
			map[r_n][c_n] = true;
			return table[r_n][c_n] = cell_value;
		}

		// �������� 2
		if (!FindCellIndexes(arg2_i, arg2_s, row_numbers, column_headers, index2_r, index2_c, cell_value)) 
		{
			map[r_n][c_n] = true;
			return table[r_n][c_n] = cell_value;
		}

		// ������ �������� � ������
		string g_arg1 = GetResultAtCell(index1_c, index1_r, column_headers, row_numbers, table, map);
		string g_arg2 = GetResultAtCell(index2_c, index2_r, column_headers, row_numbers, table, map);
		cell_value = Order(op_chr, g_arg1, g_arg2, cell_value);

		// ���� ��������� ���������� �� ������ // ����� // ========================================== //
	}

	map[r_n][c_n] = true;
	return table[r_n][c_n] = cell_value;
}
