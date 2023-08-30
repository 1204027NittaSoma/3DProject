#pragma once

class BaseScene
{
public:
	BaseScene() { Init(); }
	~BaseScene(){}

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void DrawSprite();
	void DrawDebug();
	
//	virtual void Release();

	// �Q�Ɓi���j
	// const(�萔)
	const std::list<std::shared_ptr<KdGameObject>>& GetObjList()
	{
		return m_objList;
	}

	void AddObject(const std::shared_ptr<KdGameObject>& _obj)
	{
		m_objList.push_back(_obj);
	}

protected:

	virtual void Event();
	virtual void Init();

	std::list<std::shared_ptr<KdGameObject>> m_objList;
};