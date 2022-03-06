#pragma once

#include "to_json.h"
#include "from_json.h"
#include <fstream>
#include "Transform.h"

class JsonManager
{
	DECLARE_SINGLE(JsonManager);

public:
	// Ctrl Shift B 코드 바뀔때마다.
	void Save(const std::string& path, std::shared_ptr<GameObject> object)
	{
		object->ConvertData(ConvertType::Save);

		std::string jsonString = io::to_json(*object);
		std::ofstream ost(path + ".json");
		ost << jsonString;
	}

	void Load(const std::string& path, std::shared_ptr<GameObject> object)
	{
		std::ifstream ost(path + ".json");
		std::string jsonString;
		
		std::ifstream readFile;             //읽을 목적의 파일 선언
		readFile.open(path + ".json");    //파일 열기
		if (readFile.is_open())    //파일이 열렸는지 확인
		{
			while (!readFile.eof())    //파일 끝까지 읽었는지 확인
			{
				char arr[256];
				readFile.getline(arr, 256);    //한줄씩 읽어오기
				jsonString += arr;
			}
		}
		readFile.close();    //파일 닫기

		io::from_json(jsonString, object);

	}

private:

};

