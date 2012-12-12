#include "DeferredRendering.h"

#include <GL/glew.h>
#include <GL/glfw.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Config.h"
#include "Exception.h"
#include "GameObject.h"
#include "Light.h"
#include "ModelData.h"
#include "Player.h"
#include "SceneMgr.h"
#include "Shader.h"
#include "Terrain.h"
#include "RenderDevice.h"
#include "ResourcesMgr.h"

#include <iostream>

void DeferredRenderer::Init()
{
    width = sConfig->GetDefault("width", 800);
    height = sConfig->GetDefault("height", 600);

    InitFSQuad();

    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);

    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &normalTexture);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, depthTexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &lightTexture);
    glBindTexture(GL_TEXTURE_2D, lightTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, lightTexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &depthTest);
    glBindRenderbuffer(GL_RENDERBUFFER, depthTest);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height); 
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthTest);

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 }; 
    glDrawBuffers(sizeof(buffers)/sizeof(GLenum), buffers);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (Status != GL_FRAMEBUFFER_COMPLETE)
        throw Exception("[FrameBuffer][E] frame buffer incomplete!");

    std::cout << Status << std::endl;

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void DeferredRenderer::Bind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
}

void DeferredRenderer::Unbind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void DeferredRenderer::GeometryPass()
{
    Bind();

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 }; 
    glDrawBuffers(3, buffers);

    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader* shader = sResourcesMgr->GetShader("deferred_geopass.glsl");
    shader->Bind();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Camera* camera = sSceneMgr->GetCamera();
    Terrain* terrain = sSceneMgr->GetTerrain();

    glm::mat4 mvp = camera->GetProjMatrix() * camera->GetViewMatrix();
    shader->SetUniform("in_M", glm::mat4(1.0f));
    shader->SetUniform("in_MVP", mvp);

    shader->SetUniform("Texture", 0);

    OGLHelper::ActivateTexture(GL_TEXTURE0, sResourcesMgr->GetTextureId("placeholder.tga"));

    terrain->OnRender();

    GameObjectsMap objects = sSceneMgr->GetObjects();
    for (auto i = objects.begin(); i != objects.end(); ++i)
    {
        GameObject* ob = i->second;

        glm::mat4 mvp = camera->GetProjMatrix() * camera->GetViewMatrix() * ob->GetModelMatrix();
        shader->SetUniform("in_M", ob->GetModelMatrix());
        shader->SetUniform("in_MVP", mvp);

        shader->SetUniform("Texture", 0);

        OGLHelper::ActivateTexture(GL_TEXTURE0, sResourcesMgr->GetTextureId(ob->GetTexture()));

        ob->OnRender();
    }

    for (uint8 i = 0; i < sSceneMgr->GetPointLights().size(); ++i)
    {
        glm::mat4 m = glm::scale(glm::translate(glm::mat4(1.0f), sSceneMgr->GetPointLights()[i].Position), glm::vec3(0.15f));
        glm::mat4 mvp = camera->GetProjMatrix() * camera->GetViewMatrix() * m;
        shader->SetUniform("in_M", m);
        shader->SetUniform("in_MVP", mvp);

        shader->SetUniform("Texture", 0);

        OGLHelper::ActivateTexture(GL_TEXTURE0, sResourcesMgr->GetTextureId("light.tga"));

        ModelData* data = sResourcesMgr->GetModelData("sphere.obj");
        OGLHelper::DrawTriangleStrip(data->vao);
    }

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    shader->Unbind();
    Unbind();
}

void DeferredRenderer::DirectionalLightPass(glm::vec3 dir, glm::vec3 color)
{
    Shader* shader = sResourcesMgr->GetShader("deferred_dirlightpass.glsl");
    shader->Bind();

    OGLHelper::ActivateTexture(GL_TEXTURE0, colorTexture);
    shader->SetUniform("ColorTexture", 0);
    
    OGLHelper::ActivateTexture(GL_TEXTURE1, normalTexture);
    shader->SetUniform("NormalTexture", 1);

    OGLHelper::ActivateTexture(GL_TEXTURE2, depthTexture);
    shader->SetUniform("DepthTexture", 2);

    Camera* camera = sSceneMgr->GetCamera();

    shader->SetUniform("in_CameraPosition", camera->GetPosition());
    shader->SetUniform("in_InvVP", glm::inverse(camera->GetProjMatrix() * camera->GetViewMatrix()));
    shader->SetDirectionalLight(dir, color);

    // render full screen quad
    OGLHelper::DrawTriangles(quadVAO);

    shader->Unbind();
}

