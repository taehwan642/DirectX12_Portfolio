#include "pch.h"
#include "RenderTargetScript.h"
#include "Input.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "RaycastManager.h"
#include "ImGuiManager.h"
#include "Engine.h"
#include "TransformComponent.h"

RenderTargetScript::~RenderTargetScript()
{

}

void RenderTargetScript::LateUpdate()
{
	if (INPUT->GetButtonDown(KEY_TYPE::F1))
	{
		for (int i = 0; i < GetTransform()->GetChildCount(); ++i)
		{
			std::shared_ptr<GameObject> childObject = GetTransform()->GetChild(i)->GetGameObject();
			childObject->SetActive(!childObject->GetActive());
		}
	}
}