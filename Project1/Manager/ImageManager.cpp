#include <DxLib.h>
#include "ImageManager.h"

ImageManager ImageManager::hInstance;

bool ImageManager::LoadTexture(std::string ImageName, std::string FilePath)
{
	// this ImageName  already Exists
	if (fileName_.find(ImageName) == fileName_.end())return false;
	
	fileName_.try_emplace(ImageName);	// map's key add in ImageName
	fileName_[ImageName] = FilePath;	// substruct FilePath

	imageHandle_.try_emplace(FilePath);
	int Handle;
	// check successful load Image
	if ((Handle = LoadGraph(FilePath.data(), true)) == -1)return false;
	imageHandle_[FilePath].second = { Handle };
	imageHandle_[FilePath].first[0] = &imageHandle_[FilePath].second[0];
	return true;
	
}

bool ImageManager::LoadDivTexture(std::string ImageName, std::string FilePath, int xNum, int yNum,int allNum)
{
	// this ImageName  already Exists
	if (fileName_.find(ImageName) != fileName_.end())return false;


	fileName_.try_emplace(ImageName);	// map's key add in ImageName
	fileName_[ImageName] = FilePath;	// substruct FilePath

	imageHandle_.try_emplace(FilePath);
	int width, height;
	
	int Handle;
	// check successful load Image
	if (Handle = LoadGraph(FilePath.data(), true) == -1)return false;
	GetGraphSize(Handle, &width, &height);
	imageHandle_[FilePath].second.resize(allNum);
	for (int  no= 0; no < width; no++)
	{
		imageHandle_[FilePath].first.emplace_back(&imageHandle_[FilePath].second[no * height]);
	}

	LoadDivGraph(FilePath.data(), allNum, xNum, yNum, width / xNum, height / yNum, &imageHandle_[FilePath].second[0]);

	return true;
}

// GetTexture
pairImageHandle ImageManager::GetTxHdl(std::string ImageName)
{
	return imageHandle_[fileName_[ImageName]];
}

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
}
