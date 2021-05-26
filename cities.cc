#include "cities.hh"


//Calculates distance between two points using sqrt( (x_2 - x_1)^2 + (y_2 - y_1)^2 )
double distance_between(Cities::coord_t start, Cities::coord_t end){
    return std::sqrt( std::pow( (end.first - start.first), 2) + std::pow( (end.second - start.second), 2));
}

//Calculates total distance between points in cityList, in order given by ordering 
double Cities::total_path_distance(const permutation_t& ordering) const{
    if(ordering.size() > 0){
        double total_distance = 0;   
        //Calculate distance between points up to last element
        for (unsigned int i = 0; i < ordering.size() - 1; i++){ 
            total_distance += distance_between(this->cityList[ordering[i]], this->cityList[ordering[i+1]]);                                 
        } 
        //Calculate distance between last point and first point
        total_distance += distance_between(this->cityList[ordering.back()], this->cityList[ordering.front()]);
        return total_distance;  
    }
    return 0;
}
//Return a new cities object where the internal representation of the cities reflects the order as indexed by the permutation. 
//For example, if the first element of the permutation is '3', then the first coordinate pair of the new cities object is the fourth pair in the original object
Cities Cities::reorder(const permutation_t& ordering) const{
    Cities newCity;
    newCity.cityList = this->cityList;
    for (unsigned int i = 0; i < ordering.size(); i++) {
        unsigned int newOrder = ordering[i];
            newCity.cityList[i] = this->cityList[newOrder];
    }
    newCity.travelOrder = ordering;
    return newCity;
}

//Returns a new random permutation of values in range 0 to len - 1
Cities::permutation_t Cities::random_permutation(unsigned len){
    Cities::permutation_t new_order; 
    //Fill vector will all values in range
    for(unsigned i = 0; i < len; i++){
        new_order.push_back(i);
    }
    //Randomly shuffle values in vector and return
    std::random_device rd;
    std::mt19937 mt(rd());
    std::shuffle(new_order.begin(), new_order.end(), mt);
    return new_order;
}