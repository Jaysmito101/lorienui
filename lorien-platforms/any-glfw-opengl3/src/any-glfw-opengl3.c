#include "lorien/platforms/any-glfw-opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ---------- GLFW Callbacks/Utilities ----------

static void __lorPlatformUpdate(lor_PlatformAnyGLFWOpengl3Ptr pPlatform, bool poolEvents);

static lor_Result __lorPrepareGLFW(bool undecoratedWindow)
{
    if (!glfwInit())
    {
        LOR_ERROR("Failed to initialize GLFW.");
        return LOR_RESULT_OBJECT_CREATION_FAILED;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_DECORATED, !undecoratedWindow);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    return LOR_RESULT_SUCCESS;
}

void __lorGLFWErrorCallback(int error, const char *description)
{
    LOR_ERROR_VERBOSE("GLFW Error: %d - %s\n", error, description);
}

void __lorGLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle key events
}

void __lorGLFWCharCallback(GLFWwindow *window, unsigned int codepoint)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle char events
}

void __lorGLFWDropCallback(GLFWwindow *window, int count, const char **paths)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle drop events
}

void __lorGLFWScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle scroll events
}

void __lorGLFWCursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle cursor position events
}

void __lorGLFWWindowPosCallback(GLFWwindow *window, int xpos, int ypos)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle window position events
}

void __lorGLFWWindowSizeCallback(GLFWwindow *window, int width, int height)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle window size events
}

void __lorGLFWCursorEnterCallback(GLFWwindow *window, int entered)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle cursor enter events
}

void __lorGLFWWindowCloseCallback(GLFWwindow *window)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    pInputState->sShouldWindowClose = true;
}

void __lorGLFWMouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle mouse button events
}

void __lorGLFWFramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    pPlatform->sFramebufferSize.sWidth = width;
    pPlatform->sFramebufferSize.sHeight = height;
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    pInputState->sFramebufferSize.sWidth = (float)width;
    pInputState->sFramebufferSize.sHeight = (float)height;

    // This is needed as GLFW pauses update while resizing
    __lorPlatformUpdate(pPlatform, true); // update the platform state
}

void __lorGLFWWindowMaximizeCallback(GLFWwindow *window, int maximized)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle window maximize events
}

void __lorGLFWWindowFocusCallback(GLFWwindow *window, int focused)
{
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle window focus events
}

// ---------- GLFW Callbacks/Utilities End ----------

// ---------- OpenGL Utilities ----------

// add shader sources
static const char *__lorVertexShaderSrc = "#version 330 core\n"
                                          "layout(location = 0) in vec3 aPosition;\n"
                                          "layout(location = 2) in vec4 aColor;\n"
                                          "uniform vec2 uViewportSize;\n"
                                          "out vec4 vColor;\n"
                                          "void main() {\n"
                                          "    gl_Position = vec4(aPosition.xy / uViewportSize.xy * 2.0 - 1.0, aPosition.z, 1.0);\n"
                                          "    vColor = aColor;\n"
                                          "}\n";

static const char *__lorFragmentShaderSrc = "#version 330 core\n"
                                            "in vec4 vColor;\n"
                                            "uniform vec2 uRectSize;\n"
                                            "out vec4 FragColor;\n"
                                            "void main() {\n"
                                            "    if (gl_FragCoord.x < 0.0 || gl_FragCoord.x > uRectSize.x || gl_FragCoord.y < 0.0 || gl_FragCoord.y > uRectSize.y) {\n"
                                            "        discard;\n"
                                            "    }\n"
                                            "    FragColor = vColor;\n"
                                            "}\n";

static void __lorGLMaintainBoundState(bool save)
{
    static GLint sVertexArrayObject = 0;
    static GLint sVertexBuffer = 0;
    static GLint sIndexBuffer = 0;
    static GLint sShaderProgram = 0;
    static GLint sFramebuffer = 0;
    static GLint sTexture0 = 0;
    if (save)
    {
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &sVertexArrayObject);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &sVertexBuffer);
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &sIndexBuffer);
        glGetIntegerv(GL_CURRENT_PROGRAM, &sShaderProgram);
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &sFramebuffer);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &sTexture0);
    }
    else
    {
        glBindVertexArray(sVertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, sVertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sIndexBuffer);
        glUseProgram(sShaderProgram);
        glBindFramebuffer(GL_FRAMEBUFFER, sFramebuffer);
        glBindTexture(GL_TEXTURE_2D, sTexture0);
    }
}

