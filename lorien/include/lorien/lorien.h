#ifndef LORIEN_H
#define LORIEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <ctype.h>
#include <stdarg.h>
#include <stddef.h>
#include <inttypes.h>
#include <wchar.h>
#include <locale.h>

// define API
#ifndef LOR_API

#ifdef LOR_SHARED
#ifdef _WIN32
#ifdef LOR_DLL_EXPORTS
#define LOR_API __declspec(dllexport)
#else
#define LOR_API __declspec(dllimport)
#endif // LOR_DLL_EXPORTS
#else // _WIN32
#define LOR_API __attribute__((visibility("default")))
#endif // _WIN32
#else // LOR_SHARED
#define LOR_API
#endif // LOR_SHARED
#endif // LOR_API


// macros
#ifdef LOR_DEBUG
#define LOR_ASSERT(x) assert(x)
#define LOR_ASSERT_MSG(x, msg) assert(x && msg)
#else
#define LOR_ASSERT(x) ((void)0)
#define LOR_ASSERT_MSG(x, msg) ((void)0)
#endif

#ifndef LOR_WARN
#define LOR_WARN(msg) fprintf(stderr, "Warning: %s\n", msg)
#endif

#ifndef LOR_ERROR
#define LOR_ERROR(msg) fprintf(stderr, "Error: %s\n", msg)
#endif

#ifndef LOR_ERROR_VERBOSE
#define LOR_ERROR_VERBOSE(...) fprintf(stderr, "Error: " __VA_ARGS__)
#endif

#ifndef LOR_MALLOC
#define LOR_MALLOC(size) malloc(size)
#endif

#ifndef LOR_CALLOC
#define LOR_CALLOC(count, size) calloc(count, size)
#endif

#ifndef LOR_REALLOC
#define LOR_REALLOC(ptr, size) realloc(ptr, size)
#endif

#ifndef LOR_FREE
#define LOR_FREE(ptr) free(ptr)
#endif


// in lor color is a rgba inside a uint32t
#define LOR_PACK_RGBA(r, g, b, a) ((uint32_t)((((uint32_t)(a) & 0xFF) << 24) | (((uint32_t)(b) & 0xFF) << 16) | (((uint32_t)(g) & 0xFF) << 8) | ((uint32_t)(r) & 0xFF)))

// enums
enum lor_StructTypes {
    LOR_STRUCT_APPLICATION_CONFIG,
    LOR_STRUCT_APPLICATION,
    LOR_STRUCT_PLATFORM_CONFIG,
    LOR_STRUCT_UI_CONFIG,
    LOR_STRUCT_UI,
    LOR_STRUCT_UI_ELEMENT_EMPTY,
    LOR_STRUCT_UI_LAYOUT,
    LOR_STRUCT_ALLOCATOR,
    LOR_STRUCT_INPUT_STATE,
};
typedef enum lor_StructTypes lor_StructTypes;

enum lor_UILayoutTypes {
    LOR_UI_LAYOUT_TYPE_FLOW,
    LOR_UI_LAYOUT_TYPE_GRID,
    LOR_UI_LAYOUT_TYPE_V_STACK,
    LOR_UI_LAYOUT_TYPE_H_STACK,
};
typedef enum lor_UILayoutTypes lor_UILayoutTypes;

enum lor_AllocationType {
    LOR_ALLOCATION_TYPE_GENERAL,
};
typedef enum lor_AllocationType lor_AllocationType;

enum lor_Result {
    LOR_RESULT_SUCCESS,
    LOR_RESULT_FAILURE,
    LOR_RESULT_OUT_OF_MEMORY,
    LOR_RESULT_INVALID_ARGUMENT,
    LOR_RESULT_NOT_IMPLEMENTED,
    LOR_RESULT_UNKNOWN_ERROR,
    LOR_RESULT_OBJECT_CREATION_FAILED,
    LOR_RESULT_INVALID_STATE
};
typedef enum lor_Result lor_Result;

// struct forward declarations
struct lor_ApplicationConfig;
struct lor_Application;

struct lor_PlatformConfig;

struct lor_UIConfig;
struct lor_UI;
struct lor_UIElementEmpty;
struct lor_UILayout;
struct lor_Allocator;
struct lor_PlatformError;

// function types
typedef lor_Result (*lor_BuildUIFn)(void*, struct lor_Allocator*, struct lor_UI**);
typedef void (*lor_AllocatorDestroyFn)(void*);
typedef lor_Result (*lor_AllocatorAllocateFn)(void*, size_t, lor_AllocationType, void**);
typedef void (*lor_AllocatorAutoFreeFn)(void*, void*);
typedef void (*lor_AllocatorFastFreeFn)(void*, lor_AllocationType, void*);
typedef void (*lor_AllocatorDefragmentFn)(void*);

typedef void (*lor_PlatformPreloadFn)(void*, struct lor_Allocator*);
typedef lor_Result (*lor_PlatformBuildApplicationFn)(void*, struct lor_Allocator*, struct lor_Application**);
typedef void (*lor_PlatformErrorFn)(void*, struct lor_PlatformError*);
typedef bool (*lor_PlatformUpdateFn)(void*);
typedef void (*lor_PlatformRenderFn)(void*);
typedef void (*lor_PlatformDestroyFn)(void*, struct lor_Allocator*);

