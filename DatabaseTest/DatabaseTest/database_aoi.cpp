#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlerror>
#include <QSqlQuery>
#include <QRegExp>
#include <QStringList>
#include <QDateTime>
#include <QTextCodec>
#include <QBitArray>
#include <assert.h> //���������ֹ����ִ�е�ͷ�ļ�

#include <stdio.h>
#include <time.h>
#include <ctime>

#include "database_aoi.h"

#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ << "; Func:" << __func__ <<"]"

// ���캯��
JS_DATABASE::JS_DATABASE(QString connection_name, QString host_IP, QString user_name, QString passward, QString database_name)
{
	// ������Ӷ�Ӧ����������map��ʼ��
	defect_index_map_init();

	// ������Ӷ�Ӧ���ֵ����map��ʼ��
	value_num_map_init();

	// ���ݿ�����������Ϣ��ʼ��
	this->host_IP = host_IP;
	this->user_name = user_name;
	this->passward = passward;
	this->database_name = database_name;

	// ���MYSQL���ݿ�����
	this->db = QSqlDatabase::addDatabase("QMYSQL", connection_name);
	// �鿴���ݿ������Ƿ���Ч
	this->is_driver_valid = this->db.isValid();
}


// ��������
JS_DATABASE::~JS_DATABASE()
{
	if (this->db.isOpen())
	{
		this->db.close();
	}
}

// �������ݿ�����
bool JS_DATABASE::connect()
{
	// �ж����ݿ������Ƿ���Ч
	if (!is_driver_valid)
	{
		qDebug() << "Connect failed, the QMYSQL driver is invalid!";
		this->is_connected = false;
		return false;
	}

	// ����������Ϣ
	this->db.setHostName(this->host_IP);
	this->db.setUserName(this->user_name);
	this->db.setPassword(this->passward);
	this->db.setDatabaseName(this->database_name);

	// ���Խ�������
	if (!this->db.open())
	{
		qDebug() << this->db.lastError().text();
		this->is_connected = false;
		return false;
	}

	this->is_connected = true;
	return true;
}


std::vector<QString> JS_DATABASE::findDefectsToDetect(QString ele_name)
{
	//1.��ѯԪ����Ҫ����ȱ������

	//�����ж����ݿ�����Ƿ��Ѿ�����
	assert(is_connected); //���ݿ�δ����ʱ��ֹ���� 

	//�������в�ѯ��Ԫ����Ӧ�ü���ȱ�����͵��ַ���
	QSqlQuery query(this->db);
	QString sql;

	sql = "select ng_type from `production_result` where `ele_name` = '";
	sql += ele_name;
	sql += "'";

	query.exec(sql);
	query.next();

	QString ngType = query.value("ng_type").toString();

	cout << "ngType = " << ngType << endl;

	//2.����Ҫ����ȱ�ݶ�λѡ��Ҫʹ�õķֱ�
	std::vector<QString> targetTables;

	for (int i = 0; i < ngType.size(); ++i)
	{
		if (ngType[i] == "1")
			targetTables.push_back(defect_index_map[i]);
	}

	return targetTables;
}


