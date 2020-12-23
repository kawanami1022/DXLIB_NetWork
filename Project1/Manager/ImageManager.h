#pragma once
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>
#include <string>

#define IpImage ImageManager::GethInstance()

using pairImageHandle = std::pair < std::vector<int*>, std::vector<int>>;

class ImageManager
{
public:
	static ImageManager& GethInstance()
	{
		Create();
		return *hInstance;
	}
	static void Create()
	{
		if (hInstance == nullptr)
		{
			hInstance = new ImageManager();
		}
	}
	static void Destroy()
	{
		delete hInstance;
	}

	bool LoadTexture(std::string ImageName, std::string FilePath);
	bool LoadDivTexture(std::string ImageName, std::string FilePath,int xNum,int yNum, int allNum);
	pairImageHandle GetTxHdl(std::string ImageName);
private:

	std::unordered_map<std::string, std::string> fileName_;
	std::unordered_map<std::string, pairImageHandle> imageHandle_;

	static ImageManager *hInstance;
	ImageManager();
	~ImageManager();
};

