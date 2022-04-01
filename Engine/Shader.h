#pragma once
#include "Object.h"

enum class SHADER_TYPE : uint8
{
	DEFERRED,
	FORWARD,
	LIGHTING,
	PARTICLE,
	COMPUTE,
	SHADOW,
};

enum class RASTERIZER_TYPE : uint8
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	WIREFRAME,
};

enum class DEPTH_STENCIL_TYPE : uint8
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_DEPTH_TEST, // ±Ì¿Ã ≈◊Ω∫∆Æ(X) + ±Ì¿Ã ±‚∑œ(O)
	NO_DEPTH_TEST_NO_WRITE, // ±Ì¿Ã ≈◊Ω∫∆Æ(X) + ±Ì¿Ã ±‚∑œ(X)
	LESS_NO_WRITE, // ±Ì¿Ã ≈◊Ω∫∆Æ(O) + ±Ì¿Ã ±‚∑œ(X)
};

enum class BLEND_TYPE : uint8
{
	DEFAULT,
	ALPHA_BLEND,
	ONE_TO_ONE_BLEND,
	END,
};

struct ShaderInfo
{
	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
	BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;
	D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

struct ShaderArg
{
	std::string vs = "VS_Main";
	std::string hs;
	std::string ds;
	std::string gs;
	std::string ps = "PS_Main";

	std::string cs;
};

class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

	void CreateGraphicsShader(const std::wstring& path, ShaderInfo info = ShaderInfo(), ShaderArg arg = ShaderArg());
	void CreateComputeShader(const std::wstring& path, const std::string& name, const std::string& version);
	
	void Update();

	SHADER_TYPE GetShaderType() { return _info.shaderType; }

	static D3D12_PRIMITIVE_TOPOLOGY_TYPE GetTopologyType(D3D_PRIMITIVE_TOPOLOGY topology);

private:
	void CreateShader(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreateHullShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreateDomainShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreateGeometryShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version);

private:
	friend class Resources;
	friend class RTTRShaderValue;

	ShaderInfo _info;
	ShaderArg _arg;

	ComPtr<ID3D12PipelineState>			_pipelineState;

	// GraphicsShader
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_hsBlob;
	ComPtr<ID3DBlob>					_dsBlob;
	ComPtr<ID3DBlob>					_gsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _graphicsPipelineDesc = {};

	// ComputeShader
	ComPtr<ID3DBlob>					_csBlob;
	D3D12_COMPUTE_PIPELINE_STATE_DESC   _computePipelineDesc = {};

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

