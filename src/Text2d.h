#ifndef H_TEXT2D
#define H_TEXT2D

#include <string>

#include "Common.h"
#include "VertexArrayObject.h"

class Camera;
class RenderDevice;
class Shader;

class Text2D
{
    public:
        ~Text2D();

        void Init();
        void Print(RenderDevice*, std::string, int, int, int);

        void unloadFont();

    private:
        uint32 textureId;

        Shader* shader;
        VertexArrayObject vao;
};

#endif
