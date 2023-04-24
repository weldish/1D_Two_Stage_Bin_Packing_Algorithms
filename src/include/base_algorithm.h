//
// Created by weldeab solomon on 28/01/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_BASE_ALGORITHM_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_BASE_ALGORITHM_H



#include "item.h"
#include "instance.h"
#include "bin.h"
#include <algorithm>

using namespace onedpacking;


class BaseAlgorithm
{
public:
    BaseAlgorithm(std::string algo_name, const Instance &instance);
    virtual ~BaseAlgorithm();
    // checks it the problem is already solved.
    bool isSolved() const;
    // returns the number of bins used to pack all items: that is the solution value.
    unsigned long getNumOfSolutionBins() const;
    // this refers to the number of bins that are currently activated
    std::vector<Bin*> getActivatedBinsCopy() const;
    // returns the number of items in a given instance
    int getNumOfItems() const;
    // return the capacity of an empty bin.
    int getBinCapacity() const;

    void setSolution(std::vector<Bin*>& bins);
    void clearSolution();

    // solves an instance with Item-Centric algorithms
    virtual unsigned long solveInstWithIC(int hint_nb_bins = 0) = 0;

    // solves an instance with single stage Multi-Bin Pairing algorithms
    virtual int solveInstWithMBP(int LB, int UB);

    // solve an instance with Two-Stage algorithms
    virtual int solveInstWith2SA(float b_factor, int LB, int UB);


protected:
    virtual Bin* createNewBin(); // this intiates a new empty bin
    virtual bool checkItemToBin(Item* item, Bin* bin) const;
    virtual void addItemToBin(Item* item, Bin* bin);
    void setNbOfBinsActuallyUsed(std::vector<Bin*>::iterator first_bin_it, std::vector<Bin*>::iterator end_bin_it);
protected:
    const std::string m_algo_name;
    int m_next_bin_index; // for when creating bins
    const int m_bin_max_capacity; // for when creating bins
    std::vector<Item*> m_items;
    std::vector<Bin*> m_bins_activated;
    int m_nb_bins_actually_used;
    const Instance& m_instance;
    // for when creating bins: Whether a newly created bin should be put at the end of the list
    // with WFD for example, it is better to put the newly activated bin in the beginning.
    bool m_create_bins_at_end;
    bool m_is_solved;

};



#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_BASE_ALGORITHM_H
