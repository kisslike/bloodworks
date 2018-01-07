#version 150

in vec2 pos;
in vec2 uv;
in float time;

in float color;
in vec2 moveSpeed;
in float initialScale;
in float scaleSpeed;
in float initialAlpha;
in float fadeOutSpeed;

uniform float uCurrentTime;
uniform sampler2D uTexture0;
uniform mat3 uViewMatrix;

out vec4 vColor;
out vec2 vVertexUV;


void main(void) 
{
	float dt = uCurrentTime - time;
	dt = min(dt, 10.0);
	
	float curScale = initialScale + scaleSpeed * dt;
	vec3 worldPos = vec3(pos + moveSpeed * dt + (uv * 2.0 - vec2(1.0, 1.0)) * curScale, 1.0);
	vec3 viewPos = uViewMatrix * worldPos;
	gl_Position = vec4(viewPos.x, viewPos.y, 0.0, 1.0);
	
	vec3 finalColor = vec3(color);
	
	
	float t = dt;
	float whiteDuration = 0.01 * fadeOutSpeed;
	if (t < whiteDuration)
	{
		float l = t / whiteDuration;
		finalColor.r = mix(1.0, 1.0, l);
		finalColor.g = mix(0.3, 0.5, l);
		finalColor.b = mix(0.0, 0.0, l);
	}
	else 
	{
		t -= whiteDuration;
		float yellowDuration = 0.02 * fadeOutSpeed;
		if (t < yellowDuration)
		{
			float l = t / yellowDuration;
			finalColor.r = mix(0.8, color, l);
			finalColor.g = mix(0.4, color, l);
			finalColor.b = mix(0.0, color, l);
		}
		else
		{
			finalColor = vec3(color);
		}
		
	}
	vColor = vec4(finalColor, initialAlpha - fadeOutSpeed * dt);
	
	vVertexUV = uv;
}
