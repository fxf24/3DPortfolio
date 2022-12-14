#include "stdafx.h"
#include "Main.h"

Main::Main()
{
}
Main::~Main()
{
}

void Main::Init()
{
	SCENE->AddScene("SC1", new Scene1());
	SCENE->AddScene("BOSS", new BossScene());
	SCENE->ChangeScene("SC1")->Init();
	//SCENE->ChangeScene("BOSS")->Init();
}

void Main::Release()
{
	SCENE->Release();
}


void Main::Update()
{
	if (INPUT->KeyDown(VK_ESCAPE))
	{
		esc = !esc;
	}

	if (!esc)
	SCENE->Update();
}

void Main::LateUpdate()
{
	if (!esc)
	SCENE->LateUpdate();
}

void Main::Render()
{
	SCENE->Render();
}

void Main::ResizeScreen()
{
	SCENE->ResizeScreen();
}

void Main::PreRender()
{
	SCENE->PreRender();
}


int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR param, int command)
{
	App.SetAppName(L"HackandSlahRPG");
	App.SetInstance(instance);
	WIN->Create();
	D3D->Create();
	Main* main = new Main();
	main->Init();
	int wParam = (int)WIN->Run(main);
	main->Release();
	SafeDelete(main);
	D3D->DeleteSingleton();
	WIN->DeleteSingleton();
	return wParam;
}
