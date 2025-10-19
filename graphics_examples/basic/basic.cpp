/* Basic GLFW3 Example and GLAD example
   with an OpenGL context for AC41001/AC51008, 
   Iain Martin 2022.
*/

/* Link to libraries, could define these as linker inputs in the project settings instead if you prefer */
#ifdef _DEBUG
#pragma comment(lib, "glfw3D.lib")
#else
#pragma comment(lib, "glfw3.lib")
#endif

// Link to basic OpenGL functionality (not enough on its own)
#pragma comment(lib, "opengl32.lib")

/* Include standard libraries */
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

/* Include GL_Load and GLFW headers */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/* Define some global objects that we'll use to render */
GLuint positionBufferObject;
GLuint secondPositionBufferObject; // Personal modification here, to display another triangle
GLuint program;
GLuint vao;
GLfloat x;
GLfloat y; // Personal modification here, to make another vertex move
GLfloat inc;

double startTime; // Personal modification - For advance feature: Animation speed control

/* Array of vertex positions */
GLfloat vertexPositions[] = {
    0.75f, 0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f,
};

// Personal modification: Second triangle
GLfloat secondVertexPositions[] = {
    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 1.0f,
};

/* Build shaders from strings containing shader source code */
GLuint BuildShader(GLenum eShaderType, const std::string &shaderText) {
    GLuint shader = glCreateShader(eShaderType);
    const char *strFileData = shaderText.c_str();
    glShaderSource(shader, 1, &strFileData, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        // Output the compile errors

        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[GLint(infoLogLength) + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

        const char *strShaderType = NULL;
        switch (eShaderType) {
            case GL_VERTEX_SHADER:
                strShaderType = "vertex";
                break;
            case GL_GEOMETRY_SHADER:
                strShaderType = "geometry";
                break;
            case GL_FRAGMENT_SHADER:
                strShaderType = "fragment";
                break;
        }

        std::cerr << "Compile error in " << strShaderType << "\n\t" << strInfoLog << std::endl;
        delete[] strInfoLog;

        // Personal modification: From std::exception to std::runtime_error
        throw std::runtime_error("Shader compile exception");
    }

    return shader;
}

/* Error callback, outputs error to stl error stream */
static void error_callback(int error, const char *description) {
    fputs(description, stderr);
}

/* Key response callback */
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    // Personal modification for feature
    if (key == GLFW_KEY_C) {
        y += (GLfloat) 10 * inc;
    }
}

/* Window reshape callback
   Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}


/* Our own initialisation function */
void init() {
    // Personal modification
    // === Create and bind a VAO (Vertex Array Object) ===
    // Not adding fails to display the green triangle, somehow
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    /* Define animation variables */
    x = 0;
    y = 0;
    inc = 0.001f;

    /* Create a vertex buffer object to store our array of vertices */
    /* A vertex buffer is a memory object that is created and owned by
       the OpenGL context */

    /* Generate buffer names (unique index identifiers) */
    glGenBuffers(1, &positionBufferObject);

    /* Specify the current active buffer object by identifier */
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);

    /* Allocates OpenGL memory for storing data or indices, any data
       previously defined is deleted */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_DYNAMIC_DRAW);

    // Personal modification BELOW - for the second triangle
    glGenBuffers(1, &secondPositionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, secondPositionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondVertexPositions), secondVertexPositions, GL_STATIC_DRAW);

    /* Stop using buffer object for target (GL_ARRAY_BUFFER) because buffer name = 0 */
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Define the vertex shader code as a string */
    // Personal modification: From 330 to 410 core
    const std::string vertexShader(
        "#version 410 core\n"
        "layout(location = 0) in vec4 position;\n"
        "uniform vec2 offset;\n" // Personal modification: offset, for advanced animation speed control feature
        "void main()\n"
        "{\n"
        "   gl_Position = position + vec4(offset, 0.0, 0.0);\n" // Personal modification: for advanced animation speed control feature: + vec4(offset, 0.0, 0.0)
        "   gl_PointSize = 10.0;\n" // Personal modification for drawing points
        "}\n"
    );

    /* Define the fragment shader as a string */
    // Personal modification: From 330 to 410 core
    const std::string fragmentShader(
        "#version 410 core\n"
        "out vec4 outputColor;\n"
        "uniform vec4 inColor;\n" // Personal modification: Dynamically pass in color attribute
        "void main()\n"
        "{\n"
        "   outputColor = inColor;\n" // Personal modification: From vec4(0.0f, 1.0f, 0.0f, 1.0f) to inColor
        "}\n"
    );

    /* Build both shaders */
    GLuint vertShader = BuildShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fragShader = BuildShader(GL_FRAGMENT_SHADER, fragmentShader);

    /* Create a shader program object and link the vertex and fragment shaders
    into a single shader program */
    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    /* Output and shader compilation errors */
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[GLint(infoLogLength) + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
        throw std::runtime_error("Shader could not be linked.");
    }

    // Personal modification BELOW
    // Enable transparency blending for overlays
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Personal modification BELOW
    // For advanced animation speed control feature
    startTime = glfwGetTime();
}


