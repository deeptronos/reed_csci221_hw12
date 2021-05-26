/*
 * API file for Travelling-Salesperson Cities class and utilities
 */

#pragma once

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>
#include <random>
#include <utility>
#include <cassert>
#include <fstream>
#include <istream>
#include <ostream>


// Representation of an ordering of cities
class Cities {
 public:
  // A pair of integral coordinates for each city
  using coord_t = std::pair<int, int>;

  // An ordering of the cities in cities_t. Each value represents a unique index
  // into the current city ordering.
  using permutation_t = std::vector<unsigned int>;
  Cities() = default;

  ~Cities() = default;

  // Given a permutation, return a new Cities object where the order of the
  // cities reflects the original order of this class after reordering with
  // the given ordering. So for example, the ordering { 1, 0 } simply swaps
  // the first two elements (coordinates) in the new Cities object.
  Cities reorder(const permutation_t& ordering) const;

  // For a given permutation of the cities in this object,
  // compute how long (distance) it would take to traverse all the cities in the
  // order of the permutation, and then returning to the first city.
  // The distance between any two cities is computed as the Euclidean 
  // distance on a plane between their coordinates.
  double total_path_distance(const permutation_t& ordering) const;

  //Returns a new random permutation for ordering of cities.
  static permutation_t random_permutation(unsigned len);

  //Accessor method that returns length of cityList
  unsigned get_length() const{
    return cityList.size();
  }

  unsigned size() const{ //Eitan's implementation
    return cityList.size();
  }

  //////////////////////////////////Operator Overloads//////////////////////////////////
  friend std::istream& operator>> (std::istream& input, Cities& city) {
    int x, y;                                                            // declare coordinates
    while(!input.eof()){                                     
        input >> x;                                                      // get the x-coordinate                                          
        input >> y;                                                      // get the y-coordinate
        auto coords = std::make_pair(x, y);                              // make a std::pair out of extracted numbers
        city.cityList.push_back(coords);                                 // push back coordinate pair in cityList                             
    }
    city.cityList.pop_back();                                            // using eof-based loop results in duplicate of last element; remove last element
    return input;                                                        // return the input stream
  }

  friend std::ostream& operator<< (std::ostream& output, Cities& city) {
    for(auto coords : city.cityList){
    // store the x-coordinate, followed by a space, and then the y-coordinate into the output stream 
      output << coords.first << " " << coords.second << std::endl;                                                                                        
    }
    return output;
   }

  // Outputs cityList of a Cities object to the console
   friend void output_list(Cities& city) {
        for(auto coords : city.cityList){
            std::cout << coords.first << ", " << coords.second << std::endl;
        }
   }

 
 private:
  std::vector<coord_t> cityList;
  permutation_t travelOrder;
};
