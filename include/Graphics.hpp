#pragma once

#include "Math.hpp"
#include <vector>

namespace Graphics
{
class Buffer
{
  private:
    unsigned _id;
    unsigned _type, _size;
    const void* _data;

  public:
    Buffer(unsigned type, unsigned count, unsigned elementSize, const void* data);
    ~Buffer();

    void Bind() const;
    void Unbind() const;
};

class VertexArray
{
  private:
    unsigned _id;
    unsigned _index = 0;

  public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;
    void AddBuffer(Buffer& buffer);
    void Enable() const;
};

class Model
{
  private:
    std::vector<Math::Vertex> _vertices;
    std::vector<unsigned> _indices;
    Math::Transform* _transform;

    VertexArray _vao;
    Buffer _vbo, _ebo;
    unsigned _draw_mode;

  public:
    Model(
        unsigned drawMode, const std::vector<Math::Vertex>& vertices,
        const std::vector<unsigned>& indices
    );
    ~Model();

    const VertexArray& GetVertexArray() const;
    Math::Transform* GetTransform();
    unsigned GetDrawMode();
    size_t GetElementsCount();
};

class ShaderProgram
{
  private:
    unsigned _id;
    unsigned _uniform_time_location;
    unsigned _uniform_transform_location;

    unsigned CompileShader(unsigned type, const char* source);

  public:
    ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    ~ShaderProgram();

    unsigned GetId();
    void SetTime(const float& time);
    void SetTransform(const float* matrix);
};

class Renderer
{
  private:
    Model* _model;
    ShaderProgram* _shader_program;

    float _time = 0.0f;
    const float _time_step = 0.01f;

  public:
    Renderer();
    ~Renderer();

    void Render();

    Model* GetModel();
    const ShaderProgram* GetShaderProgram() const;

    void SetModel(Model* model);
    void SetShaderProgram(ShaderProgram* shaderProgram);
};

} // namespace Graphics
