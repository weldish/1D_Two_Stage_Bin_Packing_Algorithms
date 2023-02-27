//
// Created by weldeab solomon on 25/02/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_INSTANCE_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_INSTANCE_H

#include "item.h"
#include <vector>


namespace onedpacking {

    class Instance
    {
    public:
        Instance( const std::string instance_name,
                  const std::string filename,
                  const bool shuffle_items = true);

        virtual ~Instance();

        const std::string getInstanceName() const;
        const int getNumberOfItems() const;
        const int getMaxBinCapacity() const;
        const std::vector<Item*>& getInstanceItems() const;
        const bool areItemsShuffled () const;
        const int getNumOfDimensions() const;

    private:
        const std::string m_instance_name;// name of instance
        const bool m_are_items_shuffled; // weather item are shuffled or not.
        int m_number_of_items;    // number of items
        int m_number_of_dimensions; // number of dimesnions
        int m_maximum_bin_capacity;
        std::vector<Item*> m_item_list; // the list of item*




    };

//  int ReadMaxBinCapacitySize(std::string resource_str);
//  void ReadItemSize(std::string resource_str, int& bin_capacity,
//                     int& item_size, float& norm_size);
}


#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_INSTANCE_H
