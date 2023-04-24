//
// Created by weldeab solomon on 01/02/2023.
//

#include "../../include/itemCentricAlgo.h"


#include <stdexcept>




ItemCentricAlgo::ItemCentricAlgo(std::string algo_name, const Instance &instance):
        BaseAlgorithm(algo_name, instance),
        m_is_FFD_type(false),
        m_is_BFD_type(false),
        m_is_WFD_type(false),
        m_is_FF_type(false)

{ }
//WFD and BFD sort thier bins differently. hence it is left to be re-implmented by them.
void ItemCentricAlgo::sortBins()
{ }



// Generic algorithm based on first fit
// The hint is an estimate on the number of bins to allocate
unsigned long ItemCentricAlgo::solveInstWithIC(int hint_nb_bins)
{


    if(isSolved())
    {
        return getNumOfSolutionBins(); //w once solved, no need to solve it agian.
    }
    if(hint_nb_bins > 0)
    {
        m_bins_activated.reserve(hint_nb_bins); // Small memory optimisation
    }

    if (m_is_FFD_type || m_is_BFD_type || m_is_WFD_type)
    {

        // Renumber items in decreasing size
        sortItems();
    }

    bool allocated = false;
    int total_items = m_instance.getNumberOfItems();

    // For all items in the list
    auto curr_item_it = m_items.begin();
    auto end_items_it = m_items.end();
    while(curr_item_it != end_items_it)
    {
        Item * item = *curr_item_it;

        auto curr_bin_it = m_bins_activated.begin();
        allocated = false;
        while ((!allocated) and (curr_bin_it != m_bins_activated.end()))
        {
            if (checkItemToBin(item, *curr_bin_it))
            {
                addItemToBin(item, *curr_bin_it);
                allocated = true;
            }
            else
            {
                ++curr_bin_it;
            }
        }

        if (!allocated)
        {
            // The item did not fit in any bin, create a new one
            Bin* bin = createNewBin();

            // This is a quick safeguard to avoid infinite loops and running out of memory
            if (m_bins_activated.size() > total_items)
            {
                std::string s = "There seem to be a problem with algo " + m_algo_name + " and instance " + m_instance.getInstanceName() + ", created more bins than items (" + std::to_string(m_bins_activated.size()) + ").";
                throw std::runtime_error(s);
            }

            addItemToBin(item, bin);
        }

        // Advance to next item to pack
        ++curr_item_it;


        // Update bins order (only for BF-type algos)
        if (m_is_BFD_type)
        {
            // item was added to a bin
            //sort bins in the increasing of thier residual capacity.
            sortBins();
        }
        if (m_is_WFD_type)
        {
            //sort bins in the dereasing of thier residual capacity.
            sortBins();
        }
    }

    m_is_solved = true;

    return getNumOfSolutionBins();
}


void ItemCentricAlgo::sortItems()
{
    stable_sort(m_items.begin(), m_items.end(), item_Comparator_Decreasing);
}

bool ItemCentricAlgo::tryToSolve(int m_bins)
{
    std::string s = "With ItemCentric-type algorithm please call 'solveInstance' instead.";
    throw std::runtime_error(s);
}


