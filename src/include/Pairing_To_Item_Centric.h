//
// Created by weldeab solomon on 12/03/2023.
//

#ifndef INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_TO_ITEM_CENTRIC_H
#define INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_TO_ITEM_CENTRIC_H




#include "base_algorithm.h"
#include "pairing_algo_variants.h"

// a base class for two stage algorithms that construct a solution by invoking one of Multi-bin Pairing
// algorithms first , followed by one of item-centric algorithms.

class PairingToItemCentric : public BaseAlgorithm
{
public:
    PairingToItemCentric(std::string algo_name, const Instance &instance);


    // This method generates a list of breakpoints and calls the trySolveTwoStage algorithm  for every breakpoint.
    // it keeps track of the best solution
    // it returns the number of bins in the best solution
    virtual int solveInstWith2SA(float b_factor, int LB, int UB);

    virtual unsigned long solveInstWithIC(int hint_nb_bins = 0);
    virtual bool tryToSolve(int m_bins);


protected:

    // returns the number of bins used in a solution
    // attempts to solve a given instance with for a given breakpoint
    virtual int trySolveTwoStage(int breakpoint);

    // uses an item-centric algorithm to pack the items until the threshold is reached
    void solveItemCentric(std::vector<Item*>::iterator last_it, std::vector<Item*>::iterator end_it);

    //  uses a multi-pairing algorithm to pack the rest of items
    int solvePairing(int breakpoint,std::vector<Item*>::iterator start_it, std::vector<Item*>::iterator end_it);

    void updateScores(Bin* bin, std::vector<Item*>::iterator first_item, std::vector<Item*>::iterator end_it);

    float computeItemBinScore(Item* item, Bin* bin);



    void sortBins();
    void sortItems();



protected:
    bool m_is_FFD_type;
    bool m_is_BFD_type;
    bool m_is_WFD_type;
    SCORE m_score;

    std::vector<std::vector<float>> m_bin_item_scores;

};


/* ================================================ */
/* ================================================ */


class Pairing_PRODUCT_To_FFD : public PairingToItemCentric
{
public:
    Pairing_PRODUCT_To_FFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_SLACK_To_FFD : public PairingToItemCentric
{
public:
    Pairing_SLACK_To_FFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_TIGHT_FILL_To_FFD : public PairingToItemCentric
{
public:
    Pairing_TIGHT_FILL_To_FFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */



class Pairing_PRODUCT_To_BFD : public PairingToItemCentric
{
public:
    Pairing_PRODUCT_To_BFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_SLACK_To_BFD : public PairingToItemCentric
{
public:
    Pairing_SLACK_To_BFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_TIGHT_FILL_To_BFD : public PairingToItemCentric
{
public:
    Pairing_TIGHT_FILL_To_BFD (std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */


class Pairing_PRODUCT_To_WFD : public PairingToItemCentric
{
public:
    Pairing_PRODUCT_To_WFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_SLACK_To_WFD : public PairingToItemCentric
{
public:
    Pairing_SLACK_To_WFD(std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */

class Pairing_TIGHT_FILL_To_WFD : public PairingToItemCentric
{
public:
    Pairing_TIGHT_FILL_To_WFD (std::string algo_name, const Instance &instance);
};

/* ================================================ */
/* ================================================ */





#endif //INC_1D_TWO_STAGE_BIN_PACKING_ALGORITHMS_PAIRING_TO_ITEM_CENTRIC_H
