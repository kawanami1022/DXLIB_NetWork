#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <string>
#include "../strmanip/strmanip.h"
#include "TMX_File.h"


void File::Tiled_Map::prepreateTiles()
{
	titleData_.resize(static_cast<size_t>(width_ * height_));
	for (unsigned int idx = 0; idx < width_; idx++)
	{
		titleID_.emplace_back(&titleData_[idx * height_]);
	}
}

void File::Tiled_Map::SetTiles(int y, int width, std::string lineString)
{
	std::string num;
	std::stringstream ss;
	ss << lineString;

	for (int x = 0; x < width; x++)
	{
		std::getline(ss, num, ',');
		titleID_[x][y] = atoi(num.data());
		std::cout << titleID_[x][y];

	}
	std::cout << std::endl;
}




bool File::TMX_File::load_TMX(std::string FilePos)
{
	std::string lineString;
	std::stringstream ss;
	auto filePos = 0;

	std::fstream FileStream(FilePos, std::ios_base::in);

	auto opened = FileStream.is_open();

	if (!opened)
	{
		std::cout << "tmx file open failed!" << std::endl;
		return false;
	}
	else {
		std::cout << "tmx file open succeed!" << std::endl;
	}

	auto Escape = [&]() {if (FileStream.eof()) { return false; }return true; };

	//@pram: string for Extraction 
	auto GetStringPos = [&](std::string strForExt) {
		return static_cast<int>(lineString.find(strForExt));
	};

	std::string line;

	while (Escape())
	{

		std::cout << lineString << std::endl;

		if (this->mode_ == ReadTmxMode::INIT)
		{
			std::getline(FileStream, lineString);
			if (this->encoding.length() == 0)
			{
				this->encoding = strmanip::ExtractTheStrDblQt(lineString, "encoding");
				if (this->encoding.length() != 0)
					std::cout << this->encoding << std::endl;
			}

			if (this->orthogonal.length() == 0)
			{
				this->orthogonal = strmanip::ExtractTheStrDblQt(lineString, "orientation");
				if (this->orthogonal.length() != 0)
					std::cout << "orientation:" << this->orthogonal << std::endl;
			}

			if (this->width_ == 0)
			{
				this->width_ = atoi(strmanip::ExtractTheStrDblQt(lineString, "width").data());
				if (this->width_ != 0)
					std::cout << "width:" << this->width_ << std::endl;
			}

			if (this->height_ == 0)
			{
				this->height_ = atoi(strmanip::ExtractTheStrDblQt(lineString, "height").data());
				if (this->height_ != 0)
					std::cout << "height:" << this->height_ << std::endl;
			}

			if (this->tilewidth_ == 0)
			{
				this->tilewidth_ = atoi(strmanip::ExtractTheStrDblQt(lineString, "tilewidth").data());
				if (this->tilewidth_ != 0)
					std::cout << "tilewidth:" << this->tilewidth_ << std::endl;
			}

			if (this->tileheight_ == 0)
			{
				this->tileheight_ = atoi(strmanip::ExtractTheStrDblQt(lineString, "tileheight").data());
				if (this->tileheight_ != 0)
					std::cout << "tileheight:" << this->tileheight_ << std::endl;
			}

			if (this->nextlayerid_ == 0)
			{
				this->nextlayerid_ = atoi(strmanip::ExtractTheStrDblQt(lineString, "nextlayerid").data());
				if (this->nextlayerid_ != 0)
					std::cout << "nextlayerid:" << this->nextlayerid_ << std::endl;
			}

			if (lineString.find(" </editorsettings>") != std::string::npos)
			{
				this->mode_ = ReadTmxMode::GENERATE;
			}
		}

		if (this->mode_ == ReadTmxMode::GENERATE)
		{
			std::getline(FileStream, lineString);

			std::string str= strmanip::ExtractTheStrDblQt(lineString, "name");

			if (lineString.find("layer") != std::string::npos)
			{
				if (str.length() != 0)
				{
					this->name_.push_back(str);
					this->tiledMap_.try_emplace(str, Tiled_Map());
				}
			}
			if (this->name_.size() != 0) { str = this->name_.back(); }

			if (str.length() != 0)
			{
				if (lineString.find("data encoding") != std::string::npos)
				{
					this->tiledMap_[str].dataEncoding_ = strmanip::ExtractTheStrDblQt(lineString, "data encoding");
					if (this->tiledMap_[str].dataEncoding_.length() != 0)

						std::cout << this->tiledMap_[this->name_.back()].dataEncoding_ << std::endl;
					// embed the ID of the tile in this area.
					if (this->tiledMap_[this->name_.back()].dataEncoding_ == "csv")
					{
						this->tiledMap_[this->name_.back()].height_ = this->height_;
						this->tiledMap_[this->name_.back()].width_ = this->width_;
						if (this->tiledMap_[this->name_.back()].height_ != 0 &&
							this->tiledMap_[this->name_.back()].width_ != 0)
						{
							this->tiledMap_[this->name_.back()].prepreateTiles();
							this->mode_ = ReadTmxMode::INSERT;
							continue;
						}
						else
						{
							std::cout << "failed Inited!" << std::endl;
							return false;
						}
					}
				}
			}
		}

		if (this->mode_ == ReadTmxMode::INSERT)
		{
			unsigned int idx = 0;
			this->mode_ = ReadTmxMode::GENERATE;
			while (idx <= this->tiledMap_[this->name_.back()].height_-1)
			{
				//std::cout << "lineString:" << lineString << std::setw(20) << idx << std::endl;
				std::getline(FileStream, lineString);
				this->tiledMap_[this->name_.back()].SetTiles(idx,
					this->tiledMap_[this->name_.back()].width_, lineString);

				idx++;
			}

			while (lineString.find("</layer>") != std::string::npos)
			{

				std::getline(FileStream, lineString);
				std::cout << lineString << std::setw(20) << std::endl;
			}
		}
	}
	return true;
}
