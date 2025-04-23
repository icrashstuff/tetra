// dear imgui: Renderer Backend for SDL_GPU
// This needs to be used along with the SDL3 Platform Backend

// Implemented features:
//  [X] Renderer: User texture binding. Use simply cast a reference to your SDL_GPUTextureSamplerBinding to ImTextureID.
//  [X] Renderer: Large meshes support (64k+ vertices) with 16-bit indices.

// The aim of imgui_impl_sdlgpu3.h/.cpp is to be usable in your engine without any modification.
// IF YOU FEEL YOU NEED TO MAKE ANY CHANGE TO THIS CODE, please share them and your feedback at https://github.com/ocornut/imgui/

// You can use unmodified imgui_impl_* files in your project. See examples/ folder for examples of using this.
// Prefer including the entire imgui/ repository into your project (either as a copy or as a submodule), and only build the backends you need.
// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// Important note to the reader who wish to integrate imgui_impl_sdlgpu3.cpp/.h in their own engine/app.
// - Unline other backends, the user must call the function Imgui_ImplSDLGPU_PrepareDrawData BEFORE issuing a SDL_GPURenderPass containing ImGui_ImplSDLGPU_RenderDrawData.
//   Calling the function is MANDATORY, otherwise the ImGui will not upload neither the vertex nor the index buffer for the GPU. See imgui_impl_sdlgpu3.cpp for more info.

#pragma once
#include "../imgui.h"      // IMGUI_IMPL_API
#ifndef IMGUI_DISABLE
#include <SDL3/SDL_gpu.h>

// Initialization data, for ImGui_ImplSDLGPU_Init()
// - Remember to set ColorTargetFormat to the correct format. If you're rendering to the swapchain, call SDL_GetGPUSwapchainTextureFormat to query the right value
struct ImGui_ImplSDLGPU3_InitInfo
{
    SDL_GPUDevice*       Device             = nullptr;
    SDL_GPUTextureFormat ColorTargetFormat  = SDL_GPU_TEXTUREFORMAT_INVALID;
    SDL_GPUSampleCount   MSAASamples        = SDL_GPU_SAMPLECOUNT_1;
};

// SDL_GPU Data [tetra]: Make these data structures public

// [tetra]: Special Draw callback value to request renderer backend to bind the pipeline stored in ImDrawCmd::UserCallbackData
#define ImDrawCallback_ChangePipeline (ImDrawCallback)(-16)

// Reusable buffers used for rendering 1 current in-flight frame, for ImGui_ImplSDLGPU3_RenderDrawData()
struct ImGui_ImplSDLGPU3_FrameData
{
    SDL_GPUBuffer*      VertexBuffer     = nullptr;
    SDL_GPUBuffer*      IndexBuffer      = nullptr;
    uint32_t            VertexBufferSize = 0;
    uint32_t            IndexBufferSize  = 0;
};

struct ImGui_ImplSDLGPU3_Data
{
    ImGui_ImplSDLGPU3_InitInfo   InitInfo;

    // Graphics pipeline & shaders
    SDL_GPUShader*               VertexShader   = nullptr;
    SDL_GPUShader*               FragmentShader = nullptr;
    SDL_GPUGraphicsPipeline*     Pipeline       = nullptr;

    // Font data
    SDL_GPUSampler*              FontSampler = nullptr;
    SDL_GPUTexture*              FontTexture = nullptr;
    SDL_GPUTextureSamplerBinding FontBinding = { nullptr, nullptr };

    // Frame data for main window
    ImGui_ImplSDLGPU3_FrameData  MainWindowFrameData;
};

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
IMGUI_IMPL_API bool     ImGui_ImplSDLGPU3_Init(ImGui_ImplSDLGPU3_InitInfo* info);
IMGUI_IMPL_API void     ImGui_ImplSDLGPU3_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplSDLGPU3_NewFrame();
IMGUI_IMPL_API void     Imgui_ImplSDLGPU3_PrepareDrawData(ImDrawData* draw_data, SDL_GPUCommandBuffer* command_buffer);
IMGUI_IMPL_API void     ImGui_ImplSDLGPU3_RenderDrawData(ImDrawData* draw_data, SDL_GPUCommandBuffer* command_buffer, SDL_GPURenderPass* render_pass, SDL_GPUGraphicsPipeline* pipeline = nullptr);

IMGUI_IMPL_API void     ImGui_ImplSDLGPU3_CreateDeviceObjects();
IMGUI_IMPL_API void     ImGui_ImplSDLGPU3_DestroyDeviceObjects();
IMGUI_IMPL_API void     ImGui_ImplSDLGPU3_CreateFontsTexture();
IMGUI_IMPL_API void     ImGui_ImplSDLGPU3_DestroyFontsTexture();

#endif // #ifndef IMGUI_DISABLE
