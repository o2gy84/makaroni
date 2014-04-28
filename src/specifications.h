#ifndef SPECIFICATIONS_H
#define SPECIFICATIONS_H

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

namespace Holy
{

    static const QString kMimeTypeName = "holy_makaronen";

    static const QString kFontFamily = "curier";

    static const int kInitMakaronPositionX = 50;     // x - coord of left makaronen
    static const int kInitMakaronPositionY = 80;     // y - coord of left makaronen
    static const int kMakaronSpacing = 10;           // distance between makaronen
    static const int kMinDistance = 40;              // distance need to be dragged to drop makaron another place

    static const int kPastaWidth = 30;

    inline string intToString(int _val)
    {
        stringstream ss;
        string s;
        ss<<_val;
        ss>>s;
        return s;
    }
}


#endif // SPECIFICATIONS_H
