#include "stdafx.h"
#include "Mutant.h"

Mutant::Mutant()
{
    actor = Actor::Create();
    actor->LoadFile("Mutant.xml");
    slash = new SlashTrail();
    slash->Top = actor->Find("Start");
    slash->Bottom = actor->Find("End");

    slash->interval = 0.004f;
    slash->maxTrail = 50;
    slash->material->diffuseMap = RESOURCE->textures.Load("trail.png");
    slash->material->diffuse.w = 1.0f;

    slash->material->diffuse.x = 0.0f;
    slash->material->diffuse.y = 0.0f;
    slash->material->diffuse.z = 1.0f;

}

Mutant::~Mutant()
{
}

void Mutant::Update()
{
    if (INPUT->KeyDown('Z'))
    {
        actor->anim->ChangeAnimation(AnimationState::ONCE_LAST,0, 0.1f);
      
    }
    if (slash->isPlaying == false and actor->anim->GetPlayTime() >= 0.3f)
    {
        slash->Play();
    }
    if (slash->isPlaying == true and actor->anim->GetPlayTime() >= 0.9f)
    {
        slash->Stop();
    }
   
   

    slash->Update();
    actor->Update();
}

void Mutant::Render(shared_ptr<Shader> pShader)
{
    actor->Render(pShader);
    slash->Render();
}