static uint32_t __lorGLGetUniformLocation(uint32_t shaderProgram, const char *name)
{
    LOR_ASSERT_MSG(shaderProgram != 0, "Invalid shader program provided.");
    LOR_ASSERT_MSG(name != NULL, "Invalid uniform name provided.");

    int32_t location = glGetUniformLocation(shaderProgram, name);
    if (location == -1)
    {
        LOR_ERROR_VERBOSE("Failed to get uniform location for %s\n", name);
    }
    return (uint32_t)location;
}

static void __lorFlushBatchRenderer(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatch)
{
}

static void __lorGLCreateMeshBuffers(uint32_t *pVertexArrayObject, uint32_t *pVertexBuffer, uint32_t *pIndexBuffer)
{
    LOR_ASSERT_MSG(pVertexArrayObject != NULL, "Invalid vertex array object pointer provided.");
    LOR_ASSERT_MSG(pVertexBuffer != NULL, "Invalid vertex buffer pointer provided.");
    LOR_ASSERT_MSG(pIndexBuffer != NULL, "Invalid index buffer pointer provided.");

    __lorGLMaintainBoundState(true); // save the current state

    glGenVertexArrays(1, pVertexArrayObject);
    glBindVertexArray(*pVertexArrayObject);

    glGenBuffers(1, pVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, *pVertexBuffer);

    glGenBuffers(1, pIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *pIndexBuffer);

    __lorGLMaintainBoundState(false); // restore the previous state
}

static void __lorGLAllocateMeshBuffers(uint32_t vertexBuffer, uint32_t indexBuffer, size_t vertexSize, size_t indexSize)
{
    LOR_ASSERT_MSG(vertexBuffer != 0, "Invalid vertex buffer provided.");
    LOR_ASSERT_MSG(indexBuffer != 0, "Invalid index buffer provided.");
    LOR_ASSERT_MSG(vertexSize > 0, "Invalid vertex size provided.");
    LOR_ASSERT_MSG(indexSize > 0, "Invalid index size provided.");

    __lorGLMaintainBoundState(true); // save the current state

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexSize * sizeof(lor_Vertex), NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(uint32_t), NULL, GL_DYNAMIC_DRAW);

    __lorGLMaintainBoundState(false); // restore the previous state
}

static void __lorGLDeleteMeshBuffers(uint32_t vertexArrayObject, uint32_t vertexBuffer, uint32_t indexBuffer)
{
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);
}

static void __lorGLSetupMeshBufferAttribArray(uint32_t vertexArrayObject, uint32_t vertexBuffer, uint32_t indexBuffer)
{
    __lorGLMaintainBoundState(true); // save the current state

    glBindVertexArray(vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    // Set up vertex attributes (position, color, texture coordinates, etc.)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(lor_Vertex), (void *)offsetof(lor_Vertex, sX));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(lor_Vertex), (void *)offsetof(lor_Vertex, sU));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(lor_Vertex), (void *)offsetof(lor_Vertex, sR));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_INT, GL_FALSE, sizeof(lor_Vertex), (void *)offsetof(lor_Vertex, sFlag0));
    glEnableVertexAttribArray(3);

    __lorGLMaintainBoundState(false); // restore the previous state
}

static void __lorGLCreateShaderProgram(uint32_t *pShaderProgram)
{
    LOR_ASSERT_MSG(pShaderProgram != NULL, "Invalid shader program pointer provided.");

    __lorGLMaintainBoundState(true); // save the current state

    uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &__lorVertexShaderSrc, NULL);
    glCompileShader(vs);

    uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &__lorFragmentShaderSrc, NULL);
    glCompileShader(fs);

    uint32_t prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);

    glDeleteShader(vs);
    glDeleteShader(fs);

    *pShaderProgram = prog;

    __lorGLMaintainBoundState(false); // restore the previous state
}

// ---------- OpenGL Utilities End ----------

// ---------- Batch Renderer Functions ----------

