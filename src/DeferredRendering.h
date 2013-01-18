#ifndef H_DEFERREDRENDERING
#define H_DEFERREDRENDERING

#include "Common.h"
#include "VertexArrayObject.h"

class DeferredRenderer
{
    public:
        DeferredRenderer();
        ~DeferredRenderer();

        void Init();
        void InitFSQuad();

        void Bind();
        void Unbind();

        void GeometryPass();
        void LightsPass();

        void DirectionalLightPass(glm::vec3, glm::vec3);
        void PointLightPass(glm::vec3 position, glm::vec3 color, float radius, float intensity);

        void FinalPass();
        void ShadowPass();

        uint32 frameBuffer;

        uint32 colorTexture;
        uint32 normalTexture;
        uint32 depthTexture;
        uint32 lightTexture;

        uint32 shadowTexture;

        VertexArrayObject quadVAO;

        uint32 width;
        uint32 height;
};

#endif