typedef bool (*lor_ConfirmationFn)(void*);

// structs
struct lor_Allocator {
    enum lor_StructTypes sType;
    lor_AllocatorDestroyFn fDestroyAllocator;
    lor_AllocatorAllocateFn fAllocate;
    lor_AllocatorAutoFreeFn fAutoFree;
    lor_AllocatorFastFreeFn fFastFree;
    lor_AllocatorDefragmentFn fDefragment;
    void* pUserData;
};
typedef struct lor_Allocator lor_Allocator;
typedef lor_Allocator* lor_AllocatorPtr;
struct lor_InputState {
    lor_StructTypes sType;

    bool sShouldWindowClose;
};
typedef struct lor_InputState lor_InputState;
typedef lor_InputState* lor_InputStatePtr;

struct lor_UIConfig {
    lor_StructTypes sType;
    struct lor_UILayout* pRoot;
};
typedef struct lor_UIConfig lor_UIConfig;
typedef lor_UIConfig* lor_UIConfigPtr;


struct lor_UI {
    lor_StructTypes sType;
    void* pUserData;
};
typedef struct lor_UI lor_UI;
typedef lor_UI* lor_UIPtr;

struct lor_UIElementEmpty {
    lor_StructTypes sType;
    uint32_t sWidth;
    uint32_t sHeight;
    uint32_t sFillColor;
};
typedef struct lor_UIElementEmpty lor_UIElementEmpty;
typedef lor_UIElementEmpty* lor_UIElementEmptyPtr;

struct lor_UILayout {
    lor_StructTypes sType;

    uint32_t sLayoutType;
    size_t sChildrenCount;
    void* pChildren; // array of children
};
typedef struct lor_UILayout lor_UILayout;
typedef lor_UILayout* lor_UILayoutPtr;

struct lor_ApplicationConfig {
    lor_StructTypes sType;
    lor_BuildUIFn fBuildUI;
    lor_AllocatorPtr pAllocator;
    lor_ConfirmationFn fShouldClose;
    void* pUserData;
};
typedef struct lor_ApplicationConfig lor_ApplicationConfig;
typedef lor_ApplicationConfig* lor_ApplicationConfigPtr;

struct lor_Application {
    lor_StructTypes sType;
    
    lor_Allocator sAllocator;
    bool sIsAllocatorOwned;

    lor_InputState sCurrentInputState;
    lor_InputState sPreviousInputState;
    
    lor_ConfirmationFn fShouldClose;
    void* pUserData;
};
typedef struct lor_Application lor_Application;
typedef lor_Application* lor_ApplicationPtr;


struct lor_PlatformConfig {
    lor_StructTypes sType;
    lor_PlatformPreloadFn fPreloadPlatform;
    lor_PlatformBuildApplicationFn fBuildApplication;
    lor_PlatformErrorFn fErrorPlatform;
    lor_PlatformUpdateFn fUpdatePlatform;
    lor_PlatformRenderFn fRenderPlatform;
    lor_PlatformDestroyFn fDestroyPlatform;
    lor_AllocatorPtr pAllocator;
    void* pCustomPlatformConfig;
    void* pUserData;
};
typedef struct lor_PlatformConfig lor_PlatformConfig;
typedef lor_PlatformConfig* lor_PlatformConfigPtr;

struct lor_PlatformError {
    lor_StructTypes sType;
    uint32_t sErrorCode;
    const char* sErrorMessage;
    void* pExtraData;
};
typedef struct lor_PlatformError lor_PlatformError;
typedef lor_PlatformError* lor_PlatformErrorPtr;



// functions
LOR_API lor_Result lorGetDefaultAllocator(lor_AllocatorPtr pAllocator);

LOR_API void lorAllocatorDestroy(lor_AllocatorPtr pAllocator);
LOR_API lor_Result lorAllocatorAllocate(lor_AllocatorPtr pAllocator, size_t size, lor_AllocationType type, void** ppOut);
LOR_API void lorAllocatorAutoFree(lor_AllocatorPtr pAllocator, void* pObject);
LOR_API void lorAllocatorFastFree(lor_AllocatorPtr pAllocator, lor_AllocationType type, void* pObject);
LOR_API void lorAllocatorDefragment(lor_AllocatorPtr pAllocator);

LOR_API void lorInputStateNewFrame(lor_InputStatePtr pInputState);
LOR_API void lorInputStateSwap(lor_InputStatePtr pCurrentState, lor_InputStatePtr pPreviousState);
LOR_API void lorInputStateReset(lor_InputStatePtr pInputState);

LOR_API lor_Result lorApplicationBuild(lor_ApplicationConfigPtr pConfig, lor_ApplicationPtr* ppApplication);
LOR_API void lorApplicationDestroy(lor_ApplicationPtr pApplication);
LOR_API bool lorApplicationUpdate(lor_ApplicationPtr pApplication);
LOR_API void lorApplicationNewFrame(lor_ApplicationPtr pApplication);
LOR_API lor_InputStatePtr lorApplicationGetInputState(lor_ApplicationPtr pApplication);



#endif // define LORIEN_H