static void __lorBatchRendererUploadBuffers(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer)
{
    glBindBuffer(GL_ARRAY_BUFFER, pBatchRenderer->sVertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, pBatchRenderer->sVertexCount * sizeof(lor_Vertex), pBatchRenderer->pVertices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pBatchRenderer->sIndexBuffer);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, pBatchRenderer->sIndexCount * sizeof(uint32_t), pBatchRenderer->pIndices);
}

static void __lorBatchRendererUpdateUniforms(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer)
{
    glUseProgram(pBatchRenderer->sShaderProgram);
    lor_Size viewportSize = lorRectGetSize(&pBatchRenderer->viewportRect);
    lor_Size rectSize = lorRectGetSize(&pBatchRenderer->sCurrentRect);
    glUniform2f(__lorGLGetUniformLocation(pBatchRenderer->sShaderProgram, "uViewportSize"), viewportSize.sWidth, viewportSize.sHeight);
    glUniform2f(__lorGLGetUniformLocation(pBatchRenderer->sShaderProgram, "uRectSize"), rectSize.sWidth, rectSize.sHeight);
}

static void __lorBatchRendererRenderMesh(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer)
{
    glBindVertexArray(pBatchRenderer->sVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, pBatchRenderer->sVertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pBatchRenderer->sIndexBuffer);
    glDrawElements(GL_TRIANGLES, (GLsizei)pBatchRenderer->sIndexCount, GL_UNSIGNED_INT, 0);
}

static bool __lorIsBufferSizesEnough(size_t requiredVertexSize, size_t requiredIndexSize, lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer)
{
    LOR_ASSERT_MSG(pBatchRenderer != NULL, "Invalid batch renderer provided.");
    return (requiredVertexSize + pBatchRenderer->sVertexCount <= pBatchRenderer->sVertexCapacity) &&
           (requiredIndexSize + pBatchRenderer->sIndexCount <= pBatchRenderer->sIndexCapacity);
}



static void __lorBatchRendererAddLayoutDebuggingPrim(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer, const lor_RenderablePrimitivePtr pPrimitive)
{
    float minX = pPrimitive->sRect.sMinX;
    float minY = pPrimitive->sRect.sMinY;
    float maxX = pPrimitive->sRect.sMaxX;
    float maxY = pPrimitive->sRect.sMaxY;

    // color from id (temporary )
    float r = (pPrimitive->sId % 255) / 255.0f;
    float g = ((pPrimitive->sId * 37) % 255) / 255.0f;
    float b = ((pPrimitive->sId * 91) % 255) / 255.0f;
    float a = 1.0f;
    lor_Vertex verts[4] = {
        {minX, minY, 0.0f, 0, 0, r, g, b, a, 0, 0, 0},
        {maxX, minY, 0.0f, 0, 0, r, g, b, a, 0, 0, 0},
        {maxX, maxY, 0.0f, 0, 0, r, g, b, a, 0, 0, 0},
        {minX, maxY, 0.0f, 0, 0, r, g, b, a, 0, 0, 0}};
    uint32_t idx[6] = {0, 1, 2, 2, 3, 0};
    lorPlatformAnyGLFWOpengl3BatchRendererAddVertices(pBatchRenderer, verts, sizeof(verts) / sizeof(lor_Vertex), idx,
                                                      sizeof(idx) / sizeof(uint32_t));
}

static void __lorBatchRendererAddMeshPrim(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer, const lor_RenderablePrimitivePtr pPrimitive)
{
    LOR_ASSERT_MSG(pPrimitive->sData.sType == LOR_PRIM_TYPE_MESH, "Invalid primitive type provided.");
    LOR_ASSERT_MSG(pPrimitive->sData.sMesh.pVertices != NULL, "Invalid mesh vertices provided.");
    LOR_ASSERT_MSG(pPrimitive->sData.sMesh.pIndices != NULL, "Invalid mesh indices provided.");

    LOR_ASSERT_MSG(pPrimitive->sData.sMesh.sVertexCount > 0, "Invalid mesh vertex count provided.");
    LOR_ASSERT_MSG(pPrimitive->sData.sMesh.sIndexCount > 0, "Invalid mesh index count provided.");

    lorPlatformAnyGLFWOpengl3BatchRendererAddVertices(pBatchRenderer, pPrimitive->sData.sMesh.pVertices,
                                                      pPrimitive->sData.sMesh.sVertexCount, pPrimitive->sData.sMesh.pIndices,
                                                      pPrimitive->sData.sMesh.sIndexCount);
}

