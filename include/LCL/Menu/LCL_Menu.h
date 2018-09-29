/*
	TOpt: An Efficient Quantum Compiler that Reduces the T Count
	Copyright (C) 2018  Luke E. Heyfron, Earl T. Campbell
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef LCL_MENU_HEADER
#define LCL_MENU_HEADER

#include <cstring>
#include <string>

#include <iostream>
using namespace std;

class LCL_Menu {
    private:
        typedef void (*procPtr) (void);

        int n_menu_items;
        int max_menu_items;
        char* menu_key;
        char quit_key;
        string* quit_message;
        string* menu_title;
        string** menu_messages;
        procPtr* menu_actions;
    public:
        LCL_Menu(int inMaxMenu, char inQuitKey, const char* inQuitMessage);
        ~LCL_Menu();
        void run();

        void setMenuTitle(const char* inMessage);
        void addMenuItem(char inKey, const char* inMessage, procPtr inFunc);
};

#endif // MENU_HEADER
