//Example on the usage of generated mipmaps.

#include "main.hpp"
#include <iostream>

enum Textures {
    WALL_TEXTURE,
    FLOOR_TEXTURE,
    CEILING_TEXTURE,
    TEXTURE_SIZE
};

const std::string TextureFilePaths[TEXTURE_SIZE] = { QUOTE(TEXTUREDIR/wall.png),
                                                     QUOTE(TEXTUREDIR/floor.png),
                                                     QUOTE(TEXTUREDIR/ceiling.png) };

const std::string SampleDescription = "Tunnel: Mipmapped Texture Filtering";
int main(int argc, char* argv[])
{
    std::cout << SampleDescription << '\n';

    //Initialize the graphics portion of SDL
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL was unable to initialize, fail out\n";
        exit(-1);
    }

    //Request the context be OpenGL 4.0 for our feature set
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    //Lets create a OpenGL window
    SDL_Window *mainWindow = SDL_CreateWindow(SampleDescription.c_str(), 100, 100, 800, 600, SDL_WINDOW_OPENGL);

    //Check that the SDL/OpenGL window was created
    if(!mainWindow)
    {
        std::cout << "The SDL_CreateWindow method failed\n";
        exit(-1);
    }

    //The OpenGL Context (instance of OpenGL) that we will use
    SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);
    SDL_GL_MakeCurrent(mainWindow, mainContext);

    if(!mainContext)
    {

        std::cout << SDL_GetError() << '\n';
        exit(-1);
    }

    //Force GLEW to use experimental draw calls, but they are supported by the card
    glewExperimental = true;

    //Initialize GLEW in order to ensure we don't call GL methods the driver can't understand
    GLenum glewError = glewInit();

    if(GLEW_OK != glewError)
    {
        std::cout << "GLEW Error: " << glewGetErrorString(glewError) << '\n';
        exit(-1);
    }

    //With the context set we will setup a OpenGL debug context callback
    glDebugMessageCallbackARB((GLDEBUGPROCARB)gl_debug_callback, NULL);

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
    std::cout << "Shader Dir: " << QUOTE(SOURCEDIR/Source/Tutorial18/Shaders/Main.vs) << '\n';
    std::string vertexShaderSource = LoadFileToString(QUOTE(SOURCEDIR/Source/Tutorial18/Shaders/Main.vs.glsl));
    //Make a pointer to make glShaderSource happy (I REALLY hate this part of the solution)
    const char *sourceVertexShaderBegin = vertexShaderSource.c_str();
    //Create a instance of a vertex shader (Create a shader)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Load the shader source into our Vertex Shader instance.
    glShaderSource(vertexShader, 1, (const GLchar **) &sourceVertexShaderBegin, nullptr);
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
        glGetShaderInfoLog(vertexShader, 1024, nullptr, openGLCompilerError);
        std::cout << openGLCompilerError << '\n';
        exit(-1);
    }

    //Now lets build a fragment shader
    std::string fragmentShaderSource = LoadFileToString(QUOTE(SOURCEDIR/Source/Tutorial18/Shaders/Main.fs.glsl));
    const char *sourceFragmentShaderBegin = fragmentShaderSource.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar **) &sourceFragmentShaderBegin, nullptr);
    fragmentShaderSource.clear();
    glCompileShader(fragmentShader);
    //Get the status of the shader compiler.
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderCompilerStatus);
    if(shaderCompilerStatus != GL_TRUE)
    {
        std::cout << "Failed to compile the fragment shader\n";
        char openGLCompilerError[1024];
        glGetShaderInfoLog(fragmentShader, 1024, nullptr, openGLCompilerError);
        std::cout << openGLCompilerError << '\n';
        exit(-1);
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
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, openGLLinkerError);
        std::cout << openGLLinkerError << '\n';
        exit(-1);
    }

    //Use the shader program that OpenGL compiled and linked.
    glUseProgram(shaderProgram);

    //*********************************************
    //Load Textures for the ceiling,walls and floor
    //*********************************************
    SDL_Surface *sysTextures[TEXTURE_SIZE];
    GLint textureFormats[TEXTURE_SIZE];

    //With the textures in system memory we will upload them to the GPU
    GLuint textures[TEXTURE_SIZE];

    //Generate the names for the textures.
    glGenTextures(3, &textures[0]);

    //Bind and upload the data for the textures.
    for(unsigned int currentTexture = 0; currentTexture < TEXTURE_SIZE; currentTexture++)
    {
        //Set the newly inited SDL_Surface pointer to nullptr
        sysTextures[currentTexture] = nullptr;

        //Load & decode the images into a new surfaces
        LoadImage(TextureFilePaths[currentTexture], sysTextures[currentTexture], textureFormats[currentTexture]);

        //Bind the ceiling texture name and upload the data
        glBindTexture(GL_TEXTURE_2D, textures[currentTexture]);

        //Allocate the storage required for the texture to upload
        glTexStorage2D(GL_TEXTURE_2D,                                                     // 2D texture
                       8,                                                                 // 8 mipmap levels
                       GL_RGBA32F,                                                        // 32-bit floating-point RGBA data
                       sysTextures[currentTexture]->w, sysTextures[currentTexture]->h);   // imageWidth x imageHeight texels

        //Upload the texture data
        //Tell OpenGL the amount of storage that we need have for
        //the texture we are uploading.
        glTexSubImage2D(GL_TEXTURE_2D,                                                       //The type of texture that we are uploading.
                        0,                                                                   //The mipmapped nth level of the texture.
                        0,0,                                                                 //The texel offset in the X & Y direction within the texture array.
                        sysTextures[currentTexture]->w, sysTextures[currentTexture]->h,      //The width & height of the target texture.
                        textureFormats[currentTexture],                                      //The format of the image (Red/Green/Blue/Alpha).
                        GL_UNSIGNED_BYTE,                                                    //The data format of the pixel data.
                         sysTextures[currentTexture]->pixels);                               //The system memory location of the texture data.

        //Free the surface as the texture is now in the GPU.
        SDL_free(sysTextures[currentTexture]);
        sysTextures[currentTexture] = nullptr;

        //Set the mipmap levels
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 8);

        //Set the filtering mode when a texture in minified or magnified
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        //Generate the mipmaps on the GPU since we will not load any.
        //(Generate the mipmap for the currently bound texture.
        glGenerateMipmap(GL_TEXTURE_2D);

    }


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
        exit(-1);
    }

    //Now to calculate the aspect ratio of the window
    float windowAspectRatio = (float) windowWidth / (float) windowheight;

    //Create a projection matrix based on the aspect ratio
    glm::mat4 projectionMatrix = glm::perspective(50.0f, windowAspectRatio, 0.1f, 1000.0f);

    //Update the Vertex Shader Uniform with our projectionMatrix
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    //Get the model view matrix Uniform location from the Vertex Shader
    GLint modelViewMatrixLocation = glGetUniformLocation(shaderProgram, "modelViewMatrix");
    if(modelViewMatrixLocation == -1)
    {
        std::cout << "Could not find modelViewMatrix Uniform definition!\n";
        exit(-1);
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

    while(true)
      {
          if(SDL_PollEvent(windowEvent))
          {
              switch(windowEvent->type) {
              case SDL_QUIT:
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

          GLuint drawTextureOrder[] = { WALL_TEXTURE, FLOOR_TEXTURE, WALL_TEXTURE, CEILING_TEXTURE };
           for(unsigned int currentPlane = 0; currentPlane < 4; currentPlane++)
           {
               glm::mat4 modelViewMatrix = glm::rotate((90.0f * (float)currentPlane), glm::vec3(0.0f, 0.0f, 1.0f)) *
                                           glm::translate(glm::vec3(-0.5f, 0.0f, -10.0f)) *
                                           glm::rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
                                           glm::scale(glm::vec3(30.0f, 1.0f, 1.0f));

               //Update the modelViewMatrix
               glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

               //Bind the texture to draw with
               glBindTexture(GL_TEXTURE_2D, textures[drawTextureOrder[currentPlane]]);

               //Draw the plane to the screen
               glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
           }


          SDL_GL_SwapWindow(mainWindow);
      }

    return 0;
}