static void __lorBatchRendererAddRectPrim(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer, const lor_RenderablePrimitivePtr pPrimitive)
{
    LOR_ASSERT_MSG(pPrimitive->sData.sType == LOR_PRIM_TYPE_RECT, "Invalid primitive type provided.");

    float minX = pPrimitive->sRect.sMinX;
    float minY = pPrimitive->sRect.sMinY;
    float maxX = pPrimitive->sRect.sMaxX;
    float maxY = pPrimitive->sRect.sMaxY;

    // color from id (temporary )
    float r = lorColorGetChannelF32(pPrimitive->sColor, 0);
    float g = lorColorGetChannelF32(pPrimitive->sColor, 1);
    float b = lorColorGetChannelF32(pPrimitive->sColor, 2);
    float a = lorColorGetChannelF32(pPrimitive->sColor, 3);
    lor_Vertex verts[4] = {
        {minX, minY, 0.0f, 0, 0, r, g, b, a, 0, 0, 0},
        {maxX, minY, 0.0f, 0, 0, r, g, b, a, 0, 0, 0},
        {maxX, maxY, 0.0f, 0, 0, r, g, b, a, 0, 0, 0},
        {minX, maxY, 0.0f, 0, 0, r, g, b, a, 0, 0, 0}};
    uint32_t idx[6] = {0, 1, 2, 2, 3, 0};
    lorPlatformAnyGLFWOpengl3BatchRendererAddVertices(pBatchRenderer, verts, sizeof(verts) / sizeof(lor_Vertex), idx,
                                                      sizeof(idx) / sizeof(uint32_t));
}


lor_Result lorPlatformAnyGLFWOpengl3BatchRendererBuild(lor_AllocatorPtr pAllocator, size_t vertexCapacity, size_t indexCapacity, lor_PlatformAnyGLFWOpengl3BatchRendererPtr *ppBatchRenderer)
{
    LOR_ASSERT_MSG(pAllocator != NULL, "Invalid allocator provided.");
    LOR_ASSERT_MSG(ppBatchRenderer != NULL, "Invalid batch renderer pointer provided.");

    lor_Result result = LOR_RESULT_UNKNOWN_ERROR;
    lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer = NULL;
    if ((result = lorAllocatorAllocate(pAllocator, sizeof(lor_PlatformAnyGLFWOpengl3BatchRenderer), LOR_ALLOCATION_TYPE_GENERAL, (void **)&pBatchRenderer)) != LOR_RESULT_SUCCESS)
    {
        LOR_ERROR("Failed to allocate batch renderer.");
        return result;
    }

    memset(pBatchRenderer, 0, sizeof(lor_PlatformAnyGLFWOpengl3BatchRenderer)); // Initialize to zero
    if ((result = lorAllocatorAllocate(pAllocator, sizeof(lor_Vertex) * vertexCapacity, LOR_ALLOCATION_PLATFORM_VERTICES, (void **)&pBatchRenderer->pVertices)) != LOR_RESULT_SUCCESS)
    {
        return result;
    }
    if ((result = lorAllocatorAllocate(pAllocator, sizeof(uint32_t) * indexCapacity, LOR_ALLOCATION_PLATFORM_VERTICES, (void **)&pBatchRenderer->pIndices)) != LOR_RESULT_SUCCESS)
    {
        return result;
    }
    pBatchRenderer->sVertexCount = 0;
    pBatchRenderer->sVertexCapacity = vertexCapacity;
    pBatchRenderer->sIndexCount = 0;
    pBatchRenderer->sIndexCapacity = indexCapacity;
    pBatchRenderer->sCurrentRect = lorRectPosSize(0.0f, 0.0f, 1.0f, 1.0f);
    pBatchRenderer->viewportRect = lorRectPosSize(0.0f, 0.0f, 1.0f, 1.0f);

    pBatchRenderer->sLayoutDebuggingEnabled = false; // TODO: Make this configurable

    __lorGLCreateMeshBuffers(&pBatchRenderer->sVertexArrayObject, &pBatchRenderer->sVertexBuffer, &pBatchRenderer->sIndexBuffer);
    __lorGLAllocateMeshBuffers(pBatchRenderer->sVertexBuffer, pBatchRenderer->sIndexBuffer, vertexCapacity, indexCapacity);
    __lorGLSetupMeshBufferAttribArray(pBatchRenderer->sVertexArrayObject, pBatchRenderer->sVertexBuffer, pBatchRenderer->sIndexBuffer);
    __lorGLCreateShaderProgram(&pBatchRenderer->sShaderProgram);

    *ppBatchRenderer = pBatchRenderer;

    return LOR_RESULT_SUCCESS;
}

