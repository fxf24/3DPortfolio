#include "stdafx.h"

Scene1::Scene1()
{
    sky = Sky::Create();
    Cam = Camera::Create();
  
    player = new CubeMan();
    cubeManTopRay.direction = Vector3(0, -1, 0);

    Map = Terrain::Create();
    Map->LoadFile("Wasteland.xml");
    Map->CreateStructuredBuffer();

    Point = Actor::Create();
    Point->LoadFile("Sphere.xml");

    Point2 = Light::Create("L2");
    RT = new RenderTarget();
    PostEffect = UI::Create();
    PostEffect->LoadFile("Window2.xml");

    cubeMap = new CubeMap(1000,1000);
    sphere = Actor::Create();
    sphere->LoadFile("Sphere2.xml");
    sphere->visible = false;
    /*sphere->material = make_shared<Material>();
    sphere->material->environmentMap = make_shared<Texture>();
    sphere->material->environment = 1.0f;*/
    cubeMappingShader = new Shader();
    cubeMappingShader->LoadFile("0.ExamCubeMap.hlsl");
    cubeMappingShader->LoadGeometry();

    cubeMappingShader2 = new Shader();
    cubeMappingShader2->LoadFile("4.CubeMap.hlsl");
    cubeMappingShader2->LoadGeometry();

    cubeMappingShader3 = new Shader();
    cubeMappingShader3->LoadFile("5.CubeMap.hlsl");
    cubeMappingShader3->LoadGeometry();

    rain = Rain::Create();
    rain->LoadFile("Rain.xml");
    //rain->Reset();

    skill = Rain::Create();
    //skill->LoadFile("skill.xml");
    skill->desc.velocity = Vector3(0, -100, 0);
    skill->desc.range = Vector3(50, 100, 50);

    skill->visible = false;
    skill->material = make_shared<Material>();
    skill->material->LoadFile("skill.mtl");

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
    cubeMap->RenderDetail();

    Camera::ControlMainCam();
    LIGHT->RenderDetail();
    //
    ////Ui->RenderHierarchy();
    //
    ImGui::Begin("Hierarchy");
    Point->RenderHierarchy();
    Point2->RenderHierarchy();
    sky->RenderHierarchy();
    player->RenderHierarchy();
    Map->RenderHierarchy();
    Cam->RenderHierarchy();
    sphere->RenderHierarchy();
    rain->RenderHierarchy();
    skill->RenderHierarchy();
    ImGui::End();


    Cam->Update();
    player->Update();
    Map->Update();
    Point->Update();
    sphere->Update();
    Point2->Update();
    sky->Update();
    PostEffect->Update();
    rain->Update();
    skill->Update();
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
            skill->SetWorldPos(Hit);
            skill->visible = true;
            skill_time = 0.0f;
        }
    }

    if (skill_time > 1.0f)
        skill->visible = false;

    skill_time += DELTA;

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
        if (Map->RayCastingCollider(CharacterToMouse, Hit2, dis))
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
        lerpValue += DELTA / Dis.Length() * 20.0f;

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
        }
    }
}

void Scene1::PreRender()
{
    //반사
    /*Vector3 Dir = sphere->GetWorldPos() - Camera::main->GetWorldPos();
    float dis = Dir.Length();
    Dir.Normalize();
    Vector3 reflect = Vector3::Reflect(Dir, sphere->GetUp());
    Vector3 Pos = sphere->GetWorldPos() - reflect * dis;
    Point->SetWorldPos(Pos);*/
    Vector3 Pos;
    Vector3 Dir = sphere->GetWorldPos() - Camera::main->GetWorldPos();
    float dis = Dir.Length();
    Dir.Normalize();

    if (cubeMap->desc2.CubeMapType == 0)
    {
        Vector3 reflect = Vector3::Reflect(Dir, sphere->GetUp());
        Pos = sphere->GetWorldPos() - reflect * dis;
        //Point->SetWorldPos(Pos);
    }
    else if (cubeMap->desc2.CubeMapType == 1)
    {
        Vector3 refract = Vector3::Refract(Dir, sphere->GetUp(), cubeMap->desc2.RefractIndex);
        Pos = sphere->GetWorldPos() - refract * dis;
        //Pos = Camera::main->GetWorldPos();
        //Point->SetWorldPos(Pos);
    }
    else if (cubeMap->desc2.CubeMapType == 2)
    {
        cubeMap->desc2.RefractIndex = 1.05f;
        Vector3 refract = Vector3::Refract(Dir, sphere->GetUp(), cubeMap->desc2.RefractIndex);
        Pos = sphere->GetWorldPos() - refract * dis;
        //Pos = Camera::main->GetWorldPos();
        //Point->SetWorldPos(Pos);

        cubeMap->rot.x -= 0.5f * DELTA;
    }
    //굴절
    
    //atan2(1, 1);
    //환경맵 그리기
    LIGHT->Set();
    cubeMap->Set(Pos);
    //cubeMap->Set(sphere->GetWorldPos());
    sky->Render(cubeMappingShader);
    player->Render(cubeMappingShader2);
    Map->Render(cubeMappingShader3);


    //포스트이펙트 텍스쳐에 그리기
    RT->Set();
    Cam->Set();
    sky->Render();
    Point->Render();
    Point2->Render();
    player->Render();
    Map->Render();
    rain->Render();
    skill->Render();
    sphere->material->environmentMap = cubeMap->GetRTVSRV();
    BLEND->Set(true);
    sphere->Render();
    BLEND->Set(false);
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
