#include <iostream>
#include <gtest/gtest.h>
#include "ManualRewardMap.h"
#include <random>


TEST(UnitTests,DescendingVsDistance){
    //For the example below, NN path generated with large distance weight should yield a path with total distance 2, 
    //whereas the descending priority approach should yield a path with total distance 3

    ManualRewardMap manual_reward_map;

    std::pair<double,double> site_1_coords={1,0}; //(x,y)
    std::pair<double,double> site_2_coords={2,0};

    double site_1_reward_val=0.2;
    double site_2_reward_val=0.9;


    site manual_site_1;
    manual_site_1.coordinates=site_1_coords;
    manual_site_1.reward_val=site_1_reward_val;

    site manual_site_2;
    manual_site_2.coordinates=site_2_coords;
    manual_site_2.reward_val=site_2_reward_val;

    manual_reward_map.add_site(manual_site_1);
    manual_reward_map.add_site(manual_site_2);



    std::pair<double,double> starting_position={0,0};

    manual_reward_map.map_side_length_x_=21;
    manual_reward_map.map_side_length_y_=21;
    manual_reward_map.max_x_=2;
    manual_reward_map.max_y_=2;


    manual_reward_map.set_initial_position(starting_position);

    manual_reward_map.generate_map();

    std::pair<std::vector<site>,double>  paths_and_distance_descending_priority=manual_reward_map.generate_paths_descending_priority();
    std::vector<site> paths_descending_priority=paths_and_distance_descending_priority.first;
    double distance_descending_priority=paths_and_distance_descending_priority.second;

    double distance_weight_1=100;
    std::pair<std::vector<site>,double>  paths_and_distance_NN=manual_reward_map.generate_paths_distance_weighted_NN(distance_weight_1);
    std::vector<site> paths_NN=paths_and_distance_NN.first;
    double distance_NN=paths_and_distance_NN.second;
    EXPECT_TRUE(distance_descending_priority==3) << "Descending priority method path length not what was expected. Actual value: " << distance_descending_priority << "\n";
    EXPECT_TRUE(distance_NN==2) << "Nearest neighbor (high distance weight) method path length not what was expected. Actual value: " << distance_NN << "\n";
}



TEST(UnitTests,ZeroWeightDistance){
    //NN path generated with zero distance weight should yield a path equal in length to the one generated by simply going in descending order of priority

    ManualRewardMap manual_reward_map;

    //Going to keep demonstrations of all site-adding workflows (manually entered parameters vs. randomly generated, adding site objects by directly passing site objects or entering (x,y,reward_val)) to make sure none of those break
    std::vector<site> site_list={};
    
    std::pair<double,double> site_1_coords={0.8,0.8}; //(x,y)
    std::pair<double,double> site_2_coords={0.1,0.9};

    double site_1_reward_val=0.8;
    double site_2_reward_val=0.2;

    site manual_site_1;
    manual_site_1.coordinates=site_1_coords;
    manual_site_1.reward_val=site_1_reward_val;
    site_list.push_back(manual_site_1);

    site manual_site_2;
    manual_site_2.coordinates=site_2_coords;
    manual_site_2.reward_val=site_2_reward_val;
    site_list.push_back(manual_site_2);

    manual_reward_map.add_site(manual_site_1);
    manual_reward_map.add_site(manual_site_2);


    manual_reward_map.add_site(1.5,0.1,0.4);
    manual_reward_map.add_site(1.0,0.3,0.9);


    std::random_device test_random_device;
    std::mt19937 engine(test_random_device());
    double sites_x_lower_bound=-1;
    double sites_x_upper_bound=1;
    double sites_y_lower_bound=-1;
    double sites_y_upper_bound=1;
    double sites_reward_val_lower_bound=0;
    double sites_reward_val_upper_bound=1.5;

    std::uniform_real_distribution<> site_x_distribution(sites_x_lower_bound,sites_x_upper_bound);
    std::uniform_real_distribution<> site_y_distribution(sites_y_lower_bound,sites_y_upper_bound);
    std::uniform_real_distribution<> site_reward_val_distribution(sites_reward_val_lower_bound,sites_reward_val_upper_bound);



    std::pair<double,double> site_5_coords={site_x_distribution(engine),site_y_distribution(engine)};
    std::pair<double,double> site_6_coords={site_x_distribution(engine),site_y_distribution(engine)};
    double site_5_reward_val=site_reward_val_distribution(engine);
    double site_6_reward_val=site_reward_val_distribution(engine);

    site manual_site_5;
    manual_site_5.coordinates=site_5_coords;
    manual_site_5.reward_val=site_5_reward_val;

    site manual_site_6;
    manual_site_6.coordinates=site_6_coords;
    manual_site_6.reward_val=site_6_reward_val;

    manual_reward_map.add_site(manual_site_5);
    manual_reward_map.add_site(manual_site_6);

    manual_reward_map.add_site(site_x_distribution(engine),site_y_distribution(engine),site_reward_val_distribution(engine));
    manual_reward_map.add_site(site_x_distribution(engine),site_y_distribution(engine),site_reward_val_distribution(engine));


    //set the starting point of the path
    std::pair<double,double> starting_position={0,0};

    //set map parameters as desired
    manual_reward_map.map_side_length_x_=21;
    manual_reward_map.map_side_length_y_=21;
    manual_reward_map.max_x_=2;
    manual_reward_map.max_y_=2;


    manual_reward_map.set_initial_position(starting_position);

    manual_reward_map.generate_map();

    std::pair<std::vector<site>,double>  paths_and_distance_descending_priority=manual_reward_map.generate_paths_descending_priority();
    std::vector<site> paths_descending_priority=paths_and_distance_descending_priority.first;
    double distance_descending_priority=paths_and_distance_descending_priority.second;

    double distance_weight_1=0.00;
    std::pair<std::vector<site>,double>  paths_and_distance_NN=manual_reward_map.generate_paths_distance_weighted_NN(distance_weight_1);
    std::vector<site> paths_NN=paths_and_distance_NN.first;
    double distance_NN=paths_and_distance_NN.second;

    EXPECT_TRUE(distance_descending_priority==distance_NN) << "Total path distances from descending priority and zero-weight NN approaches weren't equal.\n";
}