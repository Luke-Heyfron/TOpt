#include "LCL/Menu/LCL_Menu.h"

#include "LCL/Menu/LCL_MenuUtils.h"

#include <cstring>
#include <string>

#include <iostream>
using namespace std;

LCL_Menu::LCL_Menu(int inMaxMenu, char inQuitKey, const char* inQuitMessage) {
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

LCL_Menu::~LCL_Menu() {
    delete [] menu_key;
    delete quit_message;
    delete menu_title;
    for(int i = 0; i < max_menu_items; i++) {
        if(menu_messages[i]) delete menu_messages[i];
    }
    delete [] menu_messages;
    delete [] menu_actions;
}

void LCL_Menu::run() {
    bool exit = false;
    while(!exit) {
        if(menu_title->c_str()) cout << menu_title->c_str() << endl;
        for(int i = 0; i < n_menu_items; i++) {
            cout << "\t" << menu_key[i] << "\t" << menu_messages[i]->c_str() << endl;
        }
        cout << "\t" << quit_key << "\t" << quit_message->c_str() << endl;
        char thisChar;
        LCL_MenuUtils::getOneChar(thisChar, cin, cout, "Enter selection:");
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

void LCL_Menu::setMenuTitle(const char* inMessage) {
    menu_title->assign(inMessage);
}

void LCL_Menu::addMenuItem(char inKey, const char* inMessage, procPtr inFunc) {
    menu_key[n_menu_items] = inKey;
    menu_messages[n_menu_items] = new string(inMessage);
    menu_actions[n_menu_items] = inFunc;
    n_menu_items++;
}
