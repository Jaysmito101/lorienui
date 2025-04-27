#ifndef LORIEN_PLATFORM_ANY_GLFW_OPENGL3_H
#define LORIEN_PLATFORM_ANY_GLFW_OPENGL3_H

#include "lorien/lorien.h"

// struct forward declarationsq
struct lor_PlatformAnyGLFWOpengl3;
struct lor_PlatformAnyGLFWOpengl3Config;

struct GLFWwindow;
typedef struct GLFWwindow GLFWwindow;


// struct 
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

#endif // LORIEN_PLATFORM_ANY_GLFW_OPENGL3_H