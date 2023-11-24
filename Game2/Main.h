#pragma once
class Main : public Scene
{

private:
	Camera* cam1;
	Grid* grid;

	Mesh* mesh;
	Shader* shader;
	struct EdgeBuffer
	{
		Vector4 edges;
		Vector2 inside;
		Vector2 padding;
	}edgeBufferDesc;
	ID3D11Buffer* edgeBuffer;



public:
	Main();
	~Main();
	virtual void Init() override;
	virtual void Release() override; //����
	virtual void Update() override;
	virtual void LateUpdate() override;//����
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};
