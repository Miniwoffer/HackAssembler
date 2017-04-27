#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <bitset>
class assembler
{
public:
	assembler(std::string dir);
private:
	std::map<std::string, int> symbols;
	std::vector<std::string> code;
	bool getsymbols(std::string* line, int lineid);
	bool removecomments(std::string* line);
	std::string resolvesymbol(std::string line);
	int currram = 15;
	template<typename T, typename P>
	T remove_if(T beg, T end, P pred)
	{
		T dest = beg;
		for (T itr = beg; itr != end; ++itr)
			if (!pred(*itr))
				*(dest++) = *itr;
		return dest;
	}
	void error(std::string print)
	{
		std::cout << "ERROR: " << print << std::endl;
	}
	bool is_number(const std::string& s)
	{
		std::string::const_iterator it = s.begin();
		while (it != s.end() && isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	}
	int StringToNumber(const std::string &Text)
	{
		std::istringstream ss(Text);
		int result;
		return ss >> result ? result : 0;
	}
	int getNextRam()
	{
		currram++;
		return currram;
	}

};

