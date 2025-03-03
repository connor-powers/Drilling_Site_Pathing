#include <iostream>
#include "ManualRewardMap.h"

int main(){
    
    std::pair<double,double> site_1_coords={0.8,0.8}; //(x,y)
    std::pair<double,double> site_2_coords={0.1,0.9};
    std::pair<double,double> site_3_coords={1.5,0.1};

    double site_1_reward_val=0.8;
    double site_2_reward_val=0.2;
    double site_3_reward_val=0.4;

    site_obj manual_site_1;
    manual_site_1.coordinates=site_1_coords;
    manual_site_1.reward_val=site_1_reward_val;

    site_obj manual_site_2;
    manual_site_2.coordinates=site_2_coords;
    manual_site_2.reward_val=site_2_reward_val;

    site_obj manual_site_3;
    manual_site_3.coordinates=site_3_coords;
    manual_site_3.reward_val=site_3_reward_val;

    std::vector<site_obj> list_of_sites={manual_site_1,manual_site_2,manual_site_3};

    std::pair<double,double> starting_position={0,0};

    ManualRewardMap manual_reward_map(list_of_sites);
    manual_reward_map.map_side_length_x_=21;
    manual_reward_map.map_side_length_y_=21;
    manual_reward_map.max_x_=2;
    manual_reward_map.max_y_=2;


    manual_reward_map.set_initial_position(starting_position);

    manual_reward_map.generate_map();
    manual_reward_map.draw_map();

    // manual_reward_map.generate_paths();

    // manual_reward_map.plot_paths();





    return 0;
}