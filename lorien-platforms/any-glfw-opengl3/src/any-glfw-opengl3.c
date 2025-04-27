#include "lorien/platforms/any-glfw-opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ---------- GLFW Callbacks/Utilities ----------

static lor_Result __lorPrepareGLFW(bool undecoratedWindow) {
    if (!glfwInit()) {
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


void __lorGLFWErrorCallback(int error, const char* description) {
    LOR_ERROR_VERBOSE("GLFW Error: %d - %s\n", error, description);
}

void __lorGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle key events
}

void __lorGLFWCharCallback(GLFWwindow* window, unsigned int codepoint) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle char events
}

void __lorGLFWDropCallback(GLFWwindow* window, int count, const char** paths) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle drop events
}

void __lorGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle scroll events
}

void __lorGLFWCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle cursor position events
}

void __lorGLFWWindowPosCallback(GLFWwindow* window, int xpos, int ypos) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle window position events
}

void __lorGLFWWindowSizeCallback(GLFWwindow* window, int width, int height) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle window size events
}

void __lorGLFWCursorEnterCallback(GLFWwindow* window, int entered) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle cursor enter events
}

void __lorGLFWWindowCloseCallback(GLFWwindow* window) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    pInputState->sShouldWindowClose = true;
}

void __lorGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle mouse button events
}

void __lorGLFWFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    pPlatform->sFramebufferSize.sWidth = width;
    pPlatform->sFramebufferSize.sHeight = height;
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    pInputState->sFramebufferSize.sWidth = (float)width;
    pInputState->sFramebufferSize.sHeight = (float)height;
}

void __lorGLFWWindowMaximizeCallback(GLFWwindow* window, int maximized) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle window maximize events
}

void __lorGLFWWindowFocusCallback(GLFWwindow* window, int focused) {
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = (lor_PlatformAnyGLFWOpengl3Ptr)glfwGetWindowUserPointer(window);
    lor_InputStatePtr pInputState = lorApplicationGetInputState(pPlatform->pApplication);
    // TODO: Handle window focus events
}

// ---------- GLFW Callbacks/Utilities End ----------

// ---------- Rendering Functions ----------

// do the rendering
static void __lorRenderFrame(lor_PlatformAnyGLFWOpengl3Ptr pPlatform) {
    LOR_ASSERT_MSG(pPlatform != NULL, "Invalid platform provided.");
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, pPlatform->sFramebufferSize.sWidth, pPlatform->sFramebufferSize.sHeight);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    


    
    
}

// ---------- Rendering Functions End  ----------

// ---------- API Functions  ----------

