#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
#include <mutex>
#include "../Actor.h"

constexpr auto CHAR_WIDTH = 32;
constexpr auto CHAR_HEIGHT = 51;
constexpr auto UNIT_ID_BASE = 5;		// Player��iD��5���݂ɂ���
class Bomb;
class Map;
struct controller;
enum class NetWorkMode;

// �擪��ID��def���Z�b�g����
// host	0 def
//			5  net
//			10 auto
//			15 net 
//			20 auto
//			25 net

//gest 0  net
//       5  def
//		  10 net
//		  15 auto 
//		  20 net
//		  25 auto

// DEFF (5):���� �����̏ꍇ(���葤���炷���NET)
// NET(10~)  : �l�b�g���[�N

enum class CharState :char
{
	Alive,
	Death,
};

enum class AnimState
{
	Idel,
	Walk,
	Max
};

enum class MoveDir
{
	Down,
	Left,
	Right,
	Up,
	Deth,
	Max,
};

class Character :public Actor
{	

public:
	Character();
	Character(Position2);
	~Character();
	void Update(std::weak_ptr<Map> map);

	void DeffUpdate(std::weak_ptr<Map> map);			// �R���g���[���[��L�[�{�[�h�ňړ�
	void NetUpdate(std::weak_ptr<Map> map);			// ��̏����l�b�g���[�N����󂯂Ƃ�
	void AutoUpdate(std::weak_ptr<Map> map);			// �I�[�g�p�C���b�g
	void Draw();

	void AutoMove(std::weak_ptr<Map>&& map);

	bool SendCharData();

	// Getter Setter
	void SetUpdateFunc(std::function<void(std::weak_ptr<Map >)> func);

	void SetPos(Position2 pos)
	{
		pos_ = pos;
	}

	void SetState(CharState state)
	{
		std::lock_guard<std::mutex> lock(mtx_);
		{
			charState_ = state;
		}
	}

	void SetDir(MoveDir moveDir)
	{
		moveDir_ = moveDir;
	}

	Position2 GetPos()
	{
		return pos_;
	}

	MoveDir GetMoveDir()
	{
		return moveDir_;
	}

	// GetPlaeyrId
	int GetPlID()
	{
		return playerID_;
	}
	Position2 GetDRPos()
	{
		return Position2(pos_.x + 32, pos_.y + 32);
	}
	// ���`*��u��
	bool GetSetBb()
	{
		return setBomb_;
	}

	CharState GetCharaState()
	{
		return charState_;
	}

protected:
	Vector2Tmp<float> speed = { 4.f,4.f };
	std::vector<int> HandleId_;
	std::vector<int*> HandleData_;

	std::unique_ptr<controller> controller_=nullptr;

	MoveDir moveDir_ = MoveDir::Right;
	Position2 posUL_;
	Position2 posDR_;

	std::unordered_map<NetWorkMode,std::function<void()>> InitFunc;
	std::function<void(std::weak_ptr<Map >)> updateFunc_=nullptr;
	std::weak_ptr<Map> map_;

private:
	static int Id_;
	int playerID_;
	int animcnt_ = 0;
	std::mutex mtx_;
	CharState charState_=CharState::Alive;
	bool setBomb_;
	bool Init();

	// �ʒu����

	void MatchGridPos(Position2 plPos);
};

