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

#ifndef LOR_MAX_CHILDREN
#define LOR_MAX_CHILDREN 64
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
    LOR_STRUCT_RENDERABLE_PRIMITIVE
};
typedef enum lor_StructTypes lor_StructTypes;

enum lor_PrimTypes {
    LOR_PRIM_TYPE_VOID,
    LOR_PRIM_TYPE_RECT,
    LOR_PRIM_TYPE_CIRCLE,
    LOR_PRIM_TYPE_MESH,
};
typedef enum lor_PrimTypes lor_PrimTypes;

enum lor_UILayoutTypes {
    LOR_UI_LAYOUT_TYPE_FLOW,
    LOR_UI_LAYOUT_TYPE_GRID,
    LOR_UI_LAYOUT_TYPE_V_STACK,
    LOR_UI_LAYOUT_TYPE_H_STACK,
};
typedef enum lor_UILayoutTypes lor_UILayoutTypes;

enum lor_AllocationType {
    LOR_ALLOCATION_TYPE_GENERAL,
    LOR_ALLOCATION_TYPE_RENDERABLE_PRIMITIVE,
    LOR_ALLOCATION_PLATFORM_VERTICES,
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

// utility structs
struct lor_Size {
    float sWidth;
    float sHeight;
};
typedef struct lor_Size lor_Size;
typedef lor_Size* lor_SizePtr;

struct lor_Point {
    float sX;
    float sY;
};
typedef struct lor_Point lor_Point;
typedef lor_Point* lor_PointPtr;

struct lor_ISize {
    int32_t sWidth;
    int32_t sHeight;
};
typedef struct lor_ISize lor_ISize;
typedef lor_ISize* lor_ISizePtr;


struct lor_Rect {
    float sMinX;
    float sMinY;
    float sMaxX;
    float sMaxY;
};
typedef struct lor_Rect lor_Rect;
typedef lor_Rect* lor_RectPtr;

typedef uint32_t lor_Color;

struct lor_Vertex {
    float sX, sY, sZ;
    float sU, sV;
    float sR, sG, sB, sA;
    int32_t sFlag0, sFlag1, sFlag2;
};
typedef struct lor_Vertex lor_Vertex;
typedef lor_Vertex* lor_VertexPtr;

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
    lor_Size sFramebufferSize;
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

struct lor_RenderablePrimitiveRect {
    lor_PrimTypes sType;
};
typedef struct lor_RenderablePrimitiveRect lor_RenderablePrimitiveRect;
typedef lor_RenderablePrimitiveRect* lor_RenderablePrimitiveRectPtr;

struct lor_RenderablePrimitiveCircle {
    lor_PrimTypes sType;
    float sRadius;
    size_t sSegments; // may or may not be used by the platform 
};
typedef struct lor_RenderablePrimitiveCircle lor_RenderablePrimitiveCircle;
typedef lor_RenderablePrimitiveCircle* lor_RenderablePrimitiveCirclePtr;

struct lor_RenderablePrimitiveMesh {
    lor_PrimTypes sType;
    size_t sVertexCount;
    size_t sIndexCount;
    float* pVertices; // array of vertices
    uint32_t* pIndices; // array of indices
};
typedef struct lor_RenderablePrimitiveMesh lor_RenderablePrimitiveMesh;
typedef lor_RenderablePrimitiveMesh* lor_RenderablePrimitiveMeshPtr;

union lor_RenderablePrimitiveData {
    lor_PrimTypes sType; // for type checking
    lor_RenderablePrimitiveRect sRect;
    lor_RenderablePrimitiveCircle sCircle;
    lor_RenderablePrimitiveMesh sMesh;
};
typedef union lor_RenderablePrimitiveData lor_RenderablePrimitiveData;
typedef lor_RenderablePrimitiveData* lor_RenderablePrimitiveDataPtr;

struct lor_RenderablePrimitive {
    lor_StructTypes sType;
    lor_RenderablePrimitiveData sData;
    lor_Rect sRect;
    struct lor_RenderablePrimitive* pParent;
    struct lor_RenderablePrimitive* pChildren[LOR_MAX_CHILDREN]; // array of children
    size_t sChildrenCount;
    size_t sId;
};
typedef struct lor_RenderablePrimitive lor_RenderablePrimitive;
typedef lor_RenderablePrimitive* lor_RenderablePrimitivePtr;


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
    
