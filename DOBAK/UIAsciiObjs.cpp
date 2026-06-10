#include "AsciiArt.h"
#include "Console.h"
#include "GameState.h"
#include "UIAsciiObjs.h"
void AsciiInit(UIAsciiObjs& objs)
{
    objs.six =
    {
        "            @@@@,       ",
        "          :@@ ~@@       ",
        "         ~@@   @@       ",
        "         @@-  @@@       ",
        "        ;@@             ",
        "        @@@             ",
        "        @@$@@@$         ",
        "       -@@@=-@@@:       ",
        "       @@@@  .@@@       ",
        "       @@@:   @@@       ",
        "       @@@~   @@@       ",
        "       !@@:  .@@@       ",
        "        @@!  @@@,       ",
        "        @@@  @@@        ",
        "         @@@@@$         "
    };
    objs.seven =
    {
        "                       ",
        "       @@@@@@@@@       ",
        "       @@~...:@=       ",
        "        @     @@       ",
        "             @@*       ",
        "          =@@          ",
        "          @@@          ",
        "         @@@           ",
        "        -@@            ",
        "        @@@            ",
        "       .@@-            ",
        "        @@@.           ",
        "        @@@            ",
        "         @@-           ",
        "                       "
    };
    for (string& line : objs.six)
        line.resize(WIDTH, ' ');
    for (string& line : objs.seven)
        line.resize(WIDTH, ' ');
}

void AsciiUpdate(UIAsciiObjs& objs)
{
    for (string& line : objs.six)
        std::rotate(line.begin(), line.begin() + 1, line.end());
    for (string& line : objs.seven)
        std::rotate(line.rbegin(), line.rbegin() + 1, line.rend());
}

void AsciiRender(const UIAsciiObjs& objs)
{
    //그려줄거야.
    int six = (int)objs.six.size();
    int seven = (int)objs.seven.size();
    COORD res = GetConsoleResolution();

    SetColor(Color::LIGHT_YELLOW);
    for (int i = 0; i < six; ++i)
    {
        GotoXY(0, i);
        cout << objs.six[i];
    }

    SetColor(Color::CYAN);
    for (int i = 0; i < seven; ++i)
    {
        GotoXY(0, seven + i );
        cout << objs.seven[i];
    }
    SetColor();
}