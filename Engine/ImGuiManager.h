#pragma once

class Device;
class ImGuiManager
{
	DECLARE_SINGLE(ImGuiManager);

public:
	static ID3D12DescriptorHeap* srvDescHeap;

public:
	void Init(HWND hwnd, std::shared_ptr<Device> device);
	void Render();
	void Destroy();
};

