#include "ProjectLoader.hpp"
#include "Graphics.hpp"
#include "Logger.hpp"
#include <cstring>
#include <fstream>
#include <glad.h>
#include <sstream>

ProjectLoader* ProjectLoader::instance = nullptr;

ProjectLoader::ProjectLoader()
{
}

ProjectLoader& ProjectLoader::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new ProjectLoader();
    }
    return *instance;
}

void ProjectLoader::ParseDrawMode(const char* line, unsigned& drawMode)
{
    LOG << "Parsing DrawMode: " << drawMode << '\n';
    if (strncmp(line, "TRIANGLES", 10) == 0)
    {
        LOG << "DrawMode successfully parsed: TRIANGLES" << '\n';
        drawMode = GL_TRIANGLES;
    }
    if (strncmp(line, "LINES", 6) == 0)
    {
        LOG << "DrawMode successfully parsed: LINES" << '\n';
        drawMode = GL_LINES;
    }
    if (strncmp(line, "POINTS", 7) == 0)
    {
        LOG << "DrawMode successfully parsed: POINTS" << '\n';
        drawMode = GL_POINT;
    }
    else
        LOG_ERROR << "Failed to parse DrawMode" << '\n';
}

void ProjectLoader::ParseVertex(const char* line, std::vector<Math::Vertex>& vertices)
{
    LOG << "Starting parse Vertex" << '\n';
    float x, y, z;
    if (sscanf(line + 2, "%f %f %f", &x, &y, &z) == 3)
    {
        LOG << "Vertex successfully parsed: " << "x=" << x << ", y=" << y << ", z=" << z
            << '\n';
        vertices.push_back(Math::Vertex{x, y, z});
    }
    else
    {
        LOG_ERROR << "Failed to parse Vertex: " << line << '\n';
    }
}

void ProjectLoader::ParseIndices(const char* line, std::vector<unsigned int>& indices)
{
    unsigned int index;
    const char* ptr = line + 1;

    LOG << "Parsing indices: ";

    while (sscanf(ptr, "%u", &index) == 1)
    {
        std::cout << index << " ";
        indices.push_back(index);
        ptr = strchr(ptr, ' ');
        if (ptr != nullptr)
            ptr++;
        else
            break;
    }

    LOG << "Indices parsed" << '\n';
}

Graphics::Model* ProjectLoader::LoadModel()
{
    LOG << "Loading model" << '\n';
    std::fstream modelFile(_model_path);

    if (!modelFile.is_open())
    {
        LOG_ERROR << "Opening ModeFile failed: " << _model_path << '\n';
    }

    std::vector<Math::Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned drawMode;

    char line[256];
    modelFile.getline(line, sizeof(line));
    ParseDrawMode(line, drawMode);

    while (modelFile.getline(line, sizeof(line)))
    {
        switch (line[0])
        {
        case 'v': {
            ParseVertex(line, vertices);
            break;
        }
        case 'f': {
            ParseIndices(line, indices);
            break;
        }
        default:
            continue;
        }
    }

    modelFile.close();

    LOG << "Model loaded: drawMode=" << drawMode << ", verticesCount=" << vertices.size()
        << ", indicesCount=" << indices.size() << '\n';

    return new Graphics::Model(drawMode, vertices, indices);
}

void ProjectLoader::LoadShader(const char* shaderPath, const char* shaderSource)
{
    std::fstream shaderFile(shaderPath);

    if (!shaderFile.is_open())
    {
        LOG_ERROR << "Failed to open Shader File: " << shaderPath << '\n';
        return;
    }

    std::stringstream buffer;
    buffer << shaderFile.rdbuf();
    shaderSource = buffer.str().c_str();

    LOG << "Load Shader: " << shaderPath << '\n' << shaderSource << '\n';

    shaderFile.close();
}

Graphics::ShaderProgram* ProjectLoader::LoadShaderProgram()
{

    const char *vertexShaderSource, *fragmentShaderSource;

    LoadShader(_vertex_shader_path, vertexShaderSource);
    LoadShader(_fragment_shader_path, fragmentShaderSource);

    return new Graphics::ShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void ProjectLoader::SetVertexShaderPath(const char* path)
{
    delete _vertex_shader_path;
    _vertex_shader_path = path;
}

void ProjectLoader::SetFragmentShaderPath(const char* path)
{
    delete _fragment_shader_path;
    _fragment_shader_path = path;
}

void ProjectLoader::SetModelPath(const char* path)
{
    delete _model_path;
    _model_path = path;
}

const char* ProjectLoader::GetVertexShaderPath() const
{
    delete _vertex_shader_path;
    return _vertex_shader_path;
}

const char* ProjectLoader::GetFragmentShaderPath() const
{
    delete _fragment_shader_path;
    return _fragment_shader_path;
}

const char* ProjectLoader::GetModelPath() const
{
    delete _model_path;
    return _model_path;
}