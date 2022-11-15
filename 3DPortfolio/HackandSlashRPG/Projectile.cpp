#include "stdafx.h"

Projectile::Projectile()
{
	Xml::XMLDocument* doc = new Xml::XMLDocument();
	string path = "../Contents/GameObject/sphere.xml";
	Xml::XMLError result = doc->LoadFile(path.c_str());
	if (result != Xml::XML_SUCCESS) return;

	Xml::XMLElement* ob;
	ob = doc->FirstChildElement();
	LoadObject(ob);
}

void Projectile::Shoot(Vector3 pos, Vector3 dir, float spd)
{
	position = pos;
	direction = dir;
	speed = spd;
	visible = true;
}

void Projectile::Update()
{
	position += direction * 10.0f * speed * DELTA;
	if (position.x < -200 || position.x > 200 || position.z < -200 || position.z > 200)
		visible = false;
	GameObject::Update();
}
