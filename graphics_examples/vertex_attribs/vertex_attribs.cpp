/*
 Example to define vertex attributes on a triangle
 Shows colour interpolation by the rasterizer and passes
 colour values from vertex to fragment shader
 Iain Martin September 2018
*/


/* Link to static libraries, could define these as linker inputs in the project settings instead
if you prefer */
#ifdef _DEBUG
#pragma comment(lib, "glfw3D.lib")
#else
#pragma comment(lib, "glfw3.lib")
#endif
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
   includes the GLFW windowing functionality and shader handling */
#include "wrapper_glfw.h"
#include <iostream>

GLuint positionBufferObject, colourObject;
GLuint program;
GLuint vao;

using namespace std;

/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw) {
    // Generate index (name) for one vertex array object
    glGenVertexArrays(1, &vao);

    // Create the vertex array object and make it current
    glBindVertexArray(vao);

    /* Define vertex positions in homogeneous coordinates */
    float vertexPositions[] = {
        0.75f, 0.75f, 0.0f, 1.0f,
        0.75f, -0.75f, 0.0f, 1.0f,
        -0.75f, -0.75f, 0.0f, 1.0f,
    };

    /* Define an array of colours */
    // Personal modification: Top vertex from white (1 1 1) to red (1 0 0)
    float vertexColours[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
    };

    /* Generate a name for a vertex buffer object */
    glGenBuffers(1, &positionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &colourObject);
    glBindBuffer(GL_ARRAY_BUFFER, colourObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    try {
        program = glw->LoadShader("vert_attrib.vert", "vert_attrib.frag");
    } catch (exception &e) {
        cout << "Caught exception: " << e.what() << endl;
        cin.ignore();
        exit(0);
    }
}

// Called to update the display.
// You should call glfwSwapBuffers() after all of your rendering to display what you rendered.
void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glEnableVertexAttribArray(0);

    /* glVertexAttribPointer(index, size, type, normalised, stride, pointer)
       index relates to the layout qualifier in the vertex shader and in
       glEnableVertexAttribArray() and glDisableVertexAttribArray() */
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, colourObject);
    glEnableVertexAttribArray(1);

    /* glVertexAttribPointer(index, size, type, normalised, stride, pointer)
    index relates to the layout qualifier in the vertex shader and in
    glEnableVertexAttribArray() and glDisableVertexAttribArray() */
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

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
}

/* An error callback function to output GLFW errors */
static void error_callback(int error, const char *description) {
    fputs(description, stderr);
}

/* Entry point of program */
int main(int argc, char *argv[]) {
    GLWrapper *glw = new GLWrapper(1024, 768, "Hello Graphics World");;

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

    delete(glw);
    return 0;
}
