#pragma once
#define SEP_WEIGHT 10.0f
#define ALN_WEIGHT 1.0f
#define COH_WEIGHT 1.0f

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
	Vector3 Cohesion(Monster* neighbors[] , int length);
	Vector3 Alignment(Monster* neighbors[], int length);
	Vector3 Seperation(Monster* neighbors[], int length);
	Vector3 SteerTo(Vector3 target);
	Vector3 Flock(Monster* neighbors[], int length);


public:
	void Idle();
	void Walk();
	void Run();
	void Attack();
	Monster();
	void Update(Monster* neighbors[], int length);
	void WorldUpdate();
	Vector3 GetLastPos() { return lastPos; }
	Vector3 GetVelocity() { return velocity; }
	float GetMovementSpeed() { return movementSpeed; }
	float GetAttackSpeed() { return attackSpeed; }
};
