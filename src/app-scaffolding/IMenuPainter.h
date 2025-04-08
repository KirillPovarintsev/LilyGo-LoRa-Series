#ifndef IMENUPAINTER_H
#define IMENUPAINTER_H

struct IMenuPainter
{
    virtual void paintMenu(int currentItemIndex) = 0;
    virtual void paintItem(int index, const char* text, bool selected) = 0;
};

#endif //IMENUPAINTER_H