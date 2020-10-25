#include "File.h"

int File::GetLineString(int lineSize, std::string* lineString, std::string FileName)
{
	auto filePos = 0;
	//int lineCounter = 0;
	std::fstream fs(FileName, std::ios::in);

	if (!fs)
	{
		std::cout << "file not opened!\n" << std::endl;
		return -1;
	}
	for (int lineCounter=0; lineCounter<lineSize;lineCounter++)
	{
		auto lineSize = 0;
		getline(fs, lineString[lineCounter]);
		lineSize = static_cast<int>(lineString[lineCounter].size());
		filePos = static_cast<int>(fs.tellg());
		fs.seekp(filePos, std::ios::beg);
	}

	return 0;
}

bool File::GenerateMapData(int x, int y, std::string FileName)
{
	std::fstream fs(FileName);
	if (!fs)
	{
		std::cout << "file is not opened!" << std::endl;
		std::ofstream ofs(FileName);
		GenerateMapData(x, y, FileName);
	}
	for (int i = 0; i < x * y; i++)
	{
		if (i % x == 0&&i!=0)
			fs << std::endl;
		fs << "0,";
	}

	return true;
	
	
}
