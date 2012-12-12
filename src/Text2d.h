#ifndef H_TEXT2D
#define H_TEXT2D

#include <string>

#include "Common.h"
#include "VertexArrayObject.h"

class Camera;
class Shader;

class Text2D
{
    public:
        ~Text2D();

        void Init();
        void RenderText(std::string, int, int, int);
        void RenderSprite(int, int, int, int);
        void RenderSprite(VertexArrayObject&, int);

        void unloadFont();

    private:
        uint32 textureId;

        VertexArrayObject vao;
};

#endif
