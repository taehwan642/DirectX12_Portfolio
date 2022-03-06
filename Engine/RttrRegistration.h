#pragma once

#include "GameObject.h"
#include "Transform.h"
#include "Material.h"
#include "MeshRenderer.h"

RTTR_REGISTRATION
{
	// GameObject
	rttr::registration::class_<GameObject>("GameObject") // 이건 클래스 이름
							.property("_transform", &GameObject::_transform)
							.property("_meshRenderer", &GameObject::_meshRenderer);

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

	// Transform
	rttr::registration::class_<Transform>("Transfrom")
						.constructor<>()
						.property("_localPosition", &Transform::_localPosition)
						.property("_localRotation", &Transform::_localRotation)
						.property("_localScale", &Transform::_localScale);

	// Material
	rttr::registration::class_<Material>("Material")
		.constructor<>()
		.property("_params", &Material::_params);

	// MaterialParams
	rttr::registration::class_<MaterialParams>("MaterialParams")
		.constructor<>()
		.property("intParams", &MaterialParams::intParams)
		.property("floatParams", &MaterialParams::floatParams)
		.property("texOnParams", &MaterialParams::texOnParams)
		.property("vec2Params", &MaterialParams::vec2Params)
		.property("vec4Params", &MaterialParams::vec4Params)
		.property("matrixParams", &MaterialParams::matrixParams);

	// MeshRenderer
	rttr::registration::class_<MeshRenderer>("MeshRenderer")
		.constructor<>()
		.property("_materials", &MeshRenderer::_materials);
}