void DeferredRenderer::PointLightPass(glm::vec3 position, glm::vec3 color, float radius, float intensity)
{
    Shader* shader = sResourcesMgr->GetShader("deferred_pointlightpass.glsl");
    shader->Bind();

    OGLHelper::ActivateTexture(GL_TEXTURE0, colorTexture);
    shader->SetUniform("ColorTexture", 0);
    
    OGLHelper::ActivateTexture(GL_TEXTURE1, normalTexture);
    shader->SetUniform("NormalTexture", 1);

    OGLHelper::ActivateTexture(GL_TEXTURE2, depthTexture);
    shader->SetUniform("DepthTexture", 2);

    Camera* camera = sSceneMgr->GetCamera();

    glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), position), glm::vec3(radius));
    glm::mat4 mvp = camera->GetProjMatrix() * camera->GetViewMatrix() * model;

    shader->SetUniform("in_CameraPosition", camera->GetPosition());
    shader->SetUniform("in_InvVP", glm::inverse(camera->GetProjMatrix() * camera->GetViewMatrix()));
    shader->SetUniform("in_MVP", mvp);

    shader->SetPointLight(position, color, radius, intensity);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    
    ModelData* data = sResourcesMgr->GetModelData("cube.obj");
    OGLHelper::DrawTriangleStrip(data->vao);
    //OGLHelper::DrawTriangles(quadVAO);
    shader->Unbind();

    glDisable(GL_CULL_FACE);
}

void DeferredRenderer::InitFSQuad()
{
    Vertex vert[6];
    vert[0].position = glm::vec3(-1.0f, 1.0f, 0.0f);
    vert[0].uv = glm::vec2(0,1);
    vert[1].position = glm::vec3(-1.0f, -1.0f, 0.0f);
    vert[1].uv = glm::vec2(0,0);
    vert[2].position = glm::vec3(1.0f, 1.0f, 0.0f);
    vert[2].uv = glm::vec2(1,1);
    vert[3].position = glm::vec3(1.0f, -1.0f, 0.0f);
    vert[3].uv = glm::vec2(1,0);
    vert[4].position = glm::vec3(1.0f, 1.0f, 0.0f);
    vert[4].uv = glm::vec2(1,1);
    vert[5].position = glm::vec3(-1.0f, -1.0f, 0.0f);
    vert[5].uv = glm::vec2(0,0);

    quadVAO.ElementsCount() = sizeof(vert)/sizeof(Vertex);

    quadVAO.CreateVertexArray();
    quadVAO.CreateVertexBuffer();
    quadVAO.Bind(ID_VAO);
    quadVAO.Bind(ID_VBO);

    quadVAO.FillBuffer(GL_STATIC_DRAW, &vert[0], sizeof(vert));
    quadVAO.EnableAttrib(VertexArray::Attrib::POSITION);
    quadVAO.AddAttribToBuffer(VertexArray::Attrib::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

    quadVAO.EnableAttrib(VertexArray::Attrib::TEXCOORD);
    quadVAO.AddAttribToBuffer(VertexArray::Attrib::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(UV_VERTEX_POS));

    quadVAO.Unbind(ID_VBO);
    quadVAO.Unbind(ID_VAO);
}

void DeferredRenderer::LightsPass()
{
    Bind();

    glDrawBuffer(GL_COLOR_ATTACHMENT3);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    //DirectionalLightPass(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    for (uint8 i = 0; i < sSceneMgr->GetPointLights().size(); ++i)
        PointLightPass(sSceneMgr->GetPointLights()[i].Position, sSceneMgr->GetPointLights()[i].Color, sSceneMgr->GetPointLights()[i].Radius, sSceneMgr->GetPointLights()[i].Intensity);

    glDisable(GL_BLEND);

    Unbind();
}

void DeferredRenderer::FinalPass()
{
    Shader* shader = sResourcesMgr->GetShader("deferred_finalpass.glsl");
    shader->Bind();

    OGLHelper::ActivateTexture(GL_TEXTURE0, colorTexture);
    shader->SetUniform("ColorTexture", 0);

    OGLHelper::ActivateTexture(GL_TEXTURE1, lightTexture);
    shader->SetUniform("LightTexture", 1);

    // render full screen quad
    OGLHelper::DrawTriangles(quadVAO);

    shader->Unbind();
}
