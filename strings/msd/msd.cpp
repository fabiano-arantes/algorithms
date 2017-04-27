#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <array>

std::vector<std::string> keys;	
std::vector<std::string> aux;

const unsigned int MAX_CHARS = UCHAR_MAX + 1;

void sort(size_t l, size_t r, size_t d)
{
	std::array<size_t, MAX_CHARS> count;

	size_t i, x = 0;

	//tem que haver pelo menos 2 elementos no subarray
	if ((r - l) < 2)
	{
		return;
	}

	//zera contador
	for(i = 0; i < count.size(); ++i) count[i] = 0;

#ifdef _DEBUG
	std::cout << std::endl << "d = " << d << ", l = " << l << ", r = " << r << std::endl; 
	for (i = l; i < r && d > 0; ++i)
	{
		std::cout << keys[i] << std::endl;
	}
#endif

	for (i = l; i < r; ++i)
	{
		if (keys[i].size() > d)
		{
			++count[keys[i][d]];			
		}
	}

#ifdef _DEBUG	
	std::cout << std::endl << "Contagem: " << std::endl;	

	for (i = 0; i < count.size(); ++i)
	{
		if(count[i] > 0)
		{
			std::cout << (char) (i - 2) << "[" << (i - 2) << "]: " << (int)count[i] << " - ";			
		}
	}
#endif

	for (i = 1; i < count.size(); ++i)
	{
		count[i] += count[i - 1];
	}

#ifdef _DEBUG
	std::cout <<  std::endl << "Índices: " << std::endl;		

	for (i = 0; i < count.size() - 1; ++i)
	{
		if (count[i] != x)
		{
			std::cout << (char) i << "[" << i << "]: " << (int)count[i] << " ... ";			

			x = count[i];
		}
	}
#endif

	for (i = l; i < r; ++i)
	{
		if (keys[i].size() > d)
		{
			aux[count[keys[i][d] - 1]++] = keys[i];			
		}
	}

#ifdef _DEBUG
	std::cout <<  std::endl << "Índices 2: " << std::endl;		

	x = 0;

	for (i = 0; i < count.size() - 1; ++i)
	{
		if (count[i] != x)
		{
			std::cout << (char) i << "[" << i << "]: " << (int)count[i] << " ... ";

			x = count[i];		
		}
	}
#endif

	for (i = l; i < r; ++i)
	{
		//copy back
		keys[i] = aux[i - l];
	}

	for(i = 0; i < count.size() - 1; ++i)
	{
		sort(l + count[i], l + count[i + 1], d + 1);	
	}
}

int main (int argc, char **argv)
{
	std::ifstream ifs;
	size_t i;

	if(argc < 2)
	{
		std::cout << argv[0] << " file" << std::endl;

		return 1;
	}

	ifs.open(argv[1]);

	char buf[100];
	size_t s = 0;

	while (ifs.getline(buf, 100))
	{
		++s;
		keys.push_back(buf);
	}

#ifdef _DEBUG
	std::cout << "Linhas: " << s << std::endl;	
#endif

	aux.resize(keys.size());

	sort(0, keys.size(), 0);

//	std::cout << std::endl << "Resultado:" << std::endl;	

	for (i = 0; i < keys.size(); ++i)
	{
		std::cout << keys[i] << std::endl;			
	}

	return 0;
}
