#version 330 core
out vec4 FragColor;
in vec4 return_color;
void main()
{
   FragColor = return_color;
}