#include <iostream>
#include "RandomRewardMap.h"


int main(){

    //First, set parameters of the random map
    double map_side_length_x=21; //in pixels
    double map_side_length_y=21;
    double min_x=0;
    double max_x=1;
    double min_y=0;
    double max_y=1;
    double reward_range_min=0;
    double reward_range_max=3;
    std::pair<double,double> initial_position={0,0};
    RandomRewardMap example_map(map_side_length_x,map_side_length_y,
        min_x,max_x,
        min_y,max_y,
        reward_range_min,reward_range_max);

    //uncomment this to see map drawn before any kind of pathing takes place
    // example_map.draw_map();

    //Identify potentially interesting sites
    double site_reward_val_threshold;

    //uncomment this block to see example of limiting the number of identified sites
    // int num_sites=5;
    // site_reward_val_threshold=0.65;
    // example_map.identify_potential_sites(num_sites, site_reward_val_threshold);
    // //Since list_of_sites_ is already sorted in decreasing order of reward val, 
    // //can directly pass it here to get descending priority method path
    // std::vector<site> list_of_identified_sites=example_map.get_list_of_sites();
    // example_map.draw_map_with_paths(list_of_identified_sites,"DescendingPriority"); 


    site_reward_val_threshold=2.7;
    //When no num_sites is passed to identify_potential_sites, it includes all sites whose reward vals meet or exceed the threshold
    example_map.identify_potential_sites(site_reward_val_threshold);

    std::vector<site> new_list_of_identified_sites=example_map.get_list_of_sites();
    example_map.draw_map_with_paths(new_list_of_identified_sites,"DescendingPriority"); 
    double descending_priority_distance=calculate_total_distance_from_sequence(example_map.starting_position_,new_list_of_identified_sites);
    std::cout << "Total distance of descending priority method: " << descending_priority_distance<< "\n";

    //Now I'll demonstrate paths using a nearest-neighbor method with varying distance weight


    double distance_weight_1=0.1;
    std::pair<std::vector<site>,double>  NN_paths_and_distance_1=example_map.generate_paths_distance_weighted_NN(distance_weight_1);
    std::vector<site> NN_paths_1=NN_paths_and_distance_1.first;
    double NN_distance_1=NN_paths_and_distance_1.second;
    example_map.draw_map_with_paths(NN_paths_1,"Weighted_NN",distance_weight_1);
    std::cout << "Total distance of NN method with distance weight = " << distance_weight_1 << ": " << NN_distance_1 << "\n";
    

    double distance_weight_2=100;
    std::pair<std::vector<site>,double>  NN_paths_and_distance_2=example_map.generate_paths_distance_weighted_NN(distance_weight_2);
    std::vector<site> NN_paths_2=NN_paths_and_distance_2.first;
    double NN_distance_2=NN_paths_and_distance_2.second;
    example_map.draw_map_with_paths(NN_paths_2,"Weighted_NN",distance_weight_2);
    std::cout << "Total distance of NN method with distance weight = " << distance_weight_2 << ": " << NN_distance_2 << "\n";

    //Let's try sweeping this distance_weight and see if there's a noticeable minimum of the total distance
    double weight_increment=0.001;
    size_t num_weight_increments=10000;
    example_map.plot_NN_total_distance_swept_distance_weight(weight_increment,num_weight_increments);
    

    return 0;
}