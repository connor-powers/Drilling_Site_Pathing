#include <iostream>
#include "ManualRewardMap.h"
#include <fstream>

void ManualRewardMap::generate_map(){
    map_.resize(map_side_length_y_,map_side_length_x_);
    map_.setZero(); //fill background with zeros
    //y then x because params are (rows, cols) and cols will give us width in x

    if (min_x_>min_site_x_coord_){
        min_x_=min_site_x_coord_;
    }
    if (min_y_<min_site_y_coord_){
        min_y_=min_site_y_coord_;
    }

    if (max_x_<max_site_x_coord_){
        max_x_=max_site_x_coord_;
    }
    if (max_y_<max_site_y_coord_){
        max_y_=max_site_y_coord_;
    }

    //now need to find the pixel (row_ind,col_ind) in the map that best approximates the (x,y) coordinates of a given site
    for (site_obj site : list_of_sites_){
        std::pair<double,double> site_position=site.coordinates;
        std::pair<size_t,size_t> closest_map_index_pair=coords_to_map_indices(site_position);
        map_(closest_map_index_pair.first,closest_map_index_pair.second)=site.reward_val;
    }

    
}



std::pair<size_t,size_t> ManualRewardMap::coords_to_map_indices(std::pair<double,double> input_coords){
    double discrete_movement_x=(max_x_-min_x_)/(map_side_length_x_-1);
    double discrete_movement_y=(max_y_-min_y_)/(map_side_length_y_-1);
    double input_x=input_coords.first;
    double input_y=input_coords.second;
    
    size_t closest_col_index={0};
    size_t closest_row_index={0};

    if (input_x==max_x_){
        closest_col_index= map_side_length_x_-1;
    }
    if (input_y==max_y_){
        closest_col_index= map_side_length_y_-1;
    }
    
    //first let's find the position in the map matrix (which column) corresponding to the closest approximation of the input x coordinate
    for (size_t column_index=0;column_index<map_side_length_x_-1;column_index++){
        double current_map_x_val=min_x_+(discrete_movement_x*column_index);
        double next_map_x_val=min_x_+(discrete_movement_x*(column_index+1));

        if (current_map_x_val==input_x){
            closest_col_index=column_index;
            break;
        }
        else if ((current_map_x_val<input_x)&&(next_map_x_val>input_x)){
            if (input_x-current_map_x_val < next_map_x_val-input_x){
                //closer to current column index
                closest_col_index=column_index;
                break;
            }
            else {
                closest_col_index=column_index+1;
                break;
            }
        }
    }

    //now let's find the position in the map matrix (which row) corresponding to the closest approximation of the input y coordinate
    for (size_t row_index=0;row_index<map_side_length_y_-1;row_index++){
        double current_map_y_val=min_y_+(discrete_movement_y*row_index);
        double next_map_y_val=min_y_+(discrete_movement_y*(row_index+1));

        if (current_map_y_val==input_y){
            closest_row_index=row_index;
            break;
        }
        else if ((current_map_y_val<input_y)&&(next_map_y_val>input_y)){
            if (input_y-current_map_y_val < next_map_y_val-input_y){
                //closer to current row index
                closest_row_index=row_index;
                break;
            }
            else {
                closest_row_index=row_index+1;
                break;
            }
        }
    }
    std::pair<size_t,size_t> closest_map_index={closest_row_index,closest_col_index};
    return closest_map_index;
}


void ManualRewardMap::draw_map(){

    //first make the data file
    //want to scale the x and y axes to be in coordinate form instead of indices of a matrix 
    size_t const row_count = static_cast<size_t>(map_.rows());
    size_t const col_count = static_cast<size_t>(map_.cols());
    double discrete_movement_x=(max_x_-min_x_)/(map_side_length_x_-1);
    double discrete_movement_y=(max_y_-min_y_)/(map_side_length_y_-1);


    std::ofstream output_file("map_data.dat");
    if (output_file.is_open()){
        for (size_t row_index=0;row_index<row_count;row_index++){
            for (size_t col_index=0;col_index<col_count;col_index++)
                output_file << min_x_+row_index*discrete_movement_x <<" "<< min_y_+col_index*discrete_movement_y <<" "<< map_(row_index,col_index) << "\n";
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
        fprintf(gnuplot_pipe, "set title 'Reward Map'\n");
        fprintf(gnuplot_pipe, "set xrange [%f:%f]\n",min_x_,max_x_);
        fprintf(gnuplot_pipe, "set yrange [%f:%f]\n",min_y_,max_y_);

        fprintf(gnuplot_pipe, "set palette defined (0 'white', %f 'forest-green')\n",max_reward_val_);


        fprintf(gnuplot_pipe, "plot 'map_data.dat' using 2:1:3 with image\n");
        
        fprintf(gnuplot_pipe, "exit \n");
        pclose(gnuplot_pipe);

    }

}