/* Rendering function */
void display() {
    vertexPositions[0] = x;
    vertexPositions[1] = y; // Personal modification here

    /* Update the vertex buffer object with the modified array of vertices */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_DYNAMIC_DRAW);

    /* Define the background colour*/
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Set the current shader program to be used */
    glUseProgram(program);

    // Personal modification for drawing wireframe triangle
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Personal modification BELOW
    // SET COLOR
    GLint colorLoc = glGetUniformLocation(program, "inColor");
    glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 0.5f); // Set color to blue, 0.5f transparency

    // === Drawing the SECOND (NEW) triangle ===

    // Personal modification BELOW
    // Draw the second triangle in blue
    glBindBuffer(GL_ARRAY_BUFFER, secondPositionBufferObject);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // === Drawing the ORIGINAL triangle ===

    /* Set the current active buffer object */
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);

    /* Specifies where the dat values associated with index can be accessed in the vertex shader */
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    /* Enable the vertex array associated with the index*/
    glEnableVertexAttribArray(0);

    // Set color back to green
    glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f); // Green

    /* Constructs a sequence of geometric primitives using the elements from the currently
       bound matrix */
    // Personal modification: GL_TRIANGLES for drawing triangles, GL_POINTS for drawing points
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Personal modification BELOW
    // For advanced feature: Animation speed control feature
    // Obtain time difference, unit: seconds
    double currentTime = glfwGetTime();
    double delta = currentTime - startTime;
    // Use time to control the animation (left-right panning)
    float xOffset = sin(delta) * 0.5f; // Oscillates back and forth within the interval [-0.5, 0.5]
    // Set uniform offset
    GLint offsetLoc = glGetUniformLocation(program, "offset");
    glUniform2f(offsetLoc, xOffset, 0.0f);

    /* Disable vertex array and shader program */
    glDisableVertexAttribArray(0);
    glUseProgram(0);
}

/* Standard main program */
int main(void) {
    GLFWwindow *window;

    /* Register the error callback first to enable any GLFW errors to be processed*/
    glfwSetErrorCallback(error_callback);

    /* Init GLFW */
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // Personal modification: BELOW

    // Set OpenGL version: 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Major version num
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // Minor version num

    // Use the "Core Profile"
    // This indicates enabling only modern OpenGL features, with no compatibility for legacy interfaces
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    // macOS specific requirement: Must set "Forward Compatible"
    // Otherwise, it will crash due to Core Profile being disabled by default
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a GLFW window, bail out if it doesn't work */
    window = glfwCreateWindow(640, 480, "Hello Graphics World", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* Associate an OpenGL context with the recently created GLFW window */
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD. Exiting." << std::endl;
        return -1;
    }

    /* Register callbacks for keyboard and window resize */
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, reshape);

    // Personal modification for drawing points
    glEnable(GL_PROGRAM_POINT_SIZE);

    /* Call our own function to perform any setup work*/
    init();

    /* The event loop */
    while (!glfwWindowShouldClose(window)) {
        /* Call our own drawing function */
        display();

        /* Swap buffers: GLFW is double buffered as standard */
        glfwSwapBuffers(window);

        /* Processes registered events, causes the callbacks to be called.*/
        glfwPollEvents();

        /* Modify our animation variables */
        x += inc;
        if (x >= 2 || x <= 0) inc = -inc;

        // y += (GLfloat) 1.5 * inc;
        if (y >= 2 || y <= 0) inc = -inc;
    }

    /* Clean up */
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
