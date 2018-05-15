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
