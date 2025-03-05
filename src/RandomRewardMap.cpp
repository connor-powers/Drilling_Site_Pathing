#include <iostream>
#include <fstream>
#include "utils.h"
#include "RandomRewardMap.h"


void RandomRewardMap::identify_potential_sites(int input_desired_num_sites, double input_site_reward_val_threshold){
    //Populates the list_of_sites_ with the <input_desired_num_sites> sites with the highest reward vals above the input threshold
    std::vector<site_obj> output_list_of_sites={};

    double min_observed_reward_val=reward_range_min_;
    double max_observed_reward_val=reward_range_max_;

    size_t const row_count = static_cast<size_t>(map_.rows());
    size_t const col_count = static_cast<size_t>(map_.cols());
    double discrete_movement_x=(max_x_-min_x_)/(map_side_length_x_-1);
    double discrete_movement_y=(max_y_-min_y_)/(map_side_length_y_-1);

    for (size_t row_ind=0; row_ind<map_side_length_y_;row_ind++){
        for (size_t col_ind=0;col_ind<map_side_length_x_;col_ind++){
            double reward_val=map_(row_ind,col_ind);
            if (reward_val>max_observed_reward_val){
                max_observed_reward_val=reward_val;
            }
            if (reward_val<min_observed_reward_val){
                min_observed_reward_val=reward_val;
            }
            if (reward_val>=input_site_reward_val_threshold){
                double x_coord=min_x_+(col_ind*discrete_movement_x);
                double y_coord=min_y_+(row_ind*discrete_movement_y);
                site_obj new_site;
                new_site.coordinates.first=x_coord;
                new_site.coordinates.second=y_coord;
                new_site.reward_val=map_(row_ind,col_ind);
                output_list_of_sites.push_back(new_site);
            }
        }
    }

    std::cout << "Max observed reward val after scaling: " << max_observed_reward_val << "\n";
    std::cout << "Min observed reward val after scaling: " << min_observed_reward_val << "\n";


    //Sort the list in descending order of reward val
    std::sort(output_list_of_sites.begin(),output_list_of_sites.end());
    std::reverse(output_list_of_sites.begin(),output_list_of_sites.end());
    std::vector<site_obj> trimmed_site_list(output_list_of_sites.begin(),output_list_of_sites.begin()+input_desired_num_sites);
    list_of_sites_=trimmed_site_list;
    return;
    return;
}


void RandomRewardMap::identify_potential_sites(double input_site_reward_val_threshold){
    //If no input_num_sites is passed to the function, then populates list_of_sites_ with all sites with reward vals greater than or equal to threshold val
    std::vector<site_obj> output_list_of_sites={};

    size_t const row_count = static_cast<size_t>(map_.rows());
    size_t const col_count = static_cast<size_t>(map_.cols());
    double discrete_movement_x=(max_x_-min_x_)/(map_side_length_x_-1);
    double discrete_movement_y=(max_y_-min_y_)/(map_side_length_y_-1);

    for (size_t row_ind=0; row_ind<map_side_length_y_;row_ind++){
        for (size_t col_ind=0;col_ind<map_side_length_x_;col_ind++){
            if (map_(row_ind,col_ind)>=input_site_reward_val_threshold){
                double x_coord=min_x_+(col_ind*discrete_movement_x);
                double y_coord=min_y_+(row_ind*discrete_movement_y);
                site_obj new_site;
                new_site.coordinates.first=x_coord;
                new_site.coordinates.second=y_coord;
                new_site.reward_val=map_(row_ind,col_ind);
                output_list_of_sites.push_back(new_site);
            }
        }
    }

    //Sort the list in descending order of reward val
    std::sort(output_list_of_sites.begin(),output_list_of_sites.end());
    std::reverse(output_list_of_sites.begin(),output_list_of_sites.end());

    list_of_sites_=output_list_of_sites;
    return;
}


