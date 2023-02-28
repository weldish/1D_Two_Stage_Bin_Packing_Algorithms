//
// Created by weldeab solomon on 28/01/2023.
//

#include "../include/base_algorithm.h"

#include <iostream>


BaseAlgorithm::BaseAlgorithm(std::string algo_name, const Instance &instance):
        m_algo_name(algo_name),
        // copying a vector of item*
        m_items(std::vector<Item*>(instance.getInstanceItems())),
        m_bins_activated(std::vector<Bin*>(0)),
        m_bin_max_capacity(instance.getMaxBinCapacity()),
        m_instance(instance),
        m_num_of_dimensions(instance.getNumOfDimensions()),
        m_next_bin_index(0),
        m_is_solved(false),
        m_create_bins_at_end(true),
        m_nb_bins_actually_used(0)
{ }




// deallocate the bins this object created.
BaseAlgorithm::~BaseAlgorithm()
{
    for (Bin* bin : m_bins_activated)
    {
        if (bin != nullptr)
            delete bin;
    }
}

bool BaseAlgorithm::isSolved() const
{
    return m_is_solved;
}

int BaseAlgorithm::getNbBinsActuallyUsed() const
{
    return m_nb_bins_actually_used;
}

void BaseAlgorithm::setNbOfBinsActuallyUsed(std::vector<Bin*>::iterator first_bin_it, std::vector<Bin*>::iterator end_bin_it)
{
    for (auto curr_bin_it = first_bin_it; curr_bin_it < end_bin_it; ++curr_bin_it)
    {
        int remaining_bin_cap = (*curr_bin_it) ->getRemBinCapacity();
        int maxamimum_bin_cap = (*curr_bin_it) ->getMaxBinCapcity();
        if (remaining_bin_cap < maxamimum_bin_cap)
        {
            // update the number of bins actually used
            m_nb_bins_actually_used++;
        }
    }
}

unsigned long BaseAlgorithm::getNumOfSolutionBins() const
{
    return m_bins_activated.size();
}

const std::vector<Bin*>& BaseAlgorithm::getActivatedBins() const
{
    return m_bins_activated;
}

std::vector<Bin*> BaseAlgorithm::getActivatedBinsCopy() const
{
    std::vector<Bin*> deep_copied_bins;
    deep_copied_bins.reserve(m_bins_activated.size());
    for (Bin* bin : m_bins_activated)
    {
        deep_copied_bins.push_back(new Bin(*bin));
    }
    return deep_copied_bins;
}

const std::vector<Item*>& BaseAlgorithm::getItems() const
{
    return m_items;
}



void BaseAlgorithm::setSolution(std::vector<Bin*>& bins)
{
    clearSolution();
    m_bins_activated = bins;
    m_is_solved = true;
}

void BaseAlgorithm::clearSolution()
{
    m_is_solved = false;
    for (Bin* bin : m_bins_activated)
    {
        if (bin != nullptr)
        {
            delete bin;
        }
    }
    m_bins_activated.clear();
    m_next_bin_index = 0;
}

Bin* BaseAlgorithm::createNewBin()
{
    Bin* newly_created_bin = new Bin(m_next_bin_index, m_bin_max_capacity);
    if (m_create_bins_at_end)
    {
        m_bins_activated.push_back(newly_created_bin);
    }
    else
    {
        //in cases for when the Worst Fit algorithms for example is used to solve the instance: this is beacause emtpty bins are prefered over non-emtpy bins.
        m_bins_activated.insert(m_bins_activated.begin(), newly_created_bin);
    }
    m_next_bin_index += 1;

    return newly_created_bin;
}

bool BaseAlgorithm::checkItemToBin(Item* item, Bin* bin) const
{
    return (bin->doesItemFitToBin(item->getItemSize()));
}

void BaseAlgorithm::addItemToBin(Item* item, Bin* bin)
{
    bin->addItem(item);
}


int BaseAlgorithm::solveForMultiBreakPoints(float b_factor, int LB, int UB)
{
    return -1 ;
}


int BaseAlgorithm::getNumOfItems() const
{
    return int(m_items.size());
}

int BaseAlgorithm::getBinCapacity() const
{
    return m_bin_max_capacity;
}


int BaseAlgorithm::solveInstanceMultiBin(int LB, int UB)
{
    //std::cout << "TODO" << std::endl;

    return -2;
}


