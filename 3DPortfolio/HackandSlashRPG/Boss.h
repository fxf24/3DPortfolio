#pragma once

class Boss : public Actor
{
private:
	PlayerState state;
	Vector3     lastPos;
	float movementSpeed;
	float attackSpeed;
	float hp;

private:
	Vector3 velocity;
	float neighborRadius;
	float desiredSeparation;
	float attackRange;

public:
	void Idle();
	void Walk();
	void Run();
	void Attack();
	Boss();
	void Update();
	void WorldUpdate();
	Vector3 GetLastPos() { return lastPos; }
	Vector3 GetVelocity() { return velocity; }
	float GetMovementSpeed() { return movementSpeed; }
	float GetAttackSpeed() { return attackSpeed; }
};
