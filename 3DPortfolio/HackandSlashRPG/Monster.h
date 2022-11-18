#pragma once

class Monster : public Actor
{
private:
	PlayerState state;
	Vector3     lastPos;
	float movementSpeed;
	float attackSpeed;
	float hp;

private:

public:
	void Idle();
	void Walk();
	void Run();
	void Attack();
	Monster();
	void Update();
	void WorldUpdate();
	Vector3 GetLastPos() { return lastPos; }
	float GetMovementSpeed() { return movementSpeed; }
	float GetAttackSpeed() { return attackSpeed; }
};
