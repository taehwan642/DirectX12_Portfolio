#pragma once

#include "GameObject.h"
#include "Transform.h"
#include "TransformComponent.h"
#include "Material.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "MeshData.h"
#include "BaseCollider.h"
#include "CubeCollider.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "Camera.h"
#include "Light.h"
#include "Terrain.h"
#include "ParticleSystem.h"
#include "Animator.h"
#include "Scene.h"


#include "MonoBehaviour.h"
#include "TerrainScript.h"
#include "TestCameraScript.h"
#include "TestDragon.h"
#include "GameManagerScript.h"


// Load때 필요한 오브젝트.

struct RTTRColliderValue
{
	RTTRColliderValue() = default;
	RTTRColliderValue(std::shared_ptr<BaseCollider> collider)
	{
		type = collider->_colliderType;
	}

	ColliderType type = ColliderType::Sphere;
};

struct RTTRTextureValue
{
	RTTRTextureValue() = default;
	RTTRTextureValue(std::shared_ptr<Texture> texture)
	{
		tag = ws2s(texture->_name);
	}

	std::string tag = "";
};

struct RTTRShaderValue
{
	RTTRShaderValue() = default;
	RTTRShaderValue(std::shared_ptr<Shader> shader)
	{
		tag = ws2s(shader->_name);
	}

	std::string tag = "";
};

struct RTTRMaterialValue
{
	RTTRMaterialValue() = default;
	RTTRMaterialValue(std::shared_ptr<Material> material)
	{
		for (int i = 0; i < MATERIAL_ARG_COUNT; ++i)
		{
			if (material->_textures[i] != nullptr)
			{
				textureOnValue[i] = true;
				textureValues[i] = RTTRTextureValue(material->_textures[i]);
			}
			else
			{
				textureOnValue[i] = false;
			}

			shaderValue = RTTRShaderValue(material->_shader);
		}
		tag = ws2s(material->_name);
	}

	std::array<bool, MATERIAL_ARG_COUNT> textureOnValue{ false };
	std::array<RTTRTextureValue, MATERIAL_ARG_COUNT> textureValues{ };
	RTTRShaderValue shaderValue ;
	std::string tag = "";
};

struct RTTRMeshValue
{
	RTTRMeshValue() = default;
	RTTRMeshValue(std::shared_ptr<Mesh> mesh)
	{
		tag = ws2s(mesh->_name);
	}

	std::string tag = "";
};

struct RTTRMeshRendererValue
{
	RTTRMeshRendererValue() = default;
	RTTRMeshRendererValue(std::shared_ptr<GameObject> gameObject)
	{
		// meshRendererMaterialNumValue
		meshRendererMaterialNumValue = gameObject->_meshRenderer->_materials.size();

		for (int i = 0; i < meshRendererMaterialNumValue; ++i)
		{
			materialValues.push_back(RTTRMaterialValue(gameObject->_meshRenderer->_materials[i]));
		}

		meshValue = RTTRMeshValue(gameObject->_meshRenderer->_mesh);
	}

	int meshRendererMaterialNumValue = 0;
	std::vector<RTTRMaterialValue> materialValues{};
	RTTRMeshValue meshValue;
};

struct RTTRLightValue
{
	RTTRLightValue() = default;
	RTTRLightValue(std::shared_ptr<Light> light)
	{
		lightType = light->_lightInfo.lightType;
	}

	int lightType = 0;
};

struct RTTRAnimationValue
{
	RTTRAnimationValue() = default;
	RTTRAnimationValue(std::shared_ptr<Mesh> mesh)
	{
		animationClipValue = mesh->_animClips.size();
		boneValue = mesh->_bones.size();

		animClips = mesh->_animClips;
		bones = mesh->_bones;
	}

	int animationClipValue = 0;
	int boneValue = 0;
	// 애니메이션 관련 클립 전부 저장
	std::vector<AnimClipInfo>			animClips{};
	std::vector<BoneInfo>				bones{};
};

