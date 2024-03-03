#pragma once

constexpr char kVS_FS[] = R"(#version 300 es
precision lowp float;

out vec2 uv;

vec2 vertices[3] = vec2[3](
    vec2(-1, -1),
    vec2( 3, -1),
    vec2(-1,  3)
);

uniform float scale_x;

void main()
{
    vec4 pos = vec4(vertices[gl_VertexID],0,1);
    uv = pos.xy;
    uv.x *= scale_x;
    uv = 0.5 * uv.xy + vec2(0.5);
    gl_Position = pos;
})";

constexpr char kFS_FS[] = R"(#version 300 es
precision lowp float;

in vec2 uv;
out vec4 FragColor;

uniform sampler2D s_tex;

void main()
{
  FragColor = vec4(texture(s_tex, uv).rgb, 1.);
})";

constexpr char kFS_FS_YUV420[] = R"(#version 300 es
precision lowp float;

in vec2 uv;
out vec4 FragColor;

// uniform sampler2D tex;
uniform sampler2D s_textureY;
uniform sampler2D s_textureU;
uniform sampler2D s_textureV;

void main()
{
  // rotate uv 90 deg, flip
  // vec2 uv2 = vec2(1.-uv.y, 1.-uv.x); // Phone cam: portrait, back
  // vec2 uv2 = vec2(uv.x, 1.-uv.y);    // Mac cam: landscape, front 
  vec2 uv2 = vec2(1.-uv.x, 1.-uv.y);    // Mac cam: landscape, front, mirrored
  float y = texture(s_textureY, uv2).r;
  float u = texture(s_textureU, uv2).r;
  float v = texture(s_textureV, uv2).r;
  // https://en.wikipedia.org/wiki/YCoCg
  u = u - 0.5;
  v = v - 0.5;
  float r = y + 1.403 * v;
  float g = y - 0.344 * u - 0.714 * v;
  float b = y + 1.770 * u;
  FragColor = vec4(r, g, b, 1.0);

#if 0 // Debug
  float _b = texture(s_textureY, uv2).r;
  float _g = texture(s_textureU, uv2).r;
  float _r = texture(s_textureV, uv2).r;
  FragColor = vec4(_r, _g, _b, 1.);
#endif   
})";

constexpr char kVS_Cube[] = R"(
precision lowp float;

attribute vec3 aPos;
attribute vec3 aNormal;

varying vec3 Normal;
varying vec3 Position;

uniform mat4 m;
// uniform mat4 ti_m;
uniform mat4 mvp;

void main()
{
    // Normal = mat3(ti_m) * aNormal;
    Normal = aNormal;
    Position = vec3(m * vec4(aPos, 1.0));
    gl_Position = mvp * vec4(aPos, 1.0);
})";

constexpr char kFS_Cube[] = R"(
precision lowp float;

varying vec3 Position;
varying vec3 Normal;

void main()
{    
    gl_FragColor = vec4(0.5+0.5*Normal, 1.0);
    
#if 0 // Debug
    if (gl_FragCoord.x < 512.0) {        
        gl_FragColor = vec4(abs(Position), 1.0);
        // gl_FragColor = vec4(1., 1., 0., 1.0);
    }
    if (gl_FragCoord.x < 256.0) {
        gl_FragColor = vec4(1., 0., 0., 1.0);
    }
#endif    
})";

