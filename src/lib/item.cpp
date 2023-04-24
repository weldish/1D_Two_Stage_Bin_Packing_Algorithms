//
// Created by weldeab solomon on 22/01/2023.
//

#include "../include/item.h"


using namespace onedpacking;

// A constructor for an item
Item::Item(int item_id, int item_size):
        m_item_id(item_id),
        m_item_size(item_size)

{}

// Every item has a unique id by which we refer to it.
int Item::getItemId() const
{

    return m_item_id;
}

// returns the size of the actual item
int Item::getItemSize() const
{
    return m_item_size;
}

// for ordering items in non-decreasing order.
bool onedpacking::item_Comparator_Decreasing(Item* item1, Item* item2)
{
    return (item1->getItemSize() > item2->getItemSize());
}

