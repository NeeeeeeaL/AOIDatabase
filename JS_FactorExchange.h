#pragma once
#include <QDialog>
#include <qsettings.h>
#include "database_aoi.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qdebug.h"

struct ErrorInfo {
	int amount;
	QString name;
	std::vector<int>error_team_num;//参数在各自数组中的序号
	std::vector<QString>error_name;
	std::vector<int>error_num_type;
	
	std::vector<int>type0_factor;
	std::vector<double>type1_factor;
	std::vector<QString>type2_factor;
	std::vector<QString>type3_factor;
	void Reset_this(){
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

struct SMTInfo {
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
	ErrorInfo** ero;//ȱ�ݲ���ָ��
};

class JS_FactorExchange
{
public:
	static JS_FactorExchange* GetInstance() {
		static int ptr_num = 1;
		if (ptr_num > 0)
		{
			ptr_num--;
			return (new JS_FactorExchange);
		}
		return nullptr;
	}
	void Destroy() {
		delete this;
	}
	bool ReadInINI(QString ini_path);//����ȱ������-ȱ����Ŷ�ӦHASH��
	bool ReadInCSV(QString csv_path,int nType);//����Ԫ����-�ṹ��{��ϸ����}��ӦHASH����ntype=0,ֱ�Ӷ���;ntype=1,���������������ͬʱʹ��
	bool ReadInDB(JS_DATABASE* js_db);//���Ԫ����ʼCSV,���������Ǹ������õ�CSV
	bool WriteCSV(QString csv_path,int nType = 0);//�����޸ĺ�������CSV
	bool Reset();//���ø�ָ�루������з�����ڴ沢�������).
	QHash<QString, int>ErrorCode_Hash;
	QHash<QString, SMTInfo*>SMT_Hash;
	std::vector<QString>SMT_Name;
	std::vector<QString>ErrorCode_Name;

private:
	JS_FactorExchange();
	virtual ~JS_FactorExchange();
	int error_type_amount;
	std::vector<int>js_amount;
	std::vector<std::vector<int>>js_error_team_num;
	std::vector<std::vector<QString>>js_error_name;
	std::vector<std::vector<int>>js_error_num_type;
};

