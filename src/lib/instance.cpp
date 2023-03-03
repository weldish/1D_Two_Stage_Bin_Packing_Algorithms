//
// Created by weldeab solomon on 25/01/2023.
//

#include "../include/instance.h"

#include "../include/instance.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm> // this is for std::shuffle
#include <random>    //for the random number generator


using namespace std;
using namespace onedpacking;

#include <iostream>
auto RANDOM_SEED = 55;



Instance::Instance( const std::string instance_name,
                    const std::string filename,
                    const bool shuffle_items):
        m_instance_name(instance_name),
        m_are_items_shuffled(shuffle_items)
{

    ifstream ifile(filename.c_str());
    if(!ifile.is_open())
    {
        string s = "Could not open instance file " + filename;
        throw runtime_error(s);
    }
    try {
        string line;


        // Read the number of dimensions from the instance file
        getline(ifile, line);
        m_number_of_dimensions = stoi(line);

        // Read the capacity of the bin from the instance file which is the maximum capacity
        getline(ifile, line);
        m_maximum_bin_capacity = stoi(line);


        // Read number of items from the instance file
        getline(ifile, line);
        int total_number_of_different_items = stoi(line);

        // Read all items from the instance file
        //m_item_list.reserve(m_number_of_items);


        // demand for items of type i

        string count;
        m_number_of_items = 0;
        for(int item_id = 0; item_id < total_number_of_different_items; item_id++)
        {
            getline(ifile, line, ' ');
            getline(ifile, count);

            int item_size = std::stoi(line);
            int demand_for_type_item_id = stoi(count);

            //float norm_size = (float)item_size / (float)m_maximum_bin_capacity;

            for (int j= 0; j < demand_for_type_item_id; j++)
            {

                m_item_list.push_back(new Item(m_number_of_items, item_size));
                m_number_of_items++;
                //total_num_of_items++;
            }


        }

        // If needed, shuffle the list of items
        // Just to break optimality on handcrafted instances (such as Falkenauer's triplets)
        if (m_are_items_shuffled)
        {
            std::shuffle(m_item_list.begin(), m_item_list.end(), std::default_random_engine(RANDOM_SEED));
        }
    }
    catch (exception& e)
    {
        string s = "Problem while reading instance file to construct an instance object " + filename + ": " + e.what();
        throw runtime_error(s);
    }


}


Instance::~Instance()
{
    for (Item* item : m_item_list)
    {
        if (item != nullptr)
        {
            delete item;
        }
    }
}

const std::string Instance::getInstanceName() const
{
    return m_instance_name;
}

const int Instance::getNumOfDimensions() const
{
    return m_number_of_dimensions;
}

const int Instance::getNumberOfItems() const
{
    return m_number_of_items;
}


const int Instance::getMaxBinCapacity() const
{
    return m_maximum_bin_capacity;
}

const std::vector<Item*>& Instance::getInstanceItems()const
{
    return m_item_list;
}

const bool Instance::areItemsShuffled() const
{
    return m_are_items_shuffled;
}






