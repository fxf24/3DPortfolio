#include "stdafx.h"

Scene1::Scene1()
{
    sky = Sky::Create();
    Cam = Camera::Create();
  
    //player = new Player();
    cubeManTopRay.direction = Vector3(0, -1, 0);

    Map = Terrain::Create();
    Map->LoadFile("Wasteland.xml");
    Map->CreateStructuredBuffer();
    Map->material->shadow = 1.0f;
    Map->Update();

    shadow = new Shadow();
    postEffect = new PostEffect();

    for (int i = 0; i < 50; i++)
    {
        pjPool[i] = new Projectile();
        pjPool[i]->visible = false;

        monsters[i] = new Monster();

        monsters[i]->SetWorldPosX(RANDOM->Float(-100.0f, 100.0f));
        monsters[i]->SetWorldPosZ(RANDOM->Float(-100.0f, 100.0f));

        monsters[i]->WorldUpdate();
        while (Map->Intersect(monsters[i]->collider))
        {
            cout << "in" << i << endl;
            monsters[i]->SetWorldPosX(RANDOM->Float(-100.0f, 100.0f));
            monsters[i]->SetWorldPosZ(RANDOM->Float(-100.0f, 100.0f));
            monsters[i]->WorldUpdate();
        }
    }

    lerpValue = 1.0f;
    RlerpValue = 1.0f;
}

Scene1::~Scene1()
{
    Map->Release();
}

void Scene1::Init()
{
    Cam->LoadFile("Cam.xml");
    Camera::main = Cam;
    ResizeScreen();
    player->SetWorldPos(Vector3(-119, 0, -117));
}

void Scene1::Release()
{
   
}


void Scene1::Update()
{
    ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());
    if (ImGui::Button("Boss"))
    {
        SCENE->ChangeScene("BOSS")->Init();
    }

    //Camera::ControlMainCam();
    LIGHT->RenderDetail();
    shadow->RenderDetail();
    postEffect->RenderDetail();

    ImGui::Begin("Hierarchy");
    sky->RenderHierarchy();
    player->RenderHierarchy();
    Map->RenderHierarchy();
    Cam->RenderHierarchy();
    //monsters[0]->RenderHierarchy();
    ImGui::End();

    
    player->Update();
    Map->Update();
    sky->Update();

    Cam->SetWorldPosX(player->GetWorldPos().x);
    Cam->SetWorldPosZ(player->GetWorldPos().z - 25.0f);
    Cam->Update();

    for (int i = 0; i < 50; i++)
    {
        if (pjPool[i]->visible)
            pjPool[i]->Update();

        if (monsters[i])
            monsters[i]->Update(monsters, 50);
    }
}

