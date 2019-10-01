#define _CRT_SECURE_NO_WARNINGS

#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <stb_image.h>

#include "Shader.h"


using namespace std;


// Window current width
unsigned int windowWidth = 800;
// Window current height
unsigned int windowHeight = 600;
// Window title
const char *windowTitle = "Basic Demo";
// Window pointer
GLFWwindow *window;

// Shader object
Shader *shaderBasic;
Shader *shaderDebug;
Shader *shaderPosMap;
Shader *shaderRaycast;
Shader *shaderDebugBoth;
Shader *shaderDebugPos;

// Index (GPU) of the geometry buffer
unsigned int planeVBO;
// Index (GPU) vertex array object
unsigned int planeVAO;
// Index (GPU) of the texture

// Index (GPU) of the geometry buffer
unsigned int cubeVBO;
// Index (GPU) vertex array object
unsigned int cubeVAO;
// Index (GPU) of the texture

unsigned int textureID;

//Frame Buffer Object for position map
unsigned int posMapFBO;
//Texture for depth map
unsigned int posMap;


// Camera Start Position
glm::vec3 position = glm::vec3(0, 0, 5);
// horizontal angle : toward -Z
float horizontalAngle = -3.14;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

// Direction of the Camera
glm::vec3 direction;
// Up vector for the Camera
glm::vec3 up;


//USED FOR DELTA TIME
float currentTime = 0;
float lastTime = 0;

// Right button is currently pressed
bool rightButtonPressed = false;


/**
 * Handles the window resize
 * @param{GLFWwindow} window pointer
 * @param{int} new width of the window
 * @param{int} new height of the window
 * */
void resize(GLFWwindow *window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    // Sets the OpenGL viewport size and position
    glViewport(0, 0, windowWidth, windowHeight);
}
/**
 * Initialize the glfw library
 * @returns{bool} true if everything goes ok
 * */
bool initWindow()
{
    // Initialize glfw
    glfwInit();
    // Sets the Opegl context to Opengl 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creates the window
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);

    // The window couldn't be created
    if (!window)
    {
        std::cout << "Failed to create the glfw window" << std::endl;
        glfwTerminate(); // Stops the glfw program
        return false;
    }

    // Creates the glfwContext, this has to be made before calling initGlad()
    glfwMakeContextCurrent(window);

    // Window resize callback
    glfwSetFramebufferSizeCallback(window, resize);
    return true;
}
/**
 * Initialize the glad library
 * @returns{bool} true if everything goes ok
 * */
bool initGlad()
{
    // Initialize glad
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // If something went wrong during the glad initialization
    if (!status)
    {
        std::cout << status << std::endl;
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}
/**
 * Initialize the opengl context
 * */
void initGL()
{
    // Enables the z-buffer test
    glEnable(GL_DEPTH_TEST);
    // Sets the ViewPort
    glViewport(0, 0, windowWidth, windowHeight);
    // Sets the clear color
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}
/**
 * Builds all the geometry buffers and
 * loads them up into the GPU
 * (Builds a simple triangle)
 * */
void buildGeometry()
{
    float planeVertices[] = {
        // Bottom left vertex
        -1.0f, -1.0f, 0.0f, // Position
        1.0f, 0.0f, 0.0f,   // Color
        // Bottom right vertex
        1.0f, -1.0f, 0.0f, // Position
        0.0f, 1.0f, 0.0f,  // Color
        // Top left vertex
        -1.0f, 1.0f, 0.0f, // Position
        0.0f, 0.0f, 1.0f,  // Color
		// Top right vertex
		1.0f, 1.0f, 0.0f, // Position
		0.0f, 0.0f, 1.0f,  // Color
		// Top left vertex
		- 1.0f, 1.0f, 0.0f, // Position
		0.0f, 0.0f, 1.0f,  // Color
		// Bottom right vertex
		1.0f, -1.0f, 0.0f, // Position
		0.0f, 1.0f, 0.0f  // Color
    };

    // Creates on GPU the vertex array
    glGenVertexArrays(1, &planeVAO);
    // Creates on GPU the vertex buffer object
    glGenBuffers(1, &planeVBO);
    // Binds the vertex array to set all the its properties
    glBindVertexArray(planeVAO);
    // Sets the buffer geometry data
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);

    // Sets the vertex attributes
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    // Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);

	float cubeVertices[] = {
	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	 0.5f, 0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f,-0.5f,
	 0.5f,-0.5f,-0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	 0.5f, 0.5f,-0.5f,
	-0.5f, 0.5f,-0.5f,
	 0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,
	-0.5f, 0.5f, 0.5f,
	 0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	 0.5f,-0.5f, 0.5f
	};


	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}


