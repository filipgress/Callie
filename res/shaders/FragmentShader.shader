#version 410

in vec3 Normal;
out vec4 color;

struct DirectionalLight{
    vec4 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

uniform vec4 col;
uniform DirectionalLight directionalLight;

void main(){
    vec4 ambientColour = directionalLight.colour * directionalLight.ambientIntensity;

    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColor = directionalLight.colour * directionalLight.diffuseIntensity * diffuseFactor;

    color = col * (ambientColour + diffuseColor);
}