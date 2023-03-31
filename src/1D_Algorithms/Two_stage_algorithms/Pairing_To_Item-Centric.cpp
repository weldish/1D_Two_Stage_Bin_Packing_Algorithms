//
// Created by weldeab solomon on 12/03/2023.
//

#include "../../include/Pairing_To_Item_Centric.h"


#include <cmath>
#include <iostream>

PairingToItemCentric::PairingToItemCentric(std::string algo_name, const Instance &instance):
        BaseAlgorithm(algo_name, instance),
        m_is_FFD_type(false),
        m_is_BFD_type(false),
        m_is_WFD_type(false)

{}


// this method creates a list of breakpoints and calls the solveInstnaceMultiBin method for every breakpoint.
// it keeps track of the best solution and number of iterations that led to that best solution
// it returns the number of bins in the best solution
//the value of breakpoints lies within [1, LB)
// the value of breakpoints should round down and be an integer
int PairingToItemCentric::solveForMultiBreakPoints(float b_factor, int LB, int UB)
{
    // breakpoint is incremented by a fixed gap.
    // 0 < b_factor < 1

    int best_solution = UB; // keeping track the best solution// ???? this needs to be reconsidered

    int breakpoint = 0; // first breakpoint
    int k; // number of iterations
    int answer;
    bool found_sol = false;

    k = ceil(1/b_factor); // this specifies the number of breakpoints or iterations for a given b_factor
    int track = -1;
    for (int i= 0; i<k; i++)
    {
        breakpoint = breakpoint + b_factor * LB;

//        if (breakpoint <= LB)
//        {
        answer = trySolveTwoStage(breakpoint);

//        }




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





    }

    if(!found_sol)
    {
        return -1;
    }


    std::cout << "best_k_value: " << std::to_string(track) << std::endl;
    return best_solution;


}




int PairingToItemCentric::trySolveTwoStage(int breakpoint)
{
//   if(isSolved())
//    {
//        return getNumOfSolutionBins(); //w once solved, no need to solve it agian.
//    }

    clearSolution(); // deletes all bins that were used from a previous attempt to solve the instance


    if(breakpoint > 0)
    {
        m_bins_activated.reserve(breakpoint); // Small memory optimisation
    }

    if (m_is_FFD_type || m_is_BFD_type || m_is_WFD_type)
    {

        // Renumber items in decreasing size
        sortItems();
    }


    // For all items in the list
    auto curr_item_it = m_items.begin();
    auto end_items_it = m_items.end();

    return solvePairing(breakpoint, curr_item_it, end_items_it);

}


// solve by calling the pairing algorithm
int PairingToItemCentric::solvePairing(int breakpoint, std::vector<Item*>::iterator start_it, std::vector<Item*>::iterator end_it)
{

    if (breakpoint < 0)
    {
        std::string s("Trying to solve instance with negative number of bins");
        throw std::runtime_error(s);
    }


    // creat a breakpoint number of bins
    for (int i = 0; i < breakpoint; ++i)
    {
        createNewBin();
    }

    int nb_items = m_items.size();

    m_bin_item_scores.clear();
    //unsigned long nb_items = m_items.size();
    m_bin_item_scores.resize(breakpoint, std::vector<float>(nb_items, 0.0));


    for (Bin* bin : m_bins_activated)
    {
        updateScores(bin, m_items.begin(), m_items.end());
    }


    // settting up the algorithm finsihes here


    auto first_item_it = start_it;
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

                    float score;

                    // Scores were computed previously
                    score = m_bin_item_scores[(*curr_bin_it)->getBinId()] [(*curr_item_it)->getItemId()];


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
            auto first_remaining_item = first_item_it;

            // this method pack the rest of items by adding one bin at a time.
            solveItemCentric(first_remaining_item, end_it);

            return getNumOfSolutionBins();
        }



    }

    return getNumOfSolutionBins();
}


// solve by calling the itemCentric algorithm
void PairingToItemCentric::solveItemCentric(std::vector<Item*>::iterator last_it, std::vector<Item*>::iterator end_it)
{
    auto curr_item_it = last_it;
    auto end_items_it = end_it;
    bool allocated = false;


    while(curr_item_it != end_items_it)
    {
        Item * item = *curr_item_it;

        // ToDo instead of scaning the bins from the start, just scan only the added ones.(optimisaion)
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


            // This is a quick safe guard to avoid infinite loops and running out of memory
            int total_items = int(m_items.size());
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
            // Bin measures must have been updated when last
            // item was added to a bin
            //w sort bins in the increasing of thier residual capacity.
            sortBins();
        }
        if (m_is_WFD_type)
        {
            //w sort bins in the dereasing of thier residual capacity.
            sortBins();
        }


    }


}






