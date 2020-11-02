#pragma once
#include <unordered_map>
#include <array>
#include <functional>
#include "BaseScene.h"
enum class UpdateMode;

namespace File {
	enum class ReadTmxMode;
	struct Tiled_Map;
	class TMX_File;
}

class Timer;
using UpdateMapFunc = std::unordered_map<UpdateMode, std::function<void()>>;

enum class UpdateMode
{
	SetNetWork,			// �z�X�g/�Q�X�g/�I�t���C���̑I��
	SetHostIP,			// �Q�X�g�I������IP�A�h���X�ݒ�
	StartInit,			// �X�^�[�g�O�̏�����
	Play,				// �Q�[����
	Max
};


class LoginScene :
	public BaseScene
{
public:
	LoginScene();
	~LoginScene();

	void Init();

private:

	UniqueBase input(UniqueBase);
	UniqueBase UpDate(UniqueBase);
	void Draw();
	void DrawOwnScreen();
	//updateFunc_
	void SetNetWork();
	void SetHostIP();
	void StartInit();
	void Play();
	//DrawFunc_
	void SetNetWorkDraw();
	void SetHostIPDraw();
	void StartInitDraw();
	void PlayDraw();

	// �ϐ�
	UpdateMode mode_;
	UpdateMapFunc updateFunc_;
	UpdateMapFunc DrawFunc_;
	

	std::shared_ptr< File::TMX_File> tmxFile_;
	std::unique_ptr<int[]> tileHandle_;


	int screen_size_x=800;
	int screen_size_y=600;

	int pos_x=300;
	int pos_y=300;
	int Handle;

};
