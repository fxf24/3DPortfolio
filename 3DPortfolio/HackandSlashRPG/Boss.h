#pragma once

enum class BossState
{
	IDLE,
	WALK,
	RUN,
	ATTACK,
	JUMPATTACK,
	DIE,
};

class Boss : public Actor
{
private:
	BossState state;
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
	void JumpAttack();
	void Die();
	Boss();
	void Update();
	void WorldUpdate();
	Vector3 GetLastPos() { return lastPos; }
	Vector3 GetVelocity() { return velocity; }
	float GetMovementSpeed() { return movementSpeed; }
	float GetAttackSpeed() { return attackSpeed; }
};
