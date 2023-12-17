#pragma once

class Scene3 : public Scene
{
private:
	Deferred* deferred;   //4∞≥¿« ∑ª¥ı≈∏∞Ÿ  
	thread* Thread;
	Camera* cam;
	UI* BG;


public:
	Scene3();
	~Scene3();

	virtual void Init() override;
	virtual void Release() override; //«ÿ¡¶
	virtual void Update() override;
	virtual void LateUpdate() override;//∞ªΩ≈
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};
