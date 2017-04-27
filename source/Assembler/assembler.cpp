#include "assembler.h"


assembler::assembler(std::string dir)
{
	symbols.clear();
	symbols["SP"] = 0;
	symbols["LCL"] = 1;
	symbols["ARG"] = 2;
	symbols["THIS"] = 3;
	symbols["THAT"] = 4;
	for (int i = 0; i <= 15; i++)
	{
		symbols["R"+ std::to_string(i)] = i;
	}
	symbols["SCREEN"] = 16384;
	symbols["KBD"] = 24576;

	std::string outfilename = dir;
	std::string infilename = dir;
	std::string line;
	outfilename = outfilename.substr(0, outfilename.length() - 3);
	outfilename += "hack";
	std::ifstream infile;
	std::ofstream outfile(outfilename);

	infile.open(dir);
	if (infile.is_open())
	{
		if (outfile.is_open())
		{
			int lineid = 0;
			while (std::getline(infile, line))
			{
				if (removecomments(&line))
				{
					if (getsymbols(&line, lineid))
					{
						code.push_back(line);
						lineid++;
					}
				}
			}
			for (int i = 0; i < code.size(); i++)
			{
				code[i] = resolvesymbol(code[i]);
				outfile << code[i] << "\n";
			}
		}
		else
		{
			error("Cant open file " + outfilename);
		}
	}
	else
	{
		error("Cant open file " + infilename);
	}
	outfile.close();
	infile.close();
	std::cout << "Done" << std::endl;
}


bool assembler::removecomments(std::string* line)
{
	if (line->length() == 0)
	{
		return false;
	}
	else if (line->length() == 1)
	{
		if (*line == " ")
		{
			return false;
		}
		return true;
	}
	for (int i = 0; i < line->length() - 2; i++)
	{
		if (line->substr(i, 2) == "//")
		{
			if (i > 0)
			{
				line->assign(line->substr(0, i));
				break;
			}
			else
			{
				return false;
			}
		}
	}
	line->erase(remove_if(line->begin(), line->end(), isspace), line->end());
	if (line->length() == 0)
	{
		return false;
	}
	return true;
}

bool assembler::getsymbols(std::string* line, int lineid)
{
	if (line->substr(line->length() - 1, 1) == ")" && line->substr(0,1) == "(")
	{
		*line = line->substr(1, line->length() - 2);
		if (symbols.count(*line) == 0)
		{
			symbols[*line] = lineid;
			return false;
		}
		else
		{
			error("Symbol " + *line + " already exisists");
			return false;
		}
	}
	else
	{

	}
	return true;
}

std::string assembler::resolvesymbol(std::string line)
{
	if (line.substr(0, 1) == "@")
	{
		line = line.substr(1, line.length()-1);
		if (is_number(line))
		{
			line = std::bitset<15>(StringToNumber(line)).to_string();
		}
		else if (symbols.count(line) == 1)
		{
			line = std::bitset<15>(symbols[line]).to_string();
		}
		else
		{
			symbols[line] = getNextRam();
			line = std::bitset<15>(symbols[line]).to_string();
		}
		line.insert(0, "0");
	}
	else 
	{
		int equels = 0;
		int jumptag = 0;
		std::string ret = "111";
		for (int i = 0; i < line.length(); i++)
		{
			if (line[i] == '=')
			{
				equels = i;
			}
			if (line[i] == ';')
			{
				jumptag = i+1;
			}
		}
		int dest = 0;
		if (line.substr(0, equels).size() != 0)
		{
			for (int i = 0; i <= line.substr(0, equels).size(); i++)
			{
				if (line.substr(0, equels)[i] == 'A')
				{
					dest += 4;
				}
				if (line.substr(0, equels)[i] == 'M')
				{
					dest += 1;
				}
				if (line.substr(0, equels)[i] == 'D')
				{
					dest += 2;
				}
			}
		}
		int jump = 0;
		if (jumptag > 0)
		{
			if (line.substr(jumptag, 3) == "JGT")
			{
				jump = 1;
			}
			else if (line.substr(jumptag, 3) == "JEQ")
			{
				jump = 2;
			}
			else if (line.substr(jumptag, 3) == "JGE")
			{
				jump = 3;
			}
			else if (line.substr(jumptag, 3) == "JLT")
			{
				jump = 4;
			}
			else if (line.substr(jumptag, 3) == "JNE")
			{
				jump = 5;
			}
			else if (line.substr(jumptag, 3) == "JLE")
			{
				jump = 6;
			}
			else if (line.substr(jumptag, 3) == "JMP")
			{
				jump = 7;
			}
			line = line.substr(0, jumptag - 1);
		}
		if (equels > 0)
		{
			line = line.substr(equels + 1, line.length() - equels + 1);
		}
		int a = 0;
		int c = 0;
		if (line.size() ==  1)
		{
			if (line == "0")
			{
				c = 42;
			}
			else if (line == "1")
			{
				c = 63;
			}
			else if (line == "D")
			{
				c = 12;
			}
			else if (line == "A")
			{
				c = 48;
			}
			else if (line == "M")
			{
				a = 1;
				c = 48;
			}
		}
		else if (line.size() == 2)
		{
			if (line == "-1")
			{
				c = 58;
			}
			else if (line == "!D")
			{
				c = 13;
			}
			else if (line == "!A")
			{
				c = 49;
			}
			else if (line == "-D")
			{
				c = 15;
			}
			else if (line == "-A")
			{
				c = 51;
			}
			else if (line == "!M")
			{
				a = 1;
				c = 49;
			}
			else if (line == "-M")
			{
				a = 1;
				c = 51;
			}
		}
		else if (line.size() == 3)
		{
			if (line.substr(0, 1) == "D")
			{
				if (line.substr(2, 1) == "A" || line.substr(2, 1) == "M")
				{
					if (line.substr(2, 1) == "M")
					{
						a = 1;
					}
					if (line.substr(1, 1) == "+")
					{
						c = 2;
					}
					else if(line.substr(1, 1) == "-")
					{
						c = 19;
					}
					else if (line.substr(1, 1) == "&")
					{
						c = 0;
					}
					else if (line.substr(1, 1) == "|")
					{
						c = 21;
					}

				}
				else if (line.substr(1, 1) == "+")
				{
					c = 31;
				}
				else 
				{
					c = 14;
				}

			}
			else if (line.substr(1, 2) == "+1")
			{
				if (line.substr(0, 1) == "M")
				{
					a = 1;
				}
				c = 55;
			}
			else if (line.substr(1, 2) == "-1")
			{
				if (line.substr(0, 1) == "M")
				{
					a = 1;
				}
				c = 50;
			}
			else
			{
				if (line.substr(0, 1) == "M")
				{
					a = 1;
				}
				c = 7;
			}

		}
		line = "111" + std::bitset<1>(a).to_string() + std::bitset<6>(c).to_string() + std::bitset<3>(dest).to_string() + std::bitset<3>(jump).to_string();


	}
	//line.insert(12, " ");
	//line.insert(8, " ");
	//line.insert(4, " ");
	return line;
}
