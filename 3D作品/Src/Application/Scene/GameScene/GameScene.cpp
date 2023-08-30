#include "GameScene.h"

#include "../SceneManager.h"
#include "../../Object/Ground/Ground.h"
#include "../../Object/Chara/Player/Player.h"
#include "../../Object/Chara/Enemy/Enemy.h"
#include "../../Object/Map/City/City.h"
#include "../../Object/Map/Field/Field.h"

#include "../../Object/Camera/TPSCamera/TPSCamera.h"

void GameScene::Event()
{
	if (GetAsyncKeyState('P') & 0x8000)
	{
		SceneManager::Instance().SetNextScene(SceneManager::SceneType::Title);
	}

	if (m_city.expired() == false)
	{
		if (m_city.lock()->GetEntryDangeon())
		{
			m_player.lock()->SetPos({ 10,0,5 });
			m_field.lock()->SetEntryDangeon(m_city.lock()->GetEntryDangeon());
			m_city.lock()->ReleaseCity();
		}
	}
	

}

void GameScene::Init()
{	
	ShowCursor(false);

	//ƒJƒƒ‰(‹–ìŠp)
	std::shared_ptr<TPSCamera> camera;
	camera = std::make_shared<TPSCamera>();
	m_objList.push_back(camera);

	std::shared_ptr<City> city;
	city = std::make_shared<City>();
	m_objList.push_back(city);

	m_city = city;

	std::shared_ptr<Player> player;
	player = std::make_shared<Player>();
	camera->SetTarget(player);
	player->SetCamera(camera);
	m_objList.push_back(player);

	m_player = player;

	std::shared_ptr<Enemy> enemy;
	enemy = std::make_shared<Enemy>();
	m_objList.push_back(enemy);

	enemy = std::make_shared<Enemy>();
	enemy->SetPos({ -5,0,10 });
	m_objList.push_back(enemy);

	enemy = std::make_shared<Enemy>();
	enemy->SetPos({ 0,0,10 });
	m_objList.push_back(enemy);

	enemy = std::make_shared<Enemy>();
	enemy->SetPos({ 5,0,10 });
	m_objList.push_back(enemy);

	enemy = std::make_shared<Enemy>();
	enemy->SetPos({ 10,0,10 });
	m_objList.push_back(enemy);


	
	std::shared_ptr<Field> field;
	field = std::make_shared<Field>();	
	m_objList.push_back(field);

	m_field = field;

}
