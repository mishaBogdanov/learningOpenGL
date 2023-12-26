#version 330 core


out vec4 FragColor;

in vec2 texture_coordinates;
in vec4 givenColor;

uniform sampler2D tex0;


void main()
{
   FragColor = texture(tex0, texture_coordinates)*givenColor;
}