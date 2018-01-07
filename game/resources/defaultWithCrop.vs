#version 150

in vec2 aVertexPosition;
in vec2 aVertexUV;
in vec4 aVertexColor;

uniform vec4 uColor;
uniform sampler2D uTexture0;
uniform mat3 uWorldMatrix;
uniform mat3 uViewMatrix;
uniform vec4 uCrop;

out vec4 vColor;
out vec2 vVertexUV;
out vec3 worldPos;

void main(void) 
{
	worldPos = uWorldMatrix * vec3(aVertexPosition, 1.0);
	vec3 viewPos = uViewMatrix * worldPos;
	gl_Position = vec4(viewPos.x, viewPos.y, 0.0, 1.0);

	vColor = uColor * aVertexColor;
	vVertexUV = aVertexUV;
}
