#include "stdafx.h"

void Player::Idle()
{
	state = PlayerState::IDLE;
	anim->PlayAnimation(AnimationState::LOOP, 0);
	anim->aniScale = 1.0f;
}

void Player::Walk()
{
	state = PlayerState::WALK;
	anim->PlayAnimation(AnimationState::LOOP, 1);
	anim->aniScale = movementSpeed;
}

void Player::Run()
{
	state = PlayerState::RUN;
	anim->PlayAnimation(AnimationState::LOOP, 2);
	anim->aniScale = movementSpeed;
}

void Player::Attack()
{
	state = PlayerState::ATTACK;
	anim->PlayAnimation(AnimationState::ONCE, 3);
	anim->aniScale = attackSpeed * 2.0f;
}

Player::Player()
{
	LoadFile("Archer.xml");
	state = PlayerState::IDLE;
	Idle();

	movementSpeed = 1.0f;
	attackSpeed = 1.0f;
}

void Player::Update()
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

void Player::WorldUpdate()
{
	GameObject::Update();
}
