#pragma once
class Scene1 : public Scene
{
private:
	Camera* Cam;

	Sky* sky;
	CubeMan* player;
	Ray		cubeManTopRay;

	Actor* Point;
	Actor* Arrow;
	Light* Point2;
	Terrain* Map;

	Int2   Pos;


	//첫번쨰 랜더타겟
	CubeMap* cubeMap = nullptr;
	Shader* cubeMappingShader;
	Shader* cubeMappingShader2;
	Shader* cubeMappingShader3;
	Actor*	sphere;

	//두번째 렌더 타겟
	RenderTarget* RT = nullptr;
	UI* PostEffect;
	float refractionIdx = 1.0f;

	Rain* rain;
	Rain* skill;
	float skill_time = 1.0f;

	Vector3			from, to;
	float			lerpValue;

	float			Rfrom, Rto;
	float			RlerpValue;

	bool findPath = false;
	int route = -1;
	vector<Vector3> path;
public:
	Scene1();
	~Scene1();
	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};

