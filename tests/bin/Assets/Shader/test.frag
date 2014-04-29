#version 330

out vec4 outColor;

in Data {
	vec3 normal;
	vec4 eye;
	vec4 color;
} dataIn;

struct Material {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	float shininess;
};
uniform Material material;


struct DirectionalLight {
	vec3 dir; // camera space
};
uniform DirectionalLight light;

void main() {
	
	// set the specular term to black
	vec4 spec = vec4(0.0);

	// normalize both input vectors
	vec3 n = normalize(dataIn.normal);
	vec3 e = normalize(vec3(dataIn.eye));
	float intensity = max(dot(n,light.dir), 0.0);

	// if the vertex is lit compute the specular color
	if (intensity > 0.0) {
		// compute the half vector
		vec3 h = normalize(light.dir + e);
		// compute the specular term into spec
		float intSpec = max(dot(h,n), 0.0);
		spec = material.specular * pow(intSpec,material.shininess);
	}
	
	outColor = max(intensity *  material.diffuse + spec, material.ambient);
	//outColor = vec4(n,1);
}