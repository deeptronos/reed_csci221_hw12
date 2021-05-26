/*
 * Implementation for Chromosome class
 */


#include "chromosome.hh"


//////////////////////////////////////////////////////////////////////////////
// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const Cities* cities_ptr)
  : cities_ptr_(cities_ptr),
    order_(Cities::random_permutation(cities_ptr->size()))
{
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine randEngine (seed1);
	generator_ = randEngine;
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Clean up as necessary
Chromosome::~Chromosome()
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Perform a single mutation on this chromosome
void
Chromosome::mutate()
{
  std::uniform_int_distribution<int> distr(0, order_.size() - 1);
  //Swap random Values in order_ 
  auto randVal = distr(generator_);
  auto randVal2 = distr(generator_);
  std::iter_swap(order_.begin() + randVal, order_.begin() + randVal2);
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Return a pair of offsprings by recombining with another chromosome
// Note: this method allocates memory for the new offsprings
std::pair<Chromosome*, Chromosome*>
Chromosome::recombine(const Chromosome* other)
{
  assert(is_valid());
  assert(other->is_valid());

  std::uniform_int_distribution<int> distr(1, order_.size());
  int rand = distr(generator_);
  //Slice from 0 - random index for first child, and from random index to end for the second
  auto child1 = create_crossover_child(this, other, 0, rand );
  auto child2 = create_crossover_child(other, this, rand, order_.size() - 1);
  child1->mutate();                                                            
  child2->mutate();                                                            
  std::pair<Chromosome*, Chromosome*> family = std::make_pair(child1, child2);          

	assert(is_valid());
	assert(other->is_valid());

  return family;
}

 

//////////////////////////////////////////////////////////////////////////////
// For an ordered set of parents, return a child using the ordered crossover.
// The child will have the same values as p1 in the range [b,e),
// and all the other values in the same order as in p2.
Chromosome*
Chromosome::create_crossover_child(const Chromosome* p1, const Chromosome* p2,
                                   unsigned b, unsigned e) const
{
  Chromosome* child = p1->clone();

  // We iterate over both parents separately, copying from parent1 if the
  // value is within [b,e) and from parent2 otherwise
  unsigned i = 0, j = 0;

  for ( ; i < p1->order_.size() && j < p2->order_.size(); ++i) {
    if (i >= b && i < e) {
      child->order_[i] = p1->order_[i];
    }
    else { // Increment j as long as its value is in the [b,e) range of p1
      while (p1->is_in_range(p2->order_[j], b, e)) {
        ++j;
      }
      assert(j < p2->order_.size());
      child->order_[i] = p2->order_[j];
      j++;
    }
  }

  assert(child->is_valid());
  return child;
}

// Return a positive fitness value, with higher numbers representing
// fitter solutions (shorter total-city traversal path).
double
Chromosome::get_fitness() const
{
  return -1*calculate_total_distance();
}

// A chromsome is valid if it has no repeated values in its permutation,
// as well as no indices above the range (length) of the chromosome.
// We implement this check with a sort, which is a bit inefficient, but simple
bool
Chromosome::is_valid() const {

	std::vector<bool> seen_vec; // a vector of values seen
	seen_vec.resize(order_.size(), false); //sets all elements to false

	for (long unsigned int i = 0; i < order_.size(); i++) {
		if (seen_vec[order_[i]] == true) { // if seen before
			return false;
		} else {
			seen_vec[order_[i]] = true; // marks as now seen
		}
	}

	return true; //runs through list succesfully

}

// Find whether a certain value appears in a given range of the chromosome.
// Returns true if value is within the specified the range specified
// [begin, end) and false otherwise.
bool
Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
{
	for (auto i = begin; i < end; i++) {
		if (this->order_[i] == value) {
			return true;                                                                // scan through the list, if value is in list return true
		}
	}
	return false;                                                                       // else return false
}