// ����������
bool JS_DATABASE::insert_detect_result(DETECT_INFO_INSERT& result_info)
{
	// �ж����ݿ�����Ƿ��Ѿ�����
	if (!is_connected)
	{
		qDebug() << "The database is disconnected! Please connect first!";
		return false;
	}

	// 1.ִ�в���ǰ������ֵ����Ч�Խ����ж�

	// 1.1 ng_type��ng_trigger�ĳ���Ӧ����mapһ��
	if (result_info.ng_type.size() != defect_index_map.size() || result_info.ng_trigger.size() != defect_index_map.size())
	{
		qDebug() << "Insert failed! The length of ng_type or ng_trigger is wrong!";
		return false;
	}

	// 1.2 ng_type��ng_triggerӦ��ֻ����0��1
	if (result_info.ng_type.contains(QRegExp("[^0,1]")) || result_info.ng_trigger.contains(QRegExp("[^0,1]")))
	{
		qDebug() << "Insert failed! The content of ng_type or ng_trigger should be only 0 or 1!";
		return false;
	}

	// 1.3 ���ʱ����ĸ�ʽ�Ƿ���ȷ
	QRegExp re_timestamp("\\s*\\d\\d\\d\\d[/-]?\\d\\d[/-]?\\d\\d[ ]*\\d\\d[:]?\\d\\d[:]?\\d\\d\\s*");
	if (re_timestamp.indexIn(result_info.timestamp) == -1)
	{
		qDebug() << "Insert failed! The format of timestamp is wrong!";
		return false;
	}

	// 1.4 ��������NG������洢���ֵ�����Ĵ�С�Ƿ�һ��
	int ng_num_needcheck = 0;
	std::vector<int> vec_ng_needcheck_index;
	for (int i = 0; i < result_info.ng_type.size(); i++)
	{
		if (result_info.ng_type[i] == "1")
		{
			ng_num_needcheck += 1;
			vec_ng_needcheck_index.push_back(i);
		}
	}
	if (ng_num_needcheck != result_info.vec_values.size())
	{
		qDebug() << "Insert failed! The num of NG that need check doesn't match the size of vec_values!";
		return false;
	}


	// 2.�ڱ�production_result�в����Ԫ���Ĳ��Խ����Ϣ
	QSqlQuery query(this->db);
	QString sql;
	bool isOK;

	// 2.1 ƴ��sql���
	sql = "insert into `production_result` values (default, ";
	sql += "'" + result_info.board_name + "', ";
	sql += "'" + result_info.ele_index + "', ";
	sql += "'" + result_info.timestamp + "', ";
	sql += "'" + result_info.ele_name + "', ";
	sql += result_info.is_ng ? "1, " : "0, ";
	sql += "'" + result_info.ng_type + "', ";
	sql += "'" + result_info.ng_trigger + "', ";
	sql += "'" + result_info.pic_path + "', "; 
	sql += QString::number(result_info.loc_x) + ", ";
	sql += QString::number(result_info.loc_y) + ", ";
	sql += QString::number(result_info.width) + ", ";
	sql += QString::number(result_info.height) + ")";

	// 2.2 ִ��sql������production_result�в���
	isOK = query.exec(sql);
	if (!isOK)
	{
		qDebug() << "Insert into `production_result` failed! The reason is" << this->db.lastError().text();
		return false;
	}

	// 3.����result_info.ng_type��Ϣ������Ԫ�������NG��Ϣ�������Ӧ�ı���

	// 3.1 ������ǰ�����ȡ���ִ���1��λ����Ϊ�����Ŵ�defect_index_map��ȡ��Ӧ������ı���
	std::vector<QString> vec_tablename;
	for (int i = 0; i < ng_num_needcheck; i++)
	{
		vec_tablename.push_back(defect_index_map[vec_ng_needcheck_index[i]]);
	}

	// 3.2 ���ɲ���ÿ�ű���Ҫ��sql���
	std::vector<QString> vec_sql;
	for (int i = 0; i < ng_num_needcheck; i++)
	{
		QString sql_tmp = "insert into `" + vec_tablename[i] + "` values (default, ";
		sql_tmp += "'" + result_info.board_name + "', ";
		sql_tmp += "'" + result_info.ele_index + "', ";
		sql_tmp += "'" + result_info.timestamp + "', ";
		sql_tmp += result_info.ng_trigger[vec_ng_needcheck_index[i]] == "1" ? "1, " : "0, ";

		// �жϸ�ȱ�ݵ������Ϣ�Ƿ�Ϊ���������ж����','�ָ������ֱ���ж������ַ�','ȷ���Ƿ�Ϊ�����ֵ��
		if (result_info.vec_values[i].contains(QRegExp("[,]")))
		{
			QStringList value_list = result_info.vec_values[i].split(",");
			for (int i = 0; i < value_list.size();i++)
			{
				if (i == value_list.size() - 1)
				{
					sql_tmp += "'" + value_list[i] + "') ";
					break;
				}
				sql_tmp += "'" + value_list[i] + "', ";
			}
			vec_sql.push_back(sql_tmp);
		}
		// ��������Ϣֻ��1��
		else
		{
			sql_tmp += "'" + result_info.vec_values[i] + "') ";
			vec_sql.push_back(sql_tmp);
		}
	}

	// 3.3ִ��sql��佫ÿ��ȱ�ݼ�����ӵ���ϸ����洢����Ӧ�ı���
	for (int i = 0; i < vec_sql.size(); i++)
	{
		// qDebug() << vec_sql[i];  // ������ɵ�ÿһ��sql��䣨�����ã�
		isOK = query.exec(vec_sql[i]);
		if (!isOK)
		{
			qDebug() << vec_sql[i] <<  "failed! The reason is" << this->db.lastError().text();
			return false;
		}
	}

	return true;
}

