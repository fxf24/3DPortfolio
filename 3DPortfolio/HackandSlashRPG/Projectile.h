#pragma once
class Projectile : public GameObject
{
private:
	Vector3 direction;
	float speed = 1.0f;

public:
	float damage = 10.0f;


public:
	Projectile();
	void Shoot(Vector3 pos, Vector3 dir, float speed);
	void Update() override;
};

