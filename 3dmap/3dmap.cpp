

#include <iostream>
#include <Windows.h>
#include <chrono>
using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 40.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFov = 3.14 / 4.0;
float fDepth = 16.0f;
int main()
{
    wchar_t* screen = new wchar_t[nScreenHeight*nScreenWidth];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    wstring map;

    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#....######....#";
    map += L"#....#.........#";
    map += L"#....#....#....#";
    map += L"#....#....#....#";
    map += L"#....#....#....#";
    map += L"#....######....#";
    map += L"#....######....#";
    map += L"#..............#";
    map += L"#..............#"; 
    map += L"#..............#";
    map += L"################";



    auto tp1 = chrono::system_clock::now();
    auto tp2 = chrono::system_clock::now();

    while (true) {

        tp2 = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();


        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)  {
            fPlayerA -= (1.0f) * fElapsedTime;

        }
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
            fPlayerA += (1.0f) * fElapsedTime;

        }
        if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
            fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
        }
        if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
            fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;

        }

        for (int x = 0; x < nScreenWidth; x++) {
            
            float fRayAngle = (fPlayerA - fFov / 2.0f)
                                + ((float)x / (float)nScreenWidth)
                                * fFov;

            float fDistanceToWall = 0;
            bool bHitWall = false;

            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            while (!bHitWall && fDistanceToWall < fDepth) {

                fDistanceToWall += 0.1f;

                int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);
            
            
                if (nTestX < 0 || nTestY >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
                    bHitWall = true;
                    fDistanceToWall = fDepth;
                }
                else {
                
                    if (map[nTestY * nMapWidth + nTestX] == '#') {
                        
                        bHitWall = true;

                    }
                
                }
            }
       
            int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
            int nFloor = nScreenHeight - nCeiling;
            
            short nShade = ' ';

            if (fDistanceToWall <= fDepth / 4.0f) nShade = 0x2588;
            else if (fDistanceToWall <= fDepth / 3.0f) nShade = 0x2593;
            else if (fDistanceToWall <= fDepth / 2.0f) nShade = 0x2592;
            else if (fDistanceToWall <= fDepth / 1.0f) nShade = 0x2591;
            else nShade = ' ';

            for (int y = 0; y < nScreenHeight; y++) {
            
                if (y < nCeiling) {
                    screen[y * nScreenWidth + x] = ' ';
                }
                else if(y > nCeiling && y <= nFloor) {
                    screen[y * nScreenWidth + x] = nShade;
                }
                else {
                    screen[y * nScreenWidth + x] = ' ';
                }
            }

        }
        

        screen[nScreenHeight * nScreenWidth - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }


    //std::cout << "Hello World!\n";
}

