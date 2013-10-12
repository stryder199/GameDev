#version 330

//////////////////////
////Input Vars
in vec3 inputVertex;
in vec3 inputNormal;
in vec3 inputColor;

//////////////////////
////Output Vars
out vec3 normal;
out vec3 viewDirection;
out vec3 color;

//////////////////////
////Uniform Vars
uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPosition;

void main()
{
	vec4 worldPosition;

	//Calculate the position of the vertex against the world, view and proj matrix
	gl_Position = worldMatrix * vec4(inputVertex, 1.0f);
	gl_Position = viewMatrix * gl_Position;
	gl_Position = projectionMatrix * gl_Position;

	//Set the color in the out var
	color = inputColor;

	//Calculate the normal against the world matrix
	normal = mat3(worldMatrix) * inputNormal;
	normal = normalize(normal);

	//Calculate the position of the vertex only against the worldMatrix
	worldPosition = worldMatrix * vec4(inputVertex, 1.0f);

	//Determine the viewing direction
	viewDirection = cameraPosition - vec3(worldPosition);
	viewDirection = normalize(viewDirection);
}
