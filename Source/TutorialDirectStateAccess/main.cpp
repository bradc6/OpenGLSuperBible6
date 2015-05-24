//An example of using direct state access.
#include "main.hpp"

#define USE_CHECKERBOARD_TEXTURE 1

int main(int argc, char* argv[])
{
    std::cout << "Spinning textured cube with texture coordiates\n"
              << "Switch between the textures with '1' and '2'\n";

    //Initialize the graphics portion of SDL
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL was unable to initialize, fail out\n";
        assert(false);
    }

    //Request the context be OpenGL 4.0 for our feature set
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    //Lets create a OpenGL window
    SDL_Window *mainWindow = SDL_CreateWindow("Spinning Textured Object Model", 100, 100, 800, 600, SDL_WINDOW_OPENGL);

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

    //Your driver/graphics card must support the direct state access extension
    assert(GLEW_EXT_direct_state_access);

    //Your driver/graphics card must support the debugging
    assert(GLEW_KHR_debug);

    //Enable the debugger extensions
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    //With the context set we will setup a OpenGL debug context callback
    glDebugMessageCallback((GLDEBUGPROCARB)gl_debug_callback, NULL);

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
    std::cout << "Shader Dir: " << QUOTE(SOURCEDIR/Source/Tutorial17/Shaders/Main.glsl.vert) << '\n';
    GLuint vertexShader = CompileGLShader(QUOTE(SOURCEDIR/Source/Tutorial17/Shaders/Main.glsl.vert), GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileGLShader(QUOTE(SOURCEDIR/Source/Tutorial17/Shaders/Main.glsl.frag),GL_FRAGMENT_SHADER);

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
        std::cout << "Failed to link shader program\n";
        char openGLLinkerError[1024];
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, openGLLinkerError);
        std::cout << openGLLinkerError << '\n';
        assert(false);
    }

    //Use the shader program that OpenGL compiled and linked.
    glUseProgram(shaderProgram);

    //***************************************
    //Load a texture to wrap around our model
    //***************************************

    //Get the sampler2D from the fragment shader
    GLuint main2DSampler = glNULL;

    //Get the location of the uniform
    main2DSampler = glGetUniformLocation(shaderProgram, "samplerTexture");
    if(main2DSampler == GL_INVALID_VALUE)
    {
            std::cout << "The value was not generated in the current shader program";
            assert(0);
    }

    //Set it to Zero?
    glUniform1i(main2DSampler, 0);

    //*********************************
    //Simple Checkboard texture pattern
    //*********************************
    GLuint checkBoardTexture = glNULL;
    //Generate the texture name
    glGenTextures(1, &checkBoardTexture);

    //Bind the texture to the context
    glBindTexture(GL_TEXTURE_2D, checkBoardTexture);

    //Allocate texture storage for the image
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 16, 16);

    //"Update"/Upload the data to the GPU
    glTexSubImage2D(GL_TEXTURE_2D, 0,0,0,16,16, GL_RGBA, GL_UNSIGNED_BYTE, checkerBoardTexture);

    glTextureParameteriEXT(checkBoardTexture, GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteriEXT(checkBoardTexture, GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);

    //Generate the mipmaps on the GPU since we will not load any.
    //(Generate the mipmap for the currently bound texture.
    glGenerateMipmap(GL_TEXTURE_2D);

    glTextureParameteriEXT(checkBoardTexture, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteriEXT(checkBoardTexture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //First we need to load a 2d texture into system
    //memory.
    SDL_Surface *sysMainTexture = nullptr;
    GLint loadedTextureFormat;

    //Load & decode the image into a new surface
    LoadImage(QUOTE(TEXTUREDIR/VeryLargeSampleTexture.png), sysMainTexture, loadedTextureFormat);

    GLuint loadedImageTexture = glNULL;

    //Generate a name for the texture.
    glGenTextures(1, &loadedImageTexture);

    //Bind the texture to the context
    glBindTexture(GL_TEXTURE_2D, loadedImageTexture);

    //Allocate the storage for the texture.
    // Specify the amount of storage we need for the texture
    glTexStorage2D(GL_TEXTURE_2D,                                      // 2D texture
                   8,                                                  // 8 mipmap levels
                   GL_RGBA32F,                                         // 32-bit floating-point RGBA data
                   sysMainTexture->w, sysMainTexture->h);    // imageWidth x imageHeight texels

    //Tell OpenGL the amount of storage that we need have for
    //the texture we are uploading.
    glTexSubImage2D(GL_TEXTURE_2D,                                              //The type of texture that we are uploading.
                    0,                                                          //The mipmapped nth level of the texture.
                    0,0,                                                        //The texel offset in the X & Y direction within the texture array.
                    sysMainTexture->w, sysMainTexture->h,             //The width & height of the target texture.
                    loadedTextureFormat,                                         //The format of the image (Red/Green/Blue/Alpha).
                    GL_UNSIGNED_BYTE,                                           //The data format of the pixel data.
                    sysMainTexture->pixels);                               //The system memory location of the texture data.

    //This is the old way to do the same as above (This is for OpenGL 4.1-)
    //https://www.opengl.org/wiki/Common_Mistakes#Creating_a_complete_texture
    /*

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 sysMainTexture->w, sysMainTexture->h,
                 0,
                 mainTextureFormat,
                 GL_UNSIGNED_BYTE,
                 sysMainTexture->pixels);*/

    //Free the surface as the texture is now in the GPU.
    SDL_free(sysMainTexture);
    sysMainTexture = NULL;

    //Set the mipmap levels
    glTextureParameteriEXT(loadedImageTexture, GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTextureParameteriEXT(loadedImageTexture, GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);

    //Generate the mipmaps on the GPU since we will not load any.
    //(Generate the mipmap for the currently bound texture.
    glGenerateMipmap(GL_TEXTURE_2D);

    glTextureParameteriEXT(loadedImageTexture, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteriEXT(loadedImageTexture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //**********************************************
    //Create a vertex array and upload it to the GPU
    //**********************************************

    //We will be using TinyObjLoader, as we don't really want to
    //parse the wavefront obj spec.

    //Load the shapes and materials (Even though we will not use the materials)
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string tinyObjError = tinyobj::LoadObj(shapes,
                                                materials,
                                                QUOTE(MODELDIR/TexturedCube.obj),
                                                QUOTE(MODELDIR));

    //Check if any errors occured while loading
    //the wavefront obj
    if(!tinyObjError.empty())
    {
        std::cout << tinyObjError.c_str() << '\n';
        assert(0);
    }

    //Ensure that there is at least one shape in
    //the object.
    if(shapes.size() < 1)
    {
        throw "No shapes were found";
    }


    //Generate a buffer name to use
    GLuint vertexObjectBuffer = glNULL;

    glGenBuffers(1, &vertexObjectBuffer);

    //Now bind the newly created buffer to the context using the GL_ARRAY_BUFFER
    //binding point
    glBindBuffer(GL_ARRAY_BUFFER, vertexObjectBuffer);

    //With the buffer bound to our OpenGL context, we can now create the size
    //of the buffer to be (allocation of size), it will be of data source
    //squareVertices, which will be GL_STATIC_DRAW (const) (The data is copied from system memory
    //to graphics memory)
    glBufferData(GL_ARRAY_BUFFER, (shapes[0].mesh.positions.size() * sizeof(GLfloat)), &shapes[0].mesh.positions[0], GL_STATIC_DRAW);

    //This function has NOTHING to do with pointers (legacy)
    //Instead it is a pointer to a buffer object that the Vertex Shader
    //will fill up with the data we give it. (We initialize it to 0)
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          nullptr);

    //Once we fill the glVertexAttrib buffer with the data we want
    //we will enable it, telling OpenGL to use it instead of glVertexAttrib*()
    GLint modelPositionAttibuteLocation = glNULL;
    modelPositionAttibuteLocation = glGetAttribLocation(shaderProgram, "position");
    if(GL_INVALID_OPERATION == modelPositionAttibuteLocation)
    {
        throw "Failed to get Position attribute from the vertex shader";
    }
    glEnableVertexAttribArray(modelPositionAttibuteLocation);

    //**********************************
    //Setup the Element order to draw in
    //**********************************
    GLuint elementBufferObject = glNULL;

    //Generate a name for the buffer
    glGenBuffers(1, &elementBufferObject);

    //Bind it to the context
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

    //Upload the data to the element buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (shapes[0].mesh.indices.size() * sizeof(GLuint)),
                  &shapes[0].mesh.indices[0],
                   GL_STATIC_DRAW);


    //******************************
    //Upload the texture coordinates
    //******************************
    //First we will get the attribute from the vertex shader
    GLuint textureCoordinatesBuffer = glNULL;

    glGenBuffers(1, &textureCoordinatesBuffer);

    //Now bind the newly created buffer to the context using the GL_ARRAY_BUFFER
    //binding point
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordinatesBuffer);

    //With the buffer bound to our OpenGL context, we can now create the size
    //of the buffer to be (allocation of size), it will be of data source
    //cube textureCoordinates, which will be GL_STATIC_DRAW (const) (The data is copied from system memory
    //to graphics memory)
    glBufferData(GL_ARRAY_BUFFER, (shapes[0].mesh.texcoords.size() * sizeof(GLfloat)), &shapes[0].mesh.texcoords[0], GL_STATIC_DRAW);


    //Once we fill the texturecoordinates buffer with the data we want
    //we will enable it, telling OpenGL to use it instead of glVertexAttrib*()
    GLint textureCoordinatesAttibuteLocation = glNULL;
    textureCoordinatesAttibuteLocation = glGetAttribLocation(shaderProgram, "textureCoordinate");
    if(GL_INVALID_OPERATION == textureCoordinatesAttibuteLocation)
    {
        throw "Failed to get TextureCoordinate attribute from the vertex shader";
    }

    //Setup the texture coordinates buffer, here we tell OpenGL how
    //the data is organized.
    glVertexAttribPointer(
                textureCoordinatesAttibuteLocation, //The attribute (location) in the vertex shader this buffer will be used
                2,                                  //The size of each element that is in the bound buffer
                GL_FLOAT,                           //The type of data that is in the bound buffer
                GL_FALSE,                           //Determines if OpenGL should normalize the data values.
                0,                                  //The stride ie. The offset of the bytes between consecutive vertex attributes.
                nullptr);                                 //The offset of the first attribute in the buffer store. (Offset of first object)

    //Enable the vertex attribute as we have added data to it to be used.
    glEnableVertexAttribArray(textureCoordinatesAttibuteLocation);

    //***************************
    //Setup the projection matrix
    //***************************
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
    glm::mat4 projectionMatrix = glm::perspective(float(M_PI / 3.60), windowAspectRatio, 0.1f, 1000.0f);

    //Update the Vertex Shader Uniform with our projectionMatrix
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    //Get the model view matrix Uniform location from the Vertex Shader
    GLint modelViewMatrixLocation = glGetUniformLocation(shaderProgram, "modelViewMatrix");
    if(modelViewMatrixLocation == -1)
    {
        std::cout << "Could not find modelViewMatrix Uniform definition!\n";
        assert(false);
    }

    //Enable the use of Depth testing to check if certain objects
    //are too far away on the screen
    glEnable(GL_DEPTH_TEST);

    //Check the depth of vertices in on Z (Must be less then or equal to 1)
    glDepthFunc(GL_LEQUAL);

    //Create a window event in order to know when the mainWindow "Close" is pressed
    SDL_Event *windowEvent = new SDL_Event;

    //Enable wireframe mode to debug vertices
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    bool runRender = true;
    while(runRender)
    {
        if(SDL_PollEvent(windowEvent))
        {
            switch(windowEvent->type) {
            case SDL_QUIT:
            {
                runRender = false;
                break;
            }
            case SDL_KEYDOWN:
            {
                if(windowEvent->key.keysym.sym == SDLK_ESCAPE) {
                    runRender = false;
                    break;
                }

                if(windowEvent->key.keysym.sym == SDLK_1) {
                    glBindTexture(GL_TEXTURE_2D, loadedImageTexture);
                }

                if(windowEvent->key.keysym.sym == SDLK_2) {
                    glBindTexture(GL_TEXTURE_2D, checkBoardTexture);
                }


                //Change the filter of the loaded image (kitten)
                if(windowEvent->key.keysym.sym == SDLK_q) {
                    GLint filterMode;
                    glGetTextureParameterivEXT(loadedImageTexture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &filterMode);
                    if(filterMode == GL_NEAREST)
                        glTextureParameteriEXT(loadedImageTexture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    else
                        glTextureParameteriEXT(loadedImageTexture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }

                //DSA Changing the texture
                //To demo this, view the the checkerboard pattern,(2) then switch back to
                //to kitten (1), then change the checkerboard's MAG filter(w) then return to
                //the checkerboard. Notice there is no binding occuring when getting/setting the parameters.
                //Changes the filter of the checkerboard texture
                if(windowEvent->key.keysym.sym == SDLK_w) {
                    GLint filterMode;
                    glGetTextureParameterivEXT(checkBoardTexture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &filterMode);
                    if(filterMode == GL_NEAREST)
                        glTextureParameteriEXT(checkBoardTexture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    else
                        glTextureParameteriEXT(checkBoardTexture, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }

                break;
            }
            default:
            {
                break;
            }
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

        GLfloat time = ((GLfloat)SDL_GetTicks() * 750) / (GLfloat)CLOCKS_PER_SEC;
        GLfloat slowTime = time / 3.0f;
        glm::mat4 modelViewMatrix = glm::translate(glm::vec3(0.0f, 0.0f, -4.0f)) *
                                    glm::translate(glm::vec3(sinf(2.1f * slowTime) * 0.5f,
                                                    cosf(1.7f * slowTime) * 0.5f,
                                                (sinf(1.3f * slowTime) * cosf(1.5f * slowTime) * 2.0f))) *
                                    glm::rotate(float(time * (M_PI/4)), glm::vec3(0.0f, 1.0f, 0.0f)) *
                                    glm::rotate(float(time * (M_PI/2.22)), glm::vec3(1.0f, 0.0f, 0.0f));

        glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

        glDrawElements(GL_TRIANGLES, shapes[0].mesh.indices.size(), GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(mainWindow);
    }

    //Clean Up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glDeleteBuffers(1, &vertexObjectBuffer);
    glDeleteBuffers(1, &elementBufferObject);
    glDeleteBuffers(1, &textureCoordinatesBuffer);

    glDeleteTextures(1, &loadedImageTexture);
    glDeleteTextures(1, &checkBoardTexture);

    glDeleteProgram(shaderProgram);

    //Clean up our data structures
    SDL_GL_DeleteContext(mainContext);
    return 0;
}