void RandomRewardMap::draw_map(){


    //First make the data file
    //Want to scale the x and y axes to be in coordinate form instead of indices of a matrix 
    size_t const row_count = static_cast<size_t>(map_.rows());
    size_t const col_count = static_cast<size_t>(map_.cols());
    double discrete_movement_x=(max_x_-min_x_)/(map_side_length_x_-1);
    double discrete_movement_y=(max_y_-min_y_)/(map_side_length_y_-1);


    std::ofstream output_file("map_data.dat");
    if (output_file.is_open()){
        for (size_t row_index=0;row_index<row_count;row_index++){
            for (size_t col_index=0;col_index<col_count;col_index++){
                output_file << min_x_+(col_index*discrete_movement_x) <<" "<< min_y_+(row_index*discrete_movement_y) <<" "<< map_(row_index,col_index) << "\n";
            }
        }
        
        output_file.close();
    }
    if (output_file.is_open()){
        std::cout << "File is unexpectedly still open\n";
    }
    //using Gnuplot
    FILE *gnuplot_pipe = popen("gnuplot -persist", "w");

    if (gnuplot_pipe){
        //formatting
        fprintf(gnuplot_pipe, "set view map\n");
        fprintf(gnuplot_pipe, "set title 'Reward Map' font 'Times,16'\n");
        fprintf(gnuplot_pipe, "set xrange [%f:%f]\n",min_x_,max_x_);
        fprintf(gnuplot_pipe, "set yrange [%f:%f]\n",min_y_,max_y_);
        fprintf(gnuplot_pipe, "set cblabel 'Reward Function' font 'Times,14'\n");
        fprintf(gnuplot_pipe, "set tics font 'Times,14'\n");
        fprintf(gnuplot_pipe, "set palette defined (%f 'white', %f 'forest-green')\n",reward_range_min_,reward_range_max_);
        fprintf(gnuplot_pipe, "plot 'map_data.dat' using 1:2:3 with image\n");
        fprintf(gnuplot_pipe, "exit \n");
        pclose(gnuplot_pipe);

    }

}




std::pair<std::vector<site_obj>,double> RandomRewardMap::generate_paths_distance_weighted_NN(const double distance_weight){
    //Nearest-neighbor algorithm taking into account both distance and site reward vals in the spirit of, e.g., https://en.wikipedia.org/wiki/Nearest_neighbour_algorithm

    std::vector<site_obj> visited_sites={};
    std::vector<site_obj> unvisited_sites=list_of_sites_;

    std::pair<double,double> current_coords=starting_position_;

    while (unvisited_sites.size()>0){
        double lowest_cost=INFINITY;
        size_t best_site_index={list_of_sites_.size()}; //should throw out of range error if this never gets changed
        //Find the site with the lowest total "cost" associated with visiting it from the current position, taking into account distance and reward function
        for (size_t site_ind=0;site_ind<unvisited_sites.size();site_ind++){
            site_obj site=unvisited_sites.at(site_ind);
            double calculated_cost=calc_cost_function_from_position_to_site(current_coords,site,distance_weight);
            if (calculated_cost<lowest_cost){
                lowest_cost=calculated_cost;
                best_site_index=site_ind;
            }
        }
        site_obj visited_site=unvisited_sites.at(best_site_index);
        current_coords=visited_site.coordinates;
        visited_sites.push_back(visited_site);
        unvisited_sites.erase(unvisited_sites.begin()+best_site_index);
    }


    double total_distance=calculate_total_distance_from_sequence(starting_position_,visited_sites);
    std::pair<std::vector<site_obj>,double> output_pair;
    output_pair.first=visited_sites;
    output_pair.second=total_distance;
    return output_pair;
}



