#pragma once

class Device;
class GraphicsCommandQueue;

struct float3
{
	float x;
	float y;
	float z;
};

class ImGuiManager
{
public:
	std::vector<float3*> positions;
	std::vector<float*> floats;

public:
	ImGuiManager(HWND hwnd, std::shared_ptr<Device> device);
	~ImGuiManager();

	void Render();
	void SetPipeline(std::shared_ptr<GraphicsCommandQueue> cmdq);

private:
	ComPtr<ID3D12DescriptorHeap> _srvDescHeap;

};

