//
// Created by weldeab solomon on 12/03/2023.
//

#include "../../include/Item_Centric_To_Pairing.h"


#include <cmath>
#include <iostream>

ItemCentricPairingAlgo::ItemCentricPairingAlgo(std::string algo_name, const Instance &instance):
        BaseAlgorithm(algo_name, instance),
        m_is_FFD_type(false),
        m_is_BFD_type(false),
        m_is_WFD_type(false)

{}

int ItemCentricPairingAlgo::solveInstWith2SA(float b_factor, int LB, int UB)
{

    int best_solution = UB; // keeping track the best solution

    int breakpoint = 0; // first breakpoint
    int k; // number of iterations
    int answer;
    bool found_sol = false;
    int track = -1;
    k = ceil(1/b_factor); // this specifies the number of breakpoints or iterations for a given b_factor

    for (int i= 0; i<k; i++)
    {
        // iterate through the list of breakpoints
        breakpoint = breakpoint + b_factor * LB;

        answer = solveInstWithMBP(breakpoint, LB, UB);

        if (answer == -1)
        {
            std::cout << "coudnt find an answer" << std::endl;
        }
        if(answer >=0)
        {
            found_sol = true;
        }

        if (answer > 0 and answer < best_solution)
        {
            //update best solution
            best_solution = answer;
            track = i;
        }
        if (best_solution == LB)
        {
            // if the current best solution is the same as LB, return the current best solution since it is optimal.
            return best_solution;
        }

    }

    if(!found_sol)
    {
        return -1;
    }

    std::cout << "best_k_value: " << std::to_string(track) << std::endl;
    return best_solution;

}


int ItemCentricPairingAlgo::solveInstWithMBP(int breakpoint, int LB, int UB)
{

    // First, try to find a solution with UB
    if (!trySolveTwoStage(breakpoint, UB))
    {
        // If no solution is found, there is no need to continue searching for a solution
        return -1;
    }

    // A solution with UB was found, store the current solution
    m_best_solution_bins.clear();
    updateBestSolutionBins(getActivatedBinsCopy());
    int m_bins;

    // Then iteratively try to improve on the solution
    while (LB < UB)
    {
        m_bins = std::floor((float)(LB + UB) / 2.0);

        if (trySolveTwoStage(breakpoint, m_bins))
        {
            // since a better solution is found, we update UB
            UB = m_bins;
            updateBestSolutionBins(getActivatedBinsCopy());
        }
        else
        {
            // m_bins is too small, update LB
            LB = m_bins + 1; // +1 to keep a potential feasible solution with LB
        }
    }

    setSolution(m_best_solution_bins);
    return UB;


}


void ItemCentricPairingAlgo::updateBestSolutionBins(const std::vector<Bin*> &activated_bins)
{
    for (Bin* bin : m_best_solution_bins)
    {

        if (bin != nullptr)
        {
            delete bin;
        }
    }
    m_best_solution_bins.clear();
    m_best_solution_bins = activated_bins;
}




bool ItemCentricPairingAlgo::trySolveTwoStage(int breakpoint, int m_bins)
{

    clearSolution(); // deletes all bins that were used from a previous attempt to solve the instance

    m_bin_item_scores.clear(); // clearing the vector of vectors for item-bin scores

    if (m_is_FFD_type || m_is_BFD_type || m_is_WFD_type)
    {

        // Reorder items in decreasing size
        sortItems();
    }


    // For all items in the list
    auto curr_item_it = m_items.begin();
    auto end_items_it = m_items.end();


    return solveItemCentric(breakpoint, m_bins, curr_item_it, end_items_it);
}






// solve by calling the itemCentric algorithm
bool ItemCentricPairingAlgo::solveItemCentric(int breakpoint, int m_bins, std::vector<Item*>::iterator start_it, std::vector<Item*>::iterator end_it)
{
    auto curr_item_it = start_it;
    auto end_items_it = end_it;
    bool allocated = false;
    int total_items = m_instance.getNumberOfItems();
    int number_of_bins_activated = 0;
    bool terminated = false;


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

            // check if the number of activated bins = breakpoint

            if (number_of_bins_activated == breakpoint)
            {
                terminated = true;
                break;
            }
            // The item did not fit in any bin, create a new one
            Bin* bin = createNewBin();
            number_of_bins_activated++;

            // This is a quick safe-guard to avoid infinite loops and running out of memory
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
            // sort bins in the increasing order of their residual capacity.
            sortBins();
        }
        if (m_is_WFD_type)
        {
            // sort bins in the decreasing order of their residual capacity.
            sortBins();
        }


    }

    // switching to second stage
    if (terminated)
    {
        // call the second algorithm
        return solvePairing(breakpoint, m_bins, curr_item_it, end_items_it, m_score);

    }

    return true;

}