// ��ѯ����ʱ����£�ĳ����ĳ��Ԫ���������Ϣ
bool JS_DATABASE::inquire_detect_result(QString timestamp, QString board, QString ele_index, DETECT_INFO_REQUIRE& result)
{
	// �ж����ݿ�����Ƿ��Ѿ�����
	if (!is_connected)
	{
		qDebug() << "The database is disconnected! Please connect first!";
		return false;
	}

	// 1.���ʱ����ĸ�ʽ�Ƿ���ȷ
	QRegExp re_timestamp("\\s*\\d\\d\\d\\d[/-]?\\d\\d[/-]?\\d\\d[ ]*\\d\\d[:]?\\d\\d[:]?\\d\\d\\s*");
	if (re_timestamp.indexIn(timestamp) == -1)
	{
		qDebug() << "Insert failed! The format of timestamp is wrong!";
		return false;
	}

	// 2.�������в�ѯ��Ԫ���ļ����
	QSqlQuery query(this->db);
	QString sql;
	bool isOK;

	// 2.1 ƴ�Ӳ�ִ��sql���
	sql = "select * from `production_result` where ";
	sql += "timestamp = '" + timestamp + "' ";
	sql += "and board = '" + board + "' ";
	sql += "and ele_index = '" + ele_index + "'";
	isOK = query.exec(sql);
	if (!isOK)
	{
		qDebug() << "Inquire option on `production_result` execute failed! The reason is" << this->db.lastError().text();
		return false;
	}

	// 2.2 ��ȡ�����������д�������л�ȡ����Ϣ
	if (!query.next())
	{
		qDebug() << "The inquire result in `production_result` is NULL!";
		return false;
	}
	// ��ȡ���ݿ��е�ͼƬ·����QStringתΪstd::string��
	QString path = query.value("pic_path").toString();
	QTextCodec* code = QTextCodec::codecForName("gb18030");
	std::string std_path = code->fromUnicode(path).data();
	result.pic_path = std_path;
	result.ele_name = query.value("ele_name").toString();
	result.b_isNG = query.value("is_ng").toInt() == 1 ? 1 : 0;
	result.ng_trigger = query.value("ng_trigger").toString();
	result.loc_x = query.value("loc_x").toInt();
	result.loc_y = query.value("loc_y").toInt();
	result.width = query.value("width").toInt();
	result.height = query.value("height").toInt();


	// 3.���������в鵽��ng_type��Ϣ����һ���Ӹ��ֱ��в��Ҿ�������������Ϣ

	// 3.1 ��ȡԪ��Ӧ�ü������������ţ���һ����ȡӦ�ò�ѯ�ı����Լ���Ӧ�����value����
	QString ng_type = query.value("ng_type").toString();
	std::vector<int> vec_ng_needcheck;			// ԭ��Ӧ�ý��м�������������
	std::vector<QString> vec_table_needinquire;	// ԭ��Ӧ�ý��м������������Ŷ�Ӧ�ı���
	std::vector<int> vec_value_num;				// ԭ��Ӧ�ý��м������������Ŷ�Ӧ�����value����
	for (int i = 0; i < ng_type.size(); i++)	// ��������ng_type�ִ�
	{
		if (ng_type[i] == '1')
		{
			vec_ng_needcheck.push_back(i);
			vec_table_needinquire.push_back(defect_index_map[i]);
			vec_value_num.push_back(value_num_map[i]);
		}
	}
	
	// 3.2 �������������Ŷ�Ӧ�ı������ɲ�ִ�����ڲ�ѯ��ͬ���sql��䣬������Ӧ�������value�������������Ϣ�ĸ�ʽ
	for (int i = 0; i < vec_table_needinquire.size(); i++)
	{
		// ����ִ�зֱ��ѯ�����sql���
		sql = "select * from ";
		sql += "`" + vec_table_needinquire[i] + "` where ";
		sql += "timestamp = '" + timestamp + "' ";
		sql += "and board = '" + board + "' ";
		sql += "and ele_index = '" + ele_index + "'";
		isOK = query.exec(sql);

		// ��ѯ���ִ��ʧ�ܻ�鵽�Ľ����Ϊ�գ��������Ӷ�Ӧ�ֱ�������Ϣ�ÿ�""
		if (!isOK)
		{
			qDebug() << "Inquire option on `" + vec_table_needinquire[i] + "` execute failed! The reason is" << this->db.lastError().text();
			result.detect_value_map[vec_ng_needcheck[i]] = "";
			continue;
		}
		if (!query.next())
		{
			qDebug() << "The result inquired in `" + vec_table_needinquire[i] + "` is NULL!";
			result.detect_value_map[vec_ng_needcheck[i]] = "";
			continue;
		}

		// �ж�value���������ж������Ҫ�����value��Ϣ��","�ָ�ƴ�ӳ�һ���ִ����������Ϣ��
		if (vec_value_num[i] == 1)
		{
			result.detect_value_map[vec_ng_needcheck[i]] = query.value(5).toString();
		} 
		else
		{
			QString multi_values;
			for (int j = 0; j < vec_value_num[i]; j++)
			{
				multi_values += query.value(5 + j).toString();
				if (j != vec_value_num[i] - 1)
				{
					multi_values += ",";
				}
			}
			result.detect_value_map[vec_ng_needcheck[i]] = multi_values;
		}
	}

	return true;
}

