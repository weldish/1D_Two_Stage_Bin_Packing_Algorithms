//
// Created by weldeab solomon on 23/01/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_BIN_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_BIN_H

#include "item.h"
#include <vector>

namespace onedpacking
{

    class Bin
    {

    public:
        Bin(int bin_id, int max_bin_capacity);  // a constructor for the bin

        Bin(const Bin& other) = default;     // copy constructor

        const int getBinId() const;

        // get maximum bin capacity(this is the size when the bin is empty)
        const int getMaxBinCapcity() const;

        // get remaining bin capacity
        const int getRemBinCapacity() const;

        // adds an item to the current bin:

        void addItem(Item* item);

        // checks if the item fits
        // this function is called before addItem
        bool doesItemFitToBin(int item_size) const;


    protected:
        const int m_bin_id;
        const int m_max_bin_capacity;
        int m_remaining_bin_capacity;


        // items are allocated to this bin by adding their id's to this bin.
        std::vector<int> m_allocated_items;


    };

    // orders bins in the increasing order of their residual capacity
    bool bin_Comparator_Rem_Cap_Increasing(Bin* bin1, Bin* bin2);

    // orders bins in the decreasing order of their residual capacity
    bool bin_Comparator_Rem_Cap_Decreasing(Bin* bin1, Bin* bin2);


}



#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_BIN_H
