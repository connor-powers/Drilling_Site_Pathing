#include <iostream>
#include "ManualRewardMap.h"

int main(){
    
    std::pair<double,double> site_1_coords={0.8,0.8}; //(x,y)
    std::pair<double,double> site_2_coords={0.1,0.9};
    std::pair<double,double> site_3_coords={1.5,0.1};
    std::pair<double,double> site_4_coords={1.0,0.3};
    std::pair<double,double> site_5_coords={0.2,1.8};
    std::pair<double,double> site_6_coords={1.7,1.9};

    double site_1_reward_val=0.8;
    double site_2_reward_val=0.2;
    double site_3_reward_val=0.4;
    double site_4_reward_val=0.9;
    double site_5_reward_val=0.71;
    double site_6_reward_val=0.99;

    site_obj manual_site_1;
    manual_site_1.coordinates=site_1_coords;
    manual_site_1.reward_val=site_1_reward_val;

    site_obj manual_site_2;
    manual_site_2.coordinates=site_2_coords;
    manual_site_2.reward_val=site_2_reward_val;

    site_obj manual_site_3;
    manual_site_3.coordinates=site_3_coords;
    manual_site_3.reward_val=site_3_reward_val;

    site_obj manual_site_4;
    manual_site_4.coordinates=site_4_coords;
    manual_site_4.reward_val=site_4_reward_val;

    site_obj manual_site_5;
    manual_site_5.coordinates=site_5_coords;
    manual_site_5.reward_val=site_5_reward_val;

    site_obj manual_site_6;
    manual_site_6.coordinates=site_6_coords;
    manual_site_6.reward_val=site_6_reward_val;

    std::vector<site_obj> list_of_sites={manual_site_1,manual_site_2,manual_site_3,manual_site_4,manual_site_5,manual_site_6};

    std::pair<double,double> starting_position={0,0};

    ManualRewardMap manual_reward_map(list_of_sites);
    manual_reward_map.map_side_length_x_=21;
    manual_reward_map.map_side_length_y_=21;
    manual_reward_map.max_x_=2;
    manual_reward_map.max_y_=2;


    manual_reward_map.set_initial_position(starting_position);

    manual_reward_map.generate_map();

    // manual_reward_map.draw_map();

    std::pair<std::vector<site_obj>,double>  paths_and_distance_1=manual_reward_map.generate_paths_descending_priority();
    std::vector<site_obj> paths_1=paths_and_distance_1.first;
    double distance_1=paths_and_distance_1.second;
    manual_reward_map.draw_map_with_paths(paths_1);
    std::cout << "Total distance of descending priority method: " << distance_1 << "\n";


    double distance_weight_1=0.01;
    std::pair<std::vector<site_obj>,double>  paths_and_distance_2=manual_reward_map.generate_paths_distance_weighted_NN(distance_weight_1);
    std::vector<site_obj> paths_2=paths_and_distance_2.first;
    double distance_2=paths_and_distance_2.second;
    manual_reward_map.draw_map_with_paths(paths_2);
    std::cout << "Total distance of NN method with distance weight = " << distance_weight_1 << ": " << distance_2 << "\n";


    double distance_weight_2=1;
    std::pair<std::vector<site_obj>,double>  paths_and_distance_3=manual_reward_map.generate_paths_distance_weighted_NN(distance_weight_2);
    std::vector<site_obj> paths_3=paths_and_distance_3.first;
    double distance_3=paths_and_distance_3.second;
    manual_reward_map.draw_map_with_paths(paths_3);
    std::cout << "Total distance of NN method with distance weight = " << distance_weight_2 << ": " << distance_3 << "\n";


    double distance_weight_3=100;
    std::pair<std::vector<site_obj>,double>  paths_and_distance_4=manual_reward_map.generate_paths_distance_weighted_NN(distance_weight_3);
    std::vector<site_obj> paths_4=paths_and_distance_4.first;
    double distance_4=paths_and_distance_4.second;
    manual_reward_map.draw_map_with_paths(paths_4);
    std::cout << "Total distance of NN method with distance weight = " << distance_weight_3 << ": " << distance_4 << "\n";


    //Let's try sweeping this distance_weight and see if there's a noticeable minimum of the total distance
    double weight_increment=0.001;
    size_t num_weight_increments=6000;
    manual_reward_map.plot_NN_total_distance_swept_distance_weight(weight_increment,num_weight_increments);


    return 0;
}