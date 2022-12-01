#version 410 core
out vec4 FragColor;

uniform vec4 PointColor;
uniform bool RoundPoint;

void main() {
  if ((dot(gl_PointCoord - 0.5, gl_PointCoord - 0.5) > 0.25) && RoundPoint) {
      discard;
  } else {
      FragColor =  PointColor;
  }
}
