#include <QtCore/QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QString>

#include "database_aoi.h"
#include "csvRW.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	
	// �������ݿ����
	QString connection_name = "connect1";	// ���ݿ��������ƣ��Զ��壬�������֣�
	QString host_IP = "127.0.0.1";			// Ŀ�����ݿ���������IP
	QString user_name = "root";				// ��¼Ŀ�����ݿ�ʱ���û���
	QString passward = "yjm666";		    // ��¼����
	QString database_name = "pcbe_stl";			// Ŀ�����ݿ�����
	JS_DATABASE aoi_db(connection_name, host_IP, user_name, passward, database_name);
	

	//���ݿ����������ֵ
	bool isOK;

	QString errorType= "000000000000000000000000000";

	// �������ݿ�����
	if (aoi_db.connect())
		qDebug() << "connect database" << database_name << "successfully!";

	// ����һ������
	//DETECT_INFO_INSERT info1;
	//info1.board_name = "board3";
	//info1.ele_name = "pc1";
	//info1.ele_index = "PAR5";
	//info1.timestamp = JS_DATABASE::get_timestamp_now();
	//info1.is_ng = true;
	//info1.ng_type = "101011000000000000000000";
	//info1.ng_trigger = "000000000000000000000000";
	//info1.pic_path = "D:\\\\test\\\\pic\\\\test.bmp";
	//info1.loc_x = 10;
	//info1.loc_y = 10;
	//info1.width = 100;
	//info1.height = 100;
	//std::vector<QString> vec_values;
	//vec_values.push_back("0.99");
	//vec_values.push_back("srs");
	//vec_values.push_back("0.87");
	//vec_values.push_back("0.95");
	//info1.vec_values = vec_values;
	//isOK = aoi_db.insert_detect_result(info1);
	//if (isOK)
	//	qDebug() << "insert into database success!";

	// ��ѯһ������
	DETECT_INFO_REQUIRE result;
	isOK = aoi_db.inquire_detect_result("2020-09-26 15:49:49", "board3", "PAR5", result);
	if (isOK)
		qDebug() << "inquire from database success!";

	qDebug() << "finish!";

	//����Ԫ������ѯҪ����ȱ��
	QString eleName = "pc1";
	std::vector<QString> subTables;

	subTables = aoi_db.findDefectsToDetect(eleName);
	int ismatch = aoi_db.usersLogin("Lisa", "456");
	int time = aoi_db.timeCost();

	qDebug() << "isRename = " << ismatch << endl;

	qDebug() << "subTables.size() = " << subTables.size() << endl;

	for (int i = 0; i < subTables.size(); ++i)
	{
		qDebug() << "subTables[" << i << "] = " << subTables[i] ;
	}

	//��ȡcsv�ļ�
	csvRW ToRead;
	if (ToRead.read("test.csv"))
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
	
	/*************************************saveToDB()��������***********************************/
	SMTInfo* boardInfo = new SMTInfo();
	boardInfo->smt_name = "TestBoard1127";
	boardInfo->smt_code = "test1127";
	boardInfo->smt_length = 1235;
	boardInfo->smt_width = 165;
	boardInfo->smt_height = 4571;
	boardInfo->smt_angle = 6471;
	boardInfo->error_types = "1";
	boardInfo->img_path = "D:\\\\test\\\\pic\\\\test.bmp";

	ErrorInfo* ero = new ErrorInfo();
	boardInfo->ero = &ero;
	boardInfo->ero[0]->Reset_this();//��ʼ������
	boardInfo->ero[0]->amount = 5;//��һ��ȱ�ݷֱ�(ero[0])��5������
	boardInfo->ero[0]->error_num_type = {0, 0, 1, 3, 3};
	boardInfo->ero[0]->error_team_num = { 0, 1, 0, 0, 1 };
	boardInfo->ero[0]->name = QString::fromLocal8Bit("�缫ĩ��ͻ��");
	boardInfo->ero[0]->type0_factor = {50, 70};
	boardInfo->ero[0]->type1_factor = { 32.7 };
	boardInfo->ero[0]->type2_factor = { "upper, boundary" };
	boardInfo->ero[0]->type3_factor = { "x, y",  "hello, world!"};

	QHash<QString, QString> hashList;
	hashList.insert(QString::fromLocal8Bit("�缫ĩ��ͻ��"), "1_op_ending_extrude");

	bool insertData = aoi_db.saveToDB("testBoard", boardInfo, hashList);
	cout << "insertData = " << insertData << endl;
	delete boardInfo;
	delete ero;

	/*************************************loadFromDB()��������***********************************/
	//SMTInfo* loadInfo = new SMTInfo();
	//QHash<QString, QString> hashList;
	//hashList.insert(QString::fromLocal8Bit("�缫ĩ��ͻ��"), "1_op_ending_extrude");
	//bool loadData = aoi_db.loadFromDB("test", loadInfo, hashList);

    return a.exec();
}
