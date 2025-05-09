#include "ManualRewardMap.h"

#include <fstream>
#include <iostream>

#include "utils.h"

void ManualRewardMap::add_site(site input_site) {
  double site_x = input_site.coordinates.first;
  double site_y = input_site.coordinates.second;
  if (site_x < min_site_x_coord_) {
    min_site_x_coord_ = site_x;
  }

  if (site_x > max_site_x_coord_) {
    max_site_x_coord_ = site_x;
  }

  if (site_y < min_site_y_coord_) {
    min_site_y_coord_ = site_y;
  }

  if (site_y > max_site_y_coord_) {
    max_site_y_coord_ = site_y;
  }
  if (input_site.reward_val > max_reward_val_) {
    max_reward_val_ = input_site.reward_val;
  }

  list_of_sites_.push_back(input_site);
}

void ManualRewardMap::add_site(double x_coord, double y_coord,
                               double reward_val) {
  // overloading to allow direct addition of a site through its coordinates and
  // reward val

  if (x_coord < min_site_x_coord_) {
    min_site_x_coord_ = x_coord;
  }

  if (x_coord > max_site_x_coord_) {
    max_site_x_coord_ = x_coord;
  }

  if (y_coord < min_site_y_coord_) {
    min_site_y_coord_ = y_coord;
  }

  if (y_coord > max_site_y_coord_) {
    max_site_y_coord_ = y_coord;
  }
  if (reward_val > max_reward_val_) {
    max_reward_val_ = reward_val;
  }
  site tmp_site;
  std::pair<double, double> tmp_coord_pair;
  tmp_coord_pair.first = x_coord;
  tmp_coord_pair.second = y_coord;

  tmp_site.coordinates = tmp_coord_pair;
  tmp_site.reward_val = reward_val;
  list_of_sites_.push_back(tmp_site);
}

void ManualRewardMap::generate_map() {
  map_.resize(map_side_length_y_, map_side_length_x_);
  map_.setZero();  // Fill background with zeros
  // y then x because params are (rows, cols) and cols will give us width in x

  if (min_site_x_coord_ <= min_x_) {
    min_x_ = min_site_x_coord_ - 0.1;
  }
  if (min_site_y_coord_ <= min_y_) {
    min_y_ = min_site_y_coord_ - 0.1;
  }

  if (max_site_x_coord_ >= max_x_) {
    max_x_ = max_site_x_coord_ + 0.1;
  }
  if (max_site_y_coord_ >= max_y_) {
    max_y_ = max_site_y_coord_ + 0.1;
  }

  // Now need to find the pixel (row_ind,col_ind) in the map that best
  // approximates the (x,y) coordinates of a given site
  for (site site_obj : list_of_sites_) {
    std::pair<double, double> site_position = site_obj.coordinates;
    std::pair<size_t, size_t> closest_map_index_pair =
        coords_to_map_indices(site_position);
    map_(closest_map_index_pair.first, closest_map_index_pair.second) =
        site_obj.reward_val;
  }
}

std::pair<size_t, size_t> ManualRewardMap::coords_to_map_indices(
    std::pair<double, double> input_coords) {
  double discrete_movement_x = (max_x_ - min_x_) / (map_side_length_x_ - 1);
  double discrete_movement_y = (max_y_ - min_y_) / (map_side_length_y_ - 1);
  double input_x = input_coords.first;
  double input_y = input_coords.second;

  size_t closest_col_index = {0};
  size_t closest_row_index = {0};

  if (input_x == max_x_) {
    closest_col_index = map_side_length_x_ - 1;
  }
  if (input_y == max_y_) {
    closest_col_index = map_side_length_y_ - 1;
  }

  // First let's find the position in the map matrix (which column)
  // corresponding to the closest approximation of the input x coordinate
  for (size_t column_index = 0; column_index < map_side_length_x_ - 1;
       column_index++) {
    double current_map_x_val = min_x_ + (discrete_movement_x * column_index);
    double next_map_x_val = min_x_ + (discrete_movement_x * (column_index + 1));

    if (current_map_x_val == input_x) {
      closest_col_index = column_index;
      break;
    } else if ((current_map_x_val < input_x) && (next_map_x_val > input_x)) {
      if (input_x - current_map_x_val < next_map_x_val - input_x) {
        // Closer to current column index
        closest_col_index = column_index;
        break;
      } else {
        closest_col_index = column_index + 1;
        break;
      }
    }
  }

  // Now let's find the position in the map matrix (which row) corresponding to
  // the closest approximation of the input y coordinate
  for (size_t row_index = 0; row_index < map_side_length_y_ - 1; row_index++) {
    double current_map_y_val = min_y_ + (discrete_movement_y * row_index);
    double next_map_y_val = min_y_ + (discrete_movement_y * (row_index + 1));

    if (current_map_y_val == input_y) {
      closest_row_index = row_index;
      break;
    } else if ((current_map_y_val < input_y) && (next_map_y_val > input_y)) {
      if (input_y - current_map_y_val < next_map_y_val - input_y) {
        // Closer to current row index
        closest_row_index = row_index;
        break;
      } else {
        closest_row_index = row_index + 1;
        break;
      }
    }
  }
  std::pair<size_t, size_t> closest_map_index = {closest_row_index,
                                                 closest_col_index};
  return closest_map_index;
}

void ManualRewardMap::draw_map() {
  // First make the data file
  // Want to scale the x and y axes to be in coordinate form instead of indices
  // of a matrix
  size_t const row_count = static_cast<size_t>(map_.rows());
  size_t const col_count = static_cast<size_t>(map_.cols());
  double discrete_movement_x = (max_x_ - min_x_) / (map_side_length_x_ - 1);
  double discrete_movement_y = (max_y_ - min_y_) / (map_side_length_y_ - 1);

  std::ofstream output_file("map_data.dat");
  if (output_file.is_open()) {
    for (size_t row_index = 0; row_index < row_count; row_index++) {
      for (size_t col_index = 0; col_index < col_count; col_index++) {
        output_file << min_x_ + (col_index * discrete_movement_x) << " "
                    << min_y_ + (row_index * discrete_movement_y) << " "
                    << map_(row_index, col_index) << "\n";
      }
    }

    output_file.close();
  }
  if (output_file.is_open()) {
    std::cout << "File is unexpectedly still open\n";
  }
  // Using Gnuplot
  FILE *gnuplot_pipe = popen("gnuplot -persist", "w");

  if (gnuplot_pipe) {
    // Formatting
    fprintf(gnuplot_pipe, "set view map\n");
    fprintf(gnuplot_pipe, "set title 'Reward Map' font 'Arial,16'\n");
    fprintf(gnuplot_pipe, "set xrange [%f:%f]\n", min_x_, max_x_);
    fprintf(gnuplot_pipe, "set yrange [%f:%f]\n", min_y_, max_y_);
    fprintf(gnuplot_pipe, "set cblabel 'Reward Function' font 'Arial,14'\n");
    fprintf(gnuplot_pipe, "set tics font 'Helvetica,14'\n");
    fprintf(gnuplot_pipe,
            "set palette defined (0 'white', %f 'forest-green')\n",
            max_reward_val_);
    fprintf(gnuplot_pipe, "plot 'map_data.dat' using 1:2:3 with image\n");
    fprintf(gnuplot_pipe, "exit \n");
    pclose(gnuplot_pipe);
  }
}

void ManualRewardMap::draw_map_with_paths(
    const std::vector<site> input_sorted_site_list, std::string input_path_type,
    double associated_distance_weight) {
  // First make the data file
  // Want to scale the x and y axes to be in coordinate form instead of indices
  // of a matrix
  size_t const row_count = static_cast<size_t>(map_.rows());
  size_t const col_count = static_cast<size_t>(map_.cols());
  double discrete_movement_x = (max_x_ - min_x_) / (map_side_length_x_ - 1);
  double discrete_movement_y = (max_y_ - min_y_) / (map_side_length_y_ - 1);

  std::ofstream output_file("map_data.dat");
  if (output_file.is_open()) {
    for (size_t row_index = 0; row_index < row_count; row_index++) {
      for (size_t col_index = 0; col_index < col_count; col_index++) {
        output_file << min_x_ + col_index * discrete_movement_x << " "
                    << min_y_ + row_index * discrete_movement_y << " "
                    << map_(row_index, col_index) << "\n";
      }
    }

    output_file.close();
  }
  if (output_file.is_open()) {
    std::cout << "File is unexpectedly still open\n";
  }
  // Using Gnuplot
  FILE *gnuplot_pipe = popen("gnuplot -persist", "w");

  if (gnuplot_pipe) {
    // Formatting
    fprintf(gnuplot_pipe, "set view map\n");

    if (input_path_type == "DescendingPriority") {
      fprintf(gnuplot_pipe,
              "set title 'Reward Map, Descending Priority Pathing' font "
              "'Arial,16'\n");
    } else if (input_path_type == "Weighted_NN") {
      fprintf(gnuplot_pipe,
              "set title 'Reward Map, Distance-Weighted NN Pathing with "
              "Weight=%0.3f' font 'Arial,16'\n",
              associated_distance_weight);
    } else {
      fprintf(gnuplot_pipe, "set title 'Reward Map' font 'Arial,16'\n");
    }
    fprintf(gnuplot_pipe, "set xrange [%f:%f]\n", min_x_, max_x_);
    fprintf(gnuplot_pipe, "set yrange [%f:%f]\n", min_y_, max_y_);
    fprintf(gnuplot_pipe,
            "set cblabel 'Reward Function' font 'Arial,14' offset 1\n");
    fprintf(gnuplot_pipe, "set tics font 'Helvetica,14'\n");

    fprintf(gnuplot_pipe,
            "set palette defined (0 'white', %f 'forest-green')\n",
            max_reward_val_);

    // The input_sorted_site_list should list the sites in the order to visit
    // them

    if (input_sorted_site_list.size() > 1) {
      double next_site_x_coord = input_sorted_site_list.at(0).coordinates.first;
      double next_site_y_coord =
          input_sorted_site_list.at(0).coordinates.second;
      fprintf(gnuplot_pipe, "set arrow from %f,%f to %f,%f,1 filled front\n",
              starting_position_.first, starting_position_.second,
              next_site_x_coord, next_site_y_coord);
      for (int arrow_ind = 1; arrow_ind < input_sorted_site_list.size();
           arrow_ind++) {
        double previous_site_x_coord =
            input_sorted_site_list.at(arrow_ind - 1).coordinates.first;
        double previous_site_y_coord =
            input_sorted_site_list.at(arrow_ind - 1).coordinates.second;
        next_site_x_coord =
            input_sorted_site_list.at(arrow_ind).coordinates.first;
        next_site_y_coord =
            input_sorted_site_list.at(arrow_ind).coordinates.second;
        fprintf(gnuplot_pipe, "set arrow from %f,%f to %f,%f,%d filled front\n",
                previous_site_x_coord, previous_site_y_coord, next_site_x_coord,
                next_site_y_coord, arrow_ind);
      }
    }

    fprintf(gnuplot_pipe, "plot 'map_data.dat' using 1:2:3 with image\n");
    fprintf(gnuplot_pipe, "exit \n");
    pclose(gnuplot_pipe);
  }
}