struct RTTRMeshDataValue
{
	RTTRMeshDataValue() = default;
	RTTRMeshDataValue(std::shared_ptr<MeshData> data)
	{
		tag = ws2s(data->_name);
		meshRenderValue = data->_meshRenders.size();

		for (int i = 0; i < meshRenderValue; ++i)
		{
			meshValue.push_back(RTTRMeshValue(data->_meshRenders[i].mesh));
			animationValue.push_back(RTTRAnimationValue(data->_meshRenders[i].mesh));
			std::vector<RTTRMaterialValue> values;
			for (int j = 0; j < data->_meshRenders[i].materials.size(); ++j)
			{
				values.push_back(RTTRMaterialValue(data->_meshRenders[i].materials[j]));
			}
			materialValues.push_back(values);
		}

	}

	int meshRenderValue = 0;
	std::vector<std::vector<RTTRMaterialValue>> materialValues{};
	std::vector<RTTRMeshValue> meshValue{};
	std::vector<RTTRAnimationValue> animationValue{};
	std::string tag = "";
};

struct RTTRTerrainValue
{
	RTTRTerrainValue() = default;
	RTTRTerrainValue(std::shared_ptr<Terrain> terrain)
	{
		sizeX = terrain->_sizeX;
		sizeZ = terrain->_sizeZ;
	}

	int sizeX = 0;
	int sizeZ = 0;
};

struct RTTRTransformValue
{
	RTTRTransformValue() = default;
	RTTRTransformValue(std::shared_ptr<TransformComponent> transform)
	{
		if (transform->GetParent().lock() != nullptr)
			parentHashValue = transform->GetParent().lock()->GetGameObject()->GetHash();
	}

	size_t parentHashValue = -1;
};

struct RTTRGameObjectValue
{
	RTTRGameObjectValue() = default;
	RTTRGameObjectValue(std::shared_ptr<GameObject> gameObject)
	{
		tag = ws2s(gameObject->_name);
		hashValue = gameObject->GetHash();

		for (int i = 0; i < gameObject->_components.size(); ++i)
		{
			if (gameObject->_components[i] != nullptr)
				componentOnValue[i] = true;
			else
				componentOnValue[i] = false;
		}

		if (componentOnValue[static_cast<int>(COMPONENT_TYPE::TRANSFORM)] == true)
		{
			transformValue = RTTRTransformValue(gameObject->GetTransform());
		}

		if (componentOnValue[static_cast<int>(COMPONENT_TYPE::MESH_RENDERER)] == true)
		{
			meshRendererValue = RTTRMeshRendererValue(gameObject);
		}

		if (componentOnValue[static_cast<int>(COMPONENT_TYPE::COLLIDER)] == true)
		{
			colliderValue = RTTRColliderValue(gameObject->GetCollider());
		}

		if (componentOnValue[static_cast<int>(COMPONENT_TYPE::LIGHT)] == true)
		{
			lightValue = RTTRLightValue(gameObject->GetLight());
		}

		if (componentOnValue[static_cast<int>(COMPONENT_TYPE::TERRAIN)] == true)
		{
			terrainValue = RTTRTerrainValue(gameObject->GetTerrain());
		}

		for (int i = 0; i < gameObject->_scripts.size(); ++i)
		{
			RTTRMONOLOAD(GameManagerScript)
			RTTRMONOLOAD(TerrainScript)
			RTTRMONOLOAD(TestCameraScript)
			RTTRMONOLOAD(TestDragon)
		}
	}

	std::array<bool, FIXED_COMPONENT_COUNT> componentOnValue{ false };
	std::array<bool, static_cast<int>(MonoBehaviourType::END)> monobehaviourOnValue{ false };
	size_t hashValue;
	RTTRTransformValue transformValue;
	RTTRMeshRendererValue meshRendererValue;
	RTTRColliderValue colliderValue;
	RTTRLightValue lightValue;
	RTTRTerrainValue terrainValue;
	std::string tag;
};

