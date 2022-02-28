#version 330 core

precision mediump float;

layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in float a_texid;

out vec2 uv;
out float v_texid;

void main()
{
  gl_Position = vec4(a_pos, 0.0, 1.0);
  v_texid = a_texid;
  uv = a_uv;
};
