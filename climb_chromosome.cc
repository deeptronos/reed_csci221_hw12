#include "climb_chromosome.hh"

ClimbChromosome::ClimbChromosome(const Cities * cities_ptr) : Chromosome(cities_ptr) // Constructor of derived class calls constructor of parent
{
// IS THIS CODE NECESSARY? If we're calling the Chromosome constructor, will it automatically do all of this stuff?
	cities_ptr_ = cities_ptr;
	order_ = Cities::random_permutation(cities_ptr->size());
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine randEngine (seed1);
	generator_ = randEngine;
	assert(is_valid());
}

ClimbChromosome::~ClimbChromosome() {
	assert(is_valid());
}

void ClimbChromosome::mutate() {

	//Evaluate fitness of original chromosome
	auto original_fitness = get_fitness();

	std::uniform_int_distribution<int> distr(0, order_.size() - 1);

	//Pick a random point p in the chromosme
	auto p = distr(generator_);

	//Clone original chromosome for swap
	auto left_swapped = this->clone();

	if(p == 0){
		//If random index is 0, swap it with the last element of order_

		std::swap(left_swapped->order_[0], left_swapped->order_[ (left_swapped->order_.size() - 1) ]);
	}else {
		//Else swap element at p with element at p - 1

		std::swap(left_swapped->order_[p], left_swapped->order_[ (p-1) ]);

	}
	auto left_fitness = left_swapped->get_fitness();

	//Clone original chromosome for second swap
	auto right_swapped = this->clone();

	if(static_cast<unsigned long>(p) == right_swapped->get_ordering().size() - 1){ //get_ordering.size() returns an unsigned long, so we must cast p, an int, to be comparable with it
		//If random index is last element, swap it with the first element

		std::swap(right_swapped->order_[0], right_swapped->order_[(right_swapped->order_.size() - 1)]);
	}else {
		//Else swap element p with element p + 1

		std::swap(right_swapped->order_[p], right_swapped->order_[ (p+1) ]);
	}

	auto right_fitness = right_swapped->get_fitness();

	//Determine the fittest of the 3 possibilities
	double max = original_fitness;
	if(left_fitness > max){
		max = left_fitness;
	}
	if(right_fitness > max){
		max = right_fitness;
	}

	if(max == original_fitness){
		//order_ is unchanged
		delete right_swapped;
		delete left_swapped;
		return;
	}else if(max == right_fitness){
		//right_swapped is fittest
		order_ = right_swapped->get_ordering();     //Might be issue with get ordering returning an immutable reference?
		delete right_swapped;
		delete left_swapped;
		return;
	}else if(max == left_fitness){
		//left_swapped is fittest
		order_ = left_swapped->get_ordering();
		delete right_swapped;
		delete left_swapped;
		return;
	}
	delete right_swapped;
	delete left_swapped;
	return;
}

//The new mutation operator we’ll try is called “local hill climbing”, because it searches for the best solution in
// a small local “neighborhood” of the current chromosome. It works as follows (assuming N cities, i.e. a chromosome with N genes):
	// A. evaluate the fitness of the current (original) chromosome.
	// B. Pick a random point p in the chromosome.
	// C. Swap the city at index p with the city at index p-1 and evaluate the resulting chromosome's fitness. If p==0, swap with the city at index N-1 instead of at index p-1.
	// D. Swap the city at index p (from the original chromosome) with the city at index p+1 and evaluate the resulting chromosome's fitness. If p==N-1, swap with the city at index 0 instead of at index N. (hint: use modulus).
	// E. Find which of the three chromosomes (i.e., chromosomes from steps A, C, and D) produced the fittest ordering and choose that fittest chromosome as the resulting mutated chromosome.
//void ClimbChromosome::mutate() {
//	double original_fitness = get_fitness(); // Get initial fitness of chromosome.
//
//	std::uniform_int_distribution<int> distr(0, order_.size() - 1); // For picking a random point .
//
//	int p = distr(generator_); // Pick a random point p in the chromosome.
//
//	auto left_swapped = clone();
//
//
//}


