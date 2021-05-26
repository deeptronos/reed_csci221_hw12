

#include "tournament_deme.hh"

TournamentDeme::TournamentDeme(const Cities *cities_ptr, unsigned int pop_size, double mut_rate) : Deme(cities_ptr,
                                                                                                        pop_size,
                                                                                                        mut_rate) {
	// Do we need to re-implement the Deme constructor here, or will referencing the parent constructor do that for us?
			// AFAIK, we don't need to re-implement the constructor. When the code below was un-commented, the pop_ wound up being 2X the specified population parameter :P
//	// Make sure mutation rate is within the range [0-1].
//	if(mut_rate > 1.0 || mut_rate < 0.0){throw std::invalid_argument("Invalid mutation rate for a Deme. Ensure it's between 0 - 1.0.");}
//
//	mut_rate_ = mut_rate;
//	for(unsigned i = 0; i < pop_size; ++i){
//		pop_.push_back(new ClimbChromosome(cities_ptr)); // Add a newly-generated Chromosome to pop_.
//	}

}

TournamentDeme::~TournamentDeme()  = default;

Chromosome *TournamentDeme::select_parent() { // Currently, this select_parent() uses ClimbChromosome, though we are permitted to use Chromosome if we choose!
	// Amount of parents we're going to take.
	unsigned P = 16; // P must be a constant power of 2, and must be <= the size of the population, otherwise issues with this method may occur.
	if(P > pop_.size()){throw std::logic_error("Tournament P is too great. Ensure it's between 0 and the size of the TournamentDeme's population.");}

	std::vector<Chromosome*> parent_pop(P); // Make a vector for our parents, of size P.

	std::sample(pop_.begin(), pop_.end(), parent_pop.begin(), P, generator_); //  Take a random sample P-times and put them into parent_pop.
		// Make sure parent_pop.begin() actually allows std::sample to output to parent_pop

	while(parent_pop.size() > 1) {
		for (unsigned i = 0; i < parent_pop.size(); i += 2) { // Step through parent_pop in steps of 2.

			double competitor1 = parent_pop[i]->get_fitness(), competitor2 = parent_pop[i+1]->get_fitness(); // Keep fitnesses in a 2 variables so that we don't have to run get_fitness for each competitor more than once.

			if (competitor1 > competitor2) { // If i-th element's fitness > i+1-th element's fitness...
				parent_pop[i + 1] = nullptr; // Set the entry at i+1 to nullptr.

			} else { // If either competitor1 == competitor2, or competitor1 < competitor2...

				parent_pop[i] = nullptr;
			}
			// Note: We don't delete the inferior element quite yet, because that would cause issues to the loop. Thus, we replace them with nullptr.
		}
		std::vector<Chromosome*>::iterator victorRangeEnd; // Iterator that stores the position of the last element
		victorRangeEnd = std::remove(parent_pop.begin(), parent_pop.end(), nullptr); // Remove all nullptr from parent_pop.
		parent_pop.erase(victorRangeEnd, parent_pop.end()); // Erase everything from victorRangeEnd to parent_pop.end()
	}
	return parent_pop[0];
}
