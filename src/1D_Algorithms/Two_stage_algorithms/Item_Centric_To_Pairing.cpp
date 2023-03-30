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


// this method creates a list of breakpoints and calls the solveInstnaceMultiBin method for every breakpoint.
// it keeps track of the best solution and number of iterations that led to that best solution
// it returns the number of bins in the best solution
//the value of breakpoints lies within [1, LB)
// the value of breakpoints should round down and be an integer
int ItemCentricPairingAlgo::solveForMultiBreakPoints(float b_factor, int LB, int UB)
{
    // WARNING
    // FOR MONOTOCITY EXPERMINET THIS IS WHERE YOU CHNAGE THINGS
    // breakpoint is incremented by a fixed gap.
    // 0 < b_factor < 1

    int best_solution = UB; // keeping track the best solution// ???? this needs to be reconsidered

    int breakpoint = 0; // first breakpoint
    int k; // number of iterations
    int answer;
    bool found_sol = false;
    int track = -1;
    k = ceil(1/b_factor); // this specifies the number of breakpoints or iterations for a given b_factor

    for (int i= 0; i<k; i++)
    {

        breakpoint = breakpoint + b_factor * LB;

//      if (breakpoint <= LB)
//      {
        answer = solveInstanceMultiBin(breakpoint, LB, UB);
//      }




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


// this method increments the number of bins one by one if trysolvetwostage cant pack all items with the given number of bins
// returns the number of bins used
// shoud start at LB
//breakpoint lies within [1, LB)

//int ItemCentricPairingAlgo::solveInstanceMultiBin(int breakpoint, int LB, int UB)
//{
////    // for the same breakpoint, m_bins is incremented one by one untill a solution is found.
////        int bin_increment = 1;
////
////        int m_bins = LB;
////        bool solution_found = trySolveTwoStage(breakpoint, m_bins);
////        bool last_increment = false;
////        while (!solution_found and !last_increment)
////        {
////            // There are remaining items to pack
////            // But no bin can accommodate an item anymore
////
////            // Increment the number of bins
////            m_bins += bin_increment;
////
////            if (m_bins == UB)
////            {
////                last_increment = true;
////            }
////
////            solution_found = trySolveTwoStage(breakpoint, m_bins);
////         }
////
////        int answer = m_bins;
////    // if there is no solution
////        if (!solution_found)
////        {
////            answer = -1;
////        }
////        return answer;
//}


int ItemCentricPairingAlgo::solveInstanceMultiBin(int breakpoint, int LB, int UB)
{

    // First, try to find a solution with UB
    if (!trySolveTwoStage(breakpoint, UB))
    {
        // If no solution is found, there is no need to continue the search
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
            // Better solution found, update UB
            UB = m_bins;
            updateBestSolutionBins(getActivatedBinsCopy());
        }
        else
        {
            // Target too low, update LB
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
//   if(isSolved())
//    {
//        return getNumOfSolutionBins(); //w once solved, no need to solve it agian.
//    }

    clearSolution(); // deletes all bins that were used from a previous attempt to solve the instance


    m_bin_item_scores.clear(); // cleaing the vector of vectors for item-bin scores


//    if(breakpoint > 0)
//    {
//        m_bins_activated.reserve(breakpoint); // Small memory optimisation
//    }

    if (m_is_FFD_type || m_is_BFD_type || m_is_WFD_type)
    {

        // Renumber items in decreasing size
        sortItems();
    }



    //bool allocated = false;
    //int total_items = m_instance.getNumberOfItems();

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

            // This is a quick safe guard to avoid infinite loops and running out of memory
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

    // outside of the while loop

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
    //unsigned long nb_items = m_items.size();
    m_bin_item_scores.resize(m_bins, std::vector<float>(nb_items, 0.0));

    // m_bins always bigger than breakpoint by atleast one
    //create m_bins - breakpoint

    int extra_bins = m_bins-breakpoint;
//    if (extra_bins < 0)
//    {
//
//        std::string s("trying to solve the isntance without adding new bins");
//        throw std::runtime_error(s);
//    }
    // add bins to the existing ones
    for (int i = 0; i < extra_bins; ++i)
    {
        createNewBin();
    }

    for (Bin* bin : m_bins_activated)
    {
        updateScores(bin, m_items.begin(), m_items.end());
    }


    // settting up the algorithm finsihes here


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

                    float score;
//                    if (!store_scores)
//                    {
//                        // Need to recompute all scores
//                        score = computeItemBinScore((*curr_item_it), (*curr_bin_it));
//                    }
                    //else
                    // {
                    // Scores were computed previously
                    score = m_bin_item_scores[(*curr_bin_it)->getBinId()] [(*curr_item_it)->getItemId()];
                    //}

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




unsigned long ItemCentricPairingAlgo::solveInstance(int hint_nb_bins)
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


