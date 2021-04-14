#include "IOManager.h"

IOManager::IOManager(){}

std::vector<std::string*> IOManager::readFile(std::string path)
{
	std::vector<std::string*> data;
	std::string s;
	std::ifstream in;

	in.open(path);
	while (!in.eof())
	{
		getline(in, s);
		if (s.length() > 0)
		{
			data.push_back(IOManager::split(s, ';'));
		}
	}
	in.close();
	return data;
}

void IOManager::createFile(std::string path)
{
	std::ofstream out(path);
	out.close();
}

void IOManager::writeFile(std::string path, std::string f)
{
	std::ofstream out(path);
	out << f;
	out.close();
}

std::string* IOManager::split(std::string s, char r)
{
	std::vector<std::string> v;
	std::stringstream ss(s);
	std::string tkn;

	while (getline(ss, tkn, r))
	{
		v.push_back(tkn);
	}

	std::string* result = new std::string[v.size()];
	for (int i = 0; i < v.size(); i++)
	{
		result[i] = v[i];
	}
	return result;
}

bool IOManager::exists(std::string path)
{
	std::ifstream in;
	in.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try 
	{
		in.open(path);
		in.close();
		return true;
	}
	catch (const std::ifstream::failure& e) 
	{
		createFile(path);
		return false;
	}
}
