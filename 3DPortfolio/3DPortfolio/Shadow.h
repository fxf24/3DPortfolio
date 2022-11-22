#pragma once
struct ShadowDesc
{
	int		ShadowQuality = 0;
	float	ShadowBias = 0.00325f;
	Vector2 Size = Vector2(0.1f,0.1f);
};

class Shadow : public RenderTarget
{
	ShadowDesc			desc;
	static ID3D11Buffer*		shadowPSBuffer;
	static ID3D11Buffer*		shadowVSBuffer;
	Camera*				cam;
	float				textureSize;
public:
	float				range;
	static void CreateStaticMember();
	static void DeleteStaticMember();

	Shadow();
	~Shadow();

	void SetCapture(Vector3 position);
	void SetTexture();
	void ResizeScreen(float textureSize);
	void RenderDetail();
};

