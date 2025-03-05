#include <iostream>
#include "ManualRewardMap.h"

double weighted_cost_function(double input_site_reward_function_val, double input_distance_to_site, double input_distance_weight){
    return (-input_site_reward_function_val + input_distance_to_site*input_distance_weight);
}

double distance(std::pair<double,double> input_coords_1, std::pair<double,double> input_coords_2){
    //Euclidean distance between two coordinate pairs
    return std::sqrt(std::pow(input_coords_2.first-input_coords_1.first,2) + std::pow(input_coords_2.second-input_coords_1.second,2));
}

double calc_cost_function_from_position_to_site(std::pair<double,double> current_position,site_obj target_site, double input_distance_weight){

    double calculated_distance=distance(current_position,target_site.coordinates);
    double total_cost=weighted_cost_function(target_site.reward_val,calculated_distance,input_distance_weight);

    return total_cost;

}

double calculate_total_distance_from_sequence(std::pair<double,double> input_starting_position, std::vector<site_obj> input_site_sequence){
    if (input_site_sequence.size()==0){
        return 0;
    }
    //first site
    double calculated_distance=distance(input_starting_position,input_site_sequence.at(0).coordinates);
    double total_distance=calculated_distance;

    std::pair<double,double> current_position=input_site_sequence.at(0).coordinates;
    for (size_t site_ind=1;site_ind<input_site_sequence.size();site_ind++){
        calculated_distance=distance(current_position,input_site_sequence.at(site_ind).coordinates);
        total_distance+=calculated_distance;
        current_position=input_site_sequence.at(site_ind).coordinates;
    }

    return total_distance;
}