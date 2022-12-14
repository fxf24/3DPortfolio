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

	Vector3 dir;
	float JumpAttackRadius;
	float AttackRadius;
	float attackRange;
	Vector3 jumpFrom, jumpTo;
	float jumpLength;
	float jumpLerp;

public:
	float hp;

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
	float GetMovementSpeed() { return movementSpeed; }
	float GetAttackSpeed() { return attackSpeed; }
};
