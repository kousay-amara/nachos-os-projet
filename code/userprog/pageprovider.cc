#ifdef CHANGED

#include "pageprovider.h"
#include "system.h"
#include <cstring>

PageProvider::PageProvider()
{
    bitmap = new BitMap(NumPhysPages);
}

PageProvider::~PageProvider()
{
    delete bitmap;
    bitmap = NULL;
}

int PageProvider::GetEmptyPage()
{
    int nbFree = bitmap->NumClear();
    if (nbFree == 0)
        return -1;

    int start = Random() % NumPhysPages;
    int page = -1;
    for (int i = 0; i < NumPhysPages; i++)
    {
        int candidate = (start + i) % NumPhysPages;
        if (!bitmap->Test(candidate))
        {
            bitmap->Mark(candidate);
            page = candidate;
            break;
        }
    }
    if (page == -1)
        return -1;

    memset(&machine->mainMemory[page * PageSize], 0, PageSize);
    return page;
}

void PageProvider::ReleasePage(int pageNum)
{
    if (pageNum < 0 || pageNum >= NumPhysPages)
        return;
    bitmap->Clear(pageNum);
}

int PageProvider::NumAvailPage() const
{
    return bitmap->NumClear();
}

#endif // CHANGED
