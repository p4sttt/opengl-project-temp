#version 330 core
out vec4 FragColor;

uniform float u_time;

void main()
{
    FragColor = vec4(
        abs(sin(u_time)), abs(sin(u_time) + cos(u_time)) / sqrt(2), abs(cos(u_time)), 1.0f
    );
}