void lorPlatformAnyGLFWOpengl3BatchRendererDestroy(lor_AllocatorPtr pAllocator, lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer)
{
    LOR_ASSERT_MSG(pAllocator != NULL, "Invalid allocator provided.");
    LOR_ASSERT_MSG(pBatchRenderer != NULL, "Invalid batch renderer provided.");

    __lorGLDeleteMeshBuffers(pBatchRenderer->sVertexArrayObject, pBatchRenderer->sVertexBuffer, pBatchRenderer->sIndexBuffer);
    lorAllocatorFastFree(pAllocator, LOR_ALLOCATION_PLATFORM_VERTICES, pBatchRenderer->pVertices);
    lorAllocatorFastFree(pAllocator, LOR_ALLOCATION_PLATFORM_VERTICES, pBatchRenderer->pIndices);
    lorAllocatorFastFree(pAllocator, LOR_ALLOCATION_TYPE_GENERAL, pBatchRenderer);
}

void lorPlatformAnyGLFWOpengl3BatchRendererAddVertices(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer, const lor_VertexPtr pVertices, size_t vertexCount, uint32_t *indices, size_t indicexCount)
{
    LOR_ASSERT_MSG(pBatchRenderer != NULL, "Invalid batch renderer provided.");
    LOR_ASSERT_MSG(pVertices != NULL, "Invalid vertices pointer provided.");
    LOR_ASSERT_MSG(indices != NULL, "Invalid indices pointer provided.");

    LOR_ASSERT_MSG(vertexCount > 0, "Invalid vertex count provided.");
    LOR_ASSERT_MSG(indicexCount > 0, "Invalid index count provided.");

    if (!__lorIsBufferSizesEnough(vertexCount, indicexCount, pBatchRenderer))
    {
        lorPlatformAnyGLFWOpengl3BatchRendererFlush(pBatchRenderer);
    }

    LOR_ASSERT_MSG(vertexCount + pBatchRenderer->sVertexCount <= pBatchRenderer->sVertexCapacity, "Vertex count exceeds capacity.");
    LOR_ASSERT_MSG(indicexCount + pBatchRenderer->sIndexCount <= pBatchRenderer->sIndexCapacity, "Index count exceeds capacity.");

    // copy vertices and indices to the batch renderer
    memcpy(&pBatchRenderer->pVertices[pBatchRenderer->sVertexCount], pVertices, sizeof(lor_Vertex) * vertexCount);
    memcpy(&pBatchRenderer->pIndices[pBatchRenderer->sIndexCount], indices, sizeof(uint32_t) * indicexCount);

    // offset the indices
    size_t indexOffset = pBatchRenderer->sVertexCount;
    for (size_t i = pBatchRenderer->sIndexCount; i < pBatchRenderer->sIndexCount + indicexCount; ++i)
    {
        pBatchRenderer->pIndices[i] += (uint32_t)indexOffset;
    }

    pBatchRenderer->sVertexCount += vertexCount;
    pBatchRenderer->sIndexCount += indicexCount;
}

