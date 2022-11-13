#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 res;
uniform float size;

// Output fragment color
out vec4 finalColor;

void main()
{
  vec2 pos = fragTexCoord*2.0-1.0;
  float x, y, xx, yy, rr = size * size, dx, dy, w, w0;
  w0 = 0.3780 / pow(size, 1.975);
  vec2 p;
  vec4 col = vec4(0.0, 0.0, 0.0, 0.0);
  for (dx = 1.0 / res.x, x = -size, p.x = 0.5 + (pos.x * 0.5) + (x * dx); x <= size; x++, p.x += dx) {
    xx = x * x;
    for (dy = 1.0 / res.y, y = -size, p.y = 0.5 + (pos.y * 0.5) + (y * dy); y <= size; y++, p.y += dy) {
      yy = y * y;
      if (xx + yy <= rr)
      {
        w = w0 * exp((-xx - yy) / (2.0 * rr));
        col += texture2D(texture0, p) * w;
      }
    }
  }
  finalColor = col;
}