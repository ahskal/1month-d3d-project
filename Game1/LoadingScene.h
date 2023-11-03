#pragma once
class LoadingScene :public Scene
{
private:
    Camera* cam1;
    Grid* grid;
    UI* bono;
    thread* t1;
public:
    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Release() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void ResizeScreen() override;
};

