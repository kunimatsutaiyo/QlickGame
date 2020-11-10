#include <Windows.h>

#include "MyLibrary/Engine/Window.h"
#include "MyLibrary/Engine/Engine.h"
#include "MyLibrary/Engine/Input.h"
#include "MyLibrary/Engine/Graphics.h"
#include "MyLibrary/Engine/Texture.h"
#include "MyLibrary/Engine/Sound.h"

#include "MyLibrary/Object/EnemyManager.h"
#include "MyLibrary/Collision/MouseAndRect.h"

int APIENTRY WinMain(
	HINSTANCE	hInstance_,		// �C���X�^���X�n���h��
	HINSTANCE	hPrevInstance_,	// ���d�l(NULL����ɓ����Ă���̂Ŗ���)
	LPSTR		lpCmpLine_,		// �R�}���h���C��
	INT			nCmdShow_		// �\�����
)
{
	Window window;
	Engine engine;
	HWND window_handle = window.MakeWindow(hInstance_, 1920, 1080, "�|�`�|�`�Q�[��");
	engine.InitEngine(hInstance_, window_handle);



	Texture::GetInstance()->LoadTexture("Res/free_enemy.jpg", "enemy");
	Graphics* gp = Graphics::GetInstance();

	EnemyManager enemy_manager(2);

	MouseAndRect mouse_and_rect;



	// ���C�����[�v
	while (true)
	{
		// ���b�Z�[�W�\����
		MSG msg;

		// ���b�Z�[�W��M
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// WM_QUIT�ŏI��
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else {
				// ���b�Z�[�W�|��
				TranslateMessage(&msg);
				// �v���V�[�W���Ƀ��b�Z�[�W���M
				DispatchMessage(&msg);
			}
		}
		else {
			// Input�̏���
			// esc�L�[�ŏI��
			Input::GetInstance()->Update();
			if (Input::GetInstance()->GetKey(KEY_TYPE::ESCAPE_KEY))
			{
				break;
			}



			// game���� //
			// �G�̍X�V
			enemy_manager.Update();

			// �����蔻��
			if (Input::GetInstance()->OnMouseDown(MouseButton::LEFT))
			{
				D3DXVECTOR2 mouse_pos = Input::GetInstance()->GetMousePos();
				std::vector<Enemy*> vec_enmey = enemy_manager.GetEnemy();

				for (size_t i = 0; i < vec_enmey.size(); i++)
				{
					if (mouse_and_rect.Judgment(mouse_pos, vec_enmey[i]->GetPos(), vec_enmey[i]->GetSize()))
					{
						enemy_manager.PartDelete(i);
					}
				}
			}



			// draw���� //
			// �`��J�n
			if (gp->StartDraw() == true)
			{
				enemy_manager.Draw();
			}
			// �`��I��
			gp->EndDraw();
		}
	}

	// �G���W���I��
	engine.EndEngine();

	//return msg.wParam;
	return 0;
}