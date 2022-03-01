#pragma once

class Device;
class GraphicsCommandQueue;

class ImGuiManager
{
public:
	ImGuiManager(HWND hwnd, std::shared_ptr<Device> device);
	~ImGuiManager();

	void Render();
	void SetPipeline(std::shared_ptr<GraphicsCommandQueue> cmdq);

private:
	ComPtr<ID3D12DescriptorHeap> _srvDescHeap;
};

