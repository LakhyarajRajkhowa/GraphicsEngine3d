#pragma once
#include <GL/glew.h>

namespace Lengine {

    class Framebuffer {
    public:
        Framebuffer(int width, int height);
        ~Framebuffer();

        void Bind();
        void Unbind();

        void Resize(int width, int height);

        GLuint GetColorAttachment() const { return m_ColorAttachment; }
        GLuint GetID() const { return m_FBO; }

        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }

    private:
        void Create();
        void Destroy();

    private:
        GLuint m_FBO = 0;
        GLuint m_ColorAttachment = 0;
        GLuint m_DepthAttachment = 0;

        int m_Width = 0;
        int m_Height = 0;
    };

}