void lorPlatformAnyGLFWOpengl3BatchRendererAddPrim(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer, const lor_RenderablePrimitivePtr pPrimitive)
{
    LOR_ASSERT_MSG(pBatchRenderer != NULL, "Invalid batch renderer provided.");
    LOR_ASSERT_MSG(pPrimitive != NULL, "Invalid primitive provided.");

    lorRectUnionInplace(&pBatchRenderer->sCurrentRect, &pPrimitive->sRect);

    if (pBatchRenderer->sLayoutDebuggingEnabled)
    {
        __lorBatchRendererAddLayoutDebuggingPrim(pBatchRenderer, pPrimitive);
        return;
    }

    lor_PrimTypes primType = pPrimitive->sData.sType;
    switch (primType)
    {
    case LOR_PRIM_TYPE_RECT:
        __lorBatchRendererAddRectPrim(pBatchRenderer, pPrimitive);
        break;
    case LOR_PRIM_TYPE_MESH:
        __lorBatchRendererAddMeshPrim(pBatchRenderer, pPrimitive);
        break;
    case LOR_PRIM_TYPE_VOID:
        // No action needed for void type
        break;
    default:
        LOR_ERROR("Invalid primitive type provided.");
        return;
    }
}

void lorPlatformAnyGLFWOpengl3BatchRendererFlush(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer)
{
    LOR_ASSERT_MSG(pBatchRenderer != NULL, "Invalid batch renderer provided.");

    if (pBatchRenderer->sIndexCount == 0)
        return;

    __lorGLMaintainBoundState(true);
    __lorBatchRendererUploadBuffers(pBatchRenderer);
    __lorBatchRendererUpdateUniforms(pBatchRenderer);
    __lorBatchRendererRenderMesh(pBatchRenderer);
    __lorGLMaintainBoundState(false);

    pBatchRenderer->sCurrentRect = lorRectPosSize(0.0f, 0.0f, 1.0f, 1.0f);
    pBatchRenderer->sVertexCount = 0;
    pBatchRenderer->sIndexCount = 0;
}

void lorPlatformAnyGLFWOpengl3BatchRendererSetViewport(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer, float x, float y, float width, float height)
{
    LOR_ASSERT_MSG(pBatchRenderer != NULL, "Invalid batch renderer provided.");
    lorRectFillPosSize(&pBatchRenderer->viewportRect, x, y, width, height);
}

// ---------- Batch Renderer Functions End ----------

static void __lorRenderPrimitiveRecursive(lor_RenderablePrimitivePtr pNode, lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatch, lor_ApplicationPtr pApplication)
{
    LOR_ASSERT_MSG(pNode != NULL, "Invalid renderable primitive provided.");
    LOR_ASSERT_MSG(pBatch != NULL, "Invalid batch renderer provided.");
    LOR_ASSERT_MSG(pApplication != NULL, "Invalid application provided.");

    lorPlatformAnyGLFWOpengl3BatchRendererAddPrim(pBatch, pNode);

    // recurse children
    for (size_t i = 0; i < pNode->sChildrenCount; ++i)
    {
        __lorRenderPrimitiveRecursive(pNode->pChildren[i], pBatch, pApplication);
    }
}

// do the rendering
static void __lorRenderFrame(lor_PlatformAnyGLFWOpengl3Ptr pPlatform)
{
    LOR_ASSERT_MSG(pPlatform != NULL, "Invalid platform provided.");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, pPlatform->sFramebufferSize.sWidth, pPlatform->sFramebufferSize.sHeight);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lor_RenderablePrimitivePtr pRootRenderablePrimitive = pPlatform->pApplication->pRootRenderablePrimitive;

    // render the scene
    if (pRootRenderablePrimitive != NULL)
    {
        __lorRenderPrimitiveRecursive(pRootRenderablePrimitive, pPlatform->pBatchRenderer, pPlatform->pApplication);
    }
    lorPlatformAnyGLFWOpengl3BatchRendererFlush(pPlatform->pBatchRenderer);
}

// ---------- Rendering Functions End  ----------

// ---------- API Functions  ----------