bool LoadVolumeFromFile(const char* fileName) {

	//assuming that the data at hand is a 256x256x256 unsigned byte data
	int XDIM = 256, YDIM = 256, ZDIM = 256;
	const int size = XDIM * YDIM * ZDIM;


	FILE *pFile = fopen(fileName, "rb");
	if (NULL == pFile) {
		return false;
	}
	GLubyte* pVolume = new GLubyte[size];
	fread(pVolume, sizeof(GLubyte), size, pFile);
	fclose(pFile);

	//load data into a 3D texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);

	// set the texture parameters
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, XDIM, YDIM, ZDIM, 0, GL_RED, GL_UNSIGNED_BYTE, pVolume);
	delete[] pVolume;
	return true;
}
/**
 * Loads a texture into the GPU
 * @param{const char} path of the texture file
 * @returns{unsigned int} GPU texture index
 * */
unsigned int loadTexture(const char *path)
{
    unsigned int id;
    // Creates the texture on GPU
    glGenTextures(1, &id);
    // Loads the texture
    int textureWidth, textureHeight, numberOfChannels;
    // Flips the texture when loads it because in opengl the texture coordinates are flipped
    stbi_set_flip_vertically_on_load(true);
    // Loads the texture file data
    unsigned char *data = stbi_load(path, &textureWidth, &textureHeight, &numberOfChannels, 0);
    if (data)
    {
        // Gets the texture channel format
        GLenum format;
        switch (numberOfChannels)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        }

        // Binds the texture
        glBindTexture(GL_TEXTURE_2D, id);
        // Creates the texture
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
        // Creates the texture mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set the filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "ERROR:: Unable to load texture " << path << std::endl;
        glDeleteTextures(1, &id);
    }
    // We dont need the data texture anymore because is loaded on the GPU
    stbi_image_free(data);

    return id;
}
/**
 * Initialize everything
 * @returns{bool} true if everything goes ok
 * */
bool init()
{
    // Initialize the window, and the glad components
    if (!initWindow() || !initGlad())
        return false;

    // Initialize the opengl context
    initGL();

    // Loads the shaders
    shaderDebug = new Shader("assets/shaders/debug.vert", "assets/shaders/debug.frag");
	shaderBasic = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	shaderPosMap = new Shader("assets/shaders/posMap.vert", "assets/shaders/posMap.frag");
	shaderDebugPos = new Shader("assets/shaders/debugPosMap.vert", "assets/shaders/debugPosMap.frag");
	shaderRaycast = new Shader("assets/shaders/raycast.vert", "assets/shaders/raycast.frag");
	shaderDebugBoth = new Shader("assets/shaders/debugBoth.vert", "assets/shaders/debugBoth.frag");

    // Loads all the geometry into the GPU
    buildGeometry();
    // Loads the texture into the GPU

	// configure position map FBO
	// -----------------------

	glGenFramebuffers(1, &posMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, posMapFBO);
	
	// create depth texture
	glGenTextures(1, &posMap);
	glBindTexture(GL_TEXTURE_2D, posMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posMap, 0);
	
	//glDrawBuffer(GL_NONE);
	//glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//load volume
	if (LoadVolumeFromFile("assets/volumes/bonsai_256x256x256_uint8.raw")) {
		cout <<"volumen cargado correctamente" << endl;

		glEnable(GL_TEXTURE_3D);
		glActiveTexture(textureID);

	}
	else
	{
		cout << "error cargando el volumen" << endl;
	}

	cout << "id del volumen: " << textureID << endl;
	cout << "id de la textura posicion: " << posMap << endl;

    return true;
}
/**
 * Process the keyboard input
 * There are ways of implementing this function through callbacks provide by
 * the GLFW API, check the GLFW documentation to find more
 * @param{GLFWwindow} window pointer
 * */
