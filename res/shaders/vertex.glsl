#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 u_transform;

void main()
{
    vec4 result = u_transform * vec4(aPos, 1.0);
    gl_Position = vec4(result.x, result.y, result.z, 1.0);
}