lor_Result lorPlatformAnyGLFWOpengl3Build(lor_PlatformConfigPtr pPlatformConfig, lor_PlatformAnyGLFWOpengl3Ptr *ppPlatform)
{
    LOR_ASSERT_MSG(pPlatformConfig != NULL, "Invalid platform config provided.");
    LOR_ASSERT_MSG(ppPlatform != NULL, "Invalid platform pointer provided.");

    lor_Result result = LOR_RESULT_UNKNOWN_ERROR;
    if (pPlatformConfig->sType != LOR_STRUCT_PLATFORM_CONFIG)
    {
        LOR_ERROR("Invalid struct type provided.");
        return LOR_RESULT_INVALID_ARGUMENT;
    }

    lor_Allocator allocator = {0};
    if (pPlatformConfig->pAllocator == NULL)
    {
        if ((result = lorGetDefaultAllocator(&allocator)) != LOR_RESULT_SUCCESS)
        {
            LOR_ERROR("Failed to get default allocator.");
            return result;
        }
    }
    else
    {
        allocator = *pPlatformConfig->pAllocator; // copy the allocator
    }

    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = NULL;
    if ((result = lorAllocatorAllocate(&allocator, sizeof(lor_PlatformAnyGLFWOpengl3), LOR_ALLOCATION_TYPE_GENERAL, (void **)&pPlatform)) != LOR_RESULT_SUCCESS)
    {
        LOR_ERROR("Failed to allocate platform.");
        return result;
    }

    if (pPlatformConfig->fPreloadPlatform != NULL)
    {
        pPlatformConfig->fPreloadPlatform(pPlatformConfig->pUserData, &allocator);
    }

    pPlatform->sAllocator = allocator;
    pPlatform->sIsAllocatorOwned = (pPlatformConfig->pAllocator == NULL);
    pPlatform->pUserData = pPlatformConfig->pUserData;
    pPlatform->fRenderPlatform = pPlatformConfig->fRenderPlatform;
    pPlatform->fUpdatePlatform = pPlatformConfig->fUpdatePlatform;
    pPlatform->fErrorPlatform = pPlatformConfig->fErrorPlatform;
    pPlatform->fDestroyPlatform = pPlatformConfig->fDestroyPlatform;
    pPlatform->sIsRunning = true;
    pPlatform->pApplication = NULL;
    assert(pPlatformConfig->fBuildApplication != NULL);
    if ((result = pPlatformConfig->fBuildApplication(pPlatformConfig->pUserData, &allocator, &pPlatform->pApplication)) != LOR_RESULT_SUCCESS)
    {
        LOR_ERROR("Failed to build application.");
        return result;
    }

    lor_PlatformAnyGLFWOpengl3ConfigPtr pCustomConfig = (lor_PlatformAnyGLFWOpengl3ConfigPtr)pPlatformConfig->pCustomPlatformConfig;

    // create and setup the window
    if ((result = __lorPrepareGLFW(pCustomConfig != NULL ? pCustomConfig->windowUndecorated : false)) != LOR_RESULT_SUCCESS)
    {
        LOR_ERROR("Failed to prepare GLFW.");
        return result;
    }

    pPlatform->pWindow = glfwCreateWindow(
        (pCustomConfig != NULL && pCustomConfig->windowStartingWidth) ? pCustomConfig->windowStartingWidth : 800,
        (pCustomConfig != NULL && pCustomConfig->windowStartingHeight) ? pCustomConfig->windowStartingHeight : 600,
        (pCustomConfig != NULL && pCustomConfig->windowTitle) ? pCustomConfig->windowTitle : "Lorien/Window/AnyGLFWOpenGL",
        NULL,
        NULL);
    if (pPlatform->pWindow == NULL)
    {
        LOR_ERROR("Failed to create GLFW window.");
        return LOR_RESULT_OBJECT_CREATION_FAILED;
    }
    glfwMakeContextCurrent(pPlatform->pWindow);

    if (!gladLoadGL())
    {
        LOR_ERROR("Failed to load OpenGL functions!");
        return LOR_RESULT_OBJECT_CREATION_FAILED;
    }

    glfwSetErrorCallback(__lorGLFWErrorCallback);
    glfwSetWindowUserPointer(pPlatform->pWindow, pPlatform);
    glfwSetKeyCallback(pPlatform->pWindow, __lorGLFWKeyCallback);
    glfwSetCharCallback(pPlatform->pWindow, __lorGLFWCharCallback);
    glfwSetDropCallback(pPlatform->pWindow, __lorGLFWDropCallback);
    glfwSetScrollCallback(pPlatform->pWindow, __lorGLFWScrollCallback);
    glfwSetCursorPosCallback(pPlatform->pWindow, __lorGLFWCursorPosCallback);
    glfwSetWindowPosCallback(pPlatform->pWindow, __lorGLFWWindowPosCallback);
    glfwSetWindowSizeCallback(pPlatform->pWindow, __lorGLFWWindowSizeCallback);
    glfwSetCursorEnterCallback(pPlatform->pWindow, __lorGLFWCursorEnterCallback);
    glfwSetWindowCloseCallback(pPlatform->pWindow, __lorGLFWWindowCloseCallback);
    glfwSetMouseButtonCallback(pPlatform->pWindow, __lorGLFWMouseButtonCallback);
    glfwSetFramebufferSizeCallback(pPlatform->pWindow, __lorGLFWFramebufferSizeCallback);
    glfwSetWindowMaximizeCallback(pPlatform->pWindow, __lorGLFWWindowMaximizeCallback);
    glfwSetWindowFocusCallback(pPlatform->pWindow, __lorGLFWWindowFocusCallback);

    // build the batch renderer
    if ((result = lorPlatformAnyGLFWOpengl3BatchRendererBuild(&allocator, 1000, 1000, &pPlatform->pBatchRenderer)) != LOR_RESULT_SUCCESS)
    {
        LOR_ERROR("Failed to build batch renderer.");
        return result;
    }

    *ppPlatform = pPlatform;
    return LOR_RESULT_SUCCESS;
}

