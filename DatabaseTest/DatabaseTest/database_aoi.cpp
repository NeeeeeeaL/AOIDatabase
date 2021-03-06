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
#include <assert.h> //程序出错，终止程序执行的头文件

#include <stdio.h>
#include <time.h>
#include <ctime>

#include "database_aoi.h"

#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ << "; Func:" << __func__ <<"]"

// 构造函数
JS_DATABASE::JS_DATABASE(QString connection_name, QString host_IP, QString user_name, QString passward, QString database_name)
{
	// 检测算子对应表名的索引map初始化
	defect_index_map_init();

	// 检测算子对应输出值数量map初始化
	value_num_map_init();

	// 数据库连接配置信息初始化
	this->host_IP = host_IP;
	this->user_name = user_name;
	this->passward = passward;
	this->database_name = database_name;

	// 添加MYSQL数据库驱动
	this->db = QSqlDatabase::addDatabase("QMYSQL", connection_name);
	// 查看数据库驱动是否有效
	this->is_driver_valid = this->db.isValid();
}


// 析构函数
JS_DATABASE::~JS_DATABASE()
{
	if (this->db.isOpen())
	{
		this->db.close();
	}
}

// 建立数据库连接
bool JS_DATABASE::connect()
{
	// 判断数据库驱动是否有效
	if (!is_driver_valid)
	{
		qDebug() << "Connect failed, the QMYSQL driver is invalid!";
		this->is_connected = false;
		return false;
	}

	// 配置连接信息
	this->db.setHostName(this->host_IP);
	this->db.setUserName(this->user_name);
	this->db.setPassword(this->passward);
	this->db.setDatabaseName(this->database_name);

	// 尝试建立连接
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
	//1.查询元器件要检查的缺陷类型

	//首先判断数据库对象是否已经连接
	assert(is_connected); //数据库未连接时终止程序 

	//从主表中查询该元件的应该检测的缺陷类型的字符串
	QSqlQuery query(this->db);
	QString sql;

	sql = "select ng_type from `production_result` where `ele_name` = '";
	sql += ele_name;
	sql += "'";

	query.exec(sql);
	query.next();

	QString ngType = query.value("ng_type").toString();

	cout << "ngType = " << ngType << endl;

	//2.根据要检查的缺陷定位选择要使用的分表
	std::vector<QString> targetTables;

	for (int i = 0; i < ngType.size(); ++i)
	{
		if (ngType[i] == "1")
			targetTables.push_back(defect_index_map[i]);
	}

	return targetTables;
}


