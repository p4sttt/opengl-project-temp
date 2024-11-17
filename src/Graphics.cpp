#include "Graphics.hpp"
#include "Logger.hpp"
#include "Math.hpp"
#include <glad.h>
#define GLFW_INCLUDE_NONE

Graphics::Buffer::Buffer(
    unsigned type, unsigned count, unsigned elementSize, const void* data
)
    : _type(type), _size(count * elementSize), _data(data)
{
    LOG << "Creating Buffer: type=" << _type << ", size=" << _size << ", data=" << _data
        << '\n';
    glGenBuffers(1, &_id);
    glBindBuffer(_type, _id);
    glBufferData(_type, _size, _data, GL_STATIC_DRAW);

    glBindBuffer(_type, 0);
}

Graphics::Buffer::~Buffer()
{
    glDeleteBuffers(1, &_id);
    LOG << "Kill buffer: " << _id << '\n';
}

void Graphics::Buffer::Bind() const
{
    glBindBuffer(_type, _id);
    LOG << "Bind buffer: " << _id << '\n';
}

void Graphics::Buffer::Unbind() const
{
    glBindBuffer(_type, 0);
    LOG << "Unbind buffer: " << _id << '\n';
}

Graphics::VertexArray::VertexArray()
{
    glGenVertexArrays(1, &_id);
    LOG << "Created VertexArray: " << _id << '\n';
}

Graphics::VertexArray::~VertexArray()
{
    LOG << "Kill VertexArray: " << _id << '\n';
    glDeleteVertexArrays(1, &_id);
}

void Graphics::VertexArray::Bind() const
{
    glBindVertexArray(_id);
}

void Graphics::VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void Graphics::VertexArray::AddBuffer(Buffer& buffer)
{
    LOG << "Adding Buffer to VertexArray" << '\n';
    Bind();
    buffer.Bind();

    glVertexAttribPointer(_index, 3, GL_FLOAT, GL_FALSE, sizeof(Math::Vertex), (void*)0);
    glEnableVertexAttribArray(_index);

    LOG << "Add buffer to VertexArray at index: " << _index << '\n';
    _index++;
}

void Graphics::VertexArray::Enable() const
{
    for (unsigned i = 0; i < _index; ++i)
    {
        glEnableVertexAttribArray(i);
        LOG << "Enable VertexAttribArray at index: " << i << '\n';
    }
}

Graphics::Model::Model(
    unsigned drawMode, const std::vector<Math::Vertex>& vertices,
    const std::vector<unsigned>& indices
)
    : _vertices(vertices), _indices(indices), _transform(nullptr),
      _vbo(GL_ARRAY_BUFFER, vertices.size(), sizeof(Math::Vertex), vertices.data()),
      _ebo(GL_ELEMENT_ARRAY_BUFFER, indices.size(), sizeof(unsigned), indices.data()),
      _draw_mode(drawMode)
{
    _transform = new Math::Transform();
    LOG << "Creating Model" << '\n';
    _vao.Bind();
    _vbo.Bind();
    _ebo.Bind();

    _vao.AddBuffer(_vbo);

    _vao.Unbind();
    _vbo.Unbind();
    _ebo.Unbind();

    LOG << "Model successfully created" << '\n';
}

Graphics::Model::~Model()
{
    if (_transform)
    {
        delete _transform;
        _transform = nullptr;
    }
    LOG << "Kill Model" << '\n';
}

const Graphics::VertexArray& Graphics::Model::GetVertexArray() const
{
    return _vao;
}

Math::Transform* Graphics::Model::GetTransform()
{
    return _transform;
}

unsigned Graphics::Model::GetDrawMode()
{
    return _draw_mode;
}

size_t Graphics::Model::GetElementsCount()
{
    return _indices.size();
}

unsigned Graphics::ShaderProgram::CompileShader(unsigned type, const char* source)
{

    unsigned id = glCreateShader(type);
    LOG << "Compiling Shader: " << id << '\n';

    glShaderSource(id, 1, &source, nullptr);
    glCompileShader(id);

    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char log[length];
        glGetShaderInfoLog(id, length, &length, log);
        LOG_ERROR << "Shader compilation failed: " << log;
        glDeleteShader(id);
        return 0;
    }

    LOG << "Shader successfully compiled: " << id << '\n';
    return id;
}

Graphics::ShaderProgram::ShaderProgram(
    const char* vertexShaderSource, const char* fragmentShaderSource
)
{
    _id = glCreateProgram();
    LOG << "Creating ShaderProgram: " << _id << '\n';

    unsigned vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    if (vertexShader == 0 || fragmentShader == 0)
    {
        LOG_ERROR << "Shader compilation failed, aborting linking.\n";
        return;
    }

    LOG << "Attaching vertex shader: " << vertexShader << '\n';
    glAttachShader(_id, vertexShader);

    LOG << "Attaching fragment shader: " << fragmentShader << '\n';
    glAttachShader(_id, fragmentShader);

    LOG << "Linking program: " << _id << '\n';
    glLinkProgram(_id);

    int success;
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        int length;
        glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
        char log[length];
        glGetProgramInfoLog(_id, length, &length, log);

        LOG_ERROR << "ShaderProgram linking fialed: " << log << '\n';
    }
    else
    {
        LOG << "ShaderProgram successfully created: " << _id << '\n';

        _uniform_time_location = glGetUniformLocation(_id, "u_time");
        _uniform_transform_location = glGetUniformLocation(_id, "u_transform");
    }

    delete vertexShaderSource;
    delete fragmentShaderSource;
    vertexShaderSource = nullptr;
    fragmentShaderSource = nullptr;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Graphics::ShaderProgram::~ShaderProgram()
{
    LOG << "Kill ShaderProgram: " << _id << '\n';
    glDeleteProgram(_id);
}

unsigned Graphics::ShaderProgram::GetId()
{
    return _id;
}

void Graphics::ShaderProgram::SetTime(const float& time)
{
    glUniform1f(_uniform_time_location, time);
}

void Graphics::ShaderProgram::SetTransform(const float* transform)
{
    glUniformMatrix4fv(_uniform_transform_location, 1, GL_FALSE, transform);
}

Graphics::Renderer::Renderer() : _model(nullptr), _shader_program(nullptr)
{
    LOG << "Create empty renderer" << '\n';
}

Graphics::Renderer::~Renderer()
{
    delete _model;
    _model = nullptr;
    delete _shader_program;
    _shader_program = nullptr;
}

void Graphics::Renderer::Render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(_shader_program->GetId());
    _shader_program->SetTime(_time);
    _shader_program->SetTransform(_model->GetTransform()->GetMatrix().Data());

    _model->GetVertexArray().Bind();
    glDrawElements(
        _model->GetDrawMode(), _model->GetElementsCount(), GL_UNSIGNED_INT, nullptr
    );
    _model->GetVertexArray().Unbind();

    _time += _time_step;
}

Graphics::Model* Graphics::Renderer::GetModel()
{
    return _model;
}

const Graphics::ShaderProgram* Graphics::Renderer::GetShaderProgram() const
{
    return _shader_program;
}

void Graphics::Renderer::SetModel(Model* model)
{
    LOG << "Setting Model" << '\n';

    if (_model)
    {
        delete _model;
        _model = nullptr;
    }

    _model = model;
}

void Graphics::Renderer::SetShaderProgram(ShaderProgram* ShaderProgram)
{
    LOG << "Setting ShaderProgram" << '\n';

    if (_shader_program)
    {
        delete _shader_program;
        _shader_program = nullptr;
    }

    _shader_program = ShaderProgram;
}