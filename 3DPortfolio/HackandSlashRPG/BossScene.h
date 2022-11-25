#pragma once

class BossScene : public Scene
{
private:
	Camera* Cam;

	Sky* sky;
	//Player* player;
	Ray		cubeManTopRay;

	Terrain* Map;

	Shadow* shadow;
	PostEffect* postEffect = nullptr;

	Vector3			from, to;
	float			lerpValue;

	float			Rfrom, Rto;
	float			RlerpValue;

	bool findPath = false;
	int route = -1;
	vector<Vector3> path;

	Projectile* pjPool[50];
	Boss* boss;
public:
	BossScene();
	~BossScene();
	virtual void Init() override;
	virtual void Release() override; //해제
	virtual void Update() override;
	virtual void LateUpdate() override;//갱신
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void ResizeScreen() override;
};