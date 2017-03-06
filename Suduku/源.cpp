#include <iostream>
#include <vector>
#include <cstdio>
#include <fstream>
#include <string>
using namespace std;
bool has_find_a_solution = false;
int number_of_possible = 0;
const int None = 0;
const int Success = -1;
const int Fail = 0;
const int DoNothing = -1;
const int DoThing = 0;
enum State { Lock, Unlock };

struct Cell
{
	bool possibility[10];
	enum State state;
	int value;
	int number_of_possibility();
	void impossible(int value);
};
struct Location_value
{
	int i;
	int j;
	int value;
	Location_value(int, int, int);
};
Location_value::Location_value(int tempi = 0, int tempj = 0, int tempvalue = None)
{
	i = tempi;
	j = tempj;
	value = tempvalue;
}
inline void Cell::impossible(int value)
{
	possibility[value] = false;
}

int Cell::number_of_possibility()
{
	if (state == Lock)
		return 0;

	int k = 0;
	for (int i = 1; i <= 9; i++)
		if (possibility[i] == true)
			k++;
	return k;
}

class Suduku_square
{
public:
	void fillin_delete(struct Location_value);
	struct Cell cell[10][10];
	int number_of_way();
	void line_delete(struct Location_value);
	void list_delete(struct Location_value);
	void block_delete(struct Location_value);
	Suduku_square();
	int number_of_unlock();
	int auto_fillin_delete_once();
	void print();
	struct Location_value One_possible_way();
	void first_fill();
	void FileFill(ifstream&);
	void FilePrint(ofstream&);
	bool isValid();
};

bool Suduku_square::isValid() {
	//row
	for (int i = 1; i <= 9; ++i) {
		bool occur[10]{};
		for (int j = 1; j <= 9; ++j) {
			if (cell[i][j].value != 0 && occur[cell[i][j].value])
				return false;
			else
				occur[cell[i][j].value] = true;
		}
	}

	//column
	for (int i = 1; i <= 9; ++i) {
		bool occur[10]{};
		for (int j = 1; j <= 9; ++j) {
			if (cell[j][i].value != 0 && occur[cell[j][i].value])
				return false;
			else
				occur[cell[j][i].value] = true;
		}
	}
	int order[] = { 1,4,7 };
	for (int x : order)
		for (int y : order) {
			bool occur[10]{};
			for (int i = x; i <= x + 2; ++i) {
				for (int j = y; j <= y + 2; ++j) {
					if (cell[i][j].value != 0 && occur[cell[i][j].value])
						return false;
					else
						occur[cell[i][j].value] = true;
				}
			}

		}
}
Suduku_square::Suduku_square()
{
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			for (int k = 1; k <= 9; k++)
			{
				cell[i][j].possibility[k] = true;
				cell[i][j].state = Unlock;
				cell[i][j].value = None;
			}
}
void Suduku_square::line_delete(struct Location_value a)
{
	int i = a.i;
	int value = a.value;
	for (int j = 1; j <= 9; j++)
		cell[i][j].impossible(value);
}
int Suduku_square::auto_fillin_delete_once()
{
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			if (cell[i][j].number_of_possibility() == 1)
				for (int k = 1; k <= 9; k++)
					if (cell[i][j].possibility[k] == true)
					{
						struct Location_value location_value = { i,j,k };
						fillin_delete(location_value);
						return DoThing;
					}
	return DoNothing;
}

void Suduku_square::fillin_delete(struct Location_value a)//include delete
{
	int i = a.i;
	int j = a.j;
	int value = a.value;
	cell[i][j].value = value;
	cell[i][j].state = Lock;
	line_delete(a);
	list_delete(a);
	block_delete(a);
}
void Suduku_square::list_delete(struct Location_value a)
{
	int j = a.j;
	int value = a.value;
	for (int i = 1; i <= 9; i++)
		cell[i][j].impossible(value);
}
void Suduku_square::block_delete(struct Location_value a)
{

	int tempi = ((a.i - 1) / 3) * 3 + 1;
	int tempj = ((a.j - 1) / 3) * 3 + 1;
	int value = a.value;
	for (int i = tempi; i <= tempi + 2; i++)
		for (int j = tempj; j <= tempj + 2; j++)
			cell[i][j].impossible(value);
}
int Suduku_square::number_of_unlock()
{
	int count = 0;
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			if (cell[i][j].state == Unlock)
				count++;
	return count;
}

int Suduku_square::number_of_way()
{
	if (number_of_unlock() == 0)
		return Success;

	int count = 9;
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			if (cell[i][j].state == Unlock)
				if (cell[i][j].number_of_possibility()<count)
					count = cell[i][j].number_of_possibility();
	if (count == Fail)
		return Fail;
	return count;
}
void Suduku_square::print()
{
	cout << "\n\n\n";
	cout << "这是第" << number_of_possible << "个可能" << "\n";
	for (int i = 1; i <= 9; i++)
	{
		cout << "\t\t";
		for (int j = 1; j <= 9; j++)
			cout << cell[i][j].value << "  ";
		cout << '\n';
	}
}

