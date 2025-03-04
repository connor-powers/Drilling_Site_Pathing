#include <iostream>
#include <Eigen/Dense>

using Eigen::MatrixXd;

struct site_obj
{
    std::pair<double,double> coordinates;
    double reward_val;

    bool operator < (const site_obj input_site) const {
        return (reward_val<input_site.reward_val);
    }
};


class ManualRewardMap
{
    private:

        double min_site_x_coord_;
        double max_site_x_coord_;
        double min_site_y_coord_;
        double max_site_y_coord_;
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
        std::vector<site_obj> list_of_sites_={};
        std::pair<double,double> starting_position_={};

        ManualRewardMap(std::vector<site_obj> input_list_of_sites={}){
            if (input_list_of_sites.size()>0){
                for (site_obj site : input_list_of_sites){
                    double site_x=site.coordinates.first;
                    double site_y=site.coordinates.second;
                    if (site_x<min_site_x_coord_){
                        min_site_x_coord_=site_x;
                    }

                    if (site_x>max_site_x_coord_){
                        max_site_x_coord_=site_x;
                    }

                    if (site_y<min_site_y_coord_){
                        min_site_y_coord_=site_y;
                    }

                    if (site_y>max_site_y_coord_){
                        max_site_y_coord_=site_y;
                    }
                    if (site.reward_val>max_reward_val_){
                        max_reward_val_=site.reward_val;
                    }

                    list_of_sites_.push_back(site);
                }
            }
            else {
                min_site_x_coord_=min_x_;
                min_site_y_coord_=min_y_;
                max_site_x_coord_=max_x_;
                max_site_y_coord_=max_y_;
            }
        }
    
        void add_site(site_obj input_site){
            list_of_sites_.push_back(input_site);
        }

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

        void draw_map_with_paths(const std::vector<site_obj>);

        std::pair<std::vector<site_obj>,double>  generate_paths_descending_priority();


        std::pair<std::vector<site_obj>,double> generate_paths_distance_weighted_NN(const double distance_weight);

        void plot_NN_total_distance_swept_distance_weight(double weight_increment,size_t num_weight_increments);
        
};

