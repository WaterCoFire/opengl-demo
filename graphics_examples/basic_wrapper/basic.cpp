/*
 Basic example to demonstrate a simple OpenGL program using a GLFW wrapper
 class and shader loaders
*/


/* Link to static libraries, you could define these as linker inputs in the project settings instead
if you prefer */
#ifdef _DEBUG
#pragma comment(lib, "glfw3D.lib")
#else
#pragma comment(lib, "glfw3.lib")
#endif
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
   also includes the OpenGL extension initialisation */
#include "wrapper_glfw.h"
#include <iostream>

GLuint positionBufferObject;
GLuint program;
GLuint vao;

using namespace std;

// Personal modification BELOW
// Global variables for keyboard control
float offsetX = 0.0f;
float offsetY = 0.0f;
float colorR = 0.0f, colorG = 1.0f, colorB = 1.0f; // Initial color
GLuint offsetLocation;
GLuint colorLocation;

/*
This function is called before entering the main rendering loop.
Use it for all you initialisation stuff
*/
void init(GLWrapper *glw) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    float vertexPositions[] = {
        0.75f, 0.75f, 0.0f, 1.0f,
        0.75f, -0.75f, 0.0f, 1.0f,
        -0.75f, -0.75f, 0.0f, 1.0f,
    };

    glGenBuffers(1, &positionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    try {
        program = glw->LoadShader("basic.vert", "basic.frag");
    } catch (exception &e) {
        cout << "Caught exception: " << e.what() << endl;
        cin.ignore();
        exit(0);
    }

    glw->DisplayVersion();

    // Personal modification BELOW
    // For keyboard control
    glUseProgram(program);
    offsetLocation = glGetUniformLocation(program, "offset");
    colorLocation = glGetUniformLocation(program, "color");
    glUseProgram(0);
}

// Called to update the display.
// You should call glfwSwapBuffers() after all of your rendering to display what you rendered.
void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    // Personal modification BELOW
    // Update uniform
    glUniform2f(offsetLocation, offsetX, offsetY);
    glUniform3f(colorLocation, colorR, colorG, colorB);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);
    glUseProgram(0);
}


/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow *window, int k, int s, int action, int mods) {
    if (action != GLFW_PRESS) return;

    cout << "KEY: " << (char) k << endl;

    if (k == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Personal modification below: Key control
    const float moveStep = 0.05f;
    const float colorStep = 0.1f;

    switch (k) {
        case GLFW_KEY_W: offsetY += moveStep;
            break;
        case GLFW_KEY_S: offsetY -= moveStep;
            break;
        case GLFW_KEY_A: offsetX -= moveStep;
            break;
        case GLFW_KEY_D: offsetX += moveStep;
            break;
        case GLFW_KEY_R: colorR = fmin(colorR + colorStep, 1.0f);
            break;
        case GLFW_KEY_G: colorG = fmin(colorG + colorStep, 1.0f);
            break;
        case GLFW_KEY_B: colorB = fmin(colorB + colorStep, 1.0f);
            break;
    }
}

/* An error callback function to output GLFW errors*/
static void error_callback(int error, const char *description) {
    fputs(description, stderr);
}

/* Entry point of program */
int main(int argc, char *argv[]) {
    const char *title = "Hello World LOL";
    GLWrapper *glw = new GLWrapper(1024, 768, title);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD. Exiting." << endl;
        return -1;
    }

    glw->setRenderer(display);
    glw->setKeyCallback(keyCallback);
    glw->setReshapeCallback(reshape);
    glw->setErrorCallback(error_callback);

    init(glw);

    glw->eventLoop();

    delete (glw);
    return 0;
}
