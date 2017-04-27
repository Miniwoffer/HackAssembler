
#include "assembler.h"

int main(int argc, char* argv[])
{
	std::cout << "Assembler 0.1 .asm -> .hack assembler by Odin Hultgren Van der Horst" << std::endl;
	if (argv[1] == NULL)
	{
		return 1;
	}
	assembler comp(argv[1]);
	return 0;
}