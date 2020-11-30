/////////////////////////////////////////////
//读写CSV文件类 csvRW
//可在构造类时指定文件路径；.read方法读取CSV文件至.data变量中；.write方法在文件末尾写入一条数据
//返回值true操作成功，false操作失败；isreaded表示是否已读取数据到.data变量中
//////////////////////
#include "csvRW.h"

using namespace std;

int main()
{

	csvRW ToRead;
	if (ToRead.read(".//248YS5NA0A--D.csv"))
	{
		for (int i = 0; i < ToRead.data.size(); i++)
		{
			cout << i+1 << "\t";
			cout << ToRead.data[i].Symbol << "\t";
			cout << ToRead.data[i].number << "\t";
			cout << ToRead.data[i].X << "\t";
			cout << ToRead.data[i].Y << "\t";
			cout << ToRead.data[i].Theta << "\t";
			cout << endl;
		}
	}
	else
	{
		cout << "读取csv文件失败" << endl;
	}

	Position temp("ASD5", "ADDF12ADFA1", 200, 30, 45);
	if (ToRead.write(".//248YS5NA0A--D.csv", temp))
	{
		cout << "写入csv文件成功" << endl;
	}
	else
	{
		cout << "写入csv文件失败" << endl;
	}

	if (ToRead.read(".//248YS5NA0A--D.csv"))
	{
		for (int i = 0; i < ToRead.data.size(); i++)
		{
			cout << i + 1 << "\t";
			cout << ToRead.data[i].Symbol << "\t";
			cout << ToRead.data[i].number << "\t";
			cout << ToRead.data[i].X << "\t";
			cout << ToRead.data[i].Y << "\t";
			cout << ToRead.data[i].Theta << "\t";
			cout << endl;
		}
	}
	else
	{
		cout << "读取csv文件失败" << endl;
	}

	return 0;
}