#pragma once

enum class ZamongState
{
    IDLE,
    MOVE
};

class Zamong : public Actor
{

public:

    static Zamong* Create(string name = "Zamong");
    static Zamong* Create(Zamong* src);
private:
    ZamongState state;
    Vector3 dir;
    
    float	speed;

    float	time;

    float   rotSpeed;
    float   rotTime;
    class Fruit*  fruit[3];

    Vector3 last;

    Zamong();
    virtual ~Zamong();
public:
    void	Update() override;
    void	Render(shared_ptr<Shader> pShader = nullptr) override;

    void    Move(Vector3 Target);
    void    Fire(Vector3 dest, float power);
    Vector3 GetLast() { return last; };
    void    WolrdUpdate();
    ZamongState     GetState() { return state; }
};

