#version 330

//////////////////////
////Input Vars
in vec3 normal;
in vec3 viewDirection;
in vec3 color;

//////////////////////
////Output Vars
out vec4 outputColor;

//////////////////////
////Uniform Vars
uniform vec3 lightDirection;
uniform vec4 diffuseLightColor;
uniform vec4 ambientLightColor;
uniform vec4 specularLightColor;
uniform float specularPower;


void main()
{
	float diffuseCoefficient;
	vec3 lightDir;
	vec3 reflection;
	vec4 finalLightColor;
	vec4 specularColor;

	//Init the specular color
	specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	//Minimum brightness that the pixel must be set to
	outputColor = ambientLightColor;

	//Invert the light direction
	lightDir = -lightDirection;

	//Calc the amount of light on this pixel
	diffuseCoefficient = clamp(dot(normal, lightDir), 0.0f, 1.0f);

	if(diffuseCoefficient > 0.0f)
	{
		//Find the final diffuse light color if the object isnt pointing away from the light source
		outputColor += (diffuseLightColor * diffuseCoefficient);
		outputColor = clamp(outputColor, 0.0f, 1.0f);

		//Calculate the reflection vector vased on the light intensity, normal vector and light direction
		reflection = normalize(2 * diffuseCoefficient * normal - lightDir); 

		//Determine the amount of specular light based on the reflection vector, viewing direction and specular power
		float specularCoefficient = clamp(dot(reflection, viewDirection), 0.0f, 1.0f);
		specularCoefficient = pow(specularCoefficient, specularPower);
		specularColor = specularLightColor*specularCoefficient;
	}
	//Find the final color by multing the lightColor with the texture color
	outputColor = outputColor*vec4(color, 1.0f);

	//Add the specular component to the final color
	outputColor = clamp(outputColor+specularColor, 0.0f, 1.0f);
}
