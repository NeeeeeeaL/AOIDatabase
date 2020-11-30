#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

typedef struct Position
{
	Position(std::string _Symbol = "", std::string _number = "", double _X = -1.0, double _Y = -1.0, double _Theta = 0.0)
		:Symbol(_Symbol), number(_number), X(_X), Y(_Y), Theta(_Theta)
	{};
	std::string Symbol;
	std::string number;
	double X;
	double Y;
	double Theta;
}Position;

class csvRW
{
private:
	std::string path;
	std::ifstream inFile;
	std::ofstream outFile;
	std::vector<std::vector<std::string>> rawData;
	

public:
	csvRW();
	csvRW(std::string filePath);
	~csvRW();
	bool read();
	bool read(std::string filePath);
	bool create(std::string filePath);
	bool write(Position pos);
	bool write(std::string filePath, Position pos);
	bool clear();
	bool close();


public:
	bool isreaded;
	std::vector<Position> data;
};