// 插入结果数据
bool JS_DATABASE::insert_detect_result(DETECT_INFO_INSERT& result_info)
{
	// 判断数据库对象是否已经连接
	if (!is_connected)
	{
		qDebug() << "The database is disconnected! Please connect first!";
		return false;
	}

	// 1.执行操作前对输入值的有效性进行判断

	// 1.1 ng_type与ng_trigger的长度应该与map一致
	if (result_info.ng_type.size() != defect_index_map.size() || result_info.ng_trigger.size() != defect_index_map.size())
	{
		qDebug() << "Insert failed! The length of ng_type or ng_trigger is wrong!";
		return false;
	}

	// 1.2 ng_type与ng_trigger应该只包含0和1
	if (result_info.ng_type.contains(QRegExp("[^0,1]")) || result_info.ng_trigger.contains(QRegExp("[^0,1]")))
	{
		qDebug() << "Insert failed! The content of ng_type or ng_trigger should be only 0 or 1!";
		return false;
	}

	// 1.3 检查时间戳的格式是否正确
	QRegExp re_timestamp("\\s*\\d\\d\\d\\d[/-]?\\d\\d[/-]?\\d\\d[ ]*\\d\\d[:]?\\d\\d[:]?\\d\\d\\s*");
	if (re_timestamp.indexIn(result_info.timestamp) == -1)
	{
		qDebug() << "Insert failed! The format of timestamp is wrong!";
		return false;
	}

	// 1.4 检查待检测的NG数量与存储输出值容器的大小是否一致
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


	// 2.在表production_result中插入该元件的测试结果信息
	QSqlQuery query(this->db);
	QString sql;
	bool isOK;

	// 2.1 拼接sql语句
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

	// 2.2 执行sql语句向表production_result中插入
	isOK = query.exec(sql);
	if (!isOK)
	{
		qDebug() << "Insert into `production_result` failed! The reason is" << this->db.lastError().text();
		return false;
	}

	// 3.分析result_info.ng_type信息，将该元件需检测的NG信息输出到对应的表中

	// 3.1 根据先前步骤获取的字串中1的位置作为索引号从defect_index_map获取对应需操作的表名
	std::vector<QString> vec_tablename;
	for (int i = 0; i < ng_num_needcheck; i++)
	{
		vec_tablename.push_back(defect_index_map[vec_ng_needcheck_index[i]]);
	}

	// 3.2 生成操作每张表需要的sql语句
	std::vector<QString> vec_sql;
	for (int i = 0; i < ng_num_needcheck; i++)
	{
		QString sql_tmp = "insert into `" + vec_tablename[i] + "` values (default, ";
		sql_tmp += "'" + result_info.board_name + "', ";
		sql_tmp += "'" + result_info.ele_index + "', ";
		sql_tmp += "'" + result_info.timestamp + "', ";
		sql_tmp += result_info.ng_trigger[vec_ng_needcheck_index[i]] == "1" ? "1, " : "0, ";

		// 判断该缺陷的输出信息是否为多个（如果有多个则按','分隔，因此直接判断有无字符','确认是否为多输出值）
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
		// 如果输出信息只有1个
		else
		{
			sql_tmp += "'" + result_info.vec_values[i] + "') ";
			vec_sql.push_back(sql_tmp);
		}
	}

	// 3.3执行sql语句将每个缺陷检测算子的详细结果存储到对应的表中
	for (int i = 0; i < vec_sql.size(); i++)
	{
		// qDebug() << vec_sql[i];  // 输出生成的每一条sql语句（调试用）
		isOK = query.exec(vec_sql[i]);
		if (!isOK)
		{
			qDebug() << vec_sql[i] <<  "failed! The reason is" << this->db.lastError().text();
			return false;
		}
	}

	return true;
}

