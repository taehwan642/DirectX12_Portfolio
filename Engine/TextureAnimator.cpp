#include "pch.h"
#include "TextureAnimator.h"
#include "Texture.h"
#include "Engine.h"
#include "Resources.h"
#include "Timer.h"

TextureAnimator::TextureAnimator()
{
}

TextureAnimator::~TextureAnimator()
{
}

void TextureAnimator::UpdateAnimation()
{
	_elapsedTime += DELTA_TIME;
	if (_elapsedTime > _interval)
	{
		++_currentIndex;
		_elapsedTime = 0;
		_isAnimationEnd = false;

		if (_currentIndex >= _animationSize)
		{
			// Loop라면 처음부터
			if (_isLoop)
			{
				_currentIndex = 0;
			}
			// 그게 아니라면 그냥 고정
			else
			{
				_currentIndex = _animationSize - 1; // index는 0부터 시작하고, size는 1부터 시작하기 때문
				_isAnimationEnd = true;
			}
		}
	}
}

bool TextureAnimator::LoadAnimation(const std::string& path)
{
	// path로 애니메이션 모음 txt를 불러온다.
	// 한줄씩 읽어서 _tags에 넣는다.
	// 동시에 텍스처를 불러온다.
	
	std::ifstream valueFile;
	std::string finaltxtName = "../Resources/Animation/";
	finaltxtName += path.c_str();
	finaltxtName += ".txt";
	valueFile.open(finaltxtName);
	int num = 0;
	if (valueFile.is_open())
	{
		while (!valueFile.eof())
		{
			char arr[256];
			valueFile.getline(arr, 256);
			std::string line = arr;
			if (!line.empty())
			{
				std::wstring wLine = s2ws(line);
				_tags.push_back(wLine);
				_textures.push_back(GET_SINGLE(Resources)->Load<Texture>(wLine, wLine));
				++num;
			}
		}
	}
	_animationSize = num;
	valueFile.close();

	return false;
}

bool TextureAnimator::LoadAnimation(const std::wstring& key, const std::wstring& path, size_t count)
{
	_animationSize = count + 1;
	if (count <= 1)
	{
		_textures.push_back(GET_SINGLE(Resources)->Load<Texture>(key, path));
	}
	else
	{
		TCHAR finalfilepath[MAX_PATH] = L"";
		for (size_t i = 0; i <= count; ++i)
		{
			wsprintf(finalfilepath, path.c_str(), i);
			_textures.push_back(GET_SINGLE(Resources)->Load<Texture>(key + std::to_wstring(i), finalfilepath));
		}
	}
	return true;
}

std::shared_ptr<Texture> TextureAnimator::GetTexture(int index)
{
	return _textures[index];
}

std::shared_ptr<Texture> TextureAnimator::GetCurrentAnimationTexture()
{
	return _textures[_currentIndex];
}