// solve by calling the pairing algorithm
bool ItemCentricPairingAlgo::solvePairing(int breakpoint, int m_bins, std::vector<Item*>::iterator last_it, std::vector<Item*>::iterator end_it, SCORE score)
{

    int nb_items = m_items.size();
    m_bin_item_scores.resize(m_bins, std::vector<float>(nb_items, 0.0));

    // m_bins always bigger than breakpoint by atleast one

    int extra_bins = m_bins-breakpoint;

    // add bins to the existing ones
    for (int i = 0; i < extra_bins; ++i)
    {
        createNewBin();
    }

    for (Bin* bin : m_bins_activated)
    {
        updateScores(bin, m_items.begin(), m_items.end());
    }

    auto first_item_it = last_it; // first item is the last unallocated item from stage 1
    auto end_items_it = end_it;
    auto start_bin_it = m_bins_activated.begin();
    auto end_bins_it = m_bins_activated.end();


    while(first_item_it != end_items_it) // While there are items to pack
    {
        // this is all to compute maximum score for each pair of item-bin
        auto max_score_item_it = end_items_it;
        auto max_score_bin_it = end_bins_it;
        float max_score_val = std::numeric_limits<double>::lowest(); // -infinity

        // For each remaining item
        for(auto curr_item_it = first_item_it; curr_item_it != end_items_it; ++curr_item_it)
        {
            // For each bin
            for(auto curr_bin_it = start_bin_it; curr_bin_it != end_bins_it; ++curr_bin_it)
            {
                // when retrieving the highest score this doesItemFitToBin method will indirectly invalidate computed scores of item-bin pairs where the item doesnt fit into the bin: we are only considering scores that  are of feasible item-bin pair.
                if ((*curr_bin_it)->doesItemFitToBin((*curr_item_it)->getItemSize()))
                {
                    // Scores were computed previously
                    float score = m_bin_item_scores[(*curr_bin_it)->getBinId()] [(*curr_item_it)->getItemId()];

                    if (score > max_score_val)
                    {
                        max_score_val = score;
                        max_score_item_it = curr_item_it;
                        max_score_bin_it = curr_bin_it;
                    }
                }
            }
        }

        if (max_score_item_it != end_items_it)
        {
            // There is a feasible item-bin pair
            addItemToBin(*max_score_item_it, *max_score_bin_it);
            //--remaining_items;

            // Put this item at beginning of the list and advance the first iterator
            std::iter_swap(max_score_item_it, first_item_it);
            first_item_it++;


            // Update score of remaining items for that bin
            updateScores(*max_score_bin_it, first_item_it, end_items_it);


        }
        else
        {
            // There is no feasible item-bin pair, the solution is infeasible
            // Update the iterator on first remaining item before exitting

            return false;
        }



    }



//    m_is_solved = true;
    return true;
}



// sorts items in non-decreasing order for all intemcentric-type algorithms
void ItemCentricPairingAlgo::sortItems()
{
    stable_sort(m_items.begin(), m_items.end(), item_Comparator_Decreasing);
}

// sorts bins only for WFD or BFD algorithms
void ItemCentricPairingAlgo::sortBins()
{
    if (m_is_BFD_type)
    {
        // sort bins in increasing order of residual capacity
        stable_sort(m_bins_activated.begin(), m_bins_activated.end(), bin_Comparator_Rem_Cap_Increasing);
    }

    if (m_is_WFD_type)
    {
        // sort bins in decreasing order of residual capacity
        stable_sort(m_bins_activated.begin(), m_bins_activated.end(), bin_Comparator_Rem_Cap_Decreasing);
    }
}

// updating scores for that bin only by going over the remaining items and computing scores for each item-bin pair

