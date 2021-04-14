#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class IOManager
{
public:
	IOManager();
	static std::vector<std::string*> readFile(std::string path);
	static void createFile(std::string path);
	static void writeFile(std::string path, std::string f);
	static std::string* split(std::string s, char r);
	static bool exists(std::string path);
};

