#pragma once

//struct Brush
//{
//	int		shape = 0;
//	int		texture = 1;
//	int     type = 0;
//	float 	range = 10.0f;
//	float	YScale = 3.0f;
//};

enum class BrushShape
{
	Circle,
	Rect
};

enum class BrushType
{
	Linear,
	Smooth,
	Add,
	Flat
};

class Main : public Scene
{

private:
	struct Brush
	{
		Vector3 point = Vector3(0, 0, 0);
		float	range = 10.0f;

		float	shape = 0;
		float	type = 0;
		Vector2 padding;
	} _brush;
	ID3D11Buffer* brushBuffer;
	float buildSpeed = 10.0f;
	float paintSpeed = 1.0f;

	BrushShape brushShape = BrushShape::Circle;
	BrushType brushType = BrushType::Linear;
	Vector4 paint = Vector4(1, -1, -1, -1);

	Camera* cam1;
	Grid* grid;

	Terrain* map;
	Terrain* watermap;

	float WaterHeight;

	Actor* Tree;
	int TreeNum;


public:
	Main();
	~Main();

	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
	void CreateTree();
};