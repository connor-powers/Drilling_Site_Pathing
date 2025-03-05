#ifndef UTILS_H
#define UTILS_H

#include <iostream>

struct site_obj
{
    std::pair<double,double> coordinates;
    double reward_val;

    bool operator < (const site_obj input_site) const {
        return (reward_val<input_site.reward_val);
    }
    bool operator == (const site_obj input_site) const {
        return ((coordinates.first==input_site.coordinates.first)&&(coordinates.second==input_site.coordinates.second));
    }
};


double weighted_cost_function(double input_site_reward_function, double input_distance_to_site, double input_distance_weight);

double distance(std::pair<double,double> input_coords_1, std::pair<double,double> input_coords_2);

double calc_cost_function_from_position_to_site(std::pair<double,double> current_position,site_obj target_site, double input_distance_weight);

double calculate_total_distance_from_sequence(std::pair<double,double> input_starting_position, std::vector<site_obj> input_site_sequence);

#endif