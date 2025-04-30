#ifndef LORIEN_PLATFORM_ANY_GLFW_OPENGL3_H
#define LORIEN_PLATFORM_ANY_GLFW_OPENGL3_H

#include "lorien/lorien.h"

// struct forward declarationsq
struct lor_PlatformAnyGLFWOpengl3;
struct lor_PlatformAnyGLFWOpengl3Config;

struct GLFWwindow;
typedef struct GLFWwindow GLFWwindow;


// struct 
struct lor_PlatformAnyGLFWOpengl3BatchRenderer {
    uint32_t sVertexArrayObject;
    uint32_t sVertexBuffer;
    uint32_t sIndexBuffer;
    uint32_t sShaderProgram;    

    lor_Vertex* pVertices;
    size_t sVertexCount;
    size_t sVertexCapacity; 

    lor_Rect sCurrentRect;
    lor_Rect viewportRect;

    uint32_t* pIndices;
    size_t sIndexCount;
    size_t sIndexCapacity;
};
typedef struct lor_PlatformAnyGLFWOpengl3BatchRenderer lor_PlatformAnyGLFWOpengl3BatchRenderer;
typedef struct lor_PlatformAnyGLFWOpengl3BatchRenderer* lor_PlatformAnyGLFWOpengl3BatchRendererPtr;

struct lor_PlatformAnyGLFWOpengl3Config {
    bool windowUndecorated;
    int windowStartingWidth;
    int windowStartingHeight;
	const char* windowTitle;
};
typedef struct lor_PlatformAnyGLFWOpengl3Config lor_PlatformAnyGLFWOpengl3Config;
typedef struct lor_PlatformAnyGLFWOpengl3Config* lor_PlatformAnyGLFWOpengl3ConfigPtr;

struct lor_PlatformAnyGLFWOpengl3 {
    lor_ApplicationPtr pApplication;
    lor_Allocator sAllocator;
    bool sIsAllocatorOwned;
    bool sIsRunning;

    lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer;

    lor_ISize sFramebufferSize;
	GLFWwindow* pWindow;
    
    lor_PlatformErrorFn fErrorPlatform;
    lor_PlatformUpdateFn fUpdatePlatform;
    lor_PlatformRenderFn fRenderPlatform;
    lor_PlatformDestroyFn fDestroyPlatform;

    void* pUserData;
};
typedef struct lor_PlatformAnyGLFWOpengl3 lor_PlatformAnyGLFWOpengl3;
typedef struct lor_PlatformAnyGLFWOpengl3* lor_PlatformAnyGLFWOpengl3Ptr;

// functions 
LOR_API lor_Result lorPlatformAnyGLFWOpengl3Build(lor_PlatformConfigPtr pPlatformConfig, lor_PlatformAnyGLFWOpengl3Ptr* ppPlatform);
LOR_API void lorPlatformAnyGLFWOpengl3Destroy(lor_PlatformAnyGLFWOpengl3Ptr pPlatform);
LOR_API void lorPlatformAnyGLFWOpengl3Run(lor_PlatformAnyGLFWOpengl3Ptr pPlatform);

LOR_API lor_Result lorPlatformAnyGLFWOpengl3BatchRendererBuild(lor_AllocatorPtr pAllocator, size_t initialVertexSize, size_t initialIndexSize, lor_PlatformAnyGLFWOpengl3BatchRendererPtr* ppBatchRenderer);
LOR_API void lorPlatformAnyGLFWOpengl3BatchRendererDestroy(lor_AllocatorPtr pAllocator, lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer);
LOR_API void lorPlatformAnyGLFWOpengl3BatchRendererAddVertices(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer, const lor_VertexPtr pVertices, size_t vertexCount, uint32_t* indices, size_t indicexCount);
LOR_API void lorPlatformAnyGLFWOpengl3BatchRendererAddPrim(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer, const lor_RenderablePrimitivePtr pPrimitive);
LOR_API void lorPlatformAnyGLFWOpengl3BatchRendererFlush(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer);
LOR_API void lorPlatformAnyGLFWOpengl3BatchRendererSetViewport(lor_PlatformAnyGLFWOpengl3BatchRendererPtr pBatchRenderer, float x, float y, float width, float height);

#endif // LORIEN_PLATFORM_ANY_GLFW_OPENGL3_H