#include "stdafx.h"

Vector3 Monster::Cohesion(Monster* neighbors[], int length)
{
	Vector3 sum = Vector3(0, 0, 0);
	int count = 0;
	for (int i = 0; i < length; i++)
	{
		if (neighbors[i])
		{
			Vector3 d = neighbors[i]->GetWorldPos() - this->GetWorldPos();
			float dis = d.Length();
			if (dis != 0 && dis < neighborRadius)
			{
				sum += neighbors[i]->GetWorldPos();
				count++;
			}
		}
	}

	if (count != 0)
	{
		return SteerTo(sum / count);
	}
	else
	{
		return sum;
	}
}

Vector3 Monster::Alignment(Monster* neighbors[], int length)
{
	Vector3 mean = Vector3(0, 0, 0);
	int count = 0;
	for (int i = 0; i < length; i++)
	{
		if (neighbors[i])
		{
			Vector3 d = neighbors[i]->GetWorldPos() - this->GetWorldPos();
			float dis = d.Length();
			if (dis != 0 && dis < neighborRadius)
			{
				mean += neighbors[i]->GetVelocity();
				count++;
			}
		}
	}

	if (count != 0)
	{
		mean /= count;
	}

	return mean;
}

Vector3 Monster::Seperation(Monster* neighbors[], int length)
{
	Vector3 mean = Vector3(0, 0, 0);
	int count = 0;
	for (int i = 0; i < length; i++)
	{
		if (neighbors[i])
		{
			Vector3 d = neighbors[i]->GetWorldPos() - this->GetWorldPos();
			float dis = d.Length();
			if (dis != 0 && dis < desiredSeparation)
			{
				Vector3 away = this->GetWorldPos() - neighbors[i]->GetWorldPos();
				away.Normalize();
				mean += away/dis;
				count++;
			}
		}
	}

	if (count != 0)
	{
		return mean / count;
	}
	return mean;
}

Vector3 Monster::SteerTo(Vector3 target)
{
	Vector3 desired = target - GetWorldPos();
	Vector3 steer = Vector3(0, 0, 0);
	float dis = desired.Length();

	if (dis != 0)
	{
		desired.Normalize();

		if (dis > 10.0f)
		{
			desired *= dis / 10.0f;
		}
		else
		{
			desired *= 1.0f;
		}

		steer = desired - velocity;
	}

	return steer;
}

Vector3 Monster::Flock(Monster* neighbors[], int length)
{
	Vector3 separation = Seperation(neighbors, length) * SEP_WEIGHT;
	Vector3 alignment = Alignment(neighbors, length) * ALN_WEIGHT;
	Vector3 cohesion = Cohesion(neighbors, length) * COH_WEIGHT;

	return separation + alignment + cohesion;
}

void Monster::Idle()
{
	state = PlayerState::IDLE;
	anim->PlayAnimation(AnimationState::LOOP, 0);
	anim->aniScale = 1.0f;
}

void Monster::Walk()
{
	state = PlayerState::WALK;
	anim->PlayAnimation(AnimationState::LOOP, 1);
	anim->aniScale = movementSpeed;
}

void Monster::Run()
{
	state = PlayerState::RUN;
	anim->PlayAnimation(AnimationState::LOOP, 2);
	anim->aniScale = movementSpeed;
}

void Monster::Attack()
{
	state = PlayerState::ATTACK;
	anim->PlayAnimation(AnimationState::ONCE, 3);
	anim->aniScale = attackSpeed * 2.0f;
}

Monster::Monster()
{
	LoadFile("Monster1.xml");
	Idle();

	hp = 10.0f;
	movementSpeed = 1.0f;
	attackSpeed = 1.0f;

	neighborRadius    = 20.0f;
	desiredSeparation = 10.0f;

	velocity = Vector3(RANDOM->Float(), 0, RANDOM->Float());
}

void Monster::Update(Monster* neighbors[], int length)
{
	Actor::Update();
	lastPos = GetWorldPos();

	Vector3 acc = Flock(neighbors, length);
	velocity += acc;
	if (velocity.Length() > 4.0f)
	{
		velocity /= 2.0f;
	}
	position += velocity * DELTA;

	if (state == PlayerState::ATTACK)
	{
		if (anim->GetPlayTime() > 0.95)
		{
			Idle();
		}
	}
}

void Monster::WorldUpdate()
{
	GameObject::Update();
}
