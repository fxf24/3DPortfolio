#include "stdafx.h"

#include "stdafx.h"

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
