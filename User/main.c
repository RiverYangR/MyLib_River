#include "main.h"


int main(void)
{
    BspInit();
    
    while(1)
    {
        ReadKeyStatus();
        Displya_UI();
        ExecuteFun();        
    }
        
    
}





