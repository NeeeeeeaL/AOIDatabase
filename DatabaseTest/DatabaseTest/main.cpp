#include <QtCore/QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QString>

#include "database_aoi.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	
	// 创建数据库对象
	QString connection_name = "connect1";	// 数据库连接名称（自定义，用于区分）
	QString host_IP = "127.0.0.1";			// 目标数据库所属主机IP
	QString user_name = "root";				// 登录目标数据库时的用户名
	QString passward = "yjm666";		    // 登录密码
	QString database_name = "aoi";			// 目标数据库名称
	JS_DATABASE aoi_db(connection_name, host_IP, user_name, passward, database_name);
	

	//数据库插入结果返回值
	bool isOK;

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
	QString name = "pc1";

	QString defectsToDetect;
	defectsToDetect = aoi_db.findDefectsToDetect(name);

	qDebug() << "defectsToDetect = " << defectsToDetect << endl;

	//根据要检查的缺陷查询分表
	std::vector<QString> subTables;
	subTables = aoi_db.findDefectsTables(defectsToDetect);

	qDebug() << "subTables.size() = " << subTables.size() << endl;

	for (int i = 0; i < subTables.size(); ++i)
	{
		qDebug() << "subTables[" << i << "] = " << subTables[i] ;
	}

    return a.exec();
}
