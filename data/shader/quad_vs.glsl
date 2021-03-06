layout (location = 0) in vec3 VS_IN_Position;
layout (location = 1) in vec2 VS_IN_TexCoord;

out vec2 PS_IN_TexCoord;

void main()
{
    PS_IN_TexCoord = VS_IN_TexCoord;
    gl_Position = vec4(VS_IN_Position, 1.0);
}