void lorPlatformAnyGLFWOpengl3Destroy(lor_PlatformAnyGLFWOpengl3Ptr pPlatform)
{
    LOR_ASSERT_MSG(pPlatform != NULL, "Invalid platform provided.");
    LOR_ASSERT_MSG(pPlatform->pWindow != NULL, "Invalid window provided.");

    lorPlatformAnyGLFWOpengl3BatchRendererDestroy(&pPlatform->sAllocator, pPlatform->pBatchRenderer);

    lor_Allocator allocator = pPlatform->sAllocator;
    lorApplicationDestroy(pPlatform->pApplication);

    glfwDestroyWindow(pPlatform->pWindow);
    glfwTerminate();

    if (pPlatform->fDestroyPlatform != NULL)
    {
        pPlatform->fDestroyPlatform(pPlatform->pUserData, &allocator);
    }
    bool isAllocatorOwned = pPlatform->sIsAllocatorOwned;
    lorAllocatorFastFree(&allocator, LOR_ALLOCATION_TYPE_GENERAL, pPlatform);
    if (isAllocatorOwned)
    {
        lorAllocatorDestroy(&allocator);
    }
}

static void __lorPlatformUpdate(lor_PlatformAnyGLFWOpengl3Ptr pPlatform, bool poolEvents) {
    lorApplicationNewFrame(pPlatform->pApplication);
    if(poolEvents) glfwPollEvents(); // This will populate the current input state in the lorApplication
    if (pPlatform->fUpdatePlatform != NULL)
    {
        pPlatform->sIsRunning = pPlatform->fUpdatePlatform(pPlatform->pUserData);
    }

    int32_t sFrameBufferWidth, sFrameBufferHeight;
    glfwGetFramebufferSize(pPlatform->pWindow, &sFrameBufferWidth, &sFrameBufferHeight);
    pPlatform->pApplication->sCurrentInputState.sFramebufferSize.sWidth = (float)sFrameBufferWidth;
    pPlatform->pApplication->sCurrentInputState.sFramebufferSize.sHeight = (float)sFrameBufferHeight;
    lorPlatformAnyGLFWOpengl3BatchRendererSetViewport(pPlatform->pBatchRenderer, 0.0f, 0.0f, (float)sFrameBufferWidth, (float)sFrameBufferHeight);

    pPlatform->sIsRunning &= lorApplicationUpdate(pPlatform->pApplication);

    __lorRenderFrame(pPlatform);
    if (pPlatform->fRenderPlatform != NULL)
    {
        pPlatform->fRenderPlatform(pPlatform->pUserData);
    }
    glfwSwapBuffers(pPlatform->pWindow);
}

void lorPlatformAnyGLFWOpengl3Run(lor_PlatformAnyGLFWOpengl3Ptr pPlatform)
{
    while (pPlatform->sIsRunning)
    {
		__lorPlatformUpdate(pPlatform, true);
    }
}

// ---------- API Functions End ----------
