//
// Created by weldeab solomon on 01/02/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEMCENTRICALGO_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEMCENTRICALGO_H


#include "base_algorithm.h"

using namespace onedpacking;


// Base class of Item-Centric Algorithms
// With placeholder functions for sorting the items and bins and solving the instance.
class ItemCentricAlgo : public BaseAlgorithm
{
public:
    ItemCentricAlgo(std::string algo_name, const Instance &instance);

    // solves an instance with a given Item-Centric algorithm
    virtual unsigned long solveInstWithIC(int hint_nb_bins = 0);

    virtual bool tryToSolve(int m_bins); // not used for item-centric algorithms


protected:
    // WFD or BFD can reimplement sortBins()
    virtual void sortBins();
    // all three, WFD or BFD or FFD don,t need to reimplement this method. they can just inherit it.
    // as they all sort items in non-increasing order.
    void sortItems();


protected:
    bool m_is_FFD_type; //Whether to sort items: we don't sort bins for FFD types
    bool m_is_FF_type;
    bool m_is_BFD_type;  //Whether to sort item and bins
    bool m_is_WFD_type; //Whether to sort items and bins
};


#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEMCENTRICALGO_H
