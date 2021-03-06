//Draw a OpenGL Triangle onto the screen with different offsets.
//Interpolate data between our Vertex and Fragment Shaders

#include "main.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    static const GLfloat squareVertices[] =
        {
            -0.25f,  0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,
             0.25f,  0.25f, -0.25f,

             0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
             0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,
            -0.25f,  0.25f,  0.25f,

            -0.25f, -0.25f,  0.25f,
             0.25f, -0.25f,  0.25f,
             0.25f, -0.25f, -0.25f,

             0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f, -0.25f,
            -0.25f, -0.25f,  0.25f,

            -0.25f,  0.25f, -0.25f,
             0.25f,  0.25f, -0.25f,
             0.25f,  0.25f,  0.25f,

             0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
            -0.25f,  0.25f, -0.25f
        };

    std::cout << "Spinning cube using the Chapter 4 Math and the Streaming of data from charpter 5\n";

    //Initialize the graphics portion of SDL
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL was unable to initialize, fail out\n";
        assert(false);
    }

    //Request the context be OpenGL 4.0 for our feature set
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //Lets create a OpenGL window
    SDL_Window *mainWindow = SDL_CreateWindow("Spinning Cube", 100, 100, 800, 600, SDL_WINDOW_OPENGL);

    //Check that the SDL/OpenGL window was created
    if(!mainWindow)
    {
        std::cout << "The SDL_CreateWindow method failed\n";
        assert(false);
    }

    //The OpenGL Context (instance of OpenGL) that we will use
    SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);
    SDL_GL_MakeCurrent(mainWindow, mainContext);

    if(!mainContext)
    {

        std::cout << SDL_GetError() << '\n';
        assert(false);
    }

    //Force GLEW to use experimental draw calls, but they are supported by the card
    glewExperimental = true;

    //Initialize GLEW in order to ensure we don't call GL methods the driver can't understand
    GLenum glewError = glewInit();

    if(GLEW_OK != glewError)
    {
        std::cout << "GLEW Error: " << glewGetErrorString(glewError) << '\n';
        assert(false);
    }

    //DO SOME OPENGL STUFF HERE

    //Declare and create a vertex array object
    //This will store the different attributes of the shaders and vertex
    GLuint savedVertexAttributes;

    //Generate the Vertex Array object  on the graphics card that we
    //will later be able to save for fast reference. This only allocates the memory
    //Next we need to bind it to be active.
    glGenVertexArrays(1, &savedVertexAttributes);

    //Bind that newly created space in order to be used
    glBindVertexArray(savedVertexAttributes);

    //Now lets build a vertex shader
    std::cout << "Shader Dir: " << QUOTE(SOURCEDIR/Source/Tutorial11/Shaders/Main.vs) << '\n';
    std::string vertexShaderSource = LoadFileToString(QUOTE(SOURCEDIR/Source/Tutorial11/Shaders/Main.glsl.vert));
    //Make a pointer to make glShaderSource happy (I REALLY hate this part of the solution)
    const char *sourceVertexShaderBegin = vertexShaderSource.c_str();
    //Create a instance of a vertex shader (Create a shader)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Load the shader source into our Vertex Shader instance.
    glShaderSource(vertexShader, 1, (const GLchar **) &sourceVertexShaderBegin, NULL);
    //Clear the system memory copy of the Vertex Shader Source
    vertexShaderSource.clear();
    //Now to compile the Vertex Shader
    glCompileShader(vertexShader);
    //Lets make a GLint to ensure that the shader compilation was successful
    GLint shaderCompilerStatus;
    //Get the status of the shader compiler.
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderCompilerStatus);
    if(shaderCompilerStatus != GL_TRUE)
    {
        std::cout << "Failed to compile the vertex shader\n";
        char openGLCompilerError[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, openGLCompilerError);
        std::cout << openGLCompilerError << '\n';
        assert(false);
    }

    //Now lets build a fragment shader
    std::string fragmentShaderSource = LoadFileToString(QUOTE(SOURCEDIR/Source/Tutorial11/Shaders/Main.glsl.frag));
    const char *sourceFragmentShaderBegin = fragmentShaderSource.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar **) &sourceFragmentShaderBegin, NULL);
    fragmentShaderSource.clear();
    glCompileShader(fragmentShader);
    //Get the status of the shader compiler.
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompilerStatus);
    if(shaderCompilerStatus != GL_TRUE)
    {
        std::cout << "Failed to compile the fragment shader\n";
        char openGLCompilerError[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, openGLCompilerError);
        std::cout << openGLCompilerError << '\n';
        assert(false);
    }

    //Now to use the shaders we just compiled, we need to create a shader program
    //(In a sense these are like different, pipelines that when linked can be switched out)
    GLuint shaderProgram = glCreateProgram();

    //Attach shaders to our newly created shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    //Link the Shader Program to create a executable shader pipeline
    //for the graphics card t ouse.
    glLinkProgram(shaderProgram);
    //Ensure that the program linked successfully
    GLint programLinkerStatus;
    //Get the status of the shader program linker.
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programLinkerStatus);
    if(programLinkerStatus != GL_TRUE)
    {
        std::cout << "Failed to linke shader program\n";
        char openGLLinkerError[1024];
        glGetProgramInfoLog(shaderProgram, 1024, NULL, openGLLinkerError);
        std::cout << openGLLinkerError << '\n';
        assert(false);
    }


    glUseProgram(shaderProgram);

    //Generate a buffer name to use
    GLuint generalBuffer;
    glGenBuffers(1, &generalBuffer);

    //Now bind the newly created buffer to the context using the GL_ARRAY_BUFFER
    //binding point
    glBindBuffer(GL_ARRAY_BUFFER, generalBuffer);

    //With the buffer bound to our OpenGL context, we can now create the size
    //of the buffer to be (allocation of size), it will be of data source
    //squareVertices, which will be GL_STATIC_DRAW (const) (The data is copied from system memory 
    //to graphics memory)
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    //This function has NOTHING to do with pointers (legacy)
    //Instead it is a pointer to a buffer object that the Vertex Shader
    //will fill up with the data we give it. (We initialize it to 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //Once we fill the glVertexAttrib buffer with the data we want
    //we will enable it, telling OpenGL to use it instead of glVertexAttrib*()
    glEnableVertexAttribArray(0);

    //Enable OpenGL culling (only render vertices/data that is viewable)
    glEnable(GL_CULL_FACE);
    //Set our culling to be clock wise.
    glFrontFace(GL_CW);

    //Enable the use of Depth testing to check if certain objects
    //are too far away on the screen
    glEnable(GL_DEPTH_TEST);

    //Check the deptch of vertices in on Z (Must be less then or equal to 1)
    glDepthFunc(GL_LEQUAL);

    //Copy the projectMatrix (in system memory) to the projectionMatrixLocation
    //(The buffer in graphics memory)
    //glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, projectionMatrix);

    //Get the SDL window width and height
    int windowWidth, windowheight;
    SDL_GetWindowSize(mainWindow, &windowWidth, &windowheight);

    //Get the projection matrix Uniform location from the Vertex Shader
    GLint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");

    if(projectionMatrixLocation == -1)
    {
        std::cout << "Could not find projectionMatrix Uniform definition!\n";
        assert(false);
    }

    //Now to calculate the aspect ratio of the window
    float windowAspectRatio = (float) windowWidth / (float) windowheight;

    //Create a projection matrix based on the aspect ratio
    glm::mat4 projectionMatrix = glm::perspective(float(M_PI / 3.60), windowAspectRatio, 0.1f, 1000.0f); //50.0f(Degrees) ~-> (M_PI / 3.60)(Radians)

    //Update the Vertex Shader Uniform with our projectionMatrix
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    //Get the model view matrix Uniform location from the Vertex Shader
    GLint modelViewMatrixLocation = glGetUniformLocation(shaderProgram, "modelViewMatrix");
    if(modelViewMatrixLocation == -1)
    {
        std::cout << "Could not find modelViewMatrix Uniform definition!\n";
        assert(false);
    }

    //Create a window event in order to know when the mainWindow "Close" is pressed
    SDL_Event *windowEvent = new SDL_Event;

    while(true)
    {
        if(SDL_PollEvent(windowEvent))
        {
            if(windowEvent->type == SDL_QUIT)
            {
                break;
            }
        }

        static const GLfloat greenColor[] = {0.0f, 0.25f, 0.0f, 1.0f};
        static const GLfloat one = 1.0f;

        //Set of the OpenGL viewPort size
        //This should probably be set to the same size as the window in most situations
        glViewport(0, 0, windowWidth, windowheight);

        //Clear the screen color
        glClearBufferfv(GL_COLOR, 0, greenColor);

        //Clear the depth buffer
        glClearBufferfv(GL_DEPTH, 0, &one);

        const GLfloat milisecondsPerSecond = 1000;
        GLfloat time = static_cast<GLfloat>(SDL_GetTicks()) / milisecondsPerSecond;

        GLfloat slowTime = time / 3.0f;
        glm::mat4 modelViewMatrix(1);
        modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0.0f, 0.0f, -4.0f)) *       //Puts the model in the center of the frustrum
                          glm::translate(modelViewMatrix, glm::vec3(sinf(2.1f * slowTime) * 0.5f, cosf(1.7f * slowTime) * 0.5f, (sinf(1.3f * slowTime) * cosf(1.5f * slowTime) * 2.0f))) * //Moves the cube up/down and back
                          glm::rotate(modelViewMatrix, float(time * (M_PI / 4)), glm::vec3(0.0f, 1.0f, 0.0f)) *   //45.0f(Degrees) -> (M_PI/4) (Radians) Rotates the cube
                          glm::rotate(modelViewMatrix, float(time * (M_PI / 2.22)), glm::vec3(1.0f, 0.0f, 0.0f)); //81.0f(Degrees) ~->(M_PI/2.22) (Radians); Rotates it on another axis

        glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));


        glDrawArrays(GL_TRIANGLES, 0, 36);
        SDL_GL_SwapWindow(mainWindow);
    }

    //Clean Up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glDeleteProgram(shaderProgram);

    //Clean up our data structures
    SDL_GL_DeleteContext(mainContext);
    return 0;
}
