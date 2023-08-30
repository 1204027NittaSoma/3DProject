#pragma once

class BaseScene;


class SceneManager
{
public:
	// シーン情報
	enum class SceneType
	{
		Title,
		Game,
	};
	
	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();
	void DrawDebug();

	// マネージャの初期化
	void Init()
	{
		ChangeScene(SceneType::Title);
	}

//	void Release();
	
	// ここではシーンは変わらない
	// あくまで予約
	void SetNextScene(SceneType _nextScene)
	{
		m_nextSceneType = _nextScene;
	}

	const std::list<std::shared_ptr<KdGameObject>>& GetObjList();

	void AddObject(const std::shared_ptr<KdGameObject>& _obj);

private:

	void ChangeScene(SceneType _sceneType);

	// 現在のシーンを管理しているポインタ
	std::shared_ptr<BaseScene> m_currentScene = nullptr;

	// 現在のシーンを管理しているデータ（変数）
	SceneType m_currentSceneType = SceneType::Title;

	// 次のシーンを管理しているデータ（変数）
	SceneType m_nextSceneType = m_currentSceneType;

// シングルトンパターン
	// 生成するインスタンスの数を１つに制限する
	// 便利すぎるが何でもかんでもシングルトンにしない！
private:
	SceneManager() { Init(); }
	~SceneManager(){}

public:

	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}
};