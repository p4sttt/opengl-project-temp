#pragma once

#include "Graphics.hpp"
#include <string>

class ProjectLoader
{
  private:
    static ProjectLoader* instance;

    const char* _vertex_shader_path =
        "/home/d4y2k/Programming/OpenGLUltimate/res/shaders/vertex.glsl";
    const char* _fragment_shader_path =
        "/home/d4y2k/Programming/OpenGLUltimate/res/shaders/fragment.glsl";
    const char* _model_path = "/home/d4y2k/Programming/OpenGLUltimate/res/model.myobj";

    ProjectLoader();
    ProjectLoader(const ProjectLoader&) = delete;
    ProjectLoader& operator=(const ProjectLoader&) = delete;

    void ParseDrawMode(const char* line, unsigned& drawMode);
    void ParseVertex(const char* line, std::vector<Math::Vertex>& vertices);
    void ParseIndices(const char* line, std::vector<unsigned int>& indices);
    void LoadShader(const char* shaderPath, const char* shaderSource);

  public:
    static ProjectLoader& GetInstance();

    Graphics::Model* LoadModel();
    Graphics::ShaderProgram* LoadShaderProgram();

    void SetVertexShaderPath(const char* path);
    void SetFragmentShaderPath(const char* path);
    void SetModelPath(const char* path);

    const char* GetVertexShaderPath() const;
    const char* GetFragmentShaderPath() const;
    const char* GetModelPath() const;
};