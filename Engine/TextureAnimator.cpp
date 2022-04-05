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
	static float elapsedTime = 0.f;
	elapsedTime += DELTA_TIME;
	if (elapsedTime > _interval)
	{
		++_currentIndex;
		elapsedTime = 0;

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
			}
		}
	}
}

bool TextureAnimator::LoadAnimation(const std::wstring& path)
{
	// path로 애니메이션 모음 txt를 불러온다.
	// 한줄씩 읽어서 _tags에 넣는다.
	// 동시에 텍스처를 불러온다.
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