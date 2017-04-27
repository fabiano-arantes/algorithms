#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <array>

int main (int argc, char **argv)
{
	std::vector<std::string> input;	
	std::vector<std::string> output;

	std::ifstream ifs;
	std::array<size_t, UCHAR_MAX + 1> count;
	size_t i;

	if(argc < 2)
	{
		std::cout << argv[0] << " file" << std::endl;

		return 1;
	}

	ifs.open(argv[1]);

	char buf[100];
	size_t s = 0;

	ifs.getline(buf, 100); //para obter o tamanho da linha
	
	std::streamsize lineSize = ifs.gcount();

	do
	{
		++s;

		buf[lineSize] = 0;

		input.push_back(buf);
	}
	while (ifs.getline(buf, 100));

	std::vector<std::string>::iterator it;

	--lineSize; //retira new line

	std::cout << "Linhas: " << s << std::endl;	
	std::cout << "Caracteres: " << lineSize << std::endl;	

	while(lineSize--)
	{
		//zera contador
		for(i = 0; i < count.size(); ++ i) count[i] = 0;

		for (it = input.begin(); it != input.end(); ++it)
		{
			++count[(*it)[lineSize] + 1];
		}
/*
		std::cout << "Contagem: " << std::endl;	
		
		for (i = 0; i < count.size(); ++i)
		{
			if(count[i] > 0)
			{
				std::cout << (char) (i - 1) << ": " << (int)count[i] << std::endl;			
			}
		}
*/
		for (i = 1; i < count.size(); ++i)
		{
			count[i] += count[i - 1];
		}

/*		std::cout << "Índices: " << std::endl;		

		for (i = 0; i < count.size(); ++i)
		{
			std::cout << (char) i << ": " << (int)count[i] << std::endl;			
		}
*/
		output.resize(input.size());

		for (it = input.begin(); it != input.end(); ++it)
		{
			output[count[(*it)[lineSize]]++] = *it;
		}

		//copy back
		input = output;
	}

	/*std::cout << "Resultado:" << std::endl;	

	for (it = output.begin(); it != output.end(); ++it)
	{
		std::cout << *it << std::endl;			
	}*/


	return 0;
}
