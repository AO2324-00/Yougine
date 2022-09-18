#pragma once
#include "Component.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace yougine::comoponents
{
    class RenderComponent : public components::Component
    {
        //�}�e���A���i�V�F�[�_�[�A�V�F�[�_�ɓn���l�j�A���b�V���B
    public:
        RenderComponent();

        void SetProgram(GLuint program);

        void SetVao(GLuint vao);

        void SetIndexBuffer(GLuint index_buffer);

        [[nodiscard]] GLuint GetProgram() const;

        [[nodiscard]] GLuint GetVao() const;

    private:
        GLuint program;

        GLuint vao;
    };
}
