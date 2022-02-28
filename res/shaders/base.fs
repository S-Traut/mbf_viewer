#version 330 core
precision mediump float;

uniform sampler2D u_tex[2];

in vec2 uv;
in float v_texid;
out vec4 frag_color;

void main()
{
  int id = int(v_texid);
  frag_color = texture(u_tex[id], uv);
};
