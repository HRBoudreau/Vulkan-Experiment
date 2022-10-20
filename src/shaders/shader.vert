#version 450

layout(location = 0) out vec3 fragColor;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(binding = 0 ) uniform UBO {
   mat4 perspectiveMatrix;
   mat4 viewMatrix;
} inst;

layout( push_constant )  uniform model {
    vec4 model;
} modelTransform;

void main() {
    vec4 modelView = vec4(inPosition, 1.0) + modelTransform.model;
    gl_Position = inst.perspectiveMatrix * inst.viewMatrix * modelView;
    fragColor = inColor;
}