// ���ָ�������ݱ�
bool JS_DATABASE::truncate_table(QString table_name)
{
	// �ж����ݿ�����Ƿ��Ѿ�����
	if (!is_connected)
	{
		qDebug() << "The database is disconnected! Please connect first!";
		return false;
	}

	// ִ��������ݱ�Ĳ���
	QSqlQuery query(this->db);
	QString sql = "truncate table `" + table_name + "`";
	bool isOK = query.exec(sql);
	if (!isOK)
	{
		qDebug() << "truncate_table `" << table_name << "` failed! The reason is" << this->db.lastError().text();
		return false;
	}
	return true;
}

//��ѯ�������ݿ����Ƿ����ض���
bool JS_DATABASE::findTable(QString tableNmae)
{
	//�����ж����ݿ�����Ƿ��Ѿ�����
	assert(is_connected); //���ݿ�δ����ʱ��ֹ���� 

	QSqlQuery query(this->db);
	QString sql;

	sql = "select count(*) from information_schema.tables where table_schema = 'aoi' and table_name = '";
	sql += tableNmae;
	sql += "'";

	query.exec(sql);
	query.next();

	if (query.value(0).toInt()) //��ѯ������1��δ��ѯ������0
		return true;
	else
		return false;
}


//�������ض���
bool JS_DATABASE::renameTable(QString originalName, QString newName)
{
	//�����ж����ݿ�����Ƿ��Ѿ�����
	assert(is_connected); //���ݿ�δ����ʱ��ֹ���� 

	//�ж��Ƿ��иñ�
	if (findTable(originalName))
	{
		QSqlQuery query(this->db);
		QString sql;

		sql = "rename table " + originalName + " to " + newName;

		if (query.exec(sql))
			return true;
		else
		{
			qDebug() << "Rename table failed! The reason is" << this->db.lastError().text();
			return false;
		}
	}
	else
	{
		qDebug() << "The table " << originalName << " does not exist!";
		return false;
	}

}

//ɾ���ض���
bool JS_DATABASE::dropTable(QString tableName)
{
	//�����ж����ݿ�����Ƿ��Ѿ�����
	assert(is_connected); //���ݿ�δ����ʱ��ֹ���� 

	//�ж��Ƿ��иñ�
	if (findTable(tableName))
	{
		QSqlQuery query(this->db);
		QString sql;

		sql = "drop table " + tableName;

		if (query.exec(sql))
			return true;
		else
		{
			qDebug() << "DropTable failed! The reason is" << this->db.lastError().text();
			return false;
		}
	}
	else
	{
		qDebug() << "The table " << tableName << " does not exist!";
		return false;
	}

	
}

