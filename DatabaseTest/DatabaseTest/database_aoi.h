#pragma once
#include <QString>
#include <QSqlDatabase>
#include <map>
#include <string>
#include <vector>
#include <QHash>

// 插入元件检测结果时传入的结构体
struct DETECT_INFO_INSERT;

// 查询元件检测结果时传出的结构体
struct DETECT_INFO_REQUIRE;

//缺陷信息结构体
struct ErrorInfo;

//元件信息结构体
struct SMTInfo;


// AOI项目数据库类
class JS_DATABASE
{
public:
	// 构造函数
	JS_DATABASE(QString connection_name, QString host_IP, QString user_name, QString passward, QString database_name);

	// 析构函数
	~JS_DATABASE();

public:
	// 建立数据库连接
	bool connect();

	//查询元器件要检查的缺陷类型
	std::vector<QString> findDefectsToDetect(QString );

	// 插入结果数据
	bool insert_detect_result(DETECT_INFO_INSERT& result_info);

	// 查询具体时间戳下，某块板的某个元件检测结果信息
	bool inquire_detect_result(QString timestamp, QString board, QString ele_index, DETECT_INFO_REQUIRE& result_info);

	// 清空指定的数据表
	bool truncate_table(QString table_name);

	//查询数据数据库中是否有特定表
	bool findTable(QString);

	//重命名特定表
	bool renameTable(QString, QString);

	//删除特定表
	bool dropTable(QString);

	//创建特定表
	bool createTable(QString);

	//查询表名A下第X列内是否有数据与输入的字符STR相等
	int matchTableIndex(QString, int, QString );

	//用户登录验证
	int usersLogin(QString, QString);

	//查询时间测试
	int timeCost();


public:
	// 获取时间戳函数 示例：2020/08/29 16:59:28
	static QString get_timestamp_now();

private:
	// 检测算子对应表名的索引map初始化
	void defect_index_map_init();

	// 检测算子对应输出值数量的map初始化
	void value_num_map_init();




public:
	// 数据库对象
	QSqlDatabase db;
	// 数据库连接名称（自定义，用于区分）
	QString connection_name;
	// 目标数据库所属主机IP
	QString host_IP;
	// 登录目标数据库时的用户名
	QString user_name;
	// 登录密码
	QString passward;
	// 目标数据库名称
	QString database_name;

	//向数据库存数据
	bool saveToDB(QString, SMTInfo*, QHash<QString, QString>);
	//从数据库中取数据
	bool loadFromDB(QString, SMTInfo*, QHash<QString, QString>);



private:
	// 检测算子对应表名的索引map
	std::map<int, QString> defect_index_map;
	// 检测算子对应输出值数量的map
	std::map<int, int> value_num_map;

private:
	// 标识数据库驱动是否有效
	bool is_driver_valid = false;
	// 标识数据库是否已经连接
	bool is_connected = false;

};

// 插入元件检测结果时传入的结构体
struct DETECT_INFO_INSERT
{
	QString board_name;		// boom板名称（每块板的唯一标识）
	QString ele_name;		// 元件名
	QString ele_index;		// 元件编码（每块板上的每个元件的唯一标识）
	QString timestamp;		// 时间戳（三种格式: 2020/08/29 16:59:28 || 2020-08-29 16:59:28 || 20200829165928）
							// 可以调用JS_DATABASE的静态成员函数get_timestamp_now获取格式为2020/08/29 16:59:28的时间戳
	bool is_ng;				// 是否NG标识位（任意一个检测算子检出为NG则该元件NG标识为true）
	QString ng_type;		// 元件需检查的缺陷类型标识字串，由字符'0'，'1'组成，长度与缺陷数量一致，暂定为24
	QString ng_trigger;		// 元件检出有NG对应缺陷的标识字串，由字符'0'，'1'组成，长度与缺陷数量一致，暂定为24
	QString pic_path;		// 结果图存储的路径（路径分隔符为4个倒斜杠'\\\\'，示例："D:\\\\test\\\\pic\\\\test.bmp"）
	int loc_x;				// 结果图在整板图中的左上角点x坐标
	int loc_y;				// 结果图在整板图中的左上角点y坐标
	int width;				// 结果图的宽度
	int height;				// 结果图的高度
	std::vector<QString> vec_values;  // 元件经算子检测的输出值（输出值的添加顺序应该是ng_type中从左往右置1位对应的检测算子输出值）
};

// 查询元件检测结果时传出的结构体
struct DETECT_INFO_REQUIRE
{
	QString ele_name;		// 元件名
	bool b_isNG;			// 结果图的NG判定结果
	QString ng_trigger;		// 元件检出NG的标识字串
	QString ng_type;		// 元件需检查的缺陷类型标识字串，由字符'0'，'1'组成，长度与缺陷数量一致，暂定为24
	std::string pic_path;	// 结果图存储的路径
	int loc_x;				// 结果图在整板图中的左上角点x坐标
	int loc_y;				// 结果图在整板图中的左上角点y坐标
	int width;				// 结果图的宽度
	int height;				// 结果图的高度
	std::map<int, QString> detect_value_map;  // 进行检测的算子输出结果
};


//缺陷信息结构体(分表信息)
struct ErrorInfo
{
	int amount; //缺陷参数个数
	QString name; //缺陷名（中文）
	std::vector<int>error_team_num;//参数在各自数组中的序号
	std::vector<QString>error_name;//参数名
	std::vector<int>error_num_type;//将数据表参数类型按参数在数据表中的顺序存储在vector中
	std::vector<int>type0_factor;//int参数数组
	std::vector<double>type1_factor;//
	std::vector<QString>type2_factor;
	std::vector<QString>type3_factor;
	void Reset_this() 
	{
		this->name = "";
		this->amount = 0;
		this->error_team_num.clear();
		this->error_name.clear();
		this->error_num_type.clear();
		this->type0_factor.clear();
		this->type1_factor.clear();
		this->type2_factor.clear();
		this->type3_factor.clear();
	}
};


//元件信息结构体(总表)
struct SMTInfo 
{
	QString smt_name;
	QString smt_code;
	QString error_types;
	QString img_path;
	int smt_x;
	int smt_y;
	int smt_length;
	int smt_width;
	int smt_height;
	int smt_angle;
	ErrorInfo** ero;
};
