#pragma once
#include <array>
#include "BaseScene.h"

enum class UpdateMode
{
	SetNetWork,			// �z�X�g/�Q�X�g/�I�t���C���̑I��
	SetHostIP,			// �Q�X�g�I������IP�A�h���X�ݒ�
	StartInit,			// �X�^�[�g�O�̏�����
	Play,				// �Q�[����
	Max
};


class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	void Init();

private:

	UniqueBase input(UniqueBase);
	UniqueBase UpDate(UniqueBase);
	void Draw();


	// �ϐ�
	UpdateMode mode;
	typedef void (TitleScene::* UpdateFunc)();
	std::array<UpdateFunc, static_cast<int>(UpdateMode::Max)> drawFunc;


	int screen_size_x=800;
	int screen_size_y=600;


	int pos_x=300;
	int pos_y=300;
	int Handle;

};

