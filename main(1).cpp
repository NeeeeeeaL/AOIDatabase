/////////////////////////////////////////////
//��дCSV�ļ��� csvRW
//���ڹ�����ʱָ���ļ�·����.read������ȡCSV�ļ���.data�����У�.write�������ļ�ĩβд��һ������
//����ֵtrue�����ɹ���false����ʧ�ܣ�isreaded��ʾ�Ƿ��Ѷ�ȡ���ݵ�.data������
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
		cout << "��ȡcsv�ļ�ʧ��" << endl;
	}

	Position temp("ASD5", "ADDF12ADFA1", 200, 30, 45);
	if (ToRead.write(".//248YS5NA0A--D.csv", temp))
	{
		cout << "д��csv�ļ��ɹ�" << endl;
	}
	else
	{
		cout << "д��csv�ļ�ʧ��" << endl;
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
		cout << "��ȡcsv�ļ�ʧ��" << endl;
	}

	return 0;
}