void Scene1::LateUpdate()
{
    Ray Mouse = Util::MouseToRay(INPUT->position, Camera::main);
    Vector3 Hit;

    if (INPUT->KeyDown(VK_MBUTTON))
    {
        Map->ComPutePicking(Mouse, Hit);

        cout << Hit.x << " : " << Hit.z << endl;
    }


    if (INPUT->KeyDown(VK_LBUTTON))
    {
        Map->ComPutePicking(Mouse, Hit);
        player->Attack();
        Vector3 Dir = Hit - player->GetWorldPos();
        Dir.y = 0;
        Dir.Normalize();
        // -PI ~ PI
        float Yaw = atan2f(Dir.x, Dir.z) + PI;
        // -PI ~ PI
        player->rotation.y = Yaw;

        lerpValue = 1.0f;
        RlerpValue = 1.0f;
        findPath = false;

        for (int i = 0; i < 50; i++)
        {
            if (!pjPool[i]->visible)
            {
                pjPool[i]->Shoot(player->GetWorldPos(), Dir, 10.0f);
                break;
            }
        }
    }

    if (INPUT->KeyDown(VK_RBUTTON))
    {
        Vector3 Hit2;
        Ray CharacterToMouse;
        Map->ComPutePicking(Mouse, Hit);
        CharacterToMouse.position = player->GetWorldPos();
        CharacterToMouse.direction = Hit - player->GetWorldPos();
        CharacterToMouse.direction.y = 0;
        float dis = CharacterToMouse.direction.Length();
        CharacterToMouse.direction.Normalize();
        findPath = false;
        path.clear();
        route = -1;
        if (dis != 0 && Map->RayCastingCollider(CharacterToMouse, Hit2, dis))
        {
            cout << "?????????? ????" << endl;
            path.push_back(player->GetWorldPos());
            deque<Vector3> way;
            Map->PathFinding(way, Map->PickNode(Hit2), Map->PickNode(Hit));
            reverse(way.begin(), way.end());
            path.insert(path.end(), way.begin(), way.end());
            path.push_back(Hit);
            findPath = true;
            route = 0;
            player->Run();
        }
        else
        {
            //cubeMan->SetWorldPos(Hit);
            from = player->GetWorldPos();
            //from.y = 0.0f;
            to = Hit;
            //to.y = 0.0f;
            lerpValue = 0.0f;

            Vector3 Dir = Hit - player->GetWorldPos();
            Dir.y = 0;
            Dir.Normalize();
            // -PI ~ PI
            float Yaw = atan2f(Dir.x, Dir.z) + PI;
            // -PI ~ PI
            player->rotation.y = Util::NormalizeAngle(player->rotation.y);

            //to Yaw;
            if (fabs(Yaw - player->rotation.y) > PI)
            {
                if (Yaw > 0)
                {
                    Rfrom = player->rotation.y + PI * 2.0f;
                    Rto = Yaw;
                }
                else
                {
                    Rfrom = player->rotation.y - PI * 2.0f;
                    Rto = Yaw;
                }
            }
            else
            {
                Rfrom = player->rotation.y;
                Rto = Yaw;
            }
            RlerpValue = 0.0f;
            //cubeMan->rotation.y = Yaw;
            player->Run();

        }
    }
    

    cubeManTopRay.position = player->GetWorldPos();
    cubeManTopRay.position.y += 1000.0f;
    Vector3 hit;

    if (findPath)
    {
        from = path[route];
        //from.y = 0.0f;
        to = path[route + 1];
        //to.y = 0.0f;
        lerpValue = 0.0f;

        Vector3 Dir = path[route + 1] - path[route];
        Dir.y = 0;
        Dir.Normalize();
        // -PI ~ PI
        float Yaw = atan2f(Dir.x, Dir.z) + PI;
        // -PI ~ PI
        player->rotation.y = Util::NormalizeAngle(player->rotation.y);

        //to Yaw;
        if (fabs(Yaw - player->rotation.y) > PI)
        {
            if (Yaw > 0)
            {
                Rfrom = player->rotation.y + PI * 2.0f;
                Rto = Yaw;
            }
            else
            {
                Rfrom = player->rotation.y - PI * 2.0f;
                Rto = Yaw;
            }
        }
        else
        {
            Rfrom = player->rotation.y;
            Rto = Yaw;
        }
        RlerpValue = 0.0f;
        findPath = false;
    }


    if (RlerpValue < 1.0f)
    {
        float minus = fabs(Rto - Rfrom);

        RlerpValue += DELTA / minus * PI * 2.0f;
        player->rotation.y = Util::Lerp(Rfrom, Rto, RlerpValue);
        if (RlerpValue > 1.0f)
        {
            player->rotation.y = Rto;
        }
    }


    if (lerpValue < 1.0f)
    {
        Vector3 coord = Util::Lerp(from, to, lerpValue);
        player->SetWorldPos(coord);
        Vector3 Dis = from - to;
        lerpValue += DELTA / Dis.Length() * 10.0f * player->GetMovementSpeed();

        Vector3 Hit2;
        if (Util::RayIntersectMap(cubeManTopRay, Map, Hit2))
        {
            player->SetWorldPosY(Hit2.y);
        }

        if (lerpValue > 1.0f)
        {
            //lerpValue = 0.0f;
            player->SetWorldPos(to);

            if (route < path.size() - 2)
            {
                route++;
                findPath = true;
            }
            else
            {
                player->Idle();
            }
        }
    }

    for (int i = 0; i < 50; i++)
    {
        if (pjPool[i]->visible)
        {
            for (int j = 0; j < 50; j++)
            {
                if (monsters[j])
                {
                    if (pjPool[i]->collider->Intersect(monsters[j]->collider))
                    {
                        Monster* temp = monsters[j];
                        monsters[j] = nullptr;
                        delete temp;
                        pjPool[i]->visible = false;
                    }
                }
            }
            
        }
    }

    if (Map->Find("Collider14")->collider->Intersect(player->collider))
    {
        SCENE->ChangeScene("BOSS")->Init();
    }
}

void Scene1::PreRender()
{
    LIGHT->Set();
    {
        shadow->SetCapture(Vector3(0, 0, 0));
        Map->ShadowMapRender();
        player->ShadowMapRender();

        for (int i = 0; i < 50; i++)
        {
            if (pjPool[i]->visible)
                pjPool[i]->ShadowMapRender();
            if (monsters[i])
                monsters[i]->ShadowMapRender();
        }
    }
   

    {
        postEffect->SetCapture();
        Cam->Set();
        sky->Render();

        shadow->SetTexture();
        Map->Render();
        player->Render();

        for (int i = 0; i < 50; i++)
        {
            if (pjPool[i]->visible)
                pjPool[i]->Render();
            if (monsters[i])
                monsters[i]->Render();
        }
    }
    
}

void Scene1::Render()
{
    //???????????? ????
    postEffect->Render();
    
}

void Scene1::ResizeScreen()
{
    Cam->width = App.GetWidth();
    Cam->height = App.GetHeight();
    Cam->viewport.width = App.GetWidth();
    Cam->viewport.height = App.GetHeight();

    if (postEffect)
    {
        postEffect->ResizeScreen(App.GetWidth(), App.GetHeight());
    }
}
