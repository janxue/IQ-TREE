/*
 * ratefree.h
 *
 *  Created on: Nov 3, 2014
 *      Author: minh
 */

#ifndef RATEFREE_H_
#define RATEFREE_H_

#include "rategamma.h"

class RateFree: virtual public RateGamma {
public:
	/**
		constructor
		@param ncat number of rate categories
		@param tree associated phylogenetic tree
        @param opt_alg optimization algorithm (1-BFGS, 2-BFGS, EM)
	*/
    RateFree(int ncat, double start_alpha, string params, bool sorted_rates, string opt_alg, PhyloTree *tree);

	virtual ~RateFree();

	/**
		@return true if this is a Gamma model (default: false)
	*/	
    virtual bool isGammaRate() { return false; }

	/**
	 * @return model name with parameters in form of e.g. GTR{a,b,c,d,e,f}
	 */
	virtual string getNameParams();

	/**
		get the proportion of sites under a specified category.
		@param category category ID from 0 to #category-1
		@return the proportion of the specified category
	*/
	virtual double getProp(int category) { return prop[category]; }

	/**
		the target function which needs to be optimized
		@param x the input vector x
		@return the function value at x
	*/
	virtual double targetFunk(double x[]);

	/**
	 * setup the bounds for joint optimization with BFGS
	 */
	virtual void setBounds(double *lower_bound, double *upper_bound, bool *bound_check);

	/**
		optimize parameters. Default is to optimize gamma shape
		@return the best likelihood
	*/
	virtual double optimizeParameters(double gradient_epsilon);

    /** 
        optimize rate parameters using EM algorithm 
        @return log-likelihood of optimized parameters
    */
    double optimizeWithEM();

	/**
		return the number of dimensions
	*/
	virtual int getNDim();

	/**
		write information
		@param out output stream
	*/
	virtual void writeInfo(ostream &out);

	/**
		write parameters, used with modeltest
		@param out output stream
	*/
	virtual void writeParameters(ostream &out);

    /**
        set number of rate categories
        @param ncat #categories
    */
	virtual void setNCategory(int ncat);

    /**
        initialize rates and prop from rate model with #category less by 1
    */
    void setRateAndProp(RateFree *input);

	/**
	 * used to normal branch lengths if mean rate is not equal to 1 (e.g. FreeRate model)
	 * @return mean rate, default = 1
	 */
	virtual double meanRates();

	/**
	 * rescale rates s.t. mean rate is equal to 1, useful for FreeRate model
	 * @return rescaling factor
	 */
	virtual double rescaleRates();

protected:

	/**
		this function is served for the multi-dimension optimization. It should pack the model parameters
		into a vector that is index from 1 (NOTE: not from 0)
		@param variables (OUT) vector of variables, indexed from 1
	*/
	virtual void setVariables(double *variables);

	/**
		this function is served for the multi-dimension optimization. It should assign the model parameters
		from a vector of variables that is index from 1 (NOTE: not from 0)
		@param variables vector of variables, indexed from 1
	*/
	virtual void getVariables(double *variables);

	/**
	 * proportion of sites for each rate categories
	 */
	double *prop;

	/** TRUE to fix parameters */
	bool fix_params;
    
    /** true to sort rate in increasing order, false otherwise */
    bool sorted_rates;

    /** 0: no, 1: rates, 2: weights */
    int optimizing_params;

    string optimize_alg;

};

#endif /* RATEFREE_H_ */
