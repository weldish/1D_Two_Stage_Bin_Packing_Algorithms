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

    bool isSolved() const;
    unsigned long getNumOfSolutionBins() const;
    const std::vector<Bin*>& getActivatedBins() const;
    std::vector<Bin*> getActivatedBinsCopy() const;
    const std::vector<Item*>& getItems() const;
    int getNbBinsActuallyUsed() const;
    int getNumOfItems() const;
    int getBinCapacity() const;
    void setSolution(std::vector<Bin*>& bins);
    void clearSolution();
    // solves an instance for item centric algorithms
    virtual unsigned long solveInstance(int hint_nb_bins = 0) = 0;
    // solves an instance with single stage multibin algorithms
    virtual int solveInstanceMultiBin(int LB, int UB);
    // solve an instace with two stage mutlibin algorithms
    virtual int solveForMultiBreakPoints(float b_factor, int LB, int UB);


protected:
    virtual Bin* createNewBin(); // this Opens a new empty bin
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
    const int m_num_of_dimensions;
    bool m_create_bins_at_end; // for when creating bins: Whether a newly created bin should be put at the end of the list
    bool m_is_solved;

};



#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_BASE_ALGORITHM_H
