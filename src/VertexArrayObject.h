#ifndef H_VERTEXARRAYOBJECT
#define H_VERTEXARRAYOBJECT

#include "Common.h"

enum 
{
    ID_VAO = 0,
    ID_VBO = 1,

    ID_MAX = 2
};

class VertexArrayObject
{
    public:
        VertexArrayObject() {}
        ~VertexArrayObject();

        uint32 GetId() const { return id[ID_VAO]; }

        void Bind(uint8);
        void Unbind(uint8);

        void FillBuffer(uint32, void*, uint32);

        void CreateVertexArray();
        void CreateVertexBuffer();

        void AddAttribToBuffer(uint32, uint8, uint32, bool, uint32, void*);
        void EnableAttrib(uint32);

        uint32& ElementsCount() { return size; }

    private:
        uint32 id[ID_MAX];
        uint32 size;
};

#endif