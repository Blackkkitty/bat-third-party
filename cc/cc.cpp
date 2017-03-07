/*
    cc
    Version     : 2.0
    Auther      : Blackkitty
    Date        : 2017-3-1
    Description : batch developing helper

    batch usage:
        cc              hide cursor
        cc k            get key
        cc K list       get key combine
        cc n            sleep(n)
        cc c n          set cursorSize=n
        cc x y          move cursor to (x,y)
*/

#include<windows.h>
#include<conio.h>
#include<vector>
#include<stdlib.h>

using namespace std;

vector< vector<int> > list;

inline int vkbhit() {
    for (int k = 1; k < 0xFE; k++)
        if (GetAsyncKeyState(k))
            return k;
    return 0;
}

inline void vkclr() {
    for (; kbhit(); getch());
}

void getListenList(char * s, vector< vector<int> > & lst) {
    int flag = 2;
    for (char*p = s; flag;) {
        if (flag == 2) {
            vector<int> tmp;
            lst.push_back(tmp);
            flag = 1;
        }
        char *q = p;
        for (; *p != ' ' && *p != '+' && *p != '\0'; p++);
        if (*p == ' ') flag = 2;
        else if (*p == '\0') flag = 0;
        *p++ = '\0';
        int vk = atoi(q);
        lst[lst.size() - 1].push_back(vk);
    }

}

inline int listen() {
    int i, j;
    for (i = 0; i < list.size(); i++) {
        for (j = 0; j < list[i].size(); j++)
            if (!GetAsyncKeyState(list[i][j]))
                break;
        if (j == list[i].size()) {
            return i + 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (argc == 1) {
        CONSOLE_CURSOR_INFO cci;
        GetConsoleCursorInfo(hOut, &cci);
        cci.bVisible = false;
        SetConsoleCursorInfo(hOut, &cci);
    }
    else if (argc == 2) {
        if (*argv[1] == '?') {
            _cputs("cc\t\thide cursor\ncc k\t\tget key\ncc K list\tget key combine\ncc n\t\tsleep(n)\ncc c n\t\tset cursorSize=n\ncc x y\t\tmove cursor to (x,y)\nby blackkitty\n");
        }
        if (*argv[1] == 'k') {
            int x = 0;
            vkclr();
            x = getch();
            if (kbhit()) x = x * 1000 + getch();
            return x;
        }
        Sleep(atoi(argv[1]));
    }
    else if (argc == 3) {
        if (*argv[1] == 'K') {
            vkclr();
            int ret = 0;
            if (*argv[2] == '_') {
                for (; !(ret = vkbhit()););
                vkclr();
                return ret;
            }
            getListenList(argv[2], list);
            for (; !(ret = listen()););
            vkclr();
            return ret;
        }

        if (*argv[1] == 'c' || *argv[1] == 'C') {
            CONSOLE_CURSOR_INFO curInfo;
            curInfo.dwSize = atoi(argv[2]);
            curInfo.bVisible = TRUE;

            SetConsoleCursorInfo(hOut, &curInfo);
        }
        else {
            SetConsoleCursorPosition(hOut, (COORD) {atoi(argv[1]), atoi(argv[2])});
        }

    }
    else {
        ;
    }
    return 0;
}