#include "stdafx.h"

void Boss::Idle()
{
	state = BossState::IDLE;
	anim->PlayAnimation(AnimationState::LOOP, 0);
	anim->aniScale = 1.0f;
}

void Boss::Walk()
{
	state = BossState::WALK;
	anim->PlayAnimation(AnimationState::LOOP, 1);
	anim->aniScale = movementSpeed;
}

void Boss::Run()
{
	state = BossState::RUN;
	anim->PlayAnimation(AnimationState::LOOP, 2);
	anim->aniScale = movementSpeed;
}

void Boss::Attack()
{
	state = BossState::ATTACK;
	anim->PlayAnimation(AnimationState::ONCE, 3);
	anim->aniScale = attackSpeed * 2.0f;
}

void Boss::JumpAttack()
{
	state = BossState::JUMPATTACK;
	anim->PlayAnimation(AnimationState::ONCE, 4);
	anim->aniScale = attackSpeed * 2.0f;
}

void Boss::Die()
{
	state = BossState::DIE;
	anim->PlayAnimation(AnimationState::ONCE, 5);
	anim->aniScale = movementSpeed;
}

Boss::Boss()
{
	LoadFile("Boss.xml");
	Idle();

	hp = 10.0f;
	movementSpeed = 1.0f;
	attackSpeed = 1.0f;

	neighborRadius = 20.0f;
	desiredSeparation = 10.0f;
	attackRange = 6.0f;

	velocity = Vector3(0, 0, 0);
}

void Boss::Update()
{
	Actor::Update();
	lastPos = GetWorldPos();
}

void Boss::WorldUpdate()
{
	GameObject::Update();
}

