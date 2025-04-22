#include <iostream>
#include <Eigen/Dense>
#include "utils.h"

using Eigen::MatrixXd;

class ManualRewardMap
{
    private:

        double min_site_x_coord_={INFINITY};
        double max_site_x_coord_={-INFINITY};
        double min_site_y_coord_={INFINITY};
        double max_site_y_coord_={-INFINITY};
        double max_reward_val_={0};



        std::pair<size_t,size_t> coords_to_map_indices(std::pair<double,double> input_coords);
    public:
        double min_x_={0};
        double max_x_={0};
        double min_y_={0};
        double max_y_={0};

        size_t map_side_length_x_={100};
        size_t map_side_length_y_={100};

        MatrixXd map_;
        std::vector<site> list_of_sites_={};
        std::pair<double,double> starting_position_={};

        ManualRewardMap(){

        }
    
        void add_site(site input_site);

        void add_site(double x_coord, double y_coord, double reward_val); //overloading to allow direct addition of a site through its coordinates and reward val

        void set_initial_position(std::pair<double,double> input_position){
            starting_position_=input_position;
        }

        void set_map_side_length_x(size_t input_length_x){
            map_side_length_x_=input_length_x;
        }

        void set_map_side_length_y(size_t input_length_y){
            map_side_length_y_=input_length_y;
        }

        void generate_map();

        void draw_map();

        void draw_map_with_paths(const std::vector<site>, std::string input_path_type="", double associated_distance_weight=0);
        // Current options for input_path_type: "DescendingPriority" if the path being plotted was generated with the descending priority method, or 
        // "Weighted_NN" if the path being plotted was generated with the distance-weighted nearest-neighbor method. 
        // If this is the case, pass the associated distance weight as the third argument.

        std::pair<std::vector<site>,double>  generate_paths_descending_priority();


        std::pair<std::vector<site>,double> generate_paths_distance_weighted_NN(const double distance_weight);

        void plot_NN_total_distance_swept_distance_weight(double weight_increment,size_t num_weight_increments);

};

