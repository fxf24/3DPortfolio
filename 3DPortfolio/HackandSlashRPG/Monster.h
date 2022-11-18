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
	Vector3 velocity;
	float neighborRadius;
	float desiredSeparation;
	void Cohesion(Monster* neighbors[] , int length);
	void Alignment(Monster* neighbors[], int length);
	void Seperation(Monster* neighbors[], int length);
	void SteerTo(Vector3 target);


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