//�����ض���
bool JS_DATABASE::createTable(QString tableName)
{
	//�����ж����ݿ�����Ƿ��Ѿ�����
	assert(is_connected); //���ݿ�δ����ʱ��ֹ���� 

	//���жϴ˱��Ƿ����
	if (!findTable(tableName))
	{
		QSqlQuery query(this->db);
		QString sql;

		sql = "create table " + tableName + " (id serial, content varchar(8000) not null,`desc` varchar(100) not null) engine innodb";

		if (query.exec(sql))
			return true;
		else
		{
			qDebug() << "createTable failed! The reason is" << this->db.lastError().text();
			return false;
		}
	}
	else
	{
		qDebug() << "The table " << tableName << " already exits!";
		return false;
	}

	
}

int JS_DATABASE::matchTableIndex(QString tableName, int colIndex, QString str)
{
	//�����ж����ݿ�����Ƿ��Ѿ�����
	assert(is_connected); //���ݿ�δ����ʱ��ֹ���� 
	
	//�ж��Ƿ��д����ݱ�
	if(findTable(tableName))
	{
		QString colName; //��Ų�ѯ�����ֶ���
		QSqlQuery query(this->db);
		QString sql1;
		QString sql2;

		//��ѯ�ֶ���
		sql1 = "select column_name from information_schema.columns where table_schema = 'aoi' and table_name = '";
		sql1 += tableName;
		sql1 += "' and ordinal_position = '";
		sql1 += QString::number(colIndex);
		sql1 += "'";

		if (query.exec(sql1))
		{
			if (query.next())
				colName = query.value(0).toString();
		}
		else
		{
			qDebug() << "Search colName failed! The reason is" << this->db.lastError().text();
			return false;
		}

		//��ѯ�ֶ�����û��str
		sql2 = "select count(*) from " + tableName + " where " + colName + " = '" + str + "'";
		if (query.exec(sql2))
		{
			query.next();
			if (query.value(0).toInt()) //���ز�ѯ��������
				return true;
			else
				return false;
		}
		else
		{
			qDebug() << "Martch str failed! The reason is" << this->db.lastError().text();
			return false;
		}

	}
	else
	{
		qDebug() << "The table " << tableName << " does not exist!";
		return false;
	}

}

//�û���¼��֤
int JS_DATABASE::usersLogin(QString userName, QString password)
{
	//�����ж����ݿ�����Ƿ��Ѿ�����
	assert(is_connected); //���ݿ�δ����ʱ��ֹ���� 

	QSqlQuery query(this->db);
	QString sql1;

	//count(*)Ϊ���������ļ�¼��������classificationΪ�û��ȼ�������û��������벻��ȷ��classification = NULL
	sql1 = "select count(*), classification from test where name = '"+ userName +"' and pwd = '" + password + "'";
	if (query.exec(sql1))
	{
		query.next();
		if (query.value(0).toInt()) //���ز�ѯ��������
			return query.value(1).toInt();//�����û��ȼ�
		else
			return -1;//�û������������
	}
	else
		return -2; //


}

//��ѯʱ�����
int JS_DATABASE::timeCost()
{
	//�����ж����ݿ�����Ƿ��Ѿ�����
	assert(is_connected); //���ݿ�δ����ʱ��ֹ���� 
	QSqlQuery query(this->db);
	QString sql;

	//��֤ÿ�β������������һ��
	srand((unsigned)time(NULL));

	clock_t startTime, endTime;
	startTime = clock();
	

	for (int i = 0; i < 3000; ++i)
	{
		int id = rand() % 100000 + 1;//�������Χ[1,100000]
		sql = "select id from test where id = '" + QString::number(id) + "'";

		if (query.exec(sql))
		{
			query.next();
			
		}
	}
	endTime = clock();

	cout << "time = " << endTime - startTime << endl;
	

	return 0;
}



// ��ȡʱ������� ��ʽʾ����2020/08/29 16:59:28
QString JS_DATABASE::get_timestamp_now()
{
	time_t currentTime = time(NULL);
	tm *ptm = localtime(&currentTime);
	char buf[20];
	sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	QString time(buf);

	return time;
}

