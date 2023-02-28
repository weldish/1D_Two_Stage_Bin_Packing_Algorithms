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
        Bin(int bin_id, int max_bin_capacity);

        Bin(const Bin& other) = default;

        const int getBinId() const;

        // get maximum bin capacity
        const int getMaxBinCapcity() const;

        // get remaining bin capacity
        const int getRemBinCapacity() const;

        // returns a list of elements allocated to the bin
        const std::vector<int>& getAllocatedList() const;

        // adding items to the bin: it adds the item to the bin by substructing the rem_bin_capacity by the item size if it fits.

        void addItem(Item* item);
        // checking if the item fits

        bool doesItemFitToBin(int item_size) const;


    protected:
        const int m_bin_id;
        const int m_max_bin_capacity;
        int m_remaining_bin_capacity;


        // items are allocated to this bin by adding their item_ids to this bin.
        std::vector<int> m_allocated_items;


    };


    bool bin_Comparator_Rem_Cap_Increasing(Bin* bin1, Bin* bin2);
    bool bin_Comparator_Rem_Cap_Decreasing(Bin* bin1, Bin* bin2);
    bool bin_Comparator_Bin_Index_Increasing(Bin* bin1,Bin* bin2);

}



#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_BIN_H
