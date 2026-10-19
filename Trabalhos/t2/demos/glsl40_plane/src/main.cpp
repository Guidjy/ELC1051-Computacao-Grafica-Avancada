#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <initializer_list>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "glslprogram.h"
#include "LineMesh.h"
#include "CardanJoint.h"

using glm::vec3;
using glm::mat4;

#define WINDOW_WIDTH	1400
#define WINDOW_HEIGHT	700

static const float PI = 3.14159265359f;
static const float DEG = PI / 180.0f;
static const float MAX_BEND = 45.0f * DEG;

// colors
static const vec3 INPUT_COLOR(0.30f, 0.60f, 1.00f);
static const vec3 OUTPUT_COLOR(1.00f, 0.55f, 0.15f);
static const vec3 CROSS_COLOR(1.00f, 0.90f, 0.20f);
static const vec3 GRID_COLOR(0.22f, 0.22f, 0.25f);
static const vec3 LIMIT_COLOR(0.35f, 0.13f, 0.13f);

// graph window in data units: x = input angle (deg), y = speed / input speed
static const float GRAPH_Y_MIN = 0.5f;
static const float GRAPH_Y_MAX = 1.5f;

GLFWwindow* window;
GLSLProgram* shader;

CardanJoint joint;
float theta1 = 0.0f;         // input shaft angle (rad)
float inputSpeed = PI;       // input angular speed w1 (rad/s), constant
bool paused = false;
float camYaw = -35.0f * DEG;
float camPitch = 25.0f * DEG;

// meshes: static scene parts, moving parts (drawn with their own model matrix) and graph parts
LineMesh* sceneMesh;     // floor grid and the -45..45 deg limit fan, world space
LineMesh* inputMesh;     // input shaft + yoke, in the input shaft's local frame
LineMesh* crossMesh;     // crosspiece, local frame
LineMesh* outputMesh;    // output shaft + yoke, local frame (before tilt and spin)
LineMesh* graphAxes;     // axes of the graph
LineMesh* graphCurves;   // speed curves, rebuilt when the angle changes
LineMesh* graphMarker;   // current point on the curve, rebuilt every frame

// glfwGetKey that fires only once per key press
char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY) (glfwGetKey(WINDOW, KEY) ? (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) : (keyOnce[KEY] = false))


// geometry

void buildSceneMeshes()
{
	const vec3 X(1, 0, 0), Y(0, 1, 0), Z(0, 0, 1);

	// floor grid
	sceneMesh = new LineMesh();
	for (int i = -5; i <= 5; i++)
	{
		sceneMesh->addLine(vec3(i, -1.5f, -5), vec3(i, -1.5f, 5), GRID_COLOR);
		sceneMesh->addLine(vec3(-5, -1.5f, i), vec3(5, -1.5f, i), GRID_COLOR);
	}
	// fan in the XY plane showing how far the secondary shaft may tilt (-45..45 deg)
	
	float len = 3.0f;
	const int arcSegments = 32;
	for (int i = 0; i < arcSegments; i++)
	{
		float a0 = -MAX_BEND + 2 * MAX_BEND * i / arcSegments;
		float a1 = -MAX_BEND + 2 * MAX_BEND * (i + 1) / arcSegments;
		sceneMesh->addLine(len * vec3(std::cos(a0), std::sin(a0), 0), len * vec3(std::cos(a1), std::sin(a1), 0), LIMIT_COLOR);
	}
	sceneMesh->addLine(vec3(0), len * vec3(std::cos(MAX_BEND), std::sin(MAX_BEND), 0), LIMIT_COLOR);
	sceneMesh->addLine(vec3(0), len * vec3(std::cos(MAX_BEND), -std::sin(MAX_BEND), 0), LIMIT_COLOR);
	sceneMesh->upload();

	// input shaft along -X, yoke fork ends at the cross pins (0, +-0.5, 0)
	inputMesh = new LineMesh();
	inputMesh->addLine(vec3(-4.0f, 0, 0), vec3(-0.7f, 0, 0), INPUT_COLOR);
	inputMesh->addLine(vec3(-0.7f, -0.5f, 0), vec3(-0.7f, 0.5f, 0), INPUT_COLOR);
	inputMesh->addLine(vec3(-0.7f, -0.5f, 0), vec3(0, -0.5f, 0), INPUT_COLOR);
	inputMesh->addLine(vec3(-0.7f, 0.5f, 0), vec3(0, 0.5f, 0), INPUT_COLOR);
	inputMesh->addCircle(vec3(-2.5f, 0, 0), Y, Z, 0.35f, 32, INPUT_COLOR);  // disk with a spoke
	inputMesh->addLine(vec3(-2.5f, 0, 0), vec3(-2.5f, 0.35f, 0), INPUT_COLOR); // to see it spin
	inputMesh->upload();

	// crosspiece arm a along Y (input yoke), arm b along Z (output yoke)
	crossMesh = new LineMesh();
	crossMesh->addLine(vec3(0, -0.5f, 0), vec3(0, 0.5f, 0), CROSS_COLOR);
	crossMesh->addLine(vec3(0, 0, -0.5f), vec3(0, 0, 0.5f), CROSS_COLOR);
	crossMesh->addCircle(vec3(0), Y, Z, 0.08f, 16, CROSS_COLOR);
	crossMesh->upload();

	// output shaft along +X, yoke fork ends at the cross pins (0, 0, +-0.5)
	outputMesh = new LineMesh();
	outputMesh->addLine(vec3(0.7f, 0, 0), vec3(4.0f, 0, 0), OUTPUT_COLOR);
	outputMesh->addLine(vec3(0.7f, 0, -0.5f), vec3(0.7f, 0, 0.5f), OUTPUT_COLOR);
	outputMesh->addLine(vec3(0.7f, 0, -0.5f), vec3(0, 0, -0.5f), OUTPUT_COLOR);
	outputMesh->addLine(vec3(0.7f, 0, 0.5f), vec3(0, 0, 0.5f), OUTPUT_COLOR);
	outputMesh->addCircle(vec3(2.5f, 0, 0), Y, Z, 0.35f, 32, OUTPUT_COLOR);
	outputMesh->addLine(vec3(2.5f, 0, 0), vec3(2.5f, 0, 0.35f), OUTPUT_COLOR);
	outputMesh->upload();
}

