#pragma once

#include <rttr/registration>
#include <rttr/registration_friend>

// 각종 include
#include <windows.h>
#include <shellapi.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <queue>
#include <fstream>

#include <filesystem>

#include "d3dx12.h"
#include "SimpleMath.h"
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

// 각종 lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "FBX\\debug\\libfbxsdk-md.lib")
#pragma comment(lib, "FBX\\debug\\libxml2-md.lib")
#pragma comment(lib, "FBX\\debug\\zlib-md.lib")
#else
#pragma comment(lib, "FBX\\release\\libfbxsdk-md.lib")
#pragma comment(lib, "FBX\\release\\libxml2-md.lib")
#pragma comment(lib, "FBX\\release\\zlib-md.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "Json\\debug\\jsoncpp.lib")
#else
#pragma comment(lib, "Json\\release\\jsoncpp.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "Rttr\\debug\\rttr_core_d.lib")
#else
#pragma comment(lib, "Rttr\\release\\rttr_core.lib")
#endif

// 각종 typedef
using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;
using Vec2		= DirectX::SimpleMath::Vector2;
using Vec3		= DirectX::SimpleMath::Vector3;
using Vec4		= DirectX::SimpleMath::Vector4;
using Matrix	= DirectX::SimpleMath::Matrix;

enum class CBV_REGISTER : uint8
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : uint8
{
	t0 = static_cast<uint8>(CBV_REGISTER::END),
	t1,
	t2,
	t3,
	t4,
	t5,
	t6,
	t7,
	t8,
	t9,

	END
};

enum class UAV_REGISTER : uint8
{
	u0 = static_cast<uint8>(SRV_REGISTER::END),
	u1,
	u2,
	u3,
	u4,

	END,
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	CBV_REGISTER_COUNT = CBV_REGISTER::END,
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::END) - CBV_REGISTER_COUNT,
	CBV_SRV_REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT,
	UAV_REGISTER_COUNT = static_cast<uint8>(UAV_REGISTER::END) - CBV_SRV_REGISTER_COUNT,
	TOTAL_REGISTER_COUNT = CBV_SRV_REGISTER_COUNT + UAV_REGISTER_COUNT
};

enum class OBJECT_TYPE : uint8
{
	NONE,
	GAMEOBJECT, // PREFAB
	COMPONENT,
	MATERIAL,
	MESH,
	MESH_DATA,
	SHADER,
	TEXTURE,

	END
};

enum
{
	OBJECT_TYPE_COUNT = static_cast<uint8>(OBJECT_TYPE::END)
};

struct WindowInfo
{
	HWND	hwnd; // 출력 윈도우
	int32	width; // 너비
	int32	height; // 높이
	bool	windowed; // 창모드 or 전체화면
};

enum class ConvertType : byte
{
	Load, Save
};

struct Vertex
{
	Vertex() {}
	~Vertex() {}

	Vertex(Vec3 p, Vec2 u, Vec3 n, Vec3 t)
		: pos(p), uv(u), normal(n), tangent(t)
	{
	}

	Vec3 pos;
	Vec2 uv;
	Vec3 normal;
	Vec3 tangent;
	Vec4 weights;
	Vec4 indices;
};


#define DECLARE_SINGLE(type)		\
private:							\
	type() {}						\
	~type() {}						\
public:								\
	static type* GetInstance()		\
	{								\
		static type instance;		\
		return &instance;			\
	}								\

#define GET_SINGLE(type)	type::GetInstance()

#define DEVICE				GEngine->GetDevice()->GetDevice()
#define GRAPHICS_CMD_LIST	GEngine->GetGraphicsCmdQueue()->GetGraphicsCmdList()
#define RESOURCE_CMD_LIST	GEngine->GetGraphicsCmdQueue()->GetResourceCmdList()
#define COMPUTE_CMD_LIST	GEngine->GetComputeCmdQueue()->GetComputeCmdList()

#define GRAPHICS_ROOT_SIGNATURE		GEngine->GetRootSignature()->GetGraphicsRootSignature()
#define COMPUTE_ROOT_SIGNATURE		GEngine->GetRootSignature()->GetComputeRootSignature()

#define IMGUIMANAGER		GEngine->GetImGuiManager()

#define INPUT				GET_SINGLE(Input)
#define DELTA_TIME			GET_SINGLE(Timer)->GetDeltaTime()

#define CONST_BUFFER(type)	GEngine->GetConstantBuffer(type)

// 함수 객체 ClassNameToString
template<typename T>
struct ClassNameToString
{
	std::string operator()()
	{
		std::string result;
		std::string name = typeid(T).name();
		result = name.substr(6);
		return result;
	}
};

#define MONOBEHAVIOUR(type)	type() { _className = s2ws(ClassNameToString< type >()()); }

#define RTTRMONOSAVE(type)																\
if (std::dynamic_pointer_cast<type>(gameObject->_scripts[i]) != nullptr)	\
{																						\
	monobehaviourOnValue[static_cast<int>(MonoBehaviourType:: type)] = true;\
}																						\

#define RTTRMONOLOAD(type)																		\
if (value.monobehaviourOnValue[static_cast<int>(MonoBehaviourType:: type)] == true)	\
{																								\
	object->AddComponent(std::make_shared<type>());								\
}																								

#define RTTRMONOREGISTER(type) \
rttr::registration::class_<type>( ClassNameToString< type >()() ) \
	.constructor<>() \

#define IMGUIADDMONOBEHAVIOUR(type)\
if (_currentGameObject->GetComponent<type>() == nullptr && ImGui::Button(ClassNameToString< type >()().c_str()))\
{\
	_currentGameObject->AddComponent(std::make_shared<type>());\
}\

struct TransformParams
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProjection;
	Matrix matWV;
	Matrix matWVP;
	Matrix matViewInv;
};

struct AnimFrameParams
{
	Vec4	scale;
	Vec4	rotation; // Quaternion
	Vec4	translation;
};

extern std::unique_ptr<class Engine> GEngine;

// Utils
std::wstring s2ws(const std::string& s);
std::string ws2s(const std::wstring& s);

template <typename T>
struct array_deleter
{
	void operator()(T const* p)
	{
		delete[] p;
	}
};

#define TOOL

#define PI ((FLOAT)  3.141592654f)
#define RadianToDegree(radian) ((radian) * 180.f / PI)
#define DegreeToRadian(degree) ((degree) * (PI / 180.f))