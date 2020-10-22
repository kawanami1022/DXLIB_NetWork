#pragma once
#pragma once
#include <unordered_map>
#include <memory>
#include <string>
namespace File
{
	enum class ReadTmxMode
	{
		INIT,
		GENERATE,
		INSERT,
	};


	struct Tiled_Map
	{
		unsigned int width_ = 0;
		unsigned int height_ = 0;
		std::string dataEncoding_;
		std::vector<int*> titleID_;
		std::vector <int> titleData_;
		void prepreateTiles();
		void SetTiles(int y, int width, std::string);

	};


	class TMX_File
	{
	public:
		ReadTmxMode mode_ = ReadTmxMode::INIT;
		std::string encoding;
		std::string orthogonal;

		std::vector<std::string> name_;
		std::unordered_map<std::string, Tiled_Map> tiledMap_;
		unsigned int nextlayerid_ = 0;

		int width_ = 0;
		int height_ = 0;

		int tilewidth_ = 0;
		int tileheight_ = 0;

		bool load_TMX(std::string FilePos);
	};

}