// ������Ӷ�Ӧ����������map��ʼ��
void JS_DATABASE::defect_index_map_init()
{
	this->defect_index_map[0] = "op_ending_extrude";				// ĩ��ͻ��
	this->defect_index_map[1] = "op_wrong_element";					// ����
	this->defect_index_map[2] = "op_backwards";						// װ��
	this->defect_index_map[3] = "op_ending_overlap";				// ĩ���ص�
	this->defect_index_map[4] = "op_missing_element";				// ȱ��
	this->defect_index_map[5] = "op_electrode_area";				// �缫���
	this->defect_index_map[6] = "op_polarity_error";				// ���Դ���
	this->defect_index_map[7] = "op_electrode_lateral_bending";		// �缫��������
	this->defect_index_map[8] = "op_element_offset";				// Ԫ��ƫ��
	this->defect_index_map[9] = "op_broadside_extrude";				// ���ͻ��
	this->defect_index_map[10] = "op_electrode_extrude";			// �缫ͻ��
	this->defect_index_map[11] = "op_pad_infiltration";				// ���̽���
	this->defect_index_map[12] = "op12";
	this->defect_index_map[13] = "op13";
	this->defect_index_map[14] = "op14";
	this->defect_index_map[15] = "op15";
	this->defect_index_map[16] = "op16";
	this->defect_index_map[17] = "op17";
	this->defect_index_map[18] = "op18";
	this->defect_index_map[19] = "op19";
	this->defect_index_map[20] = "op20";
	this->defect_index_map[21] = "op21";
	this->defect_index_map[22] = "op22";
	this->defect_index_map[23] = "op23";
}

// ������Ӷ�Ӧ���ֵ������map��ʼ��
void JS_DATABASE::value_num_map_init()
{
	this->value_num_map[0] = 1;		// ĩ��ͻ��
	this->value_num_map[1] = 1;		// ����
	this->value_num_map[2] = 1;		// װ��
	this->value_num_map[3] = 1;		// ĩ���ص�
	this->value_num_map[4] = 2;		// ȱ��
	this->value_num_map[5] = 1;		// �缫���
	this->value_num_map[6] = 1;		// ���Դ���
	this->value_num_map[7] = 1;		// �缫��������
	this->value_num_map[8] = 3;		// Ԫ��ƫ��
	this->value_num_map[9] = 2;		// ���ͻ��
	this->value_num_map[10] = 1;	// �缫ͻ��
	this->value_num_map[11] = 1;	// ���̽���
	this->value_num_map[12] = 1;
	this->value_num_map[13] = 1;
	this->value_num_map[14] = 1;
	this->value_num_map[15] = 1;
	this->value_num_map[16] = 1;
	this->value_num_map[17] = 1;
	this->value_num_map[18] = 1;
	this->value_num_map[19] = 1;
	this->value_num_map[20] = 1;
	this->value_num_map[21] = 1;
	this->value_num_map[22] = 1;
	this->value_num_map[23] = 1;
}


