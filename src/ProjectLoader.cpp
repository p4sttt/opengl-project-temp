#include "ProjectLoader.hpp"
#include "Graphics.hpp"
#include "Logger.hpp"
#include <cstring>
#include <fstream>
#include <glad.h>
#include <sstream>

ProjectLoader ProjectLoader::_instance;

ProjectLoader::ProjectLoader()
{
    LOG_INFO << "ProjectLoader was created" << '\n';
}

ProjectLoader& ProjectLoader::GetInstance()
{
    return _instance;
}

void ProjectLoader::ParseDrawMode(const char* line, unsigned& drawMode)
{
    LOG << "Parsing DrawMode: " << drawMode << '\n';
    if (strncmp(line, "TRIANGLES", 10) == 0)
    {
        LOG << "DrawMode successfully parsed: TRIANGLES" << '\n';
        drawMode = GL_TRIANGLES;
    }
    else if (strncmp(line, "TRIANGLES_STRIP", 16) == 0)
    {
        LOG << "DrawMode successfully parsed: TRIANGLES_STRIP" << '\n';
        drawMode = GL_TRIANGLE_STRIP;
    }
    else if (strncmp(line, "LINES", 6) == 0)
    {
        LOG << "DrawMode successfully parsed: LINES" << '\n';
        drawMode = GL_LINES;
    }
    else if (strncmp(line, "LINES_STRIP", 12) == 0) 
    {
        LOG << "DrawMode successfully parsed: LINES_STRIP" << '\n';
        drawMode = GL_LINE_STRIP;
    }
    else if (strncmp(line, "POINTS", 7) == 0)
    {
        LOG << "DrawMode successfully parsed: POINTS" << '\n';
        drawMode = GL_POINT;
    }
    else
    {
        LOG_ERROR << "Failed to parse DrawMode" << '\n';
    }
}

void ProjectLoader::ParseVertex(
    const char* line, std::vector<Math::Vertex>& vertices
)
{
    LOG << "Starting parse Vertex" << '\n';
    float x, y, z;
    if (sscanf(line + 2, "%f %f %f", &x, &y, &z) == 3)
    {
        LOG << "Vertex successfully parsed: " << "x=" << x << ", y=" << y
            << ", z=" << z << '\n';
        vertices.push_back(Math::Vertex{x, y, z});
    }
    else
    {
        LOG_ERROR << "Failed to parse Vertex: " << line << '\n';
    }
}

void ProjectLoader::ParseIndices(
    const char* line, std::vector<unsigned int>& indices
)
{
    unsigned int index;
    const char* ptr = line + 2;

    LOG << "Parsing indices: ";

    while (sscanf(ptr, "%u", &index) == 1)
    {
        std::cout << index << " ";
        indices.push_back(index);
        ptr = strchr(ptr, ' ');
        if (ptr != nullptr)
            ptr++;
        else
        {
            std::cout << '\n';
            break;
        }
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

    LOG << "Model loaded: drawMode=" << drawMode
        << ", verticesCount=" << vertices.size()
        << ", indicesCount=" << indices.size() << '\n';

    return new Graphics::Model(drawMode, vertices, indices);
}

void ProjectLoader::LoadShader(const char* shaderPath, char** shaderSource)
{
    std::fstream shaderFile(shaderPath);

    if (!shaderFile.is_open())
    {
        LOG_ERROR << "Failed to open Shader File: " << shaderPath << '\n';
        return;
    }

    std::stringstream buffer;
    buffer << shaderFile.rdbuf();
    std::string sourceBuf = buffer.str();

    *shaderSource = new char[sourceBuf.size() + 1];
    std::copy(sourceBuf.begin(), sourceBuf.end(), *shaderSource);
    (*shaderSource)[sourceBuf.size()] = '\0';

    LOG << "Load Shader: " << shaderPath << '\n' << *shaderSource << '\n';

    shaderFile.close();
}

Graphics::ShaderProgram* ProjectLoader::LoadShaderProgram()
{

    char *vertexShaderSource = nullptr, *fragmentShaderSource = nullptr;

    LoadShader(_vertex_shader_path, &vertexShaderSource);
    LoadShader(_fragment_shader_path, &fragmentShaderSource);

    return new Graphics::ShaderProgram(
        vertexShaderSource, fragmentShaderSource
    );
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
    return _vertex_shader_path;
}

const char* ProjectLoader::GetFragmentShaderPath() const
{
    return _fragment_shader_path;
}

const char* ProjectLoader::GetModelPath() const
{
    return _model_path;
}
