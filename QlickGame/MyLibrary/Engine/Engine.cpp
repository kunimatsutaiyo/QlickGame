#include "Engine.h"
#include "Texture.h"
#include "Graphics.h"
#include "Input.h"
#include "Sound.h"

bool Engine::InitEngine(HINSTANCE hInstance_, HWND window_handle_)
{
	Texture::GetInstance()->InitTexture();
	if (Graphics::GetInstance()->InitDirectGraphics(window_handle_) == false)
	{
		return false;
	}
	if (Input::GetInstance()->Init(hInstance_, window_handle_) == false)
	{
		return false;
	}
	if (Sound::GetInstance()->InitSound() == false)
	{
		return false;
	}

	return true;
}

void Engine::EndEngine()
{
	Texture::GetInstance()->AllReleaseTexture();
	Graphics::GetInstance()->ReleaseDirectGraphics();
	Input::GetInstance()->Release();
	Sound::GetInstance()->ReleaseSound();

	Texture::DestroyInstance();
	Graphics::DestroyInstance();
	Input::DestroyInstance();
	Sound::DestroyInstance();
}