void buildGraphAxes()
{
	graphAxes = new LineMesh();
	vec3 axisColor(0.7f);
	graphAxes->addLine(vec3(0, GRAPH_Y_MIN, 0), vec3(360, GRAPH_Y_MIN, 0), axisColor);
	graphAxes->addLine(vec3(0, GRAPH_Y_MIN, 0), vec3(0, GRAPH_Y_MAX, 0), axisColor);
	graphAxes->upload();

	graphCurves = new LineMesh();
	graphMarker = new LineMesh();
}

// adds the curve w_out / w_in over one input turn for the given bend angle
void addSpeedCurve(LineMesh* mesh, float beta, vec3 color)
{
	CardanJoint j = joint;
	j.beta = beta;
	const int samples = 360;
	float prevX = 0, prevY = j.speedRatio(0);
	for (int i = 1; i <= samples; i++)
	{
		float x = 360.0f * i / samples;
		float y = j.speedRatio(x * DEG);
		mesh->addLine(vec3(prevX, prevY, 0), vec3(x, y, 0), color);
		prevX = x;
		prevY = y;
	}
}

void rebuildGraphCurves()
{
	graphCurves->clear();
	// input speed is constant -> horizontal line at 1
	graphCurves->addLine(vec3(0, 1, 0), vec3(360, 1, 0), INPUT_COLOR);
	addSpeedCurve(graphCurves, joint.beta, OUTPUT_COLOR);
	graphCurves->upload();
}

void rebuildGraphMarker(float ratio)
{
	float x = theta1 / DEG;
	graphMarker->clear();
	graphMarker->addLine(vec3(x, GRAPH_Y_MIN, 0), vec3(x, GRAPH_Y_MAX, 0), vec3(0.6f));
	// x and y have very different units here, so the "circle" gets a separate radius per axis
	graphMarker->addCircle(vec3(x, ratio, 0), vec3(3.6f, 0, 0), vec3(0, 0.02f, 0), 1.0f, 16, OUTPUT_COLOR);
	graphMarker->upload();
}


// frame

void processInput(float dt)
{
	bool changed = false;
	float bendSpeed = 20.0f * DEG * dt;
	if (glfwGetKey(window, GLFW_KEY_UP))    { joint.beta += bendSpeed; changed = true; }
	if (glfwGetKey(window, GLFW_KEY_DOWN))  { joint.beta -= bendSpeed; changed = true; }
	joint.beta = glm::clamp(joint.beta, -MAX_BEND, MAX_BEND);

	if (glfwGetKey(window, GLFW_KEY_A)) camYaw -= 60.0f * DEG * dt;
	if (glfwGetKey(window, GLFW_KEY_D)) camYaw += 60.0f * DEG * dt;
	if (glfwGetKey(window, GLFW_KEY_W)) camPitch += 40.0f * DEG * dt;
	if (glfwGetKey(window, GLFW_KEY_S)) camPitch -= 40.0f * DEG * dt;
	camPitch = glm::clamp(camPitch, -80.0f * DEG, 80.0f * DEG);

	if (glfwGetKeyOnce(window, GLFW_KEY_EQUAL) || glfwGetKeyOnce(window, GLFW_KEY_KP_ADD))
		inputSpeed = std::fmin(inputSpeed + 0.5f, 4 * PI);
	if (glfwGetKeyOnce(window, GLFW_KEY_MINUS) || glfwGetKeyOnce(window, GLFW_KEY_KP_SUBTRACT))
		inputSpeed = std::fmax(inputSpeed - 0.5f, 0.5f);
	if (glfwGetKeyOnce(window, GLFW_KEY_SPACE))
		paused = !paused;
	if (glfwGetKeyOnce(window, 'R'))
	{
		joint.beta = 30.0f * DEG;
		inputSpeed = PI;
		theta1 = 0.0f;
		changed = true;
	}

	if (changed)
		rebuildGraphCurves();
}

