#include "stdafx.h"

#include "stdafx.h"

void Monster::Cohesion(Monster* neighbors[], int length)
{
	Vector3 sum = Vector3(0, 0, 0);
	int count = 0;
	for (int i = 0; i < length; i++)
	{
		 Vector3 d = neighbors[i]->GetWorldPos() - this->GetWorldPos();
		 float dis = d.Length();
		 if (dis != 0 && dis < neighborRadius)
		 {
			 sum += neighbors[i]->GetWorldPos();
			 count++;
		 }
	}

	if (count != 0)
	{
		SteerTo(sum / count);
	}
}

void Monster::Alignment(Monster* neighbors[], int length)
{
}

void Monster::Seperation(Monster* neighbors[], int length)
{
}

void Monster::SteerTo(Vector3 target)
{
	Vector3 desired = target - GetWorldPos();
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
	}

	/*desired.normalize()

		# 원하는 벡터의 크기를 계산하기 위한 두 옵션(1 --거리에 기초하여, 2 --최대 스피드)
		if d& lt; 100.0
			desired.multiply(MAX_SPEED * (d / 100.0)) # 이 제동은 임의적으로 정했다.
		else
			desired.multiply(MAX_SPEED)

			# Steering = Desired minus Velocity
			steer = desired.subtract(@velocity)
			steer.limit(MAX_FORCE)  # 방향 전환 정도에 제한을 둔다.
	else
		steer = new Vector(0, 0)

		return steer*/
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

	neighborRadius    = 10.0f;
	desiredSeparation = 3.0f;
}

void Monster::Update()
{
	Actor::Update();
	lastPos = GetWorldPos();

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
