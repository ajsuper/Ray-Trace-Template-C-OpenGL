#version 450 core
out vec4 FragColor;
in vec2 TexCoords;
uniform float time;
uniform vec2 resolution;
uniform vec3 cameraPos;
uniform vec3 cameraDir;

struct rayStruct {
    vec3 origin;
    vec3 direction;
} ray;


float rand(vec2 uv, float timeX)
{
    float n = fract(sin(dot(uv*timeX, vec2(12.9898, 78.233))) * 43758.5453);
    return n;
}


vec3 rayStartDirection(vec2 uv, vec2 res, vec3 cameraPosition, vec3 cameraDirection, float fov) {
    float aspectRatio = res.x / res.y;
    float scale = tan(radians(fov * 0.5));
    vec2 pixelNDC = vec2(
        (-uv.x * scale) * aspectRatio, // Apply aspect ratio to x-coordinate
        uv.y * scale
    );
    vec3 targetPosition = cameraPosition + cameraDirection;
    vec3 lookat = normalize(targetPosition - cameraPosition);
    vec3 right = cross(lookat, vec3(0, 1, 0));
    vec3 actualUp = cross(right, lookat);
    vec3 rayDirection = normalize(pixelNDC.x * right + pixelNDC.y * actualUp + lookat);
    return rayDirection;
}


vec4 castRay(){
    ray.direction = rayStartDirection(TexCoords, resolution, cameraPos, cameraDir, 90);
    ray.origin = cameraPos;
    vec4 color = vec4(ray.direction, 0);
    return vec4(color);
}


void main()
{
    //resolution = vec2(100, 100);
    float idk = time;
    FragColor = castRay();
}
