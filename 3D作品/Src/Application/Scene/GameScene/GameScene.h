#pragma once

#include "../BaseScene/BaseScene.h"

class City;
class Field;
class Player;

class GameScene :public BaseScene
{
public:
	GameScene() { Init(); }
	~GameScene() {}
private:

	void Event()override;
	void Init()override;

	
	std::weak_ptr<City> m_city;
	std::weak_ptr<Field> m_field;
	std::weak_ptr<Player> m_player;
	

};