void ItemCentricPairingAlgo::updateScores(Bin* bin, std::vector<Item*>::iterator first_item, std::vector<Item*>::iterator end_it)
{
    std::vector<float>& item_scores = m_bin_item_scores[bin->getBinId()];
    for (auto item_it = first_item; item_it != end_it; item_it++)
    {
        //m_bin_item_scores[bin->getId()][(*item_it)->getId()];
        item_scores[(*item_it)->getItemId()] = computeItemBinScore(*item_it, bin);
    }
}



float ItemCentricPairingAlgo::computeItemBinScore(Item *item, Bin *bin)
{
    float score_value = 0.0;

    switch(m_score)
    {
        case SCORE::PRODUCT:
        {
            // score = item_size * residual bin capacity
            score_value = item->getItemSize() * bin->getRemBinCapacity();
        }
            break;

        case SCORE::SLACK:
        {
            // score = -(bin_residual_capacity - item_size)
            score_value = -(bin->getRemBinCapacity() - item->getItemSize());
        }
            break;
        case SCORE::TIGHT_FILL:
        {
            // score = item_size / residual bin capacity
            score_value = (item->getItemSize() / (float)bin->getRemBinCapacity());
        }
            break;
    }

    return score_value;
}




unsigned long ItemCentricPairingAlgo::solveInstWithIC(int hint_nb_bins)
{
    std::string s = "With itemCentricpairing-type algorithm please call solveForMultiBreakPointsinstead.";
    throw std::runtime_error(s);
}

bool ItemCentricPairingAlgo::tryToSolve(int m_bins)
{
    std::string s = "With itemCentricpairing-type algorithm call solveForMultiBreakPoints instead";
    throw std::runtime_error(s);
}







/* ================================================ */


FFD_Pairing_PRODUCT::FFD_Pairing_PRODUCT(std::string algo_name, const Instance &instance):
        ItemCentricPairingAlgo(algo_name, instance)
{
    m_score = SCORE::PRODUCT;
    m_is_FFD_type = true;
}

/* ================================================ */

FFD_Pairing_SLACK::FFD_Pairing_SLACK(std::string algo_name, const Instance &instance):
        ItemCentricPairingAlgo(algo_name, instance)
{
    m_score = SCORE::SLACK;
    m_is_FFD_type = true;
}

/* ================================================ */

FFD_Pairing_TIGHT_FILL::FFD_Pairing_TIGHT_FILL(std::string algo_name, const Instance &instance):
        ItemCentricPairingAlgo(algo_name, instance)
{
    m_score = SCORE::TIGHT_FILL;
    m_is_FFD_type = true;
}

/* ================================================ */


/* ================================================ */


BFD_Pairing_PRODUCT::BFD_Pairing_PRODUCT(std::string algo_name, const Instance &instance):
        ItemCentricPairingAlgo(algo_name, instance)
{
    m_score = SCORE::PRODUCT;
    m_is_BFD_type = true;
}

/* ================================================ */

BFD_Pairing_SLACK::BFD_Pairing_SLACK(std::string algo_name, const Instance &instance):
        ItemCentricPairingAlgo(algo_name, instance)
{
    m_score = SCORE::SLACK;
    m_is_BFD_type = true;
}

/* ================================================ */

BFD_Pairing_TIGHT_FILL::BFD_Pairing_TIGHT_FILL(std::string algo_name, const Instance &instance):
        ItemCentricPairingAlgo(algo_name, instance)
{
    m_score = SCORE::TIGHT_FILL;
    m_is_BFD_type = true;
}

/* ================================================ */


/* ================================================ */


WFD_Pairing_PRODUCT::WFD_Pairing_PRODUCT(std::string algo_name, const Instance &instance):
        ItemCentricPairingAlgo(algo_name, instance)
{
    m_score = SCORE::PRODUCT;
    m_is_WFD_type = true;
}

/* ================================================ */

WFD_Pairing_SLACK::WFD_Pairing_SLACK(std::string algo_name, const Instance &instance):
        ItemCentricPairingAlgo(algo_name, instance)
{
    m_score = SCORE::SLACK;
    m_is_WFD_type = true;
}

/* ================================================ */

WFD_Pairing_TIGHT_FILL::WFD_Pairing_TIGHT_FILL(std::string algo_name, const Instance &instance):
        ItemCentricPairingAlgo(algo_name, instance)
{
    m_score = SCORE::TIGHT_FILL;
    m_is_WFD_type = true;
}

/* ================================================ */


