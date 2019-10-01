#version 330 core
// Vertex color (interpolated/fragment)
in vec3 vPos;

// Uniforms 
uniform sampler3D texture1;
uniform sampler2D texture2;
uniform vec2 windowSize;

// Fragment Color
out vec4 fragColor;

void main()
{
	

	vec4 color = vec4(0.0f,0.0f,0.0f,1.0f);
	vec2 coord = gl_FragCoord.xy/ windowSize;
	
	vec3 rayDir = vec3(texture(texture2,coord).xyz - vPos);
	vec3 rayIn = vPos;
	float D = length(rayDir);
	rayDir = normalize(rayDir);

	for(float i=0.0f;i<D;i+=1.0f/256){
	// Ai y Ci se consultan en la TF 
		color.rgb += texture(texture1,rayIn).r * vec3(texture(texture1,rayIn).r) * color.a;
		color.a *= 1 - texture(texture1,rayIn).r;
		if(1 - color.a >= 0.99f) break;
		rayIn += rayDir * 1.0f/256;
	}
	color.a = 1.0f;
	fragColor = color;


}