// 查询具体时间戳下，某块板的某个元件检测结果信息
bool JS_DATABASE::inquire_detect_result(QString timestamp, QString board, QString ele_index, DETECT_INFO_REQUIRE& result)
{
	// 判断数据库对象是否已经连接
	if (!is_connected)
	{
		qDebug() << "The database is disconnected! Please connect first!";
		return false;
	}

	// 1.检查时间戳的格式是否正确
	QRegExp re_timestamp("\\s*\\d\\d\\d\\d[/-]?\\d\\d[/-]?\\d\\d[ ]*\\d\\d[:]?\\d\\d[:]?\\d\\d\\s*");
	if (re_timestamp.indexIn(timestamp) == -1)
	{
		qDebug() << "Insert failed! The format of timestamp is wrong!";
		return false;
	}

	// 2.从主表中查询该元件的检测结果
	QSqlQuery query(this->db);
	QString sql;
	bool isOK;

	// 2.1 拼接并执行sql语句
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

	// 2.2 获取结果集，并填写从主表中获取的信息
	if (!query.next())
	{
		qDebug() << "The inquire result in `production_result` is NULL!";
		return false;
	}
	// 读取数据库中的图片路径（QString转为std::string）
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


	// 3.根据主表中查到的ng_type信息，进一步从各分表中查找具体的算子输出信息

	// 3.1 获取元件应该检测的算子索引号，进一步获取应该查询的表名以及对应输出的value数量
	QString ng_type = query.value("ng_type").toString();
	std::vector<int> vec_ng_needcheck;			// 原件应该进行检测的算子索引号
	std::vector<QString> vec_table_needinquire;	// 原件应该进行检测的算子索引号对应的表名
	std::vector<int> vec_value_num;				// 原件应该进行检测的算子索引号对应的输出value数量
	for (int i = 0; i < ng_type.size(); i++)	// 遍历解析ng_type字串
	{
		if (ng_type[i] == '1')
		{
			vec_ng_needcheck.push_back(i);
			vec_table_needinquire.push_back(defect_index_map[i]);
			vec_value_num.push_back(value_num_map[i]);
		}
	}
	
	// 3.2 根据算子索引号对应的表名生成并执行用于查询不同表的sql语句，并根据应该输出的value数量配置输出信息的格式
	for (int i = 0; i < vec_table_needinquire.size(); i++)
	{
		// 生成执行分表查询任务的sql语句
		sql = "select * from ";
		sql += "`" + vec_table_needinquire[i] + "` where ";
		sql += "timestamp = '" + timestamp + "' ";
		sql += "and board = '" + board + "' ";
		sql += "and ele_index = '" + ele_index + "'";
		isOK = query.exec(sql);

		// 查询语句执行失败或查到的结果集为空，将该算子对应分表的输出信息置空""
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

		// 判断value数量，如有多个则需要将多个value信息用","分隔拼接成一个字串填入输出信息中
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

// 清空指定的数据表
bool JS_DATABASE::truncate_table(QString table_name)
{
	// 判断数据库对象是否已经连接
	if (!is_connected)
	{
		qDebug() << "The database is disconnected! Please connect first!";
		return false;
	}

	// 执行清空数据表的操作
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

//查询数据数据库中是否有特定表
bool JS_DATABASE::findTable(QString tableNmae)
{
	//首先判断数据库对象是否已经连接
	assert(is_connected); //数据库未连接时终止程序 

	QSqlQuery query(this->db);
	QString sql;

	sql = "select count(*) from information_schema.tables where table_schema = 'aoi' and table_name = '";
	sql += tableNmae;
	sql += "'";

	query.exec(sql);
	query.next();

	if (query.value(0).toInt()) //查询到表返回1，未查询到返回0
		return true;
	else
		return false;
}


//重命名特定表
bool JS_DATABASE::renameTable(QString originalName, QString newName)
{
	//首先判断数据库对象是否已经连接
	assert(is_connected); //数据库未连接时终止程序 

	//判断是否有该表
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

//删除特定表
bool JS_DATABASE::dropTable(QString tableName)
{
	//首先判断数据库对象是否已经连接
	assert(is_connected); //数据库未连接时终止程序 

	//判断是否有该表
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

//创建特定表
bool JS_DATABASE::createTable(QString tableName)
{
	//首先判断数据库对象是否已经连接
	assert(is_connected); //数据库未连接时终止程序 

	//先判断此表是否存在
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
	//首先判断数据库对象是否已经连接
	assert(is_connected); //数据库未连接时终止程序 
	
	//判断是否有此数据表
	if(findTable(tableName))
	{
		QString colName; //存放查询到的字段名
		QSqlQuery query(this->db);
		QString sql1;
		QString sql2;

		//查询字段名
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

		//查询字段下有没有str
		sql2 = "select count(*) from " + tableName + " where " + colName + " = '" + str + "'";
		if (query.exec(sql2))
		{
			query.next();
			if (query.value(0).toInt()) //返回查询到的数量
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

//用户登录验证
int JS_DATABASE::usersLogin(QString userName, QString password)
{
	//首先判断数据库对象是否已经连接
	assert(is_connected); //数据库未连接时终止程序 

	QSqlQuery query(this->db);
	QString sql1;

	//count(*)为符合条件的记录的数量，classification为用户等级，如果用户名或密码不正确，classification = NULL
	sql1 = "select count(*), classification from test where name = '"+ userName +"' and pwd = '" + password + "'";
	if (query.exec(sql1))
	{
		query.next();
		if (query.value(0).toInt()) //返回查询到的数量
			return query.value(1).toInt();//返回用户等级
		else
			return -1;//用户名或密码错误
	}
	else
		return -2; //


}

//查询时间测试
int JS_DATABASE::timeCost()
{
	//首先判断数据库对象是否已经连接
	assert(is_connected); //数据库未连接时终止程序 
	QSqlQuery query(this->db);
	QString sql;

	//保证每次产生的随机数不一样
	srand((unsigned)time(NULL));

	clock_t startTime, endTime;
	startTime = clock();
	

	for (int i = 0; i < 3000; ++i)
	{
		int id = rand() % 100000 + 1;//随机数范围[1,100000]
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



// 获取时间戳函数 格式示例：2020/08/29 16:59:28
QString JS_DATABASE::get_timestamp_now()
{
	time_t currentTime = time(NULL);
	tm *ptm = localtime(&currentTime);
	char buf[20];
	sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	QString time(buf);

	return time;
}

// 检测算子对应表名的索引map初始化
void JS_DATABASE::defect_index_map_init()
{
	this->defect_index_map[0] = "1_op_ending_extrude";				    // 末端突出
	this->defect_index_map[1] = "2_op_wrong_element";					// 错料
	this->defect_index_map[2] = "3_op_backwards";						// 装反
	this->defect_index_map[3] = "4_op_ending_overlap";				    // 末端重叠
	this->defect_index_map[4] = "5_op_missing_element";				    // 缺件
	this->defect_index_map[5] = "6_op_electrode_area";				    // 电极面积
	this->defect_index_map[6] = "7_op_polarity_error";				    // 极性错误
	this->defect_index_map[7] = "8_op_electrode_lateral_bending";		// 电极横向弯曲
	this->defect_index_map[8] = "9_op_element_offset";				    // 元件偏移
	this->defect_index_map[9] = "10_op_broadside_extrude";				// 侧边突出
	this->defect_index_map[10] = "11_op_electrode_extrude";			    // 电极突出
	this->defect_index_map[11] = "12_op_pad_infiltration";				// 焊盘浸润
	this->defect_index_map[12] = "13_op_tin_width";                     // 填锡末端接合宽度
	this->defect_index_map[13] = "14_op_electrode_exists_or_not";       // 电极有无
	this->defect_index_map[14] = "15_op_pad_copper_leakage";            // 焊盘漏铜
	this->defect_index_map[15] = "16_op_pad_abnormal";                  // 焊盘异常
	this->defect_index_map[16] = "17_op_fill_tin_length";               // 填锡长度
	this->defect_index_map[17] = "18_op_electrode_up_copper_leakage";   // 电极顶端漏铜
	this->defect_index_map[18] = "19_op_electrode_infiltration";        // 电极浸润
	this->defect_index_map[19] = "20_op_ele_height";                    // 元件高度
	this->defect_index_map[20] = "21_op_electrode_bridge";              // 电极桥接
	this->defect_index_map[21] = "22_op_electrode_uneven";              // 电极不平
	this->defect_index_map[22] = "23_op_tin_height";                    // 填锡高度
	this->defect_index_map[23] = "24_op_pad_something";                 // 焊盘异物
	this->defect_index_map[24] = "25_op_tin_ball";                      // 锡珠
	this->defect_index_map[25] = "26_op_something";                     // 异物
	this->defect_index_map[26] = "27_op_electrode_raise";               // 元件起翘
}

// 检测算子对应输出值数量的map初始化
void JS_DATABASE::value_num_map_init()
{
	this->value_num_map[0] = 1;		// 末端突出
	this->value_num_map[1] = 1;		// 错料
	this->value_num_map[2] = 1;		// 装反
	this->value_num_map[3] = 1;		// 末端重叠
	this->value_num_map[4] = 2;		// 缺件
	this->value_num_map[5] = 1;		// 电极面积
	this->value_num_map[6] = 1;		// 极性错误
	this->value_num_map[7] = 1;		// 电极横向弯曲
	this->value_num_map[8] = 3;		// 元件偏移
	this->value_num_map[9] = 2;		// 侧边突出
	this->value_num_map[10] = 1;	// 电极突出
	this->value_num_map[11] = 1;	// 焊盘浸润
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


//向数据库存数据
bool JS_DATABASE::saveToDB(QString eleName, SMTInfo* ptrSave, QHash<QString, QString> hash)
{
	//首先判断数据库对象是否已经连接
	//数据库未连接时终止程序 
	assert(is_connected); 

	QSqlQuery query(this->db);

	/**************************向总表中插入元器件信息**************************/
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

	/****************************向缺陷分表中插入信息***************************/
	QString errorType = ptrSave->error_types;
	int numOfError = 0;
	for (int i = 0; i < errorType.size(); ++i)
	{
		if (errorType[i] == '1')
			numOfError++;
	}
	int index = 0;
	ErrorInfo* pError = *(ptrSave->ero + index);//->的优先级高于*

	//当pError指向的空间没有值后结束循环
		while (index < numOfError)
	{

		//缺陷分表
		QString errorName = pError->name;
		//确定要插入的分表名
		QString subSheet = hash[errorName];

		//if (pError == nullptr)
		//	continue;

		QString subSql = "insert into `" + subSheet + "` values (";

		//遍历要插入的参数
		for(int j = 0; j < pError->amount; ++j)
		{
			switch (pError->error_num_type[j])
			{
				case 0: //整型
				{
					int val = pError->type0_factor[pError->error_team_num[j]];
					QString val0 = QString::number(val);
					//判断当前是否为最后一个元素
					if(j == pError->amount - 1)
						subSql += val0;
					else
						subSql += val0 + ", ";
					break;
				}
				case 1: //浮点型
				{
					double val = pError->type1_factor[pError->error_team_num[j]];
					QString val1 = QString::number(val);
					//判断当前是否为最后一个元素
					if (j == pError->amount - 1)
						subSql += val1;
					else
						subSql += val1 + ", ";
					break;
				}
				case 2: //QString
				{
					QString val = pError->type2_factor[pError->error_team_num[j]];
					//判断当前是否为最后一个元素
					if (j == pError->amount - 1)
						subSql += "'" + val + "'";
					else
						subSql += "'" + val + "', ";
					break;
				}
				case 3: //QString
				{
					QString val = pError->type3_factor[pError->error_team_num[j]];
					//判断当前是否为最后一个元素
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
	
	cout << "写入完成！" << endl;

}

//从数据库中取数据
bool JS_DATABASE::loadFromDB(QString eleName, SMTInfo* ptrLoad,std::vector<QStringList>&res)
{
	//首先判断数据库对象是否已经连接
	//数据库未连接时终止程序 
	assert(is_connected);

	QSqlQuery query(this->db);

	/**************************从总表中读取元器件信息**************************/
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

	/****************************从缺陷分表中读取信息***************************/
	QString errorType = ptrLoad->error_types;
	for (int i = 0; i < errorType.size(); ++i)
	{
		if (errorType[i] != '1')
			continue;

		//pError指向第i张数据表
		ptrLoad->ero[i] = new ErrorInfo;
		ErrorInfo* pError = ptrLoad->ero[i];
		
		QString subSheet = this->defect_index_map[i];
		if (pError == nullptr)
			continue;
		
		//查询缺陷分表中有多少个字段
		QString SqlNum = "select count(*) from information_schema.columns where table_name = '" + subSheet + "'";
		if (!query.exec(SqlNum))
			return false;
		else
			query.next();
		pError->amount = query.value(0).toInt() - 1;

		////查询各字段类型
		//QString SqlType = "select data_type from information_schema.columns where table_name = '" + subSheet + "'";
		//if (!query.exec(SqlType))
		//	return false;
		//else
		//	query.next();
		
		QStringList dataList;
		//查询一行所有数据
		QString SqlAll = "select * from `" + subSheet + "` where ele_name = '" + eleName + "'";
		if (!query.exec(SqlAll))
			return false;
		else
			query.next();
		QStringList m_list;
		for (int i = 1; i < pError->amount + 1;i++)
		{
			m_list << query.value(i).toString();
			qDebug() << query.value(i);
		}
		res.push_back(m_list);


	}
}