// sorts items in non-decreasing order for all intemcentric-type algorithms
void PairingToItemCentric::sortItems()
{
    stable_sort(m_items.begin(), m_items.end(), item_Comparator_Decreasing);
}

// sorts bins only for WFD or BFD algorithms
void PairingToItemCentric::sortBins()
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

void PairingToItemCentric::updateScores(Bin* bin, std::vector<Item*>::iterator first_item, std::vector<Item*>::iterator end_it)
{
    std::vector<float>& item_scores = m_bin_item_scores[bin->getBinId()];
    for (auto item_it = first_item; item_it != end_it; item_it++)
    {
        //m_bin_item_scores[bin->getId()][(*item_it)->getId()];
        item_scores[(*item_it)->getItemId()] = computeItemBinScore(*item_it, bin);
    }
}



float PairingToItemCentric::computeItemBinScore(Item *item, Bin *bin)
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




unsigned long PairingToItemCentric::solveInstance(int hint_nb_bins)
{
    std::string s = "With itemCentricpairing-type algorithm please call solveForMultiBreakPointsinstead.";
    throw std::runtime_error(s);
}

bool PairingToItemCentric::tryToSolve(int m_bins)
{
    std::string s = "With itemCentricpairing-type algorithm call solveForMultiBreakPoints instead";
    throw std::runtime_error(s);
}







/* ================================================ */


Pairing_PRODUCT_To_FFD::Pairing_PRODUCT_To_FFD(std::string algo_name, const Instance &instance):
        PairingToItemCentric(algo_name, instance)
{
    m_score = SCORE::PRODUCT;
    m_is_FFD_type = true;
}

/* ================================================ */

Pairing_SLACK_To_FFD::Pairing_SLACK_To_FFD(std::string algo_name, const Instance &instance):
        PairingToItemCentric(algo_name, instance)
{
    m_score = SCORE::SLACK;
    m_is_FFD_type = true;
}

/* ================================================ */

Pairing_TIGHT_FILL_To_FFD::Pairing_TIGHT_FILL_To_FFD(std::string algo_name, const Instance &instance):
        PairingToItemCentric(algo_name, instance)
{
    m_score = SCORE::TIGHT_FILL;
    m_is_FFD_type = true;
}

/* ================================================ */


/* ================================================ */


Pairing_PRODUCT_To_BFD::Pairing_PRODUCT_To_BFD(std::string algo_name, const Instance &instance):
        PairingToItemCentric(algo_name, instance)
{
    m_score = SCORE::PRODUCT;
    m_is_BFD_type = true;
}

/* ================================================ */

Pairing_SLACK_To_BFD::Pairing_SLACK_To_BFD(std::string algo_name, const Instance &instance):
        PairingToItemCentric(algo_name, instance)
{
    m_score = SCORE::SLACK;
    m_is_BFD_type = true;
}

/* ================================================ */

Pairing_TIGHT_FILL_To_BFD::Pairing_TIGHT_FILL_To_BFD(std::string algo_name, const Instance &instance):
        PairingToItemCentric(algo_name, instance)
{
    m_score = SCORE::TIGHT_FILL;
    m_is_BFD_type = true;
}

/* ================================================ */


/* ================================================ */


Pairing_PRODUCT_To_WFD::Pairing_PRODUCT_To_WFD(std::string algo_name, const Instance &instance):
        PairingToItemCentric(algo_name, instance)
{
    m_score = SCORE::PRODUCT;
    m_is_WFD_type = true;
}

/* ================================================ */

Pairing_SLACK_To_WFD::Pairing_SLACK_To_WFD(std::string algo_name, const Instance &instance):
        PairingToItemCentric(algo_name, instance)
{
    m_score = SCORE::SLACK;
    m_is_WFD_type = true;
}

/* ================================================ */

Pairing_TIGHT_FILL_To_WFD::Pairing_TIGHT_FILL_To_WFD(std::string algo_name, const Instance &instance):
        PairingToItemCentric(algo_name, instance)
{
    m_score = SCORE::TIGHT_FILL;
    m_is_WFD_type = true;
}

/* ================================================ */


