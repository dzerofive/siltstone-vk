#version 450

layout(binding = 0) uniform UBO {
        mat4 view;
        mat4 projection;
        mat4 model;
} ubo;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 color;
layout(location = 0) out vec3 frag_color;

void main() {
	gl_Position = ubo.model * vec4(in_position, 1.0);
	frag_color = color;
}
