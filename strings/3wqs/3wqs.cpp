#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <array>

std::vector<std::string> keys;	

const unsigned int MAX_CHARS = UCHAR_MAX + 1;

void exch(size_t ii, size_t jj)
{
	std::string aux;

	aux = keys[ii];
	keys[ii] = keys[jj];
	keys[jj] = aux;
}

void sort(size_t ll, size_t rr, size_t dd)
{
	unsigned char pivot = 0;
	//size_t xx = 0;
	size_t lt = 0;
	size_t ii;
	size_t gt = rr - 1; 

	if ((rr - ll) <= 1)
	{
		return;
	}

#ifdef _DEBUG
	std::cout << std::endl << "dd = " << dd << ", ll = " << ll << ", rr = " << rr << std::endl; 
	std::cout << "Antes:" << std::endl; 
	for (ii = ll; ii < rr; ++ii)
	{
		std::cout << keys[ii] << std::endl;
	}
#endif

	for (ii = ll; ii < rr; ++ii)
	{
		if (keys[ii].size() > dd)
		{
			pivot = keys[ii][dd];
			lt = ii;
			break;
		}
	}

	if (pivot == 0) //all keys are lesser than d or array size lesser than 2
	{
		return;
	}

	ii = lt + 1;

	while (ii <= gt)
	{
		if ((keys[ii].size() <= dd) || (keys[ii][dd] < pivot))
		{
			exch(lt++, ii++);
		}
		else if (keys[ii][dd] > pivot) 
		{
			exch(gt--, ii);
		}
		else
		{
			++ii;
		}
	}

#ifdef _DEBUG
	std::cout << "Depois:" << std::endl; 
	for (ii = ll; ii < rr; ++ii)
	{
		std::cout << keys[ii] << std::endl;
	}
	std::cout << "lt = " << lt << ", gt = " << gt << std::endl; 
#endif

	sort(ll, lt, dd);
	sort(lt, gt + 1, dd + 1);
	sort(gt + 1, rr, dd);	

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

	sort(0, keys.size(), 0);

//	std::cout << std::endl << "Resultado:" << std::endl;	

	for (i = 0; i < keys.size(); ++i)
	{
		std::cout << keys[i] << std::endl;			
	}

	return 0;
}