    lor_RenderablePrimitive* pRootRenderablePrimitive;
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

LOR_API size_t lorGenerateUniqueId();

LOR_API void lorRectFillMinMax(lor_Rect* pRect, float minX, float minY, float maxX, float maxY);
LOR_API void lorRectFillPosSize(lor_Rect* pRect, float posX, float posY, float sizeX, float sizeY);
LOR_API lor_Rect lorRectMinMax(float minX, float minY, float maxX, float maxY);
LOR_API lor_Rect lorRectPosSize(float posX, float posY, float sizeX, float sizeY);
LOR_API void lorRectUnionInplace(lor_Rect* pRect, const lor_Rect* pOtherRect);  
LOR_API lor_Rect lorRectUnion(const lor_Rect* pRect, const lor_Rect* pOtherRect);
LOR_API lor_Size lorRectGetSize(const lor_Rect* pRect);
LOR_API lor_Point lorRectGetCenter(const lor_Rect* pRect);
LOR_API lor_Point lorRectGetMin(const lor_Rect* pRect);

LOR_API lor_Point lorPoint(float x, float y);
LOR_API lor_ISize lorISize(int32_t width, int32_t height);
LOR_API lor_Size lorSize(float width, float height);


LOR_API lor_Result lorGetDefaultAllocator(lor_AllocatorPtr pAllocator);

LOR_API void lorAllocatorDestroy(lor_AllocatorPtr pAllocator);
LOR_API lor_Result lorAllocatorAllocate(lor_AllocatorPtr pAllocator, size_t size, lor_AllocationType type, void** ppOut);
LOR_API void lorAllocatorAutoFree(lor_AllocatorPtr pAllocator, void* pObject);
LOR_API void lorAllocatorFastFree(lor_AllocatorPtr pAllocator, lor_AllocationType type, void* pObject);
LOR_API void lorAllocatorDefragment(lor_AllocatorPtr pAllocator);

LOR_API void lorInputStateNewFrame(lor_InputStatePtr pInputState);
LOR_API void lorInputStateSwap(lor_InputStatePtr pCurrentState, lor_InputStatePtr pPreviousState);
LOR_API void lorInputStateReset(lor_InputStatePtr pInputState);

LOR_API lor_Result lorRenderablePrimitiveAllocate(lor_AllocatorPtr pAllocator, lor_RenderablePrimitivePtr* ppRenderablePrimitive, lor_PrimTypes sType);
LOR_API void lorRenderablePrimitiveDestroyRecursive(lor_AllocatorPtr pAllocator, lor_RenderablePrimitivePtr pRenderablePrimitive);
LOR_API void lorRenderablePrimitiveDestroy(lor_AllocatorPtr pAllocator, lor_RenderablePrimitivePtr pRenderablePrimitive);
LOR_API void lorRenderablePrimitiveDetach(lor_RenderablePrimitivePtr pRenderablePrimitive);
LOR_API const char* lorRenderablePrimitiveGetTypeString(lor_PrimTypes sType);
LOR_API void lorRenderablePrimitiveLogTree(lor_RenderablePrimitivePtr pRenderablePrimitive);


LOR_API lor_Result lorApplicationBuild(lor_ApplicationConfigPtr pConfig, lor_ApplicationPtr* ppApplication);
LOR_API void lorApplicationDestroy(lor_ApplicationPtr pApplication);
LOR_API bool lorApplicationUpdate(lor_ApplicationPtr pApplication);
LOR_API void lorApplicationNewFrame(lor_ApplicationPtr pApplication);
LOR_API lor_InputStatePtr lorApplicationGetInputState(lor_ApplicationPtr pApplication);



#endif // define LORIEN_H