#pragma once
class Fruit : public Actor
{
private:
    Vector3 dir;
    float   power = 0.0f;

    float   gravity = 0.0f;
    bool    isFire = false;
    float  boomPower;

    float   time = 0.0f;

    Vector3 p0;
    Vector3 p1;
    Vector3 p2;
public:
    static Fruit* Create(string name = "Fruit");
private:
    Fruit();
    virtual ~Fruit();
public:
    void	Update() override;
    void	Render(shared_ptr<Shader> pShader = nullptr) override;
    bool    Fire(Vector3 pos,Vector3 dir, float power);
    //보간할때 쓸 값
    bool    Fire2(Vector3 p0, Vector3 p1, Vector3 p2, float power);
    void    Boom();
    bool GetIsFire() { return isFire; }

};

