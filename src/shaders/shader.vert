#version 450

layout(location = 0) out vec3 fragColor;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(binding = 0 ) uniform UBO {
   mat4 projectionMatrix;
   mat4 viewTranslation;
   mat4 viewRotation;
} inst;

layout( push_constant )  uniform model {
    vec4 translate;
    mat4 rot;
} modelTransform;

void main() {
    vec4 modelView = ( modelTransform.rot*vec4(inPosition,1) ) + modelTransform.translate;
    gl_Position = inst.projectionMatrix * inst.viewRotation * inst.viewTranslation * modelView;
    fragColor = inColor;
}