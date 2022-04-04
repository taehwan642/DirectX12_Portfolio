#include "pch.h"
#include "TestCameraScript.h"
#include "TransformComponent.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "RaycastManager.h"
#include "ImGuiManager.h"
#include "Engine.h"
#include "TerrainScript.h"

TestCameraScript::~TestCameraScript()
{
}

void TestCameraScript::LateUpdate()
{
	Vec3 pos = GetTransform()->GetWorldPosition();

	if (INPUT->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetWorldTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetWorldTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetWorldTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetWorldTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		Vec3 rotation = GetTransform()->GetWorldRotation();
		rotation.x += DELTA_TIME * 0.5f;
		GetTransform()->SetWorldRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::E))
	{
		Vec3 rotation = GetTransform()->GetWorldRotation();
		rotation.x -= DELTA_TIME * 0.5f;
		GetTransform()->SetWorldRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::Z))
	{
		Vec3 rotation = GetTransform()->GetWorldRotation();
		rotation.y += DELTA_TIME * 0.5f;
		GetTransform()->SetWorldRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::C))
	{
		Vec3 rotation = GetTransform()->GetWorldRotation();
		rotation.y -= DELTA_TIME * 0.5f;
		GetTransform()->SetWorldRotation(rotation);
	}

	if (INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
	{
		const POINT& pos = INPUT->GetMousePos();
		std::shared_ptr<GameObject> pickedObject = GET_SINGLE(RaycastManager)->Pick(pos.x, pos.y);
		if (pickedObject != nullptr)
		{
#ifdef TOOL
			IMGUIMANAGER->_currentGameObject = pickedObject;
#endif
			// TODO : template을 통한 GetComponent 또는 GetScript 구현해서 받아오기. 일단 지금은 이름을 통해서 받아올거다.
			std::shared_ptr<TerrainScript> tscript = pickedObject->GetComponent<TerrainScript>();
			if (tscript != nullptr)
			{
				tscript->Picked(GET_SINGLE(RaycastManager)->rayOrigin, GET_SINGLE(RaycastManager)->rayDir, GET_SINGLE(RaycastManager)->distance);
			}
		}
		else
		{
#ifdef TOOL
			IMGUIMANAGER->_currentGameObject = nullptr;
#endif
		}
	}

	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		const POINT& pos = INPUT->GetMousePos();
		pressedPos.x = pos.x;
		pressedPos.y = pos.y;
	}

	if (INPUT->GetButton(KEY_TYPE::LBUTTON))
	{
		const POINT& pos = INPUT->GetMousePos();
		Vec3 rotation = GetTransform()->GetWorldRotation();
		
		float dx = (pos.x - pressedPos.x);
		float dy = (pos.y - pressedPos.y);

		rotation.y += dx * DELTA_TIME * 30.f;
		rotation.x += dy * DELTA_TIME * 30.f;

		pressedPos.x = pos.x;
		pressedPos.y = pos.y;

		GetTransform()->SetWorldRotation(rotation);
	}

	GetTransform()->SetWorldPosition(pos);
}