std::pair<std::vector<site>, double>
ManualRewardMap::generate_paths_descending_priority() {
  std::vector<site> sorted_list_of_sites = list_of_sites_;
  std::sort(sorted_list_of_sites.begin(), sorted_list_of_sites.end());
  std::reverse(sorted_list_of_sites.begin(), sorted_list_of_sites.end());

  double total_distance = calculate_total_distance_from_sequence(
      starting_position_, sorted_list_of_sites);
  std::pair<std::vector<site>, double> output_pair;
  output_pair.first = sorted_list_of_sites;
  output_pair.second = total_distance;
  return output_pair;
}

std::pair<std::vector<site>, double>
ManualRewardMap::generate_paths_distance_weighted_NN(
    const double distance_weight) {
  // Nearest-neighbor algorithm taking into account both distance and site
  // reward vals in the spirit of,
  //  e.g., https://en.wikipedia.org/wiki/Nearest_neighbour_algorithm

  std::vector<site> visited_sites = {};
  std::vector<site> unvisited_sites = list_of_sites_;

  std::pair<double, double> current_coords = starting_position_;

  while (unvisited_sites.size() > 0) {
    double lowest_cost = INFINITY;
    size_t best_site_index = {
        list_of_sites_.size()};  // should throw out of range error if this
                                 // never gets changed
    // Find the site with the lowest total "cost" associated with visiting it
    // from the current position,
    //  taking into account distance and reward function
    for (size_t site_ind = 0; site_ind < unvisited_sites.size(); site_ind++) {
      site site_obj = unvisited_sites.at(site_ind);
      double calculated_cost = calc_cost_function_from_position_to_site(
          current_coords, site_obj, distance_weight);
      if (calculated_cost < lowest_cost) {
        lowest_cost = calculated_cost;
        best_site_index = site_ind;
      }
    }
    site visited_site = unvisited_sites.at(best_site_index);
    current_coords = visited_site.coordinates;
    visited_sites.push_back(visited_site);
    unvisited_sites.erase(unvisited_sites.begin() + best_site_index);
  }

  double total_distance =
      calculate_total_distance_from_sequence(starting_position_, visited_sites);
  std::pair<std::vector<site>, double> output_pair;
  output_pair.first = visited_sites;
  output_pair.second = total_distance;
  return output_pair;
}

void ManualRewardMap::plot_NN_total_distance_swept_distance_weight(
    double weight_increment, size_t num_weight_increments) {
  // Using Gnuplot
  FILE *gnuplot_pipe = popen("gnuplot -persist", "w");

  if (gnuplot_pipe) {
    // Formatting
    fprintf(gnuplot_pipe, "set view map\n");
    fprintf(gnuplot_pipe, "set title 'Total Distance' font 'Arial,16'\n");
    fprintf(gnuplot_pipe, "set xlabel 'Distance Weight' font 'Arial,16'\n");
    fprintf(gnuplot_pipe, "set logscale x 10\n");

    fprintf(gnuplot_pipe, "plot '-' with lines notitle\n");
    for (size_t index = 0; index < num_weight_increments; index++) {
      double distance_weight = weight_increment * index;
      std::pair<std::vector<site>, double> output_pair =
          generate_paths_distance_weighted_NN(distance_weight);
      double total_distance = output_pair.second;

      fprintf(gnuplot_pipe, "%f %f\n", distance_weight, total_distance);
    }
    fprintf(gnuplot_pipe, "e \n");

    fprintf(gnuplot_pipe, "exit \n");
    pclose(gnuplot_pipe);
  }
}