//�����ݿ������
bool JS_DATABASE::saveToDB(QString eleName, SMTInfo* ptrSave, QHash<QString, QString> hash)
{
	//�����ж����ݿ�����Ƿ��Ѿ�����
	//���ݿ�δ����ʱ��ֹ���� 
	assert(is_connected); 

	QSqlQuery query(this->db);

	/**************************���ܱ��в���Ԫ������Ϣ**************************/
	QString sql = "insert into `elements_list` values (default, ";
	sql += "'" + ptrSave->smt_name + "', ";
	sql += "'" + ptrSave->smt_code + "', ";
	sql += QString::number(ptrSave->smt_length) + ", ";
	sql += QString::number(ptrSave->smt_width) + ", ";
	sql += QString::number(ptrSave->smt_height) + ", ";
	sql += QString::number(ptrSave->smt_angle) + ", ";
	sql += "'" + ptrSave->error_types + "', ";
	sql += "'" + ptrSave->img_path + "')";

	if (!query.exec(sql))
		return false;
	else
		query.next();

	/****************************��ȱ�ݷֱ��в�����Ϣ***************************/
	QString errorType = ptrSave->error_types;
	int numOfError = 0;
	for (int i = 0; i < errorType.size(); ++i)
	{
		if (errorType[i] == '1')
			numOfError++;
	}
	int index = 0;
	ErrorInfo* pError = *(ptrSave->ero + index);//->�����ȼ�����*

	//��pErrorָ��Ŀռ�û��ֵ�����ѭ��
		while (index < numOfError)
	{

		//ȱ�ݷֱ�
		QString errorName = pError->name;
		//ȷ��Ҫ����ķֱ���
		QString subSheet = hash[errorName];

		//if (pError == nullptr)
		//	continue;

		QString subSql = "insert into `" + subSheet + "` values (";

		//����Ҫ����Ĳ���
		for(int j = 0; j < pError->amount; ++j)
		{
			switch (pError->error_num_type[j])
			{
				case 0: //����
				{
					int val = pError->type0_factor[pError->error_team_num[j]];
					QString val0 = QString::number(val);
					//�жϵ�ǰ�Ƿ�Ϊ���һ��Ԫ��
					if(j == pError->amount - 1)
						subSql += val0;
					else
						subSql += val0 + ", ";
					break;
				}
				case 1: //������
				{
					double val = pError->type1_factor[pError->error_team_num[j]];
					QString val1 = QString::number(val);
					//�жϵ�ǰ�Ƿ�Ϊ���һ��Ԫ��
					if (j == pError->amount - 1)
						subSql += val1;
					else
						subSql += val1 + ", ";
					break;
				}
				case 2: //QString
				{
					QString val = pError->type2_factor[pError->error_team_num[j]];
					//�жϵ�ǰ�Ƿ�Ϊ���һ��Ԫ��
					if (j == pError->amount - 1)
						subSql += "'" + val + "'";
					else
						subSql += "'" + val + "', ";
					break;
				}
				case 3: //QString
				{
					QString val = pError->type3_factor[pError->error_team_num[j]];
					//�жϵ�ǰ�Ƿ�Ϊ���һ��Ԫ��
					if (j == pError->amount - 1)
						subSql += "'" + val + "'";
					else
						subSql += "'" + val + "', ";
					break;
				}
				default:
					break;
			}
				
		}

		subSql += ")";
		if (!query.exec(subSql))
			return false;
		else
			query.next();
	
		index++;
		pError = *(ptrSave->ero + index);
	}
	
	cout << "д����ɣ�" << endl;

}

//�����ݿ���ȡ����
bool JS_DATABASE::loadFromDB(QString eleName, SMTInfo* ptrLoad, QHash<QString, QString> hash)
{
	//�����ж����ݿ�����Ƿ��Ѿ�����
	//���ݿ�δ����ʱ��ֹ���� 
	assert(is_connected);

	QSqlQuery query(this->db);

	/**************************���ܱ��ж�ȡԪ������Ϣ**************************/
	QString sql = "select * from `elements_list` where ele_name = '" + eleName + "'";

	if (!query.exec(sql))
		return false;
	else
		query.next();

	ptrLoad->smt_name = query.value("ele_name").toString();
	ptrLoad->smt_code = query.value("ele_index").toString();
	ptrLoad->smt_length = query.value("ele_length").toInt();
	ptrLoad->smt_width = query.value("ele_width").toInt();
	ptrLoad->smt_height = query.value("ele_height").toInt();
	ptrLoad->smt_angle = query.value("ele_angel").toInt();
	ptrLoad->error_types = query.value("ng_trigger").toString();
	ptrLoad->img_path = query.value("pic_path").toString();

	/****************************��ȱ�ݷֱ��ж�ȡ��Ϣ***************************/
	QString errorType = ptrLoad->error_types;
	for (int i = 0; i < errorType.size(); ++i)
	{
		if (errorType[i] != '1')
			continue;

		//pErrorָ���i�����ݱ�
		ErrorInfo* pError = ptrLoad->ero[i];

		//TODO:Ӧ����һ���洢ȱ�ݷֱ��vector
		//QString subSheet = 
		if (pError == nullptr)
			continue;
		
		////��ѯȱ�ݷֱ����ж��ٸ��ֶ�
		//QString subSql = "select count(*) from information_schema.columns where table_name = '" + ;
		//pError->amount = ;

		//for (int j = 0; j < pError->amount; ++j)
		//{
		//	switch()
		//}
	}
}

