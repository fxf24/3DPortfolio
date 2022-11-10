#include "stdafx.h"

CubeMan::CubeMan()
{
	LoadFile("Archer.xml");
	state = PlayerState::IDLE;
	jumping = false;
}

void CubeMan::Update()
{
	//움직이기 전의 위치값
	lastPos = GetWorldPos();


	switch (state)
	{
	case PlayerState::IDLE:
		Idle();
		break;
	case PlayerState::WALK:
		Walk();
		break;
	}
	if (INPUT->KeyPress(VK_LEFT))
	{
		rotation.y -= DELTA;
	}
	if (INPUT->KeyPress(VK_RIGHT))
	{
		rotation.y += DELTA;
	}
	if (INPUT->KeyDown(VK_SPACE)and not jumping)
	{
		jumping = true;
		gravity = -30.0f;
	}

	if (jumping)
	{
		MoveWorldPos(-UP * gravity * DELTA);
		gravity += 30.0f * DELTA;
		/*if (gravity > 30.0f)
		{
			jumping = false;
		}*/
	}

	Actor::Update();
}

void CubeMan::WorldUpdate()
{
	Transform::Update();
}

void CubeMan::Falling()
{
	if (jumping) return;
	jumping = true;
	gravity = 0.0f;
}

void CubeMan::Landing()
{
	if (not jumping) return;
	jumping = false;
	gravity = 0.0f;
}

void CubeMan::Idle()
{
	//고개젓기,차렷자세유지
	//Find("Head")->rotation.y += mul * DELTA;

	//idle to walk
	if (INPUT->KeyPress(VK_UP) or INPUT->KeyPress(VK_DOWN))
	{
		state = PlayerState::WALK;
		anim->PlayAnimation(AnimationState::LOOP, 1);
	}
}

void CubeMan::Walk()
{
	if (INPUT->KeyPress(VK_UP))
	{
		position += -GetForward() * 10.0f * DELTA;
	}
	if (INPUT->KeyPress(VK_DOWN))
	{
		position -= -GetForward() * 10.0f * DELTA;
	}
	//팔다리휘젓기,고개고정
	//Find("Head")->rotation.y = 0.0f;

	//walk to idle
	if (INPUT->KeyPress(VK_UP) == false and INPUT->KeyPress(VK_DOWN) == false)
	{
		state = PlayerState::IDLE;
		anim->PlayAnimation(AnimationState::LOOP, 0);
	}
}
