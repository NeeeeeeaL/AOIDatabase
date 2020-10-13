#include <QtCore/QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>
#include <QString>

#include "database_aoi.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	
	// �������ݿ����
	QString connection_name = "connect1";	// ���ݿ��������ƣ��Զ��壬�������֣�
	QString host_IP = "127.0.0.1";			// Ŀ�����ݿ���������IP
	QString user_name = "root";				// ��¼Ŀ�����ݿ�ʱ���û���
	QString passward = "yjm666";		    // ��¼����
	QString database_name = "aoi";			// Ŀ�����ݿ�����
	JS_DATABASE aoi_db(connection_name, host_IP, user_name, passward, database_name);
	

	//���ݿ����������ֵ
	bool isOK;

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
	QString name = "pc1";

	QString defectsToDetect;
	defectsToDetect = aoi_db.findDefectsToDetect(name);

	qDebug() << "defectsToDetect = " << defectsToDetect << endl;

	//����Ҫ����ȱ�ݲ�ѯ�ֱ�
	std::vector<QString> subTables;
	subTables = aoi_db.findDefectsTables(defectsToDetect);

	qDebug() << "subTables.size() = " << subTables.size() << endl;

	for (int i = 0; i < subTables.size(); ++i)
	{
		qDebug() << "subTables[" << i << "] = " << subTables[i] ;
	}

    return a.exec();
}