void Suduku_square::FilePrint(ofstream& os) {
	for (int i = 1; i <= 9; ++i) {
		for (int j = 1; j <= 9; ++j) {
			os << cell[i][j].value << endl;
		}
	}
}
struct Location_value Suduku_square::One_possible_way()
{
	int limit = 2;
	do
	{
		for (int i = 1; i <= 9; i++)
			for (int j = 1; j <= 9; j++)
				if (cell[i][j].number_of_possibility() <= limit)
					if (cell[i][j].state == Unlock)
						for (int k = 1; k <= 9; k++)
							if (cell[i][j].possibility[k] == true)
							{
								struct Location_value location_value = { i,j,k };
								return location_value;
							}
		limit++;
		if (limit >= 10)
			cout << "error 02";
	} while (true);
	cout << "error 01";
}
void Suduku_square::first_fill()//start here
{
//
//	struct Location_value a(1, 1, 8);
//	struct Location_value b(2, 3, 3);
//	struct Location_value c(2, 4, 6);
//	struct Location_value d(3, 2, 7);
//	struct Location_value e(3, 5, 9);
//	struct Location_value f(3, 7, 2);
//	struct Location_value g(4, 2, 5);
//	struct Location_value h(4, 6, 7);
//	struct Location_value i(5, 5, 4);
//	struct Location_value j(5, 6, 5);
//	struct Location_value k(5, 7, 7);
//	struct Location_value l(6, 4, 1);
//	struct Location_value m(6, 8, 3);
//	struct Location_value n(7, 3, 1);
//	struct Location_value o(7, 8, 6);
//	struct Location_value p(7, 9, 8);
//	struct Location_value q(8, 3, 8);
//	struct Location_value r(8, 4, 5);
//	struct Location_value s(8, 8, 1);
//	struct Location_value t(9, 2, 9);
//	struct Location_value u(9, 7, 4);
//
//	fillin_delete(a);
//	fillin_delete(b);
//	fillin_delete(c);
//	fillin_delete(d);
//	fillin_delete(e);
//	fillin_delete(f);
//	fillin_delete(g);
//	fillin_delete(h);
//	fillin_delete(i);
//	fillin_delete(j);
//	fillin_delete(k);
//	fillin_delete(l);
//	fillin_delete(m);
//	fillin_delete(n);
//	fillin_delete(o);
//	fillin_delete(p);
//	fillin_delete(q);
//	fillin_delete(r);
//	fillin_delete(s);
//	fillin_delete(t);
//	fillin_delete(u);
//
}

void Suduku_square::FileFill(ifstream& is) {
	vector<Location_value> info;
	int x, y, z;
	while (is >> x >> y >> z) {
		info.emplace_back(x, y, z);
	}
	for (auto it : info) {
		fillin_delete(it);
	}
}
class Suduku_square action(class Suduku_square &suduku_square)
{
	int ways;
	do
	{
		//if (has_find_a_solution) break;
		ways = suduku_square.number_of_way();
		if (ways == Success)
		{
			//ofstream ofile("D:\\SudukuCppOutput.txt");
			number_of_possible++;
			//suduku_square.FilePrint(ofile);
			//has_find_a_solution = true;
			suduku_square.print();

			//return suduku_square; //change
		}

		if (ways == Fail)
		{
			return suduku_square;
		}


		if (ways == 1)
		{
			suduku_square.auto_fillin_delete_once();
		}

		if (ways>1)
		{
			struct Location_value location_value = suduku_square.One_possible_way();
			struct Suduku_square new_suduku_quare = suduku_square;
			new_suduku_quare.fillin_delete(location_value);
			action(new_suduku_quare);

			int i = location_value.i;
			int j = location_value.j;
			int value = location_value.value;
			suduku_square.cell[i][j].impossible(value);
		}
	} while (!has_find_a_solution);
}


struct Location_value analyze(string a)
{
	if (a[0] != '(')
	{
		cout << "\t错误，请重新输入\n\t";
		return Location_value(0, 0, 0);
	}
	if (a[2] != ',')
	{
		cout << "\t错误，请重新输入\n\t";
		return Location_value(0, 0, 0);
	}

	if (a[4] != ',')
	{
		cout << "\t错误，请重新输入\n\t";
		return Location_value(0, 0, 0);
	}
	if (a[6] != ')')
	{
		cout << "\t错误，请重新输入\n\t";
		return Location_value(0, 0, 0);
	}
	cout << "\t输入成功\n\t";
	int i = a[1] - '0';
	int j = a[3] - '0';
	int value = a[5] - '0';
	return Location_value(i, j, value);
}
void input(class Suduku_square& suduku_square)
{
	cout << "\t请输入(a,b,k)以在a行b列填入数字k。务必英文状态输入\n\t输入ok以结束\n\t";
	string a;
	do
	{
		getline(cin, a);
		if (a == "ok")
			break;

		Location_value temp_location_value = analyze(a);
		if (temp_location_value.i == 0)
			if (temp_location_value.j == 0)
				if (temp_location_value.value == 0)
					continue;

		suduku_square.fillin_delete(temp_location_value);
	} while (true);

}
int main()
{
	class Suduku_square suduku_square;
	//suduku_square.first_fill();//用电脑填题目
	input(suduku_square);
	//ifstream ifile("D:\\SudukuVbOutput.txt");
	//ofstream ofile("D:\\SudukuCppOutput.txt");
	//suduku_square.FileFill(ifile);
	//if (!suduku_square.isValid()) {
	//	ofstream CreateAFileToIndecateNoAnswer("noAnswer.txt");
	//} else {
	//	action(suduku_square);
	//	//system("del D:\\SudukuVbOutput.txt");//用作和vb文件通讯时使用
	//}
	//ofstream JustCreateAFile("hasFinished.txt");
	action(suduku_square);
	return 0;
}
