#pragma once
#include "../File.h"

//目的	:x軸y軸それぞれのチップ数を求める
//入力	:T *x
//		:T *y
//		:FileName
template<typename T>
int File::GetMapSizeForTxt(T* x, T* y, std::string FileName, char countChar)
{
	char checkChar;
	int lineSize = 0;	//	行ごとのファイルの大きさ
	int filePos = 0;	// ファイルの位置

	std::fstream fs(FileName, std::ios_base::in);

	// ファイルの読み込みに成功
	if (fs)
	{
		// 1行分のサイズを取得
		std::string line;
		getline(fs, line);
		lineSize = (int)(fs.tellg());
		fs.seekp(0, std::ios::beg);
		// x軸のマップチップ数取得
		while (lineSize > filePos)
		{
			// 現在の読み取り位置を取得
			// 一文字ずつ読み込む
			fs.read(&checkChar, 1);
			filePos = (int)(fs.tellg());

			fs.seekp(fs.tellp());

			// ","がないか確認する
			if (checkChar == countChar)
			{
				(*x)++;
			}
		}
		// ファイルを初期位置に戻す
		fs.seekp(0, std::ios::beg);
		// y軸のマップチップ数取得
		// ファイルの終端
		while (!fs.eof())
		{
			getline(fs, line);
			lineSize = line.length();
			filePos += lineSize;
			fs.seekp(fs.tellp());
			(*y)++;
		}
		fs.close();
		return 0;
	}
	return -1;
}

template<typename T>
int File::LoadMapDataForTxt(T size_x, T size_y, T* mapChipData, std::string FileName, char checkChar)
{
	int lineSize = 0;	// 行の大きさ

	std::fstream fs(FileName, std::ios_base::in);
	// false:ファイル読み込み失敗
	if (!fs.bad())
	{

		fs.seekp(0, std::ios::beg);


		for (int y = 0; y < size_y; y++)
		{
			int strPos = 0;
			// 1行分のサイズを取得
			std::string line;
			std::string getChipID;

			getline(fs, line);
			lineSize = (int)(fs.tellg());

			for (int x = 0; x < size_x; x++)
			{
				while (line[strPos] != checkChar)
				{
					getChipID.push_back(line[strPos]);
					strPos++;
				}
				strPos++;
				(*mapChipData) = stoi(getChipID);
				*mapChipData++;
				getChipID = "\0";
			}

			fs.seekp(fs.tellp());
		}

		return 0;
		fs.close();
	}
	return -1;
}
