#include <sstream>
#include <string>
#include "strmanip.h"

std::string strmanip::ExtractTheStrDblQt(std::string lineString, std::string stringId)
{
	std::string extStr;
	//@pram: string for Extraction 
	auto GetStringPos = [&](std::string strForExt) {
		return static_cast<int>(lineString.find(strForExt));
	};
	auto chrPos = GetStringPos(stringId);


	if (chrPos != std::string::npos)
	{
		chrPos += static_cast<int>(stringId.length());
		auto idx = 0;		//double quotation  counter
		while (chrPos < lineString.length())
		{
			auto checkChar = lineString.at(chrPos);
			if (checkChar == '"') { idx++; 	chrPos++; }
			if (idx == 1)
			{
				checkChar = lineString.at(chrPos);
				if (checkChar == '"')
				{
					idx++;
				}
				else
				{
					extStr.push_back(checkChar);
				}
			}

			if (idx >= 2) { break; }
			chrPos++;
		}

	}

	return extStr;
}