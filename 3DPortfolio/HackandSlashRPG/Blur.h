#pragma once
struct Blur
{
	int count;
	float width;
	float height;
	int select;
	Color blendColor = Color(0.5f, 0.5f, 0.5f, 0.5f);
	Vector2  center;
	float   radius = 2000.0f;
	float   padding;
};

class BlurManager :public Singleton< BlurManager>
{
	ID3D11Buffer* blurBuffer;
public:
	BlurManager();
	~BlurManager();
	Blur	blur;
	void Update();
	void Set();
};

