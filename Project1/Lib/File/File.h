#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <list>

namespace File
{
	//目的	:位置行ごとに文字列を読み込む
	//入力	:int lineSize				行数	
	//		:std::string* lineString	1行分の文字列
	//		:std::string FileName		ファイル名
	int GetLineString(int lineSize, std::string* lineString, std::string FileName);

	//目的	:マップデータ用txtファイル
	//入力	:width	横
	//		:hight	高さ
	//		:FileName ファイル名	
	bool GenerateMapData(int width, int hight, std::string FileName);

	//目的	:x軸y軸それぞれのチップ数を求める
	//入力	:T *x
	//		:T *y
	//		:FileName
	template<typename T>
	int GetMapSizeForTxt(T* x, T* y, std::string FileName, char countChar);

	//目的	:x軸y軸それぞれのチップ数を求める
	//入力	:T x
	//		:T y
	//		:T *mapChipData
	//		:FileName
	template<typename T>
	int LoadMapDataForTxt(T size_x, T size_y, T* mapChipData, std::string FileName, char checkChar);

}

#include "detail/File.h"