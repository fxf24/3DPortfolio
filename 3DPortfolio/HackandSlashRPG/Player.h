#pragma once
enum class PlayerState
{
	IDLE,
	WALK,
	RUN,
	ATTACK,
};

class Player : public Actor, public Singleton<Player>
{
private:
	PlayerState state;
	Vector3     lastPos;
	float movementSpeed;
	float attackSpeed;

private:

public:
	void Idle();
	void Walk();
	void Run();
	void Attack();
	Player();
	void Update();
	void WorldUpdate();
	Vector3 GetLastPos() { return lastPos; }
	float GetMovementSpeed() { return movementSpeed; }
	float GetAttackSpeed() { return attackSpeed; }
};

