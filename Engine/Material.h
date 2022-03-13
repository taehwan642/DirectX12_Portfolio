#pragma once
#include "Object.h"

class Shader;
class Texture;
class ImGuiManager;
class InstancingManager;

enum
{
	MATERIAL_ARG_COUNT = 4,
};

struct MaterialParams
{
	MaterialParams()
	{
		for (int32 i = 0; i < MATERIAL_ARG_COUNT; i++)
		{
			SetInt(i, 0);
			SetFloat(i, 0.f);
			SetTexOn(i, 0);
		}
	}

	void SetInt(uint8 index, int32 value) { intParams[index] = value; }
	void SetFloat(uint8 index, float value) { floatParams[index] = value; }
	void SetTexOn(uint8 index, int32 value) { texOnParams[index] = value; }
	void SetVec2(uint8 index, Vec2 value) { vec2Params[index] = value; }
	void SetVec4(uint8 index, Vec4 value) { vec4Params[index] = value; }
	void SetMatrix(uint8 index, Matrix& value) { matrixParams[index] = value; }

	std::array<int32, MATERIAL_ARG_COUNT> intParams;
	std::array<float, MATERIAL_ARG_COUNT> floatParams;
	std::array<int32, MATERIAL_ARG_COUNT> texOnParams;
	std::array<Vec2, MATERIAL_ARG_COUNT> vec2Params;
	std::array<Vec4, MATERIAL_ARG_COUNT> vec4Params;
	std::array<Matrix, MATERIAL_ARG_COUNT> matrixParams;

	//RTTR_ENABLE()
};

class Material : public Object
{
public:
	Material();
	virtual ~Material();

	std::shared_ptr<Shader> GetShader() { return _shader; }

	void SetShader(std::shared_ptr<Shader> shader) { _shader = shader; }
	void SetInt(uint8 index, int32 value) { _params.SetInt(index, value); }
	void SetFloat(uint8 index, float value) { _params.SetFloat(index, value); }
	void SetTexture(uint8 index, std::shared_ptr<Texture> texture)
	{ 
		_textures[index] = texture;
		_params.SetTexOn(index, (texture == nullptr ? 0 : 1));
	}

	void SetVec2(uint8 index, Vec2 value) { _params.SetVec2(index, value); }
	void SetVec4(uint8 index, Vec4 value) { _params.SetVec4(index, value); }
	void SetMatrix(uint8 index, Matrix& value) { _params.SetMatrix(index, value); }

	void PushGraphicsData();
	void PushComputeData();
	void Dispatch(uint32 x, uint32 y, uint32 z);

	std::shared_ptr<Material> Clone();

private:
	friend class ImGuiManager;
	friend class InstancingManager;
	friend class RTTRMaterialValue;

	std::shared_ptr<Shader>	_shader;
	MaterialParams		_params;

	std::array<std::shared_ptr<Texture>, MATERIAL_ARG_COUNT> _textures;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

