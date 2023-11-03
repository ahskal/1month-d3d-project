#pragma once

class Scene1 : public Scene
{
private:
    Camera* cam1;
    Grid* grid;
    Terrain* map;
    class Zamong* player;
    Sky* skybox;
    Sky* skybox2;

    Actor* sphere;
    CubeRenderTarget* environmentMap;
    float environmentMapSize;

    CubeRenderTarget* shadowMap;
    float shadowMapSize;

    class Mutant* mutant;

    Rain* rain;

public:
    Scene1();
    ~Scene1();

    // Scene을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Release() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void ResizeScreen() override;
};