void RandomRewardMap::draw_map_with_paths(const std::vector<site_obj> input_sorted_site_list){

    //First make the data file
    //Want to scale the x and y axes to be in coordinate form instead of indices of a matrix 
    size_t const row_count = static_cast<size_t>(map_.rows());
    size_t const col_count = static_cast<size_t>(map_.cols());
    double discrete_movement_x=(max_x_-min_x_)/(map_side_length_x_-1);
    double discrete_movement_y=(max_y_-min_y_)/(map_side_length_y_-1);


    std::ofstream output_file("map_data.dat");
    if (output_file.is_open()){
        for (size_t row_index=0;row_index<row_count;row_index++){
            for (size_t col_index=0;col_index<col_count;col_index++){
                output_file << min_x_+col_index*discrete_movement_x <<" "<< min_y_+row_index*discrete_movement_y <<" "<< map_(row_index,col_index) << "\n";
            }
        }
        
        output_file.close();
    }
    if (output_file.is_open()){
        std::cout << "File is unexpectedly still open\n";
    }
    //Using Gnuplot
    FILE *gnuplot_pipe = popen("gnuplot -persist", "w");

    if (gnuplot_pipe){
        //Formatting
        fprintf(gnuplot_pipe, "set view map\n");
        fprintf(gnuplot_pipe, "set title 'Reward Map' font 'Times,16'\n");
        fprintf(gnuplot_pipe, "set xrange [%f:%f]\n",min_x_,max_x_);
        fprintf(gnuplot_pipe, "set yrange [%f:%f]\n",min_y_,max_y_);
        fprintf(gnuplot_pipe, "set cblabel 'Reward Function' font 'Times,14' offset 1\n");
        fprintf(gnuplot_pipe, "set tics font 'Times,14'\n");

        fprintf(gnuplot_pipe, "set palette defined (%f 'white', %f 'forest-green')\n",reward_range_min_,reward_range_max_);


        //The input_sorted_site_list should list the sites in the order to visit them

        if (input_sorted_site_list.size()>1){
            double next_site_x_coord=input_sorted_site_list.at(0).coordinates.first;
            double next_site_y_coord=input_sorted_site_list.at(0).coordinates.second;
            fprintf(gnuplot_pipe, "set arrow from %f,%f to %f,%f,1 filled front\n",starting_position_.first,starting_position_.second,next_site_x_coord,next_site_y_coord);
            for (int arrow_ind=1;arrow_ind<input_sorted_site_list.size();arrow_ind++){
                double previous_site_x_coord=input_sorted_site_list.at(arrow_ind-1).coordinates.first;
                double previous_site_y_coord=input_sorted_site_list.at(arrow_ind-1).coordinates.second;
                next_site_x_coord=input_sorted_site_list.at(arrow_ind).coordinates.first;
                next_site_y_coord=input_sorted_site_list.at(arrow_ind).coordinates.second;
                fprintf(gnuplot_pipe, "set arrow from %f,%f to %f,%f,%d filled front\n",previous_site_x_coord,previous_site_y_coord,next_site_x_coord,next_site_y_coord,arrow_ind);
            }
        }

        fprintf(gnuplot_pipe, "plot 'map_data.dat' using 1:2:3 with image\n");
        fprintf(gnuplot_pipe, "exit \n");
        pclose(gnuplot_pipe);

    }

}


void RandomRewardMap::plot_NN_total_distance_swept_distance_weight(double weight_increment,size_t num_weight_increments){

    //Using Gnuplot
    FILE *gnuplot_pipe = popen("gnuplot -persist", "w");

    if (gnuplot_pipe){
        //Formatting
        fprintf(gnuplot_pipe, "set view map\n");
        fprintf(gnuplot_pipe, "set title 'Total Distance' font 'Times,16'\n");
        fprintf(gnuplot_pipe, "set xlabel 'Distance Weight' font 'Times,16'\n");
        fprintf(gnuplot_pipe, "set logscale x 10\n");


        fprintf(gnuplot_pipe, "plot '-' with lines notitle\n");
        for (size_t index=0;index<num_weight_increments;index++){
            double distance_weight=weight_increment*index;
            std::pair<std::vector<site_obj>,double> output_pair=generate_paths_distance_weighted_NN(distance_weight);
            double total_distance=output_pair.second;

            fprintf(gnuplot_pipe, "%f %f\n",distance_weight,total_distance);
        }
        fprintf(gnuplot_pipe, "e \n");

        fprintf(gnuplot_pipe, "exit \n");
        pclose(gnuplot_pipe);
    }

}