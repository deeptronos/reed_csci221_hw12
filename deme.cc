/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include "chromosome.hh"
#include "deme.hh"



//Need to initalize random number generator in the constructor, seed it constantly for debugging purposes

// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
{
	// Make sure mutation rate is within the range [0-1].
	if(mut_rate > 1.0 || mut_rate < 0.0){throw std::invalid_argument("Invalid mutation rate for a Deme. Ensure it's between 0 - 1.0.");}

	mut_rate_ = mut_rate;
	for(unsigned i = 0; i < pop_size; ++i){
		pop_.push_back(new ClimbChromosome(cities_ptr)); // Add a newly-generated ClimbChromosome to pop_.
	}
}

// Clean up as necessary
Deme::~Deme()
{
  for(Chromosome* chrom:pop_){
  	delete chrom;
  }
}

// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select() method below).
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated pop_size new chromosomes, we delete all the old ones.
void Deme::compute_next_generation()
{
  //Vector of chromosome pairs to store mutated chromosmes
  std::vector<Chromosome*> mutated_chromosomes;
  //Initialize random number generator
  //std::random_device rd;
  //generator_ = std::default_random_engine(rd());
  std::uniform_real_distribution<double> distr(0,1);
  for(unsigned long int i = 0; i < pop_.size() / 2; i++){
    //Select 2 parents and two random numbers in range [0,1]
    auto first_parent = select_parent();
    auto second_parent = select_parent();
    double first_rand = distr(generator_);
    double second_rand = distr(generator_);
    

    //If random num <= mutation rate, mutate the assosciated child
    if(first_rand <= mut_rate_){
	    first_parent->mutate();
    }
    if(second_rand <= mut_rate_){
	    second_parent->mutate();
    }
    //Store potentially mutated pair in vector
    auto new_pair = first_parent->recombine(second_parent);
    //Unpack vector of chromosome pairs into vector of chromosomes
    mutated_chromosomes.push_back(new_pair.first);
    mutated_chromosomes.push_back(new_pair.second);
  }
  //release memory
  for (auto& elem:pop_){
	delete elem; 
  }
  pop_ = mutated_chromosomes;

}

// Return a copy of the chromosome with the highest fitness.
bool comp_fit(Chromosome* city_a, Chromosome* city_b)
{ //used for comparisons in get best
	  return city_a->get_fitness() < city_b->get_fitness();
}

//Finds chromosome with best fit
//
const Chromosome* Deme::get_best() const
{ 
	return *std::max_element(pop_.begin(), pop_.end(), comp_fit); 

}

// Function for parameter of std::accumulate, as used in select_parent().
double fitnessAccumulation(double sum, Chromosome* chromosome){
	return sum - chromosome->get_fitness(); // Add the fitness of chromosome to sum and return it.
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
Chromosome* Deme::select_parent()
{
	double sumOfFitness = std::accumulate(pop_.begin(), pop_.end(), 0.0, fitnessAccumulation); // Use STL to find sum.

	//Calculate R.
	std::uniform_real_distribution<double> distribution (0.0, sumOfFitness); // distribution will return a double between 0 and sumOfFitness when called with a generator.
	double R = distribution(generator_); // generate R.


	double P = 0.0; // Initialize P.


	for(Chromosome* chromo:pop_){ // For each chromosome in our population...

		P -= chromo->get_fitness(); // Add the fitness of the chromosome to P.

		if(P >= R){ //If P exceeds R...
			return chromo; // Return the current chromosome!
		}
	}

	return nullptr; // Default return statement :P

}
