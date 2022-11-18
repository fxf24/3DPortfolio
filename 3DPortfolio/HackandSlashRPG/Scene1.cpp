#include "stdafx.h"

Scene1::Scene1()
{
    sky = Sky::Create();
    Cam = Camera::Create();
  
    player = new Player();
    cubeManTopRay.direction = Vector3(0, -1, 0);

    Map = Terrain::Create();
    Map->LoadFile("Wasteland.xml");
    Map->CreateStructuredBuffer();

    RT = new RenderTarget();
    PostEffect = UI::Create();
    PostEffect->LoadFile("Window2.xml");

    monster = new Monster();
    for (int i = 0; i < 50; i++)
    {
        pjPool[i] = new Projectile();
        pjPool[i]->visible = false;

        monsters[i] = new Monster();

        monsters[i]->SetWorldPosX(RANDOM->Float(-100.0f, 100.0f));
        monsters[i]->SetWorldPosZ(RANDOM->Float(-100.0f, 100.0f));
    }
}

Scene1::~Scene1()
{
    Map->Release();
}

void Scene1::Init()
{
    time = 0.0f;
    Cam->LoadFile("Cam.xml");
    Camera::main = Cam;
    ResizeScreen();
}

void Scene1::Release()
{
   
}


void Scene1::Update()
{
    if (ImGui::Button("ChangeScene"))
    {
        SCENE->ChangeScene("SC2", 1.0f)->Init();
        time = 0.0f;
        return;
    }
    if (state == SceneState::FADEIN)
    {
        BLUR->blur.radius = 2000.0f;
        BLUR->blur.blendColor
            = Color::Lerp(Color(0, 0, 0), Color(0.5f, 0.5f, 0.5f), time);
        time += DELTA;
        if (time > 1.0f)
        {
            state = SceneState::NONE;
        }
    }
    else if (state == SceneState::FADEOUT)
    {
        time += DELTA;
        BLUR->blur.blendColor
            = Color::Lerp(Color(0.5f, 0.5f, 0.5f), Color(0, 0, 0), time);
    }

    BLUR->Update();



    Pos.x = (player->GetWorldPos().x + 128.0f) / (256.0f / 3.0f);
    Pos.y = (player->GetWorldPos().z - 128.0f) / (-256.0f / 3.0f);

    ImGui::Text("X: %d  Y: %d ", Pos.x, Pos.y);

    ImGui::Text("FPS: %d", TIMER->GetFramePerSecond());


    Camera::ControlMainCam();
    LIGHT->RenderDetail();
    //
    ////Ui->RenderHierarchy();
    //
    ImGui::Begin("Hierarchy");
    sky->RenderHierarchy();
    player->RenderHierarchy();
    monster->RenderHierarchy();
    Map->RenderHierarchy();
    Cam->RenderHierarchy();
    ImGui::End();

    Cam->Update();
    player->Update();
    monster->Update();
    Map->Update();
    sky->Update();
    PostEffect->Update();

    for (int i = 0; i < 50; i++)
    {
        if (pjPool[i]->visible)
            pjPool[i]->Update();

        monsters[i]->Update();
    }
}

void Scene1::LateUpdate()
{
    Ray Mouse = Util::MouseToRay(INPUT->position, Camera::main);
    Vector3 Hit;
    //if (Map->ComPutePicking(Mouse, Hit))
    if (Map->ComPutePicking(Mouse, Hit))
    {
        //Player->SetWorldPos(Hit);

        if (INPUT->KeyDown(VK_LBUTTON))
        {
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
    }

    cubeManTopRay.position = player->GetWorldPos();
    cubeManTopRay.position.y += 1000.0f;
    Vector3 hit;

    if (INPUT->KeyDown(VK_F1))
    {
        Ray Mouse = Util::MouseToRay(INPUT->position, Camera::main);
        Vector3 Hit;
        if (Map->RayCastingCollider(Mouse, Hit))
        {
            cout << "콜라이더에 막힘" << endl;
        }
    }

    if (INPUT->KeyDown(VK_MBUTTON))
    {

        Ray Mouse = Util::MouseToRay(INPUT->position, Camera::main);
        Vector3 Hit, Hit2;
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
            cout << "콜라이더에 막힘" << endl;
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
        lerpValue += DELTA / Dis.Length() * 20.0f * player->GetMovementSpeed();

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
}

void Scene1::PreRender()
{
    LIGHT->Set();
    RT->Set();
    Cam->Set();
    sky->Render();
    player->Render();
    monster->Render();
    Map->Render();
    for (int i = 0; i < 50; i++)
    {
        if (pjPool[i]->visible)
            pjPool[i]->Render();

        monsters[i]->Render();
    }
}

void Scene1::Render()
{
    //포스트이펙트 렌더
    BLUR->Set();
    PostEffect->material->diffuseMap->srv = RT->GetRTVSRV();
    PostEffect->Render();
}

void Scene1::ResizeScreen()
{
    Cam->width = App.GetWidth();
    Cam->height = App.GetHeight();
    Cam->viewport.width = App.GetWidth();
    Cam->viewport.height = App.GetHeight();

    if (RT)
    {
        RT->ResizeScreen(Cam->viewport.width, Cam->viewport.height);
    }
}