lor_Result lorPlatformAnyGLFWOpengl3Build(lor_PlatformConfigPtr pPlatformConfig, lor_PlatformAnyGLFWOpengl3Ptr* ppPlatform) {
    LOR_ASSERT_MSG(pPlatformConfig != NULL, "Invalid platform config provided.");
    LOR_ASSERT_MSG(ppPlatform != NULL, "Invalid platform pointer provided.");
    
    lor_Result result = LOR_RESULT_UNKNOWN_ERROR;
    if (pPlatformConfig->sType != LOR_STRUCT_PLATFORM_CONFIG) {
        LOR_ERROR("Invalid struct type provided.");
        return LOR_RESULT_INVALID_ARGUMENT;
    }
    
    lor_Allocator allocator = { 0 };
    if (pPlatformConfig->pAllocator == NULL) {
        if ((result = lorGetDefaultAllocator(&allocator)) != LOR_RESULT_SUCCESS) {
            LOR_ERROR("Failed to get default allocator.");
            return result;
        }
    } else {
        allocator = *pPlatformConfig->pAllocator; // copy the allocator
    }
    
    lor_PlatformAnyGLFWOpengl3Ptr pPlatform = NULL;
    if ((result = lorAllocatorAllocate(&allocator, sizeof(lor_PlatformAnyGLFWOpengl3), LOR_ALLOCATION_TYPE_GENERAL, (void**)&pPlatform)) != LOR_RESULT_SUCCESS) {
        LOR_ERROR("Failed to allocate platform.");
        return result;
    }
    
    if (pPlatformConfig->fPreloadPlatform != NULL) {
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
    if ((result = pPlatformConfig->fBuildApplication(pPlatformConfig->pUserData, &allocator, &pPlatform->pApplication)) != LOR_RESULT_SUCCESS) {
        LOR_ERROR("Failed to build application.");
        return result;
    }
    
	lor_PlatformAnyGLFWOpengl3ConfigPtr pCustomConfig = (lor_PlatformAnyGLFWOpengl3ConfigPtr)pPlatformConfig->pCustomPlatformConfig;
    
    // create and setup the window
	if ((result = __lorPrepareGLFW(pCustomConfig != NULL ? pCustomConfig->windowUndecorated : false)) != LOR_RESULT_SUCCESS) {
        LOR_ERROR("Failed to prepare GLFW.");
		return result;
	}
    
	pPlatform->pWindow = glfwCreateWindow(
        (pCustomConfig != NULL && pCustomConfig->windowStartingWidth) ? pCustomConfig->windowStartingWidth : 800,
		(pCustomConfig != NULL && pCustomConfig->windowStartingHeight) ? pCustomConfig->windowStartingHeight : 600,
		(pCustomConfig != NULL && pCustomConfig->windowTitle) ? pCustomConfig->windowTitle : "Lorien/Window/AnyGLFWOpenGL",
		NULL,
		NULL
	);
	if (pPlatform->pWindow == NULL) {
        LOR_ERROR("Failed to create GLFW window.");
		return LOR_RESULT_OBJECT_CREATION_FAILED;
	}
	glfwMakeContextCurrent(pPlatform->pWindow);
    
    if (!gladLoadGL()) {
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
    
    *ppPlatform = pPlatform;
    return LOR_RESULT_SUCCESS;
}

void lorPlatformAnyGLFWOpengl3Destroy(lor_PlatformAnyGLFWOpengl3Ptr pPlatform) {
    LOR_ASSERT_MSG(pPlatform != NULL, "Invalid platform provided.");
    LOR_ASSERT_MSG(pPlatform->pWindow != NULL, "Invalid window provided.");


    lor_Allocator allocator = pPlatform->sAllocator;
    lorApplicationDestroy(pPlatform->pApplication);
    
	glfwDestroyWindow(pPlatform->pWindow);
    glfwTerminate();
    
    if (pPlatform->fDestroyPlatform != NULL) {
        pPlatform->fDestroyPlatform(pPlatform->pUserData, &allocator);
    }
	bool isAllocatorOwned = pPlatform->sIsAllocatorOwned;
    lorAllocatorFastFree(&allocator, LOR_ALLOCATION_TYPE_GENERAL, pPlatform);
    if (isAllocatorOwned) { 
        lorAllocatorDestroy(&allocator);
    }
}

void lorPlatformAnyGLFWOpengl3Run(lor_PlatformAnyGLFWOpengl3Ptr pPlatform) {
    while (pPlatform->sIsRunning) {
        lorApplicationNewFrame(pPlatform->pApplication);

        glfwPollEvents(); // This will populate the current input state in the lorApplication
        if (pPlatform->fUpdatePlatform != NULL) {
            pPlatform->sIsRunning = pPlatform->fUpdatePlatform(pPlatform->pUserData);
        }
        
        pPlatform->sIsRunning &= lorApplicationUpdate(pPlatform->pApplication);
        
        __lorRenderFrame(pPlatform);       
        if (pPlatform->fRenderPlatform != NULL) {
            pPlatform->fRenderPlatform(pPlatform->pUserData);
        }
		glfwSwapBuffers(pPlatform->pWindow);
    }
}

// ---------- API Functions End ----------