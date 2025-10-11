example shaders

:vertex
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;

out vec3 a_colour;

void main()
{
  gl_Position = vec4( position, 1.0 );
  a_colour = colour;
}

:fragment
#version 330

in vec3 a_colour;

out vec4 color;

void main()
{
   color = vec4(a_colour, 1.0);
   //color = vec4(1.0, 0.0, 0.0, 1.0);  // Red color (no attribute input)
}
