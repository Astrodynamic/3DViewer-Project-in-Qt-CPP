#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

uniform vec2    u_viewportInvSize;
uniform float   u_thickness;

void main() {
    vec4 p1 = gl_in[0].gl_Position;
    vec4 p2 = gl_in[1].gl_Position;
    vec4 p3 = gl_in[2].gl_Position;

    vec2 dir    = normalize((p2.xy/p2.w - p1.xy/p1.w) * u_viewportInvSize);
    vec2 offset = vec2(-dir.y, dir.x) * u_thickness / u_viewportInvSize;

    gl_Position = p1 + vec4(offset.xy * p1.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 - vec4(offset.xy * p1.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p2 + vec4(offset.xy * p2.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p2 - vec4(offset.xy * p2.w, 0.0, 0.0);
    EmitVertex();

    dir    = normalize((p3.xy/p3.w - p2.xy/p2.w) * u_viewportInvSize);
    offset = vec2(-dir.y, dir.x) * u_thickness / u_viewportInvSize;

    gl_Position = p2 + vec4(offset.xy * p2.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p2 - vec4(offset.xy * p2.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p3 + vec4(offset.xy * p3.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p3 - vec4(offset.xy * p3.w, 0.0, 0.0);
    EmitVertex();

    dir    = normalize((p1.xy/p1.w - p3.xy/p3.w) * u_viewportInvSize);
    offset = vec2(-dir.y, dir.x) * u_thickness / u_viewportInvSize;

    gl_Position = p1 + vec4(offset.xy * p1.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p1 - vec4(offset.xy * p1.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p3 + vec4(offset.xy * p3.w, 0.0, 0.0);
    EmitVertex();
    gl_Position = p3 - vec4(offset.xy * p3.w, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
