#include <iostream>
#include "ManualRewardMap.h"
#include <random>

int main(){

    ManualRewardMap manual_reward_map;

    //Site parameters can be entered manually or randomly generated. 
    //Demonstrations of workflows with manually-entered site parameters:

    //Explicitly creating site objects:
    std::vector<site_obj> site_list={};
    
    std::pair<double,double> site_1_coords={0.8,0.8}; //(x,y)
    std::pair<double,double> site_2_coords={0.1,0.9};

    double site_1_reward_val=0.8;
    double site_2_reward_val=0.2;

    site_obj manual_site_1;
    manual_site_1.coordinates=site_1_coords;
    manual_site_1.reward_val=site_1_reward_val;
    site_list.push_back(manual_site_1);

    site_obj manual_site_2;
    manual_site_2.coordinates=site_2_coords;
    manual_site_2.reward_val=site_2_reward_val;
    site_list.push_back(manual_site_2);

    manual_reward_map.add_site(manual_site_1);
    manual_reward_map.add_site(manual_site_2);


    //You can also add sites by just entering their coordinates and reward value. Format: (x,y,reward_val)
    manual_reward_map.add_site(1.5,0.1,0.4);
    manual_reward_map.add_site(1.0,0.3,0.9);


    //You can also randomly generate sites
    std::random_device test_random_device;
    std::mt19937 engine(test_random_device());
    double sites_x_lower_bound=-1;
    double sites_x_upper_bound=0.8;
    double sites_y_lower_bound=0.2;
    double sites_y_upper_bound=0.8;
    double sites_reward_val_lower_bound=0.3;
    double sites_reward_val_upper_bound=1;

    std::uniform_real_distribution<> site_x_distribution(sites_x_lower_bound,sites_x_upper_bound);
    std::uniform_real_distribution<> site_y_distribution(sites_y_lower_bound,sites_y_upper_bound);
    std::uniform_real_distribution<> site_reward_val_distribution(sites_reward_val_lower_bound,sites_reward_val_upper_bound);

    //Now one can still explicitly create and add site objects:


    std::pair<double,double> site_5_coords;
    site_5_coords.first=site_x_distribution(engine);
    site_5_coords.second=site_y_distribution(engine);


    std::pair<double,double> site_6_coords;
    site_6_coords.first=site_x_distribution(engine);
    site_6_coords.second=site_y_distribution(engine);

    double site_5_reward_val=site_reward_val_distribution(engine);
    double site_6_reward_val=site_reward_val_distribution(engine);

    site_obj manual_site_5;
    manual_site_5.coordinates=site_5_coords;
    manual_site_5.reward_val=site_5_reward_val;

    site_obj manual_site_6;
    manual_site_6.coordinates=site_6_coords;
    manual_site_6.reward_val=site_6_reward_val;

    manual_reward_map.add_site(manual_site_5);
    manual_reward_map.add_site(manual_site_6);

    //Or add the sites directly via generated values
    manual_reward_map.add_site(site_x_distribution(engine),site_y_distribution(engine),site_reward_val_distribution(engine));
    manual_reward_map.add_site(site_x_distribution(engine),site_y_distribution(engine),site_reward_val_distribution(engine));

    //Set the starting point of the path
    std::pair<double,double> starting_position={0,0};

    //Set map parameters as desired
    manual_reward_map.map_side_length_x_=51;
    manual_reward_map.map_side_length_y_=51;
    manual_reward_map.max_x_=1;
    manual_reward_map.max_y_=1;


    manual_reward_map.set_initial_position(starting_position);

    manual_reward_map.generate_map();


    // manual_reward_map.draw_map(); //uncomment to draw map without paths


    std::pair<std::vector<site_obj>,double>  paths_and_distance_descending_priority=manual_reward_map.generate_paths_descending_priority();
    std::vector<site_obj> paths_descending_priority=paths_and_distance_descending_priority.first;
    double distance_descending_priority=paths_and_distance_descending_priority.second;
    manual_reward_map.draw_map_with_paths(paths_descending_priority,"DescendingPriority");
    std::cout << "Total distance of descending priority method: " << distance_descending_priority << "\n";



    double distance_weight_med=1;
    std::pair<std::vector<site_obj>,double>  paths_and_distance_NN_midweight=manual_reward_map.generate_paths_distance_weighted_NN(distance_weight_med);
    std::vector<site_obj> paths_NN_midweight=paths_and_distance_NN_midweight.first;
    double distance_NN_midweight=paths_and_distance_NN_midweight.second;
    manual_reward_map.draw_map_with_paths(paths_NN_midweight,"Weighted_NN",distance_weight_med);
    std::cout << "Total distance of NN method with distance weight = " << distance_weight_med << ": " << distance_NN_midweight << "\n";


    double distance_weight_high=100;
    std::pair<std::vector<site_obj>,double>  paths_and_distance_NN_highweight=manual_reward_map.generate_paths_distance_weighted_NN(distance_weight_high);
    std::vector<site_obj> paths_NN_highweight=paths_and_distance_NN_highweight.first;
    double distance_NN_highweight=paths_and_distance_NN_highweight.second;
    manual_reward_map.draw_map_with_paths(paths_NN_highweight,"Weighted_NN",distance_weight_high);
    std::cout << "Total distance of NN method with distance weight = " << distance_weight_high << ": " << distance_NN_highweight << "\n";


    //Let's try sweeping this distance_weight and see if there's a noticeable minimum of the total distance
    double weight_increment=0.001;
    size_t num_weight_increments=100000;
    manual_reward_map.plot_NN_total_distance_swept_distance_weight(weight_increment,num_weight_increments);


    return 0;
}