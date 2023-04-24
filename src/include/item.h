//
// Created by weldeab solomon on 22/01/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEM_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEM_H

#include <vector>
#include <string>

namespace onedpacking {

    class Item
    {

    public:

        Item(int item_id, int item_size);

        int getItemId() const; // return the id of an item

        int getItemSize() const; // returns item size


    protected:

        int m_item_id;  // Starts from 0

        const int m_item_size;  //  the size of the item


    };

    // this orders items in non-increasing order of their size
    bool item_Comparator_Decreasing(Item* item1, Item* item2);

}



#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEM_H
