/*
Climb_chromosme inherits from chromsome
*/
#pragma once
#include "chromosome.hh"

class ClimbChromosome : public Chromosome {

public:
//Implement the derived class' constructor
ClimbChromosome(const Cities*);

//Always explicitly write a virtual destructor
virtual ~ClimbChromosome();

//Override clone
ClimbChromosome* clone() const override {
	auto chromo = new ClimbChromosome(cities_ptr_);
	return chromo;
}

//virtual void mutate() override;

//The new mutation operator we’ll try is called “local hill climbing”, because it searches for the best solution in
// a small local “neighborhood” of the current chromosome. It works as follows (assuming N cities, i.e. a chromosome with N genes):
	// A. evaluate the fitness of the current (original) chromosome.
	// B. Pick a random point p in the chromosome.
	// C. Swap the city at index p with the city at index p-1 and evaluate the resulting chromosome's fitness. If p==0, swap with the city at index N-1 instead of at index p-1.
	// D. Swap the city at index p (from the original chromosome) with the city at index p+1 and evaluate the resulting chromosome's fitness. If p==N-1, swap with the city at index 0 instead of at index N. (hint: use modulus).
	// E. Find which of the three chromosomes (i.e., chromosomes from steps A, C, and D) produced the fittest ordering and choose that fittest chromosome as the resulting mutated chromosome.

//Overide mutate
virtual void mutate() override;





};
