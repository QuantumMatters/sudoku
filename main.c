// C program for generating a sudoku game
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int min(int int_array[], int num_elements) {
    int min = int_array[0];
    for(int i=1; i<num_elements; i++)
    {
        if (int_array[i] < min)
        {
            min = int_array[i];
        }
    }
    return min;
}

int get_upper_limit(int region_sum, int row_sum, int column_sum) {
    /* Given the sums of the region, row, and column relevant to a new element, determine the upper
    limit for the new element: adding the value of the new element cannot cause any of the sums to
    exceed 9.
    */
    int remainder_array[3] = {region_sum, row_sum, column_sum};
    int upper_limit = min(remainder_array, 3);
    return upper_limit;
}

void get_taken_values(int *taken_values, int board[9][9], int position[2]) {
    /* For a given position, iterate through the corresponding row, colum, and region values of the
    board to identify the values {0,..0} that have already been taken.

    int taken_values[9] := integer array containing 0s and 1s, with 1s indicating the given value
                           corresponding to the index has been taken.
                           i.e. taken_values[1] == 1 => the value 2 has been taken
                                taken_values[2] == 0 => the value 3 is available
    */
    int possible_values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int region_x_start = (position[0] / 3) * 3;
    int region_y_start = (position[1] / 3) * 3;
    int row_value;
    int col_value;
    int reg_value;

    // Go through the row and column
    for(int i=0; i<9; i++) {
        row_value = board[position[0], i];
        col_value = board[i, position[1]];

        // Assign 1 to values that are already taken
        if (row_value > 0) {
            taken_values[row_value - 1] = 1;
        }
        if (col_value > 0) {
            taken_values[col_value - 1] = 1;
        }
    }

    // Go through the region
    for(int i=region_x_start; i<region_x_start+3; i++) {
        for(int j=region_y_start; j<region_y_start+3; j++) {
            reg_value = board[i][j];
            if (reg_value > 0) {
                taken_values[reg_value - 1] = 1;
            }
        }
    }
}

void flip_boolean_array(int *array, int num_elements) {
    for(int i=0; i<num_elements; i++) {
        if (array[i] == 0) {
            array[i] = 1;
        }
    }
}

void get_available_values(int *available_values, int board[9][9], int position[2]) {
    /* Identify which values are available for assignment for a new element with a given board and
    position of the new element.
    */
    get_taken_values(available_values, board, position);
    flip_boolean_array(available_values, 9);
}

int pick_value(int available_values[9], int upper_limit) {
    /* Picks a value for a new element given an array of available values and an upper limit.
    */
    int value;
    int go = 1;
    int i = 0;
    int max_loops = 1000;
    while (go) {
        value = rand() % upper_limit + 1;
        if (available_values[value - 1] == 1) {
            // this means we found a viable value.
            go = 0;
        }
        i++;
        if (i == max_loops) {
            go = 0;
            printf("pick_value exceeded max_loops.");
        }
    }
    return value;
}

int main(void) {
    int board[9][9];
    int region_sum[3][3];
    int row_sum[9];
    int column_sum[9];
    int value;
    int position[2];

    // Use current time as seed for random generator
    srand(time(0));

    // Build the board
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            position[0] = i;
            position[1] = j;

            // Find a value for the given position
            printf("i, j: %d, %d", i, j);
            int upper_limit = get_upper_limit(region_sum[i][j], row_sum[i], column_sum[j]);
            printf("upper_limit: %d", upper_limit);
            int available_values[9];
            get_available_values(available_values, board, position);
            value = pick_value(available_values, upper_limit);

            // Update board with value
            board[i][j] = value;

            // Update sums
            region_sum[i][j] += value;
            row_sum[i] += value;
            printf("%d ", value);
        }
        printf("\n");
    }

    return 0;
}