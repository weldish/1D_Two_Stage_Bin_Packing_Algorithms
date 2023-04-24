//
//  Created by weldeab solomon on 23/01/2023.
//

#include "../include/bin.h"

using namespace std;
using namespace onedpacking;

// A constructor for a bin
Bin::Bin(int bin_id, int max_bin_capacity):
        m_bin_id(bin_id),
        m_max_bin_capacity(max_bin_capacity),
        m_remaining_bin_capacity(max_bin_capacity)
{}

// every bin has unique id by which we refer to it.
const int Bin::getBinId() const
{
    return m_bin_id;
}

// get the capacity of the bin (this is the size when it is empty)
const int Bin::getMaxBinCapcity() const
{
    return m_max_bin_capacity;
}

// get the residual capacity of the bin
const int Bin::getRemBinCapacity() const
{
    return m_remaining_bin_capacity;
}

// for adding an item to the bin

void Bin::addItem(Item* item)
{
    // by now we know that the item fits in the bin.
    // adding the item to this bin
    m_allocated_items.push_back(item->getItemId());
    // get the size of the item to be allocated.
    const int item_size = item->getItemSize();
    // subtract the size of the item from the residual capacity of this bin.
    m_remaining_bin_capacity = m_remaining_bin_capacity-item_size;

}

// checking if the item fits

bool Bin::doesItemFitToBin(int item_size) const
{
    if (m_remaining_bin_capacity < item_size)
    {
        return false;
    }
    return true;
}

//ordering bins in the increasing order of their residual bin capacity
bool onedpacking::bin_Comparator_Rem_Cap_Increasing(Bin* bin1, Bin* bin2)
{

    return(bin1->getRemBinCapacity() < bin2->getRemBinCapacity());

}

// ordering bins in the decreasing order of their their residual bin capacity.

bool onedpacking::bin_Comparator_Rem_Cap_Decreasing(Bin* bin1, Bin* bin2)
{

    return(bin1->getRemBinCapacity() > bin2->getRemBinCapacity());

}




