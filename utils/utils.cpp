#include "utils.h"

QColor utils::colorByClass(uint8_t cellClass)
{
    switch (cellClass) {
    case 0:
        return Qt::red;
    case 1:
        return Qt::green;
    case 2:
        return Qt::blue;
    case 3:
        return QColor(139, 0, 0);
    case 4:
        return QColor(173, 255, 47);
    case 5:
        return QColor(255, 255, 0);
    case 6:
        return QColor(0, 255, 255);
    case 7:
        return QColor(70, 130, 180);
    case 8:
        return QColor(165, 42, 42);
    case 9:
        return QColor(222, 184, 135);
    case 10:
        return QColor(25, 25, 112);
    case 11:
        return QColor(173, 255, 47);
    case 12:
        return QColor(0, 100, 0);
    case 13:
        return QColor(128, 128, 0);
    default:
        return Qt::black;
    }
}
