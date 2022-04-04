#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "Texture.h"
#include "RenderTargetGroup.h"
#ifdef TOOL
#include "ImGuiManager.h"
#endif
class Engine
{
public:
	void Init(const WindowInfo& info);
	void Update();

public:
	const WindowInfo& GetWindow() { return _window; }
	std::shared_ptr<Device> GetDevice() { return _device; }
	std::shared_ptr<GraphicsCommandQueue> GetGraphicsCmdQueue() { return _graphicsCmdQueue; }
	std::shared_ptr<ComputeCommandQueue> GetComputeCmdQueue() { return _computeCmdQueue; }
	std::shared_ptr<SwapChain> GetSwapChain() { return _swapChain; }
	std::shared_ptr<RootSignature> GetRootSignature() { return _rootSignature; }
	std::shared_ptr<GraphicsDescriptorHeap> GetGraphicsDescHeap() { return _graphicsDescHeap; }
	std::shared_ptr<ComputeDescriptorHeap> GetComputeDescHeap() { return _computeDescHeap; }

	std::shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }
	std::shared_ptr<RenderTargetGroup> GetRTGroup(RENDER_TARGET_GROUP_TYPE type) { return _rtGroups[static_cast<uint8>(type)]; }

#ifdef TOOL
	std::shared_ptr<ImGuiManager> GetImGuiManager() { return _imguiManager; }
#endif
public:
	void Render();
	void RenderBegin();
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

private:
	void ShowFps();
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);
	void CreateRenderTargetGroups();

private:
	// 그려질 화면 크기 관련
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};

	std::shared_ptr<Device> _device = std::make_shared<Device>();
	std::shared_ptr<GraphicsCommandQueue> _graphicsCmdQueue = std::make_shared<GraphicsCommandQueue>();
	std::shared_ptr<ComputeCommandQueue> _computeCmdQueue = std::make_shared<ComputeCommandQueue>();
	std::shared_ptr<SwapChain> _swapChain = std::make_shared<SwapChain>();
	std::shared_ptr<RootSignature> _rootSignature = std::make_shared<RootSignature>();
	std::shared_ptr<GraphicsDescriptorHeap> _graphicsDescHeap = std::make_shared<GraphicsDescriptorHeap>();
	std::shared_ptr<ComputeDescriptorHeap> _computeDescHeap = std::make_shared<ComputeDescriptorHeap>();

	std::vector<std::shared_ptr<ConstantBuffer>> _constantBuffers;
	std::array<std::shared_ptr<RenderTargetGroup>, RENDER_TARGET_GROUP_COUNT> _rtGroups;
#ifdef TOOL
	std::shared_ptr<ImGuiManager> _imguiManager;
#endif
};

