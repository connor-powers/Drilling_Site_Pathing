#include <Eigen/Dense>
#include <iostream>
#include <random>

#include "utils.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

class RandomRewardMap {
 private:
  std::vector<site> list_of_sites_ = {};

  std::pair<size_t, size_t> coords_to_map_indices(
      std::pair<double, double> input_coords);

 public:
  std::pair<double, double> starting_position_;
  double min_x_ = {0};
  double max_x_ = {0};
  double min_y_ = {0};
  double max_y_ = {0};
  double reward_range_min_ = {0};
  double reward_range_max_ = {1};

  size_t map_side_length_x_ = {100};
  size_t map_side_length_y_ = {100};

  MatrixXd map_;

  RandomRewardMap(double map_side_length_x, double map_side_length_y,
                  double min_x, double max_x, double min_y, double max_y,
                  double reward_range_min, double reward_range_max) {
    map_side_length_x_ = map_side_length_x;
    map_side_length_y_ = map_side_length_y;
    min_x_ = min_x;
    min_y_ = min_y;
    max_x_ = max_x;
    max_y_ = max_y;
    reward_range_min_ = reward_range_min;
    reward_range_max_ = reward_range_max;
    starting_position_.first = 0;
    starting_position_.second = 0;

    std::random_device test_random_device;
    std::mt19937 engine(test_random_device());
    std::uniform_real_distribution<> reward_val_distribution(reward_range_min_,
                                                             reward_range_max_);

    // map_=MatrixXd::Random(map_side_length_y_,map_side_length_x_);
    map_ = MatrixXd::NullaryExpr(map_side_length_y_, map_side_length_x_,
                                 [&engine, &reward_val_distribution]() {
                                   return reward_val_distribution(engine);
                                 });

    // rescale to desired range since by default Random generates floats between
    // -1 and 1
    //  map_*=((reward_range_max_-reward_range_min_)/2); //because initially has
    //  a range of 2 from going between -1 and 1

    // MatrixXd
    // tmp_mat=MatrixXd::Constant(map_side_length_y_,map_side_length_x_,reward_range_min_+((reward_range_max_-reward_range_min_)/2));
    // map_+=tmp_mat;
  }

  void set_initial_position(std::pair<double, double> input_position) {
    starting_position_ = input_position;
  }

  void identify_potential_sites(int input_num_sites,
                                double input_site_reward_val_threshold = 0.5);
  void identify_potential_sites(double input_site_reward_val_threshold = 0.5);

  void draw_map();

  void draw_map_with_paths(const std::vector<site>,
                           std::string input_path_type = "",
                           double associated_distance_weight = 0);
  // Current options for input_path_type: "DescendingPriority" if the path being
  // plotted was generated with the descending priority method, or "Weighted_NN"
  //if the path being plotted was generated with the distance-weighted
  //nearest-neighbor method.
  //  If this is the case, pass the associated distance weight as the third
  //  argument.

  std::pair<std::vector<site>, double> generate_paths_distance_weighted_NN(
      const double distance_weight);

  void plot_NN_total_distance_swept_distance_weight(
      double weight_increment, size_t num_weight_increments);

  std::vector<site> get_list_of_sites() { return list_of_sites_; }
};
