#include <string>
#include <iostream>

#include "sticker.h"
#include "painter.h"

using std::string;
using std::cout;
using std::endl;


int main(int argc, char* arg[]) {
    // Create sample sticker object
    Sticker sticker("LuceSolara");
    sticker.setLogo("LuceSolara");
    sticker.setArticle("078/5PL green");
    sticker.setType("E14 max 60W");
    sticker.setNumber(5);
    sticker.setVoltage("220V/~50Hz");
    sticker.setIP("IP20");
    sticker.setFlame(3);
    sticker.setLedType(1);
    sticker.setProtection(19);

    if (socle_painter(sticker)) {
        cout << "Fail to paint " << sticker.toString() << endl;
    } else {
        cout << "Paint " << sticker.toString() << " OK" << endl;
    }

    return 0;
}