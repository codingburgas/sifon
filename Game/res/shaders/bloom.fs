#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 res;

// GAUSSIAN BLUR SETTINGS {{{
uniform float directions /* = 32.0 */; // BLUR DIRECTIONS (Default 16.0 - More is better but slower)
uniform float quality /* = 7.0 */; // BLUR QUALITY (Default 4.0 - More is better but slower)
uniform float size /* = 32.0 */; // BLUR SIZE (Radius)
// GAUSSIAN BLUR SETTINGS }}}

// Output fragment color
out vec4 finalColor;

void main()
{

//const float directions = 32.0; // BLUR DIRECTIONS (Default 16.0 - More is better but slower)
//const float quality = 7.0; // BLUR QUALITY (Default 4.0 - More is better but slower)
//const float size = 32.0; // BLUR SIZE (Radius)
    float Pi = 6.28318530718; // Pi*2
   
    vec2 Radius = size/res.xy;
    
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragTexCoord;
    // Pixel colour
    vec4 Color = texture(texture0, uv);
    
    // Blur calculations
    for( float d=0.0; d<Pi; d+=Pi/directions)
    {
		for(float i=1.0/quality; i<=1.0; i+=1.0/quality)
        {
			Color += texture( texture0, uv+vec2(cos(d),sin(d))*Radius*i);		
        }
    }
    
    // Output to screen
    Color /= quality * directions - 15.0;
    finalColor =  Color;
}