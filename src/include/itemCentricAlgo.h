//
// Created by weldeab solomon on 01/02/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEMCENTRICALGO_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEMCENTRICALGO_H


#include "base_algorithm.h"

using namespace onedpacking;


// Base class of AlgoFit tailored for vector bin packing
// With placeholder functions to sort the items, sort the bins
// and determine whether an item can be placed in a bin
class ItemCentricAlgo : public BaseAlgorithm
{
public:
    ItemCentricAlgo(std::string algo_name, const Instance &instance);

    virtual unsigned long solveInstance(int hint_nb_bins = 0);
    virtual bool tryToSolve(int m_bins);
    //virtual int solveInstanceMultiBin(int LB, int UB); // Not used for ItemCentric algos

protected:
    //w WFD or BFD can reimplemet sortBins()
    virtual void sortBins();
    // WFD or BFD or FFD dont need to reimpelement this method. they can just inherit it.
    void sortItems();


protected:
    bool m_is_FFD_type; //w Whether to sort items: we don't sort bins for FFD types
    bool m_is_FF_type;
    bool m_is_BFD_type;  //w Whether to sort item and bins
    bool m_is_WFD_type; //w whether to sort items and bins
};


#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_ITEMCENTRICALGO_H
