//
// Created by weldeab solomon on 11/04/2023.
//

#include "../include/lower_bound.h"

#include <cmath>


// lower bound on minimum number of bins required
int LB(const Instance & instance)
{

    int sum_of_item_sizes = 0;
    int item_size;

    for (Item* item : instance.getInstanceItems())
    {
        item_size = item->getItemSize();

        sum_of_item_sizes += item_size;

    }

    int bin_capacity = instance.getMaxBinCapacity();

    int LB = std::ceil(((float)sum_of_item_sizes) / (bin_capacity));


    return LB;
}
