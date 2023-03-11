#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;

float smooth_step( float edge0, float edge1, float x )
{
    //float p = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    //float v = p * p * (3.0 - 2.0 * p);
    
   float v = smoothstep( edge0, edge1, x );
    return v;
}

void main()
{    
    if(TexCoords.y>0.5)
    {
        color = vec4(spriteColor,1.0f*smooth_step(0.0f,0.4f,1-TexCoords.y))*texture(sprite,TexCoords);
    }
    else
    {
    color = vec4(spriteColor, 1.0) * texture(sprite, TexCoords);
    }
}