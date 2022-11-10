#include "stdafx.h"

void Shop::invenUpdate()
{
	for (auto it = inven.begin(); it != inven.end(); it++)
	{
		if (it->second->num > 0)
		{
			SafeReset(Ui->Find(it->first)->material->diffuseMap);
			Ui->Find(it->first)->material->diffuse.w = 1.0f;
			Ui->Find(it->first)->material->diffuseMap = make_shared<Texture>();
			Ui->Find(it->first)->material->diffuseMap->
				LoadFile(it->second->imgFile);
		}
		else
		{
			SafeReset(Ui->Find(it->first)->material->diffuseMap);
			Ui->Find(it->first)->material->diffuse.w = 0.0f;
		}
	}
}

void Shop::Init(Inven* pInven)
{
	Ui = UI::Create();
	Ui->LoadFile("Window2.xml");

	Item* temp = new Item();
	temp->num = 3;
	temp->name = "redPotion";
	temp->imgFile = "2000000.png";
	inven["00"] = temp;


	temp = new Item();
	temp->num = 1;
	temp->name = "bluePotion";
	temp->imgFile = "2000003.png";
	inven["01"] = temp;


	this->pInven = pInven;


	invenUpdate();
}

void Shop::Update()
{
	if (not show) return;

	for (auto it = inven.begin(); it != inven.end(); it++)
	{
		UI* temp =(UI *) Ui->Find(it->first);
		if ((temp)->MouseOver() and INPUT->KeyDown(VK_RBUTTON))
		{
			//cout << it->first << endl;
			for (auto it2 = pInven->inven.begin(); it2 != pInven->inven.end(); it2++)
			{
				if (it->second->name == it2->second->name)
				{
					it2->second->num++;
					pInven->Gold -= 100;
					break;
				}
			}
		}
		

	}
	
	Ui->Update();
}

void Shop::Render()
{
	if (not show) return;

	DEPTH->Set(false);
	Ui->Render();
	DEPTH->Set(true);
}
