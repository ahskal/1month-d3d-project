#pragma once
class Deferred
{

private:
    RenderTarget* diffuseTarget;
    RenderTarget* specularTarget;
    RenderTarget* normalTarget;
    RenderTarget* posTarget;
    RenderTarget* ambientTarget;
    RenderTarget* emissiveTarget;

    RenderTarget* rtvs[6];
    ID3D11ShaderResourceView* srvs[6];
public:
    Deferred(UINT width = App.GetWidth(),
        UINT height = App.GetHeight());
    ~Deferred();
    void SetTarget(Color clear = Color(0, 0, 0, 1));
    void SetRGBTexture(int slot);
    //â������ ����
    void ResizeScreen(float width, float height);
    void RenderDetail();
};

