#pragma once

#include "Graphics.hpp"
#include <string>

class ProjectLoader
{
  private:
    static ProjectLoader _instance;

    const char* _vertex_shader_path = "../res/shaders/vertex.glsl";
    const char* _fragment_shader_path = "../res/shaders/fragment.glsl";
    const char* _model_path = "../res/model.myobj";

    ProjectLoader();
    ProjectLoader(const ProjectLoader&) = delete;
    ProjectLoader& operator=(const ProjectLoader&) = delete;
    ~ProjectLoader() = default;

    void ParseDrawMode(const char* line, unsigned& drawMode);
    void ParseVertex(const char* line, std::vector<Math::Vertex>& vertices);
    void ParseIndices(const char* line, std::vector<unsigned int>& indices);
    void LoadShader(const char* shaderPath, char** shaderSource);

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