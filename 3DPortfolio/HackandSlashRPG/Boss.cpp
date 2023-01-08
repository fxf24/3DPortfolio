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

	hp = 100.0f;
	movementSpeed = 1.0f;
	attackSpeed = 1.0f;

	JumpAttackRadius = 50.0f;
	AttackRadius = 30.0f;
	attackRange = 8.0f;

	dir = Vector3(0, 0, 0);
	jumpLerp = 1.0f;

}

void Boss::Update()
{
	Actor::Update();
	lastPos = GetWorldPos();

	Vector3 ep = player->GetWorldPos() - position;

	if (ep.Length() < AttackRadius && state == BossState::IDLE)
	{
		Walk();
	}
	else if (ep.Length() < JumpAttackRadius && state == BossState::IDLE)
	{
		jumpFrom = GetWorldPos();
		jumpTo = player->GetWorldPos();
		jumpLerp = 0.0f;
		jumpLength = ep.Length();
		ep.Normalize();
		dir = ep;
		JumpAttack();
		cout << "JumpAttack" << endl;
	}
	else if (ep.Length() < attackRange && state == BossState::WALK)
	{
		Attack();
		dir = Vector3();
	}
	else if (ep.Length() > AttackRadius && state != BossState::IDLE && state != BossState::JUMPATTACK && state != BossState::DIE)
	{
		Idle();
		dir = Vector3();
	}

	if (state == BossState::WALK)
	{
		ep.Normalize();
		dir = ep;
		position += dir * DELTA * 5.0f;
	}
	if (state == BossState::ATTACK)
	{
		if (anim->GetPlayTime() > 0.4)
		{
			Find("mixamorig:LeftHand")->collider->visible = true;
		}

		if (anim->GetPlayTime() > 0.95)
		{
			Find("mixamorig:LeftHand")->collider->visible = false;
			Idle();
		}
	}
	if (state == BossState::JUMPATTACK)
	{
		Vector3 coord = Util::Lerp(jumpFrom, jumpTo, jumpLerp);
		SetWorldPos(coord);
		jumpLerp += DELTA / jumpLength * 20.0f * GetMovementSpeed();

		cout << jumpLerp << endl;
		if (jumpLerp > 1.0f)
		{
			SetWorldPos(jumpTo);
			Idle();
			cout << "in" << endl;
		}
	}
	if (state == BossState::DIE)
	{

	}
	if (state != BossState::IDLE && state != BossState::DIE)
	{
		ep.Normalize();
		float Yaw = atan2f(ep.x, ep.z) + PI;
		// -PI ~ PI
		rotation.y = Yaw;
	}
}

void Boss::WorldUpdate()
{
	GameObject::Update();
}

