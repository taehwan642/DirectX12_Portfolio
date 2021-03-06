#include "pch.h"
#include "EffectManagerScript.h"
#include "Effect.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Engine.h"
#include "Resources.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectPool.h"

EffectManagerScript::~EffectManagerScript()
{
}

void EffectManagerScript::LateUpdate()
{
	
}

void EffectManagerScript::SpawnEffect(const std::string& path, const Vec3& worldPosition)
{
	if (std::shared_ptr<GameObject> poolObj = GET_SINGLE(ObjectPool)->GetPoolObject("Effect"); poolObj != nullptr)
	{
		poolObj->GetComponent<Effect>()->Spawn(path, worldPosition);
	}
	else
	{
		std::shared_ptr<GameObject> object = std::make_shared<GameObject>();
		object->AddComponent(std::make_shared<TransformComponent>());
		object->GetTransform()->SetWorldScale(Vec3::One);

		std::shared_ptr<MeshRenderer> mr = std::make_shared<MeshRenderer>();
		mr->SetMaterial(GET_SINGLE(Resources)->Get<Material>(L"Effect")->Clone());
		mr->SetMesh(GET_SINGLE(Resources)->LoadRectangleMesh());
		object->AddComponent(mr);

		std::shared_ptr<Effect> effect = std::make_shared<Effect>();
		object->AddComponent(effect);
		effect->Spawn(path, worldPosition);
		effect->_camera = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"Main_Camera");

		object->SetName(L"Child" + std::to_wstring(GetTransform()->GetChildCount()));
		object->GenerateHash();
		object->GetTransform()->SetParent(GetTransform());

		GET_SINGLE(ObjectPool)->AddPoolObject("Effect", object);

		GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(object);
	}
}

#ifdef TOOL
void EffectManagerScript::DragAndDrop(size_t hash)
{
}
#endif