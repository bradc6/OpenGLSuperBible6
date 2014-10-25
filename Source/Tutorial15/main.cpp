#include "main.hpp"

int main()
{
    std::cout << "Sample code of using simple texture coordinates\n";

    //Initialize the graphics portion of SDL
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL was unable to initialize, fail out\n";
        exit(-1);
    }

    //Request the context be OpenGL 4.2 for our feature set
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //Lets create a OpenGL window
    SDL_Window *mainWindow = SDL_CreateWindow("Simple Texture Coordinates", 100, 100,
                                              WINDOW_RESOLUTION_WIDTH, WINDOW_RESOLUTION_HEIGHT,
                                              SDL_WINDOW_OPENGL);

    //Check that the SDL/OpenGL window was created
    if(!mainWindow)
    {
        std::cout << std::string("The SDL_CreateWindow method failed,\n") + SDL_GetError();
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

    //DO SOME OPENGL STUFF HERE
    //********************************************
    //Load+Compile+Link the Shaders
    //********************************************

    //Lets compile and upload our vertex shader
     GLuint vertexShader = CompileGLShader(QUOTE(SOURCEDIR/Source/Tutorial15/Shaders/Main.vs.glsl), GL_VERTEX_SHADER);

    //Compile and upload our fragment shader
    GLuint fragmentShader = CompileGLShader(QUOTE(SOURCEDIR/Source/Tutorial15/Shaders/Main.fs.glsl), GL_FRAGMENT_SHADER);

    //Create a shader program
    GLuint mainShaderProgram = glCreateProgram();

    //Attach the shaders to the program and link them.
    glAttachShader(mainShaderProgram, vertexShader);
    glAttachShader(mainShaderProgram, fragmentShader);
    glLinkProgram(mainShaderProgram);

    //Use the program we linked for the shader pipeline
    glUseProgram(mainShaderProgram);

    //********************************************
    //Create a 2D Texture and upload it to the GPU
    //********************************************
    GLuint sampleTexture = glNULL;

    //Generate a name for the texture to create.
    glGenTextures(1, &sampleTexture);

    //Bind the texture to the context, setting the texture to be 2D.
    glBindTexture(GL_TEXTURE_2D, sampleTexture);

    //First we will init a surface pointer and
    //a integer to have LoadImage get the texture data and
    //detect the texture image format. (RGB, RGBA, ect)
    SDL_Surface *sampleTextureSurface = NULL;
    GLint sampleTextureFormat = glNULL;
    LoadImage(QUOTE(TEXTUREDIR/MediumSampleTexture.png), sampleTextureSurface, sampleTextureFormat);

    //With the image loaded into system memory we will now upload it
    //to the GPU.
    // Specify the amount of storage we need for the texture
    glTexStorage2D(GL_TEXTURE_2D,                                      // 2D texture
                   8,                                                  // 8 mipmap levels
                   GL_RGBA32F,                                         // 32-bit floating-point RGBA data
                   sampleTextureSurface->w, sampleTextureSurface->h);                // imageWidth x imageHeight texels

    //Tell OpenGL the amount of storage that we need have for
    //the texture we are uploading.
    glTexSubImage2D(GL_TEXTURE_2D,                                    //The type of texture that we are uploading.
                    0,                                                //The mipmapped nth level of the texture.
                    0,0,                                              //The texel offset in the X & Y direction
                                                                      //within the texture array.
                    sampleTextureSurface->w, sampleTextureSurface->h, //The width & height of the target texture.
                    sampleTextureFormat,                              //The format of the image (Red/Green/Blue/Alpha).
                    GL_UNSIGNED_BYTE,                                 //The data format of the pixel data.
                    sampleTextureSurface->pixels);                    //The system memory location of the texture data.

    //Now that the texture has been uploaded to the GPU, we can
    //free our system memory copy
    SDL_free(sampleTextureSurface);
    sampleTextureSurface = NULL;

    //**********************************************
    //Create a vertex array and upload it to the GPU
    //**********************************************

    //We will be using TinyObjLoader, as we don't really want to
    //know the wavefront obj spec.

    //Load the shapes and materials (Even though we will not use the materials)
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string tinyObjError = tinyobj::LoadObj(shapes,
                                                materials,
                                                QUOTE(MODELDIR/Torus.obj),
                                                QUOTE(MODELDIR));

    //Check if any errors occured while loading
    //the wavefront obj
    if(!tinyObjError.empty())
    {
        throw tinyObjError.c_str();
    }

    //Ensure that there is at least one shape in
    //the object.
    if(shapes.size() < 1)
    {
        throw "No shapes were found";
    }

    //Generate a buffer name to use
    GLuint torusModel = glNULL;
    glGenBuffers(1, &torusModel);

    //Bind the new buffer to our current context
    glBindBuffer(GL_ARRAY_BUFFER, torusModel);

    //We are going to upload only the first shape
    //from the wavefront object
    glBufferData(GL_ARRAY_BUFFER,
                 shapes[0].mesh.indices.size(),
                 &shapes[0].mesh.indices[0],
                 GL_STATIC_DRAW);

    //This function has NOTHING to do with pointers (legacy)
    //Instead it is a pointer to a buffer object that the Vertex Shader
    //will fill up with the data we give it. (We initialize it to 0)
    glVertexAttribPointer(0,
                          shapes[0].mesh.indices.size(),
                         GL_UNSIGNED_INT, GL_FALSE, 0 ,NULL);

    //Once we fill the glVertexAttrib buffer with the data we want
    //we will enable it, telling OpenGL to use it instead of glVertexAttrib*()
    glEnableVertexAttribArray(0);


    //Enable the use of Depth testing to check if certain objects
    //are too far away on the screen
    glEnable(GL_DEPTH_TEST);

    //Check the deptch of vertices in on Z (Must be less then or equal to 1)
    glDepthFunc(GL_LEQUAL);


    //Get the SDL window width and height
    int windowWidth, windowheight;
    SDL_GetWindowSize(mainWindow, &windowWidth, &windowheight);

    //Get the projection matrix Uniform location from the Vertex Shader
    GLint projectionMatrixLocation = glGetUniformLocation(mainShaderProgram, "projectionMatrix");

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
    GLint modelViewMatrixLocation = glGetUniformLocation(mainShaderProgram, "modelViewMatrix");
    if(modelViewMatrixLocation == -1)
    {
        std::cout << "Could not find modelViewMatrix Uniform definition!\n";
        exit(-1);
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

           //glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
           glDrawArrays(GL_TRIANGLES, 0, shapes[0].mesh.indices.size());
           SDL_GL_SwapWindow(mainWindow);
       }

    return 0;
}
