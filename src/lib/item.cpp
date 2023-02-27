//
// Created by weldeab solomon on 22/01/2023.
//

#include "item.h"


using namespace onedpacking;


Item::Item(int item_id, int item_size):
        m_item_id(item_id),
        m_item_size(item_size)

{}


int Item::getItemId() const
{

    return m_item_id;
}

// returns the size of the actual item
int Item::getItemSize() const
{
    return m_item_size;
}


bool onedpacking::item_Comparator_Decreasing(Item* item1, Item* item2)
{
    return (item1->getItemSize() > item2->getItemSize());
}

