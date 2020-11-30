#include "csvRW.h"

using namespace std;

//将string类型变量转换为常用的数值类型
template <class Type>

Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

csvRW::csvRW()
{
	isreaded = false;
	path.clear();
	this->close();
	rawData.swap(std::vector<std::vector<string>>());
}

csvRW::csvRW(string filePath)
{
	isreaded = false;
	path = filePath;
	this->close();
	rawData.swap(std::vector<std::vector<string>>());
}


csvRW::~csvRW()
{
	isreaded = false;
	path.clear();
	this->close();
	rawData.swap(std::vector<std::vector<string>>());
}


bool csvRW::read()
{
	if (path.empty())
		return false;
	this->clear();
	inFile.open(path, ios::in);
	if (inFile.is_open())
	{
		string lineStr;
		for (int i = 0; i < 3; i++)
			getline(inFile, lineStr);
		while (getline(inFile, lineStr))
		{
			stringstream ss(lineStr);
			string str;
			vector<string> lineArray;
			// 按照逗号分隔
			while (getline(ss, str, ','))
				lineArray.push_back(str);
			rawData.push_back(lineArray);
		}
		isreaded = true;
	}
	else
		return false;
	for (int i = 0; i < rawData.size(); i++)
	{
		if (rawData[i].size() != 5)
			return false;
		double value[3] = { -1.0,-1.0,0.0 };
		for (int j = 2; j < 5; j++)
		{
			value[j - 2] = stringToNum<double>(rawData[i][j]);
		}
		Position temp(rawData[i][0], rawData[i][1], value[0], value[1], value[2]);
		data.push_back(temp);
	}
	return true;
}

bool csvRW::read(std::string filePath)
{
	path = filePath;
	if (this->read())
		return true;
	else
		return false;
}

bool csvRW::create(std::string filePath)
{
	fstream _file;
	_file.open(filePath, ios::in);
	if (!_file)
	{
		outFile.open(path, ios::app);
		outFile << "PCB" << endl;
		outFile << "Design Notice" << "," << "GE6524" << endl;
		outFile << "Symbol" << "," << "CK parts number" << "," << "X" << "," << "Y" << "," << "θ" << endl;
		_file.close();
		return true;
	}
	else
	{
		_file.close();
		return false;
	}

}

bool csvRW::write(Position pos)
{
	if (path.empty())
		return false;
	this->create(path);
	this->close();
	outFile.open(path, ios::app);
	if (outFile.is_open())
	{
		outFile << pos.Symbol << ',' << pos.number << ',' << pos.X << ',' << pos.Y << ',' << pos.Theta << endl;
		data.push_back(pos);
	}
	else
		return false;
	return true;
}

bool csvRW::write(std::string filePath, Position pos)
{
	path = filePath;
	if (this->write(pos))
		return true;
	else
		return false;
}

bool csvRW::clear()
{
	this->close();
	isreaded = false;
	rawData.swap(vector<vector<string>>());
	data.swap(vector<Position>());
	return true;
}

bool csvRW::close()
{
	if (inFile.is_open())
		inFile.close();
	if (outFile.is_open())
		outFile.close();
	return true;
}

