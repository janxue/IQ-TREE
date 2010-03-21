//
// C++ Interface: alignment
//
// Description: 
//
//
// Author: BUI Quang Minh, Steffen Klaere, Arndt von Haeseler <minh.bui@univie.ac.at>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include <vector>
#include "pattern.h"
#include "ncl/ncl.h"
#include "tools.h"


const char STATE_UNKNOWN = 127;
const char STATE_INVALID = 128;
const int NUM_CHAR = 256;


#ifdef USE_HASH_MAP
/*
	Define the hash function of Split
*/
#if defined(WIN32) 
namespace stdext {
#else
namespace __gnu_cxx {
#endif

	template<>
	struct hash<string> {
		size_t operator()(string str) const {
			hash<const char*> hash_str;
			return hash_str(str.c_str());
		}
	};
} // namespace __gnu_cxx
#endif


#ifdef USE_HASH_MAP
typedef hash_map<string, int> PatternIntMap;
//typedef map<string, int> PatternIntMap;
#else
typedef map<string, int> PatternIntMap;
#endif
/**
Multiple Sequence Alignment. Stored by a vector of site-patterns

	@author BUI Quang Minh, Steffen Klaere, Arndt von Haeseler <minh.bui@univie.ac.at>
*/
class Alignment : public vector<Pattern>
{
public:

	/**
		constructor
	*/
    Alignment();

	/**
		constructor
		@param filename file name
		@param intype (OUT) input format of the file
	*/
    Alignment(char *filename, InputType &intype);

	/**
		read the alignment in NEXUS format
		@param filename file name
		@return 1 on success, 0 on failure
	*/
	int readNexus(char *filename);


	/**
		read the alignment in PHYLIP format
		@param filename file name
		@return 1 on success, 0 on failure
	*/
	int readPhylip(char *filename);

	/**
		extract the alignment from a nexus data block, called by readNexus()
		@param data_block data block of nexus file
	*/
    void extractDataBlock(NxsCharactersBlock *data_block);

	/**
		destructor
	*/	
    virtual ~Alignment();

	/**
		@return number of sequences
	*/
	int getNSeq() { return seq_names.size(); }


	/**
		@return number of sites (alignment columns)
	*/
	int getNSite() { return site_pattern.size(); }


	/**
		 @return number of patterns
	*/
	inline int getNPattern() { return size(); }

	/**
		@param i sequence index
		@return sequence name
	*/
	string &getSeqName(int i);

	/**
		@param seq_name sequence name
		@return corresponding ID, -1 if not found
	*/
	int getSeqID(string &seq_name);

	/**
		compute the observed distance (number of different pairs of positions per site) 
			between two sequences
		@param seq1 index of sequence 1
		@param seq2 index of sequence 2
		@return the observed distance between seq1 and seq2 (between 0.0 and 1.0)
	*/
	double computeObsDist(int seq1, int seq2);

	/**
		@param seq1 index of sequence 1
		@param seq2 index of sequence 2
		@return Juke-Cantor correction distance between seq1 and seq2
	*/
	double computeJCDist(int seq1, int seq2);

	/**
		abstract function to compute the distance between 2 sequences. The default return
		Juke-Cantor corrected distance.
		@param seq1 index of sequence 1
		@param seq2 index of sequence 2		
		@return any distance between seq1 and seq2
	*/
	virtual double computeDist(int seq1, int seq2) { return computeJCDist(seq1, seq2); }

	/**
		compute distance matrix, assume dist_mat is allocated by memory of size num_seqs * num_seqs.
		@param dist_mat (OUT) distance matrix between all pairs of sequences
	*/
	void computeDist(double *dist_mat);

	/**
		write distance matrix into a file in PHYLIP distance format
		@param file_name distance file name
		@param dist_mat distance matrix
	*/
	void printDist(const char *file_name, double *dist_mat);

	/**
		write distance matrix into a stream in PHYLIP distance format
		@param out output stream
		@param dist_mat distance matrix
	*/
	void printDist(ostream &out, double *dist_mat);

	/**
		read distance matrix from a file in PHYLIP distance format
		@param file_name distance file name
		@param dist_mat distance matrix
	*/
	void readDist(const char *file_name, double *dist_mat);

	/**
		read distance matrix from a stream in PHYLIP distance format
		@param in input stream
		@param dist_mat distance matrix
	*/
	void readDist(istream &in, double *dist_mat);

	/**
		compute empirical state frequencies from the alignment
		@param state_freq (OUT) is filled with state frequencies, assuming state_freq was allocated with 
			at least num_states entries.
	*/
	void computeStateFreq(double *state_freq);

	/**
		compute empirical rates between state pairs
		@param rates (OUT) vector of size num_states*(num_states-1)/2 for the rates
	*/
	void computeEmpiricalRate (double *rates);

	/**
		count the fraction of constant sites in the alignment, update the variable frac_const_sites
	*/
	void countConstSite();

	/**
		number of states
	*/
	int num_states;

	/**
		fraction of constant sites
	*/
	double frac_const_sites;

	/**
		add a pattern into the alignment
		@param pat the pattern
		@param site the site index of the pattern from the alignment
	*/
	void addPattern(Pattern &pat, int site);


private:

	/**
		sequence names
	*/
	vector<string> seq_names;

	/**
		Site to pattern index
	*/
	vector<int> site_pattern;

	/**
		hash map from pattern to index in the vector of patterns (the alignment)
	*/
	PatternIntMap pattern_index;

	/**
		get the appearance for a state, helpful for ambigious states
		@param state the state index
		@param state_app (OUT) state appearance
	*/
	void getAppearance(char state, double *state_app);
};

#endif
