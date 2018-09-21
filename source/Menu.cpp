/*
	TOpt: An Efficient Quantum Compiler that Reduces the T Count
	Copyright (C) 2018  Luke Heyfron

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

#include "Menu.h"

#include "MenuUtils.h"

#include <cstring>
#include <string>

#include <iostream>
using namespace std;

Menu::Menu(int inMaxMenu, char inQuitKey, const char* inQuitMessage) {
    n_menu_items = 0;
    max_menu_items = inMaxMenu;
    menu_key = new char[max_menu_items];
    quit_key = inQuitKey;
    quit_message = new string(inQuitMessage);
    menu_title = new string("");
    menu_messages = new string*[max_menu_items];
    for(int i = 0; i < max_menu_items; i++) menu_messages[i] = NULL;
    menu_actions = new procPtr[max_menu_items];
}

Menu::~Menu() {
    delete [] menu_key;
    delete quit_message;
    delete menu_title;
    for(int i = 0; i < max_menu_items; i++) {
        if(menu_messages[i]) delete menu_messages[i];
    }
    delete [] menu_messages;
    delete [] menu_actions;
}

void Menu::run() {
    bool exit = false;
    while(!exit) {
        if(menu_title->c_str()) cout << menu_title->c_str() << endl;
        for(int i = 0; i < n_menu_items; i++) {
            cout << menu_key[i] << "\t" << menu_messages[i]->c_str() << endl;
        }
        cout << quit_key << "\t" << quit_message->c_str() << endl;
        char thisChar;
        MenuUtils::getOneChar(thisChar, cin, cout, "Enter selection:");
        if(thisChar==quit_key) {
            exit = true;
        } else {
            bool selected = false;
            for(int i = 0; (i < n_menu_items)&&(!selected); i++) {
                if(thisChar==menu_key[i]) {
                    selected = true;
                    menu_actions[i]();
                }
            }
            if(!selected) {
                cout << "Error! Not a recognised command." << endl;
            }
        }
    }
}

void Menu::setMenuTitle(const char* inMessage) {
    menu_title->assign(inMessage);
}

void Menu::addMenuItem(char inKey, const char* inMessage, procPtr inFunc) {
    menu_key[n_menu_items] = inKey;
    menu_messages[n_menu_items] = new string(inMessage);
    menu_actions[n_menu_items] = inFunc;
    n_menu_items++;
}
