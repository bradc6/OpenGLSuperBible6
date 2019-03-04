#include "main.hpp"
#include "Camera.hpp"

//Force the use of Radians only
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <memory>

#define glNULL 0
const std::string programDescription = "Simple Model Viewer";

float degreesToRadians(float degree)
{
    return degree * (M_1_PI / 180.0f);
}

int main()
{
    std::cout << programDescription << '\n';

    //Initialize the graphics portion of SDL
    assert(SDL_InitSubSystem(SDL_INIT_VIDEO) >= 0); //"SDL was unable to initialize"

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    //Lets create a OpenGL window
    SDL_Window *mainWindow = SDL_CreateWindow(programDescription.c_str(), 100, 100, 800, 600, SDL_WINDOW_OPENGL);

    //Check that the SDL/OpenGL window was created
    assert(mainWindow); //The SDL_CreateWindow method failed

    //The OpenGL Context (instance of OpenGL) that we will use
    SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);
    SDL_GL_MakeCurrent(mainWindow, mainContext);

    assert(mainContext); //Could not create OpenGL context

    //Force GLEW to use experimental draw calls, but they are supported by the card
    glewExperimental = true;

    //Initialize GLEW in order to ensure we don't call GL methods the driver can't understand
    GLenum glewError = glewInit();

    if(GLEW_OK != glewError)
    {
        std::cout << "GLEW Error: " << glewGetErrorString(glewError) << '\n';
        assert(false);
    }

    //Your graphics card must support seperate shader objects and debug callback
    assert(GLEW_ARB_separate_shader_objects);
    assert(GLEW_ARB_debug_output);

    //With the context set we will setup a OpenGL debug context callback
    glDebugMessageCallbackARB(gl_debug_callback, nullptr);

    //***************************************
    //DO SOME OPENGL STUFF HERE
    //***************************************
    SDL_SetWindowResizable(mainWindow, SDL_TRUE);


    //**********************************************
    //Create the Vertex and Fragment shader programs
    //**********************************************
    GLuint vertexShaderProgram = CreateShaderProgram(QUOTE(SOURCEDIR/Source/SimpleScene/Shaders/Main.glsl.vert), GL_VERTEX_SHADER);
    GLuint fragmentShaderProgram = CreateShaderProgram(QUOTE(SOURCEDIR/Source/SimpleScene/Shaders/Main.glsl.frag), GL_FRAGMENT_SHADER);

    //Create a OpenGL shader pipeline
    GLuint mainShaderPipeline = glNULL;
    glGenProgramPipelines(1, &mainShaderPipeline);

    //Lets apply the shader programs from before and use them in our pipeline
    glUseProgramStages(mainShaderPipeline, GL_VERTEX_SHADER_BIT, vertexShaderProgram);
    glUseProgramStages(mainShaderPipeline, GL_FRAGMENT_SHADER_BIT, fragmentShaderProgram);

    //Bind the pipeline for use in the context
    glBindProgramPipeline(mainShaderPipeline);

    //Ensure that the pipeline is good
    glValidateProgramPipeline(mainShaderPipeline);

    //******************************************
    //Load in a model to view
    //******************************************
    struct Model
    {
        GLuint glModelVertexBuffer = glNULL;
        GLuint glModelIndicies = glNULL;
        GLuint glVertexAttributes = glNULL;

        unsigned int NumberOfIndicies = 0;

        glm::mat4 modelViewMatrix = glm::mat4(1);

    };

    const std::string modelFilepaths[] =
    {
        QUOTE(MODELDIR/torus.obj),
        QUOTE(MODELDIR/monkey.obj)
    };

    std::vector<Model> models;

    for(unsigned int currentModel = 0; currentModel < sizeof(modelFilepaths) / sizeof(modelFilepaths[0]); currentModel++)
    {
        models.emplace_back();
        Model* targetModel = &models.back();

        //Load the model from disk -> system memory -> GPU memory -> dump system memory copy
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string tinyObjError = tinyobj::LoadObj(shapes, materials, modelFilepaths[currentModel].c_str(), QUOTE(MODELDIR));
        if(!tinyObjError.empty())
        {
            models.pop_back();
            continue;
        }

        targetModel->NumberOfIndicies = shapes[0].mesh.indices.size();

        glGenVertexArrays(1, &targetModel->glVertexAttributes);
        glBindVertexArray(targetModel->glVertexAttributes);

        glGenBuffers(1, &targetModel->glModelVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, targetModel->glModelVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, (shapes[0].mesh.positions.size() * sizeof(GLfloat)), &shapes[0].mesh.positions[0], GL_STATIC_DRAW);

        glGenBuffers(1, &targetModel->glModelIndicies);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, targetModel->glModelIndicies);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (shapes[0].mesh.indices.size() * sizeof(GLuint)), &shapes[0].mesh.indices[0], GL_STATIC_DRAW);

        //This function has NOTHING to do with pointers (legacy)
        //Instead it is a index to a buffer object that the Vertex Shader
        //will fill up with the data we give it. (We initialize it to 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        //Once we fill the glVertexAttrib buffer with the data we want
        //we will enable it, telling OpenGL to use it instead of glVertexAttrib*()
        glEnableVertexAttribArray(0);
    }


    //Create a window event in order to know when the mainWindow "Close" is pressed
    std::unique_ptr<SDL_Event> applicationEvent = std::make_unique<SDL_Event>();

    //****************************************************
    //Setup ProjectionMatrix Uniform
    //****************************************************
    GLint projectionMatrixLocation = glGetUniformLocation(vertexShaderProgram, "projectionMatrix");
    assert(projectionMatrixLocation != GL_INVALID_OPERATION);
    GLint cameraViewMatrixLocation = glGetUniformLocation(vertexShaderProgram, "cameraViewMatrix");
    assert(cameraViewMatrixLocation != GL_INVALID_OPERATION);
    GLint modelMatrixLocation = glGetUniformLocation(vertexShaderProgram, "modelMatrix");
    assert(modelMatrixLocation != GL_INVALID_OPERATION);

    //Create a projection matrix based on the aspect ratio
    glm::mat4 projectionMatrix;

    //State vars
    bool projectionMatrixChanged = true;

    //Enable wireframe mode, as we don't have sufficent shaders.
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    int windowWidth, windowHeight;
    float windowAspectRatio;
    SDL_GetWindowSize(mainWindow, &windowWidth, &windowHeight);
    windowAspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    Camera mainCamera;

    bool applicationRunning = true;
    while(applicationRunning)
    {
        const GLfloat milisecondsPerSecond = 1000;
        GLfloat deltaTime = 0.0f;
        GLfloat lastFrameTime = 0.0f;
        GLfloat currentFrameTimeInMiliseconds = static_cast<GLfloat>(SDL_GetTicks()) / milisecondsPerSecond;


        GLfloat slowTime = time / 3.0f;



        if(projectionMatrixChanged)
        {
            //Update the Vertex Shader Uniform with our projectionMatrix
            projectionMatrix = glm::perspective(float(M_PI / 3.60), windowAspectRatio, 0.1f, 1000.0f);
            glProgramUniformMatrix4fv(vertexShaderProgram, projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
            projectionMatrixChanged = false;
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);

        //We are traversing forward or back along the forward vector
        if(state[SDL_SCANCODE_W])
            mainCamera.m_Position += mainCamera.m_Speed * mainCamera.m_Front;

        if(state[SDL_SCANCODE_S])
            mainCamera.m_Position -= mainCamera.m_Speed * mainCamera.m_Front;

        //When we are panning (Left/Right sidestep) The cross product gives us the right vector
        //From there  we can traverse "left" and "right
        //We normalize the cross product in order to have a consistent move speed.
        //From learnopengl.com "If we would not normalize the vector we would either move slow or fast based on the camera's orientation instead of at a consistent movement speed"
        if(state[SDL_SCANCODE_A])
            mainCamera.m_Position -= glm::normalize(glm::cross(mainCamera.m_Front, mainCamera.m_Up)) * mainCamera.m_Speed;
        if(state[SDL_SCANCODE_D])
            mainCamera.m_Position += glm::normalize(glm::cross(mainCamera.m_Front, mainCamera.m_Up)) * mainCamera.m_Speed;

        if(SDL_PollEvent(applicationEvent.get()))
        {
            //User wants to close the application
            if(applicationEvent->type == SDL_QUIT)
            {
                applicationRunning = false;
                break;
            }

            //Handle Window event
            if(applicationEvent->type == SDL_WINDOWEVENT)
            {
                switch(applicationEvent->window.event)
                {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    {
                        SDL_GetWindowSize(mainWindow, &windowWidth, &windowHeight);
                        windowAspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
                        projectionMatrixChanged = true;
                        break;
                    }
                    default:
                        break;
                };
            }
        }

        //Clear the previous object
        const GLfloat blackColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        const GLfloat greenColor[] = {0.0f, 0.25f, 0.0f, 1.0f};
        const GLfloat one = 1.0f;

        //Clear the framebuffer and depthbuffer
        glClearBufferfv(GL_COLOR, 0, greenColor);
        glClearBufferfv(GL_DEPTH, 0, &one);

        glm::mat4 cameraMat = glm::lookAt(mainCamera.m_Position, mainCamera.m_Position + mainCamera.m_Front, mainCamera.m_Up);

        glProgramUniformMatrix4fv(vertexShaderProgram, cameraViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(cameraMat));


        for(unsigned int i = 0; i < models.size(); i++)
        {
            glBindVertexArray(models[i].glVertexAttributes);
            glBindBuffer(GL_ARRAY_BUFFER, models[i].glModelVertexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, models[i].glModelIndicies);

            glm::mat4 currentPosition = models[i].modelViewMatrix;
            //Old swivle code
            /*currentPosition = glm::translate(currentPosition, glm::vec3(0.0f, 0.0f, -4.0f)) *
                              glm::translate(currentPosition, glm::vec3(sinf(2.1f * slowTime) * 0.5f, cosf(1.7f * slowTime) * 0.5f, (sinf(1.3f * slowTime) * cosf(1.5f * slowTime) * 2.0f))) *
                              glm::rotate(currentPosition, float(time * (M_PI/4)), glm::vec3(0.0f, 1.0f, 0.0f)) *
                              glm::rotate(currentPosition, float(time * (M_PI/2.22)), glm::vec3(1.0f, 0.0f, 0.0f));*/

            //Independent swivle code
            /*
            if(i == 1)
                currentPosition =  glm::translate(currentPosition, glm::vec3(sinf(2.1f * slowTime) * 0.5f, cosf(1.7f * slowTime) * 0.5f, (sinf(1.3f * slowTime) * cosf(1.5f * slowTime) * 2.0f)));
            else
                currentPosition =glm::rotate(currentPosition, float(time * (M_PI/4)), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(currentPosition, float(time * (M_PI/2.22)), glm::vec3(1.0f, 0.0f, 0.0f));
            */


            glProgramUniformMatrix4fv(vertexShaderProgram, modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(currentPosition));
            glDrawElements(GL_TRIANGLES, models[i].NumberOfIndicies, GL_UNSIGNED_INT, 0);
        }


        SDL_GL_SwapWindow(mainWindow);
    }

    //Clean up our data structures
    SDL_GL_DeleteContext(mainContext);
}