struct RTTRSceneValue
{
	RTTRSceneValue() = default;
	RTTRSceneValue(std::shared_ptr<Scene> scene)
	{
		for (int i = 0; i < scene->_gameObjects.size(); ++i)
		{
			gameObjects.push_back(RTTRGameObjectValue(scene->_gameObjects[i]));
		}

		for (int i = 0; i < scene->_cameras.size(); ++i)
		{
			cameraObjects.push_back(RTTRGameObjectValue(scene->_cameras[i]->GetGameObject()));
		}

		for (int i = 0; i < scene->_lights.size(); ++i)
		{
			lightObjects.push_back(RTTRGameObjectValue(scene->_lights[i]->GetGameObject()));
		}
	}
	
	std::vector<RTTRGameObjectValue> gameObjects;
	std::vector<RTTRGameObjectValue> cameraObjects;
	std::vector<RTTRGameObjectValue> lightObjects;
};

RTTR_REGISTRATION
{
#pragma region VECTOR_MATRIX
	// Vector2
	rttr::registration::class_<Vec2>("Vec2")
						.property("x", &Vec2::x)
						.property("y", &Vec2::y);

	// Vector3
	rttr::registration::class_<Vec3>("Vec3")
						.property("x", &Vec3::x)
						.property("y", &Vec3::y)
						.property("z", &Vec3::z);

	// Vector4
	rttr::registration::class_<Vec4>("Vec4")
						.property("x", &Vec4::x)
						.property("y", &Vec4::y)
						.property("z", &Vec4::z)
						.property("w", &Vec4::w);

	// Matrix
	rttr::registration::class_<Matrix>("Matrix")
						.property("m", &Matrix::m);
#pragma endregion

	// Object
	rttr::registration::class_<Object>("Object") // 이건 클래스 이름
		.property("_objectType", &Object::_objectType)
		.property("_id", &Object::_id);
		

	// GameObject
	rttr::registration::class_<GameObject>("GameObject") // 이건 클래스 이름
		.property("_name", &Object::_name)
		.property("_checkFrustum", &GameObject::_checkFrustum)
		.property("_layerIndex", &GameObject::_layerIndex)
		.property("_static", &GameObject::_static)
		.property("_hash", &GameObject::_hash)
		.property("_transform", &GameObject::_transform)
		.property("_meshRenderer", &GameObject::_meshRenderer)
		.property("_camera", &GameObject::_camera)
		.property("_light", &GameObject::_light)
		.property("_particleSystem", &GameObject::_particleSystem)
		.property("_terrain", &GameObject::_terrain)
		.property("_baseCollider", &GameObject::_baseCollider)
		.property("_animator", &GameObject::_animator);

	// Transform
	rttr::registration::class_<Transform>("Transfrom")
						.constructor<>()
						.property("_position", &Transform::_position)
						.property("_rotation", &Transform::_rotation)
						.property("_scale", &Transform::_scale);

	// TransformComponent
	rttr::registration::class_<TransformComponent>("TransformComponent")
		.constructor<>()
		.property("_localTransform", &TransformComponent::_localTransform)
		.property("_worldTransform", &TransformComponent::_worldTransform);

#pragma region MATERIAL
	// Material
	rttr::registration::class_<Material>("Material")
		.constructor<>()
		.property("_params", &Material::_params)
		.property("_shader", &Material::_shader)
		.property("_textures", &Material::_textures);

	// MaterialParams
	rttr::registration::class_<MaterialParams>("MaterialParams")
		.constructor<>()
		.property("intParams", &MaterialParams::intParams)
		.property("floatParams", &MaterialParams::floatParams)
		.property("texOnParams", &MaterialParams::texOnParams)
		.property("vec2Params", &MaterialParams::vec2Params)
		.property("vec4Params", &MaterialParams::vec4Params)
		.property("matrixParams", &MaterialParams::matrixParams);
#pragma endregion

	// MeshRenderer
	rttr::registration::class_<MeshRenderer>("MeshRenderer")
		.constructor<>()
		.property("_materials", &MeshRenderer::_materials)
		.property("_mesh", &MeshRenderer::_mesh);

#pragma region Shader
	// Shader
	rttr::registration::class_<Shader>("Shader")
		.constructor<>()
		.property("_info", &Shader::_info)
		.property("_arg", &Shader::_arg)
		.property("_name", &Object::_name);

	// ShaderInfo
	rttr::registration::class_<ShaderInfo>("ShaderInfo")
		.constructor<>()
		.property("shaderType", &ShaderInfo::shaderType)
		.property("rasterizerType", &ShaderInfo::rasterizerType)
		.property("depthStencilType", &ShaderInfo::depthStencilType)
		.property("blendType", &ShaderInfo::blendType)
		.property("topology", &ShaderInfo::topology);

	// ShaderArg
	rttr::registration::class_<ShaderArg>("ShaderArg")
		.constructor<>()
		.property("vs", &ShaderArg::vs)
		.property("hs", &ShaderArg::hs)
		.property("ds", &ShaderArg::ds)
		.property("gs", &ShaderArg::gs)
		.property("ps", &ShaderArg::ps)
		.property("cs", &ShaderArg::cs);
#pragma endregion

	// Texture
	rttr::registration::class_<Texture>("Texture")
		.constructor<>()
		.property("_name", &Object::_name);
#pragma region Mesh
	// Mesh
	rttr::registration::class_<Mesh>("Mesh")
		.constructor<>()
		.property("_name", &Object::_name);

	// KeyFrameInfo
	rttr::registration::class_<KeyFrameInfo>("KeyFrameInfo")
		.constructor<>()
		.property("time", &KeyFrameInfo::time)
		.property("frame", &KeyFrameInfo::frame)
		.property("scale", &KeyFrameInfo::scale)
		.property("rotation", &KeyFrameInfo::rotation)
		.property("translate", &KeyFrameInfo::translate);

	// BoneInfo
	rttr::registration::class_<BoneInfo>("BoneInfo")
		.constructor<>()
		.property("boneName", &BoneInfo::boneName)
		.property("parentIdx", &BoneInfo::parentIdx)
		.property("matOffset", &BoneInfo::matOffset);

	// AnimClipInfo
	rttr::registration::class_<AnimClipInfo>("AnimClipInfo")
		.constructor<>()
		.property("animName", &AnimClipInfo::animName)
		.property("frameCount", &AnimClipInfo::frameCount)
		.property("duration", &AnimClipInfo::duration)
		.property("keyFrames", &AnimClipInfo::keyFrames);
#pragma endregion

#pragma region MeshData
	// MeshData
	rttr::registration::class_<MeshData>("MeshData")
		.constructor<>()
		.property("_meshRenders", &MeshData::_meshRenders)
		.property("_name", &Object::_name);

	// MeshData
	rttr::registration::class_<MeshRenderInfo>("MeshRenderInfo")
		.constructor<>()
		.property("mesh", &MeshRenderInfo::mesh)
		.property("materials", &MeshRenderInfo::materials);
#pragma endregion
#pragma region Collider

	// BaseCollider
	rttr::registration::class_<BaseCollider>("BaseCollider")
		.constructor<ColliderType>()
		.property("_mesh", &BaseCollider::_mesh)
		.property("_material", &BaseCollider::_material)
		.property("_colliderMeshGameObject", &BaseCollider::_colliderMeshGameObject);

	// SphereCollider
	rttr::registration::class_<SphereCollider>("SphereCollider")
		.constructor<>()
		.property("_radius", &SphereCollider::_radius)
		.property("_center", &SphereCollider::_center)
		.property("_colliderType", &BaseCollider::_colliderType);

	// CubeCollider
	rttr::registration::class_<CubeCollider>("CubeCollider")
		.constructor<>()
		.property("_extents", &CubeCollider::_extents)
		.property("_center", &CubeCollider::_center)
		.property("_colliderType", &BaseCollider::_colliderType);

	// CubeCollider
	rttr::registration::class_<MeshCollider>("MeshCollider")
		.constructor<>()
		.property("_extents", &MeshCollider::_triCount)
		.property("_center", &BaseCollider::_colliderType);

#pragma endregion

	// Camera
	rttr::registration::class_<Camera>("Camera")
		.constructor<>()
		.property("_type", &Camera::_type)
		.property("_near", &Camera::_near)
		.property("_far", &Camera::_far)
		.property("_fov", &Camera::_fov)
		.property("_scale", &Camera::_scale)
		.property("_width", &Camera::_width)
		.property("_height", &Camera::_height)
		.property("_matView", &Camera::_matView)
		.property("_matProjection", &Camera::_matProjection)
		.property("_cullingMask", &Camera::_cullingMask);

#pragma region Light
	// Light
	rttr::registration::class_<Light>("Light")
		.constructor<>()
		.property("_lightInfo", &Light::_lightInfo)
		.property("_lightIndex", &Light::_lightIndex)
		.property("_volumeMesh", &Light::_volumeMesh)
		.property("_lightMaterial", &Light::_lightMaterial)
		.property("_shadowCamera", &Light::_shadowCamera);

	// LightInfo
	rttr::registration::class_<LightInfo>("LightInfo")
		.constructor<>()
		.property("color", &LightInfo::color)
		.property("position", &LightInfo::position)
		.property("direction", &LightInfo::direction)
		.property("lightType", &LightInfo::lightType)
		.property("range", &LightInfo::range)
		.property("angle", &LightInfo::angle);

	// LightColor
	rttr::registration::class_<LightColor>("LightColor")
		.constructor<>()
		.property("diffuse", &LightColor::diffuse)
		.property("ambient", &LightColor::ambient)
		.property("specular", &LightColor::specular);
#pragma endregion

	// Terrain
	rttr::registration::class_<Terrain>("Terrain")
		.constructor<>()
		.property("_sizeX", &Terrain::_sizeX)
		.property("_sizeZ", &Terrain::_sizeZ)
		.property("_maxTesselation", &Terrain::_maxTesselation)
		.property("_material", &Terrain::_material);

	// ParticleSystem
	rttr::registration::class_<ParticleSystem>("ParticleSystem")
		.constructor<>()
		.property("_maxParticle", &ParticleSystem::_maxParticle)
		.property("_computeMaterial", &ParticleSystem::_computeMaterial)
		.property("_material", &ParticleSystem::_material)
		.property("_mesh", &ParticleSystem::_mesh)
		.property("_createInterval", &ParticleSystem::_createInterval)
		.property("_accTime", &ParticleSystem::_accTime)
		.property("_minLifeTime", &ParticleSystem::_minLifeTime)
		.property("_maxLifeTime", &ParticleSystem::_maxLifeTime)
		.property("_minSpeed", &ParticleSystem::_minSpeed)
		.property("_maxSpeed", &ParticleSystem::_maxSpeed)
		.property("_startScale", &ParticleSystem::_startScale)
		.property("_endScale", &ParticleSystem::_endScale);

#pragma region Enum
	rttr::registration::enumeration<OBJECT_TYPE>("OBJECT_TYPE")
		(
			rttr::value("NONE", OBJECT_TYPE::NONE),
			rttr::value("GAMEOBJECT", OBJECT_TYPE::GAMEOBJECT),
			rttr::value("COMPONENT", OBJECT_TYPE::COMPONENT),
			rttr::value("MATERIAL", OBJECT_TYPE::MATERIAL),
			rttr::value("MESH", OBJECT_TYPE::MESH),
			rttr::value("MESH_DATA", OBJECT_TYPE::MESH_DATA),
			rttr::value("SHADER", OBJECT_TYPE::SHADER),
			rttr::value("TEXTURE", OBJECT_TYPE::TEXTURE),
			rttr::value("END", OBJECT_TYPE::END)
		);

	rttr::registration::enumeration<SHADER_TYPE>("SHADER_TYPE")
		(
			rttr::value("DEFERRED", SHADER_TYPE::DEFERRED),
			rttr::value("FORWARD", SHADER_TYPE::FORWARD),
			rttr::value("LIGHTING", SHADER_TYPE::LIGHTING),
			rttr::value("PARTICLE", SHADER_TYPE::PARTICLE),
			rttr::value("COMPUTE", SHADER_TYPE::COMPUTE),
			rttr::value("SHADOW", SHADER_TYPE::SHADOW)
		);

	rttr::registration::enumeration<RASTERIZER_TYPE>("RASTERIZER_TYPE")
		(
			rttr::value("CULL_NONE", RASTERIZER_TYPE::CULL_NONE),
			rttr::value("CULL_FRONT", RASTERIZER_TYPE::CULL_FRONT),
			rttr::value("CULL_BACK", RASTERIZER_TYPE::CULL_BACK),
			rttr::value("WIREFRAME", RASTERIZER_TYPE::WIREFRAME)
		);

	rttr::registration::enumeration<DEPTH_STENCIL_TYPE>("DEPTH_STENCIL_TYPE")
		(
			rttr::value("LESS", DEPTH_STENCIL_TYPE::LESS),
			rttr::value("LESS_EQUAL", DEPTH_STENCIL_TYPE::LESS_EQUAL),
			rttr::value("GREATER", DEPTH_STENCIL_TYPE::GREATER),
			rttr::value("GREATER_EQUAL", DEPTH_STENCIL_TYPE::GREATER_EQUAL),
			rttr::value("NO_DEPTH_TEST", DEPTH_STENCIL_TYPE::NO_DEPTH_TEST),
			rttr::value("NO_DEPTH_TEST_NO_WRITE", DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE),
			rttr::value("LESS_NO_WRITE", DEPTH_STENCIL_TYPE::LESS_NO_WRITE)
		);

	rttr::registration::enumeration<BLEND_TYPE>("BLEND_TYPE")
		(
			rttr::value("DEFAULT", BLEND_TYPE::DEFAULT),
			rttr::value("ALPHA_BLEND", BLEND_TYPE::ALPHA_BLEND),
			rttr::value("ONE_TO_ONE_BLEND", BLEND_TYPE::ONE_TO_ONE_BLEND),
			rttr::value("END", BLEND_TYPE::END)
		);

	rttr::registration::enumeration<PROJECTION_TYPE>("PROJECTION_TYPE")
		(
			rttr::value("PERSPECTIVE", PROJECTION_TYPE::PERSPECTIVE),
			rttr::value("ORTHOGRAPHIC", PROJECTION_TYPE::ORTHOGRAPHIC)
		);

	rttr::registration::enumeration<ColliderType>("ColliderType")
		(
			rttr::value("Sphere", ColliderType::Sphere),
			rttr::value("Cube", ColliderType::Cube),
			rttr::value("Mesh", ColliderType::Mesh)
		);
#pragma endregion

	// Animator
	rttr::registration::class_<Animator>("Animator")
		.constructor<>()
		.property("_computeMaterial", &Animator::_computeMaterial);

#pragma region RTTRValue
	rttr::registration::class_<RTTRGameObjectValue>("RTTRGameObjectValue")
		.constructor<>()
		.constructor<std::shared_ptr<GameObject>>()
		.property("tag", &RTTRGameObjectValue::tag)
		.property("hashValue", &RTTRGameObjectValue::hashValue)
		.property("componentOnValue", &RTTRGameObjectValue::componentOnValue)
		.property("monobehaviourOnValue", &RTTRGameObjectValue::monobehaviourOnValue)
		.property("transformValue", &RTTRGameObjectValue::transformValue)
		.property("meshRendererValue", &RTTRGameObjectValue::meshRendererValue)
		.property("colliderValue", &RTTRGameObjectValue::colliderValue)
		.property("lightValue", &RTTRGameObjectValue::lightValue)
		.property("terrainValue", &RTTRGameObjectValue::terrainValue);

	rttr::registration::class_<RTTRMaterialValue>("RTTRMaterialValue")
		.constructor<>()
		.constructor<std::shared_ptr<Material>>()
		.property("textureOnValue", &RTTRMaterialValue::textureOnValue)
		.property("textureValues", &RTTRMaterialValue::textureValues)
		.property("shaderValue", &RTTRMaterialValue::shaderValue)
		.property("tag", &RTTRMaterialValue::tag);

	rttr::registration::class_<RTTRMeshValue>("RTTRMeshValue")
		.constructor<>()
		.constructor<std::shared_ptr<Mesh>>()
		.property("tag", &RTTRMeshValue::tag);

	rttr::registration::class_<RTTRTextureValue>("RTTRTextureValue")
		.constructor<>()
		.constructor<std::shared_ptr<Texture>>()
		.property("tag", &RTTRTextureValue::tag);

	rttr::registration::class_<RTTRShaderValue>("RTTRShaderValue")
		.constructor<>()
		.constructor<std::shared_ptr<Shader>>()
		.property("tag", &RTTRShaderValue::tag);

	rttr::registration::class_<RTTRColliderValue>("RTTRColliderValue")
		.constructor<>()
		.constructor<std::shared_ptr<BaseCollider>>()
		.property("type", &RTTRColliderValue::type);

	rttr::registration::class_<RTTRTransformValue>("RTTRTransformValue")
		.constructor<>()
		.constructor<std::shared_ptr<TransformComponent>>()
		.property("parentHashValue", &RTTRTransformValue::parentHashValue);

	rttr::registration::class_<RTTRLightValue>("RTTRLightValue")
		.constructor<>()
		.constructor<std::shared_ptr<Light>>()
		.property("lightType", &RTTRLightValue::lightType);

	rttr::registration::class_<RTTRMeshDataValue>("RTTRMeshDataValue")
		.constructor<>()
		.constructor<std::shared_ptr<MeshData>>()
		.property("tag", &RTTRMeshDataValue::tag)
		.property("meshRenderValue", &RTTRMeshDataValue::meshRenderValue)
		.property("meshValue", &RTTRMeshDataValue::meshValue)
		.property("materialValues", &RTTRMeshDataValue::materialValues)
		.property("animationValue", &RTTRMeshDataValue::animationValue);

	rttr::registration::class_<RTTRAnimationValue>("RTTRAnimationValue")
		.constructor<>()
		.constructor<std::shared_ptr<Mesh>>()
		.property("boneValue", &RTTRAnimationValue::boneValue)
		.property("animationClipValue", &RTTRAnimationValue::animationClipValue)
		.property("bones", &RTTRAnimationValue::bones)
		.property("animClips", &RTTRAnimationValue::animClips);

	rttr::registration::class_<RTTRTerrainValue>("RTTRTerrainValue")
		.constructor<>()
		.constructor<std::shared_ptr<Terrain>>()
		.property("sizeX", &RTTRTerrainValue::sizeX)
		.property("sizeZ", &RTTRTerrainValue::sizeZ);

	rttr::registration::class_<RTTRMeshRendererValue>("RTTRMeshRendererValue")
		.constructor<>()
		.constructor<std::shared_ptr<GameObject>>()
		.property("materialValues", &RTTRMeshRendererValue::materialValues)
		.property("meshRendererMaterialNumValue", &RTTRMeshRendererValue::meshRendererMaterialNumValue)
		.property("meshValue", &RTTRMeshRendererValue::meshValue);

	rttr::registration::class_<RTTRSceneValue>("RTTRSceneValue")
		.constructor<>()
		.constructor<std::shared_ptr<Scene>>()
		.property("gameObjects", &RTTRSceneValue::gameObjects)
		.property("cameraObjects", &RTTRSceneValue::cameraObjects)
		.property("lightObjects", &RTTRSceneValue::lightObjects);
	
#pragma endregion

	// Scene
	rttr::registration::class_<Scene>("Scene")
		.constructor<>()
		.property("_gameObjects", &Scene::_gameObjects)
		.property("_cameras", &Scene::_cameras)
		.property("_lights", &Scene::_lights);

#pragma region Monobehaviour
	
	RTTRMONOREGISTER(GameManagerScript);

	RTTRMONOREGISTER(TerrainScript);

	RTTRMONOREGISTER(TestCameraScript)
		.property("_speed", &TestCameraScript::_speed);

	RTTRMONOREGISTER(TestDragon);
#pragma endregion
}