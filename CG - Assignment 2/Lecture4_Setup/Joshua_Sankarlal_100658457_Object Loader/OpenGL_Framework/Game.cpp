#include "Game.h"
#include "Transform.h"
#include <string>
#include "Mesh.h"

Mesh object;


const char* vertexShaderSource = 
	"#version 420\n"
	"layout(location = 0) in vec3 mesh_position;\n"
	"layout(location = 1) in vec2 mesh_UVs;\n"
	"layout(location = 2) in vec3 mesh_normal;\n"
	""
	"uniform mat4 u_model;\n"
	"uniform mat4 u_view;\n"
	"uniform mat4 u_projection;\n"
	""
	"out vec3 color;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = u_projection * u_view * u_model * vec4(mesh_position, 1.0);\n"
	"	color = vec3( 1.f,1.f,1.f);\n"
	"}\n";

const char* fragmentShaderSource = 
	"#version 420\n"
	"in vec3 color;\n"
	"out vec4 pixelColor;\n"
	"void main() { pixelColor = vec4(color, 1.0f); }\n";

Game::Game()
{
}

Game::~Game()
{
	delete updateTimer;

	//...
}

void Game::initializeGame()
{
	updateTimer = new Timer();
	object.LoadFromFile("Monkey.obj");
	//std::cout << object.GetNumFaces() << std::endl;

	int success = GL_FALSE;
	//Create shaders program 
	program = glCreateProgram();
	// ...

	//Create Vertex Shader
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		exit(0);
	}

	//Create Fragment Shader
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		exit(0);
	}

	//Build program
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) { exit(EXIT_FAILURE); };

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
	
	//Setup scene
	//glEnable(GL_DEPTH_TEST);
	glClearColor(.2f, .2f, .2f, 1); // Black.
	float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
	camera.perspective(60.0f, aspect, 1.0f, 1000.0f);
	camera.m_pLocalPosition = vec3(0.0f, 0.0f, 0.0f);
	//camera.setRotationAngleX(-45.0f);
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();	
	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	object.setPosition(vec3(0,0,0));
	camera.update(deltaTime);
	// ...
}

void Game::draw()
{
	glEnable(GL_DEPTH_TEST);
	//glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind Program

	glUseProgram(program);

	//Send MVP to the shaders
	int modelLoc = glGetUniformLocation(
		program,			
		"u_model");
	int viewLoc = glGetUniformLocation(program,
		"u_view");
	int projectionLoc = glGetUniformLocation(program,
		"u_projection");

	

	glUniformMatrix4fv(modelLoc, 1, false, object.getLocalToWorldMatrix().data);
	glUniformMatrix4fv(viewLoc, 1, false, camera.getLocalToWorldMatrix().GetInverse().data);
	glUniformMatrix4fv(projectionLoc, 1, false, camera.getProjection().data);

	//Bind Mesh
	glBindVertexArray(object.VAO);

	glDrawArrays(GL_TRIANGLES, object.VAO,object.GetNumVertices());

	glBindVertexArray(GL_NONE);
	glutSwapBuffers();

}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:

			break;
		case GLUT_RIGHT_BUTTON:
		
			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else
	{

	}
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{
}
