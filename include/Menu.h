#ifndef MENU_HEADER
#define MENU_HEADER

#include <cstring>
#include <string>

#include <iostream>
using namespace std;

class Menu {
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
        Menu(int inMaxMenu, char inQuitKey, const char* inQuitMessage);
        ~Menu();
        void run();

        void setMenuTitle(const char* inMessage);
        void addMenuItem(char inKey, const char* inMessage, procPtr inFunc);
};

#endif // MENU_HEADER
