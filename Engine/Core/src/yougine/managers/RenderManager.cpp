#include "RenderManager.h"

#include <fstream>
#include <iostream>
#include <vector>
#include "./../GameObject.h"

#include "../components/TransformComponent.h"

#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace yougine
{
    namespace components
    {
        class TransformComponent;
    }
}
namespace yougine::managers
{
    struct Vertex
    {
        GLfloat position[4];
    };

    RenderManager::RenderManager(int width, int height, ComponentList* component_list)
    {
        this->renderComponent = new comoponents::RenderComponent();
        GLenum err;
        this->width = width;
        this->height = height;
        //�J���[�o�b�t�@
        GLuint colorBuffer;
        glGenTextures(1, &colorBuffer);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // this->renderComponent->SetColorBuffer(colorBuffer);
        this->colorBuffer = colorBuffer;

        //�f�v�X�o�b�t�@
        GLuint depthBuffer;
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        // this->renderComponent->SetDepthBuffer(depthBuffer);
        this->depthBuffer = depthBuffer;

        //�t���[���o�b�t�@
        GLuint frameBuffer;
        glGenFramebuffers(1, &frameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
            this->colorBuffer, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
            this->depthBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        this->frameBuffer=frameBuffer;

        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << err << " �Ƃ����G���[������ in constructer" << std::endl;
        }
    }

    /**
     * \brief ������
     */
    void RenderManager::Initialize()
    {
    }

    /**
     * \brief �����_�����O
     */
    void RenderManager::RenderScene()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->frameBuffer);
        glViewport(0, 0, this->width, this->height);
        constexpr GLfloat color[]{ 0.0f, 0.3f, 0.5f, 0.8f }, depth(1.0f);
        glClearBufferfv(GL_COLOR, 0, color);
        glClearBufferfv(GL_DEPTH, 0, &depth);

        //�I�u�W�F�N�g���ꂼ��`��
        RenderOneGameObject(renderComponent);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << err << " �Ƃ����G���[������ in renderscene" << std::endl;
        }
    }
    float cValue = 0;
    float diff = 0.01f;
    float cameradiff = 0.01f;
    float camerax = 1;
    void SetFloatUniform(GLint program, const char* name, float value)
    {
        GLuint loc = glGetUniformLocation(program, name);
        // Send the float data
        glUniform1f(loc, value);
        GLuint err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << err << " �Ƃ����G���[������ in setfloatuniform" << std::endl;
        }
    }
    /**
     * \brief �Q�[���I�u�W�F�N�g��`�悷��
     * \param render_component �`��Ώۂ̃����_�[�R���|�[�l���g
     */
    void RenderManager::RenderOneGameObject(comoponents::RenderComponent* render_component)
    {
        // GameObject* gameobject = render_component->GetGameObject();
        // components::TransformComponent* transform;
        // transform = gameobject->GetComponent<components::TransformComponent>();
        glUseProgram(this->renderComponent->GetProgram());
        glBindVertexArray(this->renderComponent->GetVao());
        // �ˉe�s��F45&deg;�̎��E�A�A�X�y�N�g��4:3�A�\���͈́F0.1�P��  100�P��
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 400.0f);
        // �J�����s��
        camerax += cameradiff * 1.3;
        if (camerax > 3 || camerax < -3)
        {
            cameradiff *= -1;
        }
        glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, 2), // ���[���h��ԂŃJ������(4,3,3)�ɂ���܂��B
            glm::vec3(0, 0, 0), // ���_�����Ă��܂��B
            glm::vec3(0, 1, 0)  // ���������(0,-1,0�ɃZ�b�g����Ə㉺�t�]���܂��B)
        );
        // ���f���s��F�P�ʍs��(���f���͌��_�ɂ���܂��B)
        glm::mat4 Model = glm::mat4(1.0f);  // �e���f����ς���I
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP = Projection * View * Model; // �s��̊|���Z�͋t�ɂȂ邱�Ƃ��v���o���Ă��������B
        auto vShader_mvp_pointer = glGetUniformLocation(this->renderComponent->GetProgram(), "mvp");
        glUniformMatrix4fv(vShader_mvp_pointer, 1, GL_FALSE, &MVP[0][0]);
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cout << err << " �Ƃ����G���[������ in rendergameobject" << std::endl;
        }

        SetFloatUniform(renderComponent->GetProgram(), "c", cValue);

        cValue += diff;
        if(cValue>1.0f&&diff>0)
        {
            cValue *= -1;
        }
        // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    GLuint RenderManager::ShaderInit(std::string vs_shader_source, std::string fs_shader_source)
    {
        const auto program = glCreateProgram();

        //�V�F�[�_�I�u�W�F�N�g����
        const auto vsShader = glCreateShader(GL_VERTEX_SHADER);
        const auto fsShader = glCreateShader(GL_FRAGMENT_SHADER);

        //�V�F�[�_�R�[�h���I�u�W�F�N�g�ɓn��
        const char* vsShaderSource_char = vs_shader_source.c_str();
        std::cout << "vertex \n" << vs_shader_source << std::endl;
        glShaderSource(vsShader, 1, &vsShaderSource_char, nullptr);
        const char* fsShaderSource_char = fs_shader_source.c_str();
        glShaderSource(fsShader, 1, &fsShaderSource_char, nullptr);

        //�R���p�C��
        glCompileShader(vsShader);
        glCompileShader(fsShader);

        //�G���[
        PrintShaderInfoLog(vsShader, "vertex shader");
        PrintShaderInfoLog(fsShader, "fragment shader");

        //program�ɃA�^�b�`
        glAttachShader(program, vsShader);
        glDeleteShader(vsShader);
        glAttachShader(program, fsShader);
        glDeleteShader(fsShader);

        glLinkProgram(program);

        return program;
    }

    GLuint RenderManager::ShaderInitFromFilePath(const std::string vsFilePath, const std::string fsFilePath)
    {
        return ShaderInit(ReadFile(vsFilePath), ReadFile(fsFilePath));
        // return 0;
    }

    GLuint RenderManager::GetColorBuffer()
    {
        return this->colorBuffer;
    }

    void RenderManager::SetWindowSize(ImVec2 vec2)
    {
        this->width = vec2.x;
        this->height = vec2.y;
    }

    void RenderManager::MeshBufferInit()
    {
    }

    GLboolean RenderManager::PrintShaderInfoLog(GLuint shader, const char* str)
    {
        std::cout << "compile log " << std::endl;
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            std::cerr << "Compile error in " << str << std::endl;
        }

        GLsizei bufferSize;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufferSize);

        if (bufferSize > 1)
        {
            std::vector<GLchar> infoLog(bufferSize);
            GLsizei length;
            glGetShaderInfoLog(shader, bufferSize, &length, &infoLog[0]);
            std::cerr << &infoLog[0] << std::endl;
            return false;
        }
        return true;
    }

    std::string RenderManager::ReadFile(std::string filepath)
    {
        std::string content;
        std::ifstream reading_file(filepath, std::ios::in);
        std::cout << "file read .. " << std::endl;
        if (!reading_file)
        {
            std::cerr << "Could not read file " << filepath << ". File does not exist." << std::endl;
            return "";
        }
        std::string line = "";
        std::string a;
        while (std::getline(reading_file, a))
        {
            std::cout << "file reading now" << std::endl;
            content.append(a + "\n");
        }
        /*while (reading_file.eof())
        {
            std::cout << "file reading now" << std::endl;
            std::getline(reading_file, line);
            content.append(line + "\n");
        }*/
        reading_file.close();
        std::cout << "file contens is \n" << content << std::endl;

        return content;
    }
}