void processKeyboardInput(GLFWwindow *window)
{
    // Checks if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // Tells glfw to close the window as soon as possible
        glfwSetWindowShouldClose(window, true);

    // Checks if the r key is pressed
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        // Reloads the shader
        delete shaderDebug;
		delete shaderBasic;
		delete shaderPosMap;
		delete shaderRaycast;

		shaderBasic = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
		shaderDebug = new Shader("assets/shaders/debug.vert", "assets/shaders/debug.frag");
		shaderPosMap = new Shader("assets/shaders/posMap.vert", "assets/shaders/posMap.frag");
		shaderDebugPos = new Shader("assets/shaders/debugPosMap.vert", "assets/shaders/debugPosMap.frag");
		shaderRaycast = new Shader("assets/shaders/raycast.vert", "assets/shaders/raycast.frag");
		shaderDebugBoth = new Shader("assets/shaders/debugBoth.vert", "assets/shaders/debugBoth.frag");
    }

	// Check is the right click of the mouse is pressed
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		rightButtonPressed = true;
	}
	else {
		rightButtonPressed = false;
	}


	lastTime = currentTime;
	currentTime = glfwGetTime();

	float deltaTime = currentTime - lastTime;


	if (rightButtonPressed) {

		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);



		// Compute new orientation
		horizontalAngle += mouseSpeed /* * 1 */ * float(windowWidth / 2 - xpos);
		verticalAngle += mouseSpeed /* * deltaTime */ * float(windowHeight / 2 - ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		up = glm::cross(right, direction);

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			position += direction * deltaTime * speed * 4.0f;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			position -= direction * deltaTime * speed * 4.0f;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			position += right * deltaTime * speed * 4.0f;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			position -= right * deltaTime * speed * 4.0f;
		}

	}
}

/**
 * Render Function
 * */
void render()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, .5f, 1000.0f);
	//glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	glm::mat4 view = glm::lookAt(
		position, // Camera is at (4,3,3), in world space
		position + direction, // and looks at the origin
		up  // Head is up (set to 0,-1,0 to look upside-down) 
	);

	glm::mat4 model = glm::mat4(1.0f); //model matrix: an identity matrix (model will be at the origin)

	//RENDER POSITION MAP

	glCullFace(GL_FRONT);
	glEnable(GL_CULL_FACE);

	shaderPosMap->use();

	shaderPosMap->setMat4("model", model);
	shaderPosMap->setMat4("view", view);
	shaderPosMap->setMat4("projection", projection);

	glViewport(0, 0, windowWidth, windowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, posMapFBO);
	// Clears the color and depth buffers from the frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Binds the vertex array to be drawn
	glBindVertexArray(cubeVAO);
	// Renders the triangle gemotry
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	//bind back the regular framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
    // Clears the color and depth buffers from the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	/*

	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

    // Use the shader
    shaderDebugBoth->use();
    // Binds the vertex array to be drawn
    glBindVertexArray(planeVAO);
    // Renders the triangle gemotry
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

	*/
	
	

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	shaderRaycast->use();

	shaderRaycast->setMat4("model", model);
	shaderRaycast->setMat4("view", view);
	shaderRaycast->setMat4("projection", projection);
	shaderRaycast->setVec2("windowSize", glm::vec2(windowWidth, windowHeight));

	// Binds the vertex array to be drawn
	glBindVertexArray(cubeVAO);
	// Renders the triangle gemotry
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	

    // Swap the buffer
    glfwSwapBuffers(window);

	
}
/**
 * App main loop
 * */
void update()
{
    // Loop until something tells the window, that it has to be closed
    while (!glfwWindowShouldClose(window))
    {
        // Checks for keyboard inputs
        processKeyboardInput(window);

        // Renders everything
        render();

        // Check and call events
        glfwPollEvents();
    }
}
/**
 * App starting point
 * @param{int} number of arguments
 * @param{char const *[]} running arguments
 * @returns{int} app exit code
 * */
int main(int argc, char const *argv[])
{
    // Initialize all the app components
    if (!init())
    {
        // Something went wrong
        std::cin.ignore();
        return -1;
    }

    std::cout << "=====================================================" << std::endl
              << "        Press Escape to close the program            " << std::endl
              << "=====================================================" << std::endl;

    // Starts the app main loop
    update();

    // Deletes the texture from the gpu
    glDeleteTextures(1, &textureID);


    // Deletes the vertex array from the GPU
    glDeleteVertexArrays(1, &planeVAO);
    // Deletes the vertex object from the GPU
    glDeleteBuffers(1, &planeVBO);

	// Deletes the vertex array from the GPU
	glDeleteVertexArrays(1, &cubeVAO);
	// Deletes the vertex object from the GPU
	glDeleteBuffers(1, &cubeVBO);


    // Destroy the shader
    delete shaderBasic;
	delete shaderDebug;

    // Stops the glfw program
    glfwTerminate();

    return 0;
}
