//
//  Created by weldeab solomon on 23/01/2023.
//

#include "bin.h"
#include <iostream>
#include <sstream>

using namespace std;
using namespace onedpacking;

Bin::Bin(int bin_id, int max_bin_capacity):
        m_bin_id(bin_id),
        m_max_bin_capacity(max_bin_capacity),
        m_remaining_bin_capacity(max_bin_capacity)
{}


const int Bin::getBinId() const
{
    return m_bin_id;
}

// get maximum bin capacity
const int Bin::getMaxBinCapcity() const
{
    return m_max_bin_capacity;
}

// get remaining bin capacity
const int Bin::getRemBinCapacity() const
{
    return m_remaining_bin_capacity;
}

// returns a list of elements allocated to the bin
const std::vector<int>& Bin::getAllocatedList() const
{
    return m_allocated_items;
}

// adding items to the bin

void Bin::addItem(Item* item)
{
    // adding the item to this bin
    m_allocated_items.push_back(item->getItemId());
    // reading the the size of the item to be allocated.
    const int item_size = item->getItemSize();
    // substructing the reamining bin capaciy of this bin by the size of the alloacted item.
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

//w ordering bins by their residual bin capacity in an increading order
bool onedpacking::bin_Comparator_Rem_Cap_Increasing(Bin* bin1, Bin* bin2)
{

    return(bin1->getRemBinCapacity() < bin2->getRemBinCapacity());

}

//w ordering bins by their residual bin capacity in a decreasing order

bool onedpacking::bin_Comparator_Rem_Cap_Decreasing(Bin* bin1, Bin* bin2)
{

    return(bin1->getRemBinCapacity() > bin2->getRemBinCapacity());

}

// ordering bins by thier increasing index
bool onedpacking::bin_Comparator_Bin_Index_Increasing(Bin* bin1,Bin* bin2)
{
    return(bin1->getBinId() < bin2->getBinId());
}






