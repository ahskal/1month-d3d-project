#pragma once
class Mutant
{
private:
public:
    SlashTrail* slash;
    Actor* actor;
    Mutant();
    ~Mutant();

    void Update();
    void Render(shared_ptr<Shader> pShader = nullptr);
};

