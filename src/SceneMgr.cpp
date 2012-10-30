#include "SceneMgr.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "GameObject.h"
#include "Grid.h"
#include "RenderDevice.h"
#include "Shader.h"

void SceneMgr::OnInit()
{
    SetCamera(new Camera());

    tempShader = new Shader("../res/shaders/shader.vert", "../res/shaders/shader.frag"); 

    gameObjectsMap[0] = new Grid();

    GameObject* cube = new GameObject("cube", "cube.tga");

    glm::mat4 model = cube->GetModelMatrix();
    model = glm::translate(model, glm::vec3(5.0f, 0.25f, -5.0f));
    model = glm::scale(model, glm::vec3(0.25f));

    cube->GetModelMatrix() = model;

    gameObjectsMap[1] = cube;

#define POPULATE_CUBE(a,b,c,d) cube = new GameObject("cube", "cube.tga"); \
    model = cube->GetModelMatrix(); \
    model = glm::translate(model, glm::vec3(a, b, c)); \
    model = glm::scale(model, glm::vec3(0.25f)); \
    cube->GetModelMatrix() = model;\
    gameObjectsMap[d] = cube;

    POPULATE_CUBE(4.25f, 0.25f, -5.0f, 2)
    POPULATE_CUBE(5.75f, 0.25f, -5.0f, 3)
    POPULATE_CUBE(4.625f, 0.875f, -5.0f, 4)
    POPULATE_CUBE(5.375f, 0.875f, -5.0f, 5)
    POPULATE_CUBE(5.0f, 0.875f+0.625f, -5.0f, 6)

    GameObject* square = new GameObject("square", "test.tga");

    model = square->GetModelMatrix();
    model = glm::translate(model, glm::vec3(7.0f, 0.25f, -5.0f));

    square->GetModelMatrix() = model;

    gameObjectsMap[7] = square;

    text2D.Init();
}

void SceneMgr::OnUpdate(uint32 diff)
{
    GetCamera()->OnUpdate(diff);
}

void SceneMgr::OnRender(RenderDevice* rd)
{
    Shader* shader = tempShader;
    for (GameObjectsMap::const_iterator i = gameObjectsMap.begin(); i != gameObjectsMap.end(); ++i)
    {
        shader->Bind();

        float textureFlag = i->second->GetTexture() != "" ? 1.0f : 0.0f;

        rd->SetUniforms(shader, GetCamera()->GetProjMatrix(), GetCamera()->GetViewMatrix(), i->second->GetModelMatrix(), textureFlag);
        i->second->OnRender(rd);

        shader->Unbind();
    }

    text2D.Print(rd, "Controls: W,S,A,D + arrows for rotating", 100, 100, 10);
}

void SceneMgr::SetCamera(Camera* camera)
{
    this->camera = camera;
}

SceneMgr::~SceneMgr()
{
    for (GameObjectsMap::const_iterator i = gameObjectsMap.begin(); i != gameObjectsMap.end(); ++i)
        delete i->second;

    gameObjectsMap.clear();

    delete tempShader;
    delete camera;
}
