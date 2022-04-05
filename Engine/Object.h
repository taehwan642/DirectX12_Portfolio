#pragma once

class Object
{
public:
	Object(OBJECT_TYPE type);
	virtual ~Object();

	OBJECT_TYPE GetType() { return _objectType; }

	void SetName(const std::wstring& name) { _name = name; }
	const std::wstring& GetName() { return _name; }

	uint32 GetID() { return _id; }

protected:
	friend class Resources;
	virtual void Load(const std::wstring& path) { }
	virtual void Save(const std::wstring& path) { }


protected:
	OBJECT_TYPE _objectType = OBJECT_TYPE::NONE;
	std::wstring _name;

protected:
	uint32 _id = 0;

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

