#ifdef CHANGED
#pragma once

#include "bitmap.h"

class PageProvider
{
public:
    PageProvider();
    ~PageProvider();

    int GetEmptyPage(); // return physical page number or -1
    void ReleasePage(int pageNum);
    int NumAvailPage() const;

private:
    BitMap *bitmap;
};

extern PageProvider *pageProvider;

#endif // CHANGED