void drawMesh(const LineMesh* mesh, const mat4& viewProj, const mat4& model)
{
	shader->setUniform("MVP", viewProj * model);
	mesh->draw();
}

void mainLoop()
{
	double lastTime = glfwGetTime();
	double lastTitle = 0.0;
	do
	{
		double now = glfwGetTime();
		float dt = (float)(now - lastTime);
		lastTime = now;

		processInput(dt);
		if (!paused)
			theta1 = std::fmod(theta1 + inputSpeed * dt, 2 * PI);  // constant input speed

		JointPose pose = joint.solve(theta1);
		float ratio = joint.speedRatio(theta1);
		rebuildGraphMarker(ratio);

		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ---- main viewport: 3D joint over the whole window
		// glViewport maps normalized device coordinates to this part of the window
		glViewport(0, 0, fbWidth, fbHeight);
		mat4 projection = glm::perspective(45.0f * DEG, (float)fbWidth / fbHeight, 0.1f, 100.0f);
		vec3 eye = 7.0f * vec3(std::cos(camPitch) * std::sin(camYaw), std::sin(camPitch), std::cos(camPitch) * std::cos(camYaw));
		mat4 view = glm::lookAt(eye, vec3(0), vec3(0, 1, 0));
		mat4 viewProj = projection * view;

		drawMesh(sceneMesh, viewProj, mat4(1.0f));
		drawMesh(inputMesh, viewProj, pose.inputModel);
		drawMesh(crossMesh, viewProj, pose.crossModel);
		drawMesh(outputMesh, viewProj, pose.outputModel);

		// ---- small viewport in the bottom-left corner: speed graph, drawn on top of the 3D view
		// with a 2D orthographic projection in data units
		int margin = 12;
		int graphW = (int)(fbWidth * 0.32f), graphH = (int)(fbHeight * 0.34f);
		glViewport(margin, margin, graphW, graphH);
		glEnable(GL_SCISSOR_TEST);  // the scissor box limits glClear to the graph's rectangle
		glScissor(margin, margin, graphW, graphH);
		glClearColor(0.09f, 0.09f, 0.11f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.04f, 0.04f, 0.05f, 1.0f);
		glDisable(GL_SCISSOR_TEST);

		glDisable(GL_DEPTH_TEST);  // flat 2D drawing, later lines simply go on top
		mat4 graphProj = glm::ortho(-25.0f, 380.0f, GRAPH_Y_MIN - 0.08f, GRAPH_Y_MAX + 0.08f, -1.0f, 1.0f);
		drawMesh(graphAxes, graphProj, mat4(1.0f));
		drawMesh(graphCurves, graphProj, mat4(1.0f));
		drawMesh(graphMarker, graphProj, mat4(1.0f));
		glEnable(GL_DEPTH_TEST);

		// no text rendering, so the numbers go in the title bar
		if (now - lastTitle > 0.1)
		{
			char title[256];
			snprintf(title, sizeof(title),
				"Cardan joint | shaft angle %.1f deg | w_in %.2f rad/s | w_out %.2f rad/s (x%.3f)%s",
				joint.beta / DEG, inputSpeed, inputSpeed * ratio, ratio, paused ? " | PAUSED" : "");
			glfwSetWindowTitle(window, title);
			lastTitle = now;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (!glfwWindowShouldClose(window));
}


void errorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

void keyCallback(GLFWwindow* w, int key, int, int action, int)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(w, GL_TRUE);
}

void initGLFW()
{
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);  // multisampling for smoother lines

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cardan joint", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);
}

void initGLEW()
{
	glewExperimental = GL_TRUE;  // expose all core profile entry points
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
}

void initShaders()
{
	shader = new GLSLProgram();
	try {
		shader->compileShader("shader/lines.vert");
		shader->compileShader("shader/lines.frag");
		shader->link();
		shader->use();
	}
	catch (GLSLProgramException& e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

void initializeGL()
{
	glClearColor(0.04f, 0.04f, 0.05f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

void printHelp()
{
	printf("Seta cima/baixo    altera ângulo\n"
		"+/-         aumenta/diminui velocidade\n"
		"Space       pausa\n"
		"W/A/S/D     controla camera\n"
		"R           reseta\n"
		"Grafo: x = ângulo de entrada 0..360 graus, y = velocidade / velocidade de entrada 0.5..1.5\n"
		"  azul = entrada, laranja = saída no ângulo atual\n");
}

int main(void)
{
	initGLFW();
	initGLEW();
	initializeGL();
	initShaders();
	printHelp();

	joint.beta = 30.0f * DEG;

	buildSceneMeshes();
	buildGraphAxes();
	rebuildGraphCurves();

	mainLoop();

	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}
