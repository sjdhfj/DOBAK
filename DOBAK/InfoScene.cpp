#include "InfoScene.h"
#include "Console.h"
#include "SoundManager.h"
constexpr float step = 0.01f;

void InfoScene::Init(GameState& state)
{
    SetConsoleSize(WIDTH, HEIGHT);
    _curTab = InfoTab::INFO;
    _cursor = 0;
}

void InfoScene::Update(GameState& state)
{
    if (GetKeyDown(VK_LEFT) && _curTab == InfoTab::SOUNDTAB)
    {
        _curTab = InfoTab::INFO;
        _cursor = 0;
        system("cls");
        return;
    }
    if (GetKeyDown(VK_RIGHT) && _curTab == InfoTab::INFO)
    {
        _curTab = InfoTab::SOUNDTAB;
        _cursor = 0;
        system("cls");
        return;
    }

    int size = (_curTab == InfoTab::SOUNDTAB) ? 3 : 0;
    if (size > 0)
    {
        if (GetKeyDown(VK_UP))
            _cursor = std::max(0, _cursor - 1);
        if (GetKeyDown(VK_DOWN))
            _cursor = std::min(size - 1, _cursor + 1);
    }

    if (_curTab == InfoTab::SOUNDTAB)
    {
        float delta = 0.0f;
        if (GetKeyDown('A'))
            delta = -step * 10;
        else if (GetKeyDown('D'))
            delta = step * 10;
        else if (GetKey('A'))
            delta = -step;
        else if (GetKey('D'))
            delta = step;

        if (delta != 0.0f)
        {
            float volume = (_cursor == 0) ? SOUND->GetMasterVolume() : (_cursor == 1) ? SOUND->GetBGMVolume() : SOUND->GetSFXVolume();

            volume = std::max(0.0f, std::min(1.0f, volume + delta));

            if (_cursor == 0)
                SOUND->SetMasterVolume(volume);
            else if (_cursor == 1) 
                SOUND->SetBGMVolume(volume);
            else
                SOUND->SetSFXVolume(volume);
        }

        if (GetKeyDown('M'))
        {
            bool muted = (_cursor == 0) ? SOUND->GetMasterMute() : (_cursor == 1) ? SOUND->GetBGMMute() : SOUND->GetSFXMute();

            if (_cursor == 0)
                SOUND->SetMasterMute(!muted);
            else if (_cursor == 1)
                SOUND->SetBGMMute(!muted);
            else
                SOUND->SetSFXMute(!muted);
        }
    }

    if (GetKeyDown(VK_ESCAPE))
        SceneManager::GetInst()->ChangeScene("TitleScene", state);
}

void InfoScene::Render(const GameState& state)
{
    COORD res = GetConsoleResolution();
    int cx = res.X / 2;
    int cy = res.Y / 3;

    GotoXY(cx - 8, cy - 2);
    SetColor(_curTab == InfoTab::INFO ? Color::WHITE : Color::GRAY);
    cout << (_curTab == InfoTab::INFO ? "> " : "  ") << "정보";
    cout << "    ";
    SetColor(_curTab == InfoTab::SOUNDTAB ? Color::WHITE : Color::GRAY);
    cout << (_curTab == InfoTab::SOUNDTAB ? "> " : "  ") << "사운드";
    SetColor();

    if (_curTab == InfoTab::INFO)
        RenderInfoTab(cx, cy);
    else
        RenderSoundTab(cx, cy);

    cout << std::endl;
}

void InfoScene::RenderInfoTab(int cx, int cy)
{
    const string infoLabels[] =
    {
        "[ 조작 방법 ]",
        "SPACE  : 룰렛 돌리기",
        "B      : 상점가기",
        "ESC 로 돌아가기"
    };
    for (int i = 0; i < 4; ++i)
    {
        GotoXY(cx - 6, cy + i);
        if (i == 0)
            SetColor(Color::LIGHT_YELLOW);
        else if (i == 3)
            SetColor(Color::LIGHT_GRAY);
        else
            SetColor();
        cout << infoLabels[i];
    }
}

void InfoScene::RenderSoundTab(int cx, int cy)
{
    int x = cx - 12;

    SetColor(_cursor == 0 ? Color::WHITE : Color::LIGHT_GRAY);
    GotoXY(x, cy);
    cout << (_cursor == 0 ? "> " : "  ");
    DrawBar(x + 2, cy, "전체  ", (int)(SOUND->GetMasterVolume() * 100), 100, 20);
    SetColor(Color::LIGHT_RED);
    cout << (SOUND->GetMasterMute() ? "  [MUTE]" : "        ");

    SetColor(_cursor == 1 ? Color::WHITE : Color::LIGHT_GRAY);
    GotoXY(x, cy + 2);
    cout << (_cursor == 1 ? "> " : "  ");
    DrawBar(x + 2, cy + 2, "BGM   ", (int)(SOUND->GetBGMVolume() * 100), 100, 20);
    SetColor(Color::LIGHT_RED);
    cout << (SOUND->GetBGMMute() ? "  [MUTE]" : "        ");

    SetColor(_cursor == 2 ? Color::WHITE : Color::LIGHT_GRAY);
    GotoXY(x, cy + 4);
    cout << (_cursor == 2 ? "> " : "  ");
    DrawBar(x + 2, cy + 4, "효과음", (int)(SOUND->GetSFXVolume() * 100), 100, 20);
    SetColor(Color::LIGHT_RED);
    cout << (SOUND->GetSFXMute() ? "  [MUTE]" : "        ");

    GotoXY(x, cy + 7);
    SetColor(Color::LIGHT_GRAY);
    cout << "[A/D] 볼륨 조절   [M] 음소거   [방향키] 항목 선택";
    SetColor();
}