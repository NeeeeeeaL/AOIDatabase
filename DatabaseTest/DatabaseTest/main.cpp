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
	
	// 创建数据库对象
	QString connection_name = "connect1";	// 数据库连接名称（自定义，用于区分）
	QString host_IP = "127.0.0.1";			// 目标数据库所属主机IP
	QString user_name = "root";				// 登录目标数据库时的用户名
	QString passward = "yjm666";		    // 登录密码
	QString database_name = "pcbe_stl";			// 目标数据库名称
	JS_DATABASE aoi_db(connection_name, host_IP, user_name, passward, database_name);
	

	//数据库插入结果返回值
	bool isOK;

	QString errorType= "000000000000000000000000000";

	// 建立数据库连接
	if (aoi_db.connect())
		qDebug() << "connect database" << database_name << "successfully!";

	// 插入一条数据
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

	// 查询一条数据
	DETECT_INFO_REQUIRE result;
	isOK = aoi_db.inquire_detect_result("2020-09-26 15:49:49", "board3", "PAR5", result);
	if (isOK)
		qDebug() << "inquire from database success!";

	qDebug() << "finish!";

	//根据元件名查询要检查的缺陷
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

	//读取csv文件
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
		cout << "读取csv文件失败" << endl;
	}
	
	/*************************************saveToDB()函数测试***********************************/
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
	boardInfo->ero[0]->Reset_this();//初始化重置
	boardInfo->ero[0]->amount = 5;//第一张缺陷分表(ero[0])有5个参数
	boardInfo->ero[0]->error_num_type = {0, 0, 1, 3, 3};
	boardInfo->ero[0]->error_team_num = { 0, 1, 0, 0, 1 };
	boardInfo->ero[0]->name = QString::fromLocal8Bit("电极末端突出");
	boardInfo->ero[0]->type0_factor = {50, 70};
	boardInfo->ero[0]->type1_factor = { 32.7 };
	boardInfo->ero[0]->type2_factor = { "upper, boundary" };
	boardInfo->ero[0]->type3_factor = { "x, y",  "hello, world!"};

	QHash<QString, QString> hashList;
	hashList.insert(QString::fromLocal8Bit("电极末端突出"), "1_op_ending_extrude");

	bool insertData = aoi_db.saveToDB("testBoard", boardInfo, hashList);
	cout << "insertData = " << insertData << endl;
	delete boardInfo;
	delete ero;

	/*************************************loadFromDB()函数测试***********************************/
	//SMTInfo* loadInfo = new SMTInfo();
	//QHash<QString, QString> hashList;
	//hashList.insert(QString::fromLocal8Bit("电极末端突出"), "1_op_ending_extrude");
	//bool loadData = aoi_db.loadFromDB("test", loadInfo, hashList);

    return a.exec();
}
