#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Array {
  int capacity;  // How many elements can this array hold?
  int count;  // How many states does the array currently hold?
  char **elements;  // The string elements contained in the array
} Array;


/************************************
 *
 *   CREATE, DESTROY, RESIZE FUNCTIONS
 *
 ************************************/

/*****
 * Allocate memory for a new array
 *****/
Array *create_array (int capacity) {
  // Allocate memory for the Array struct
  Array *arr = malloc(sizeof(Array));

  // Set initial values for capacity and count
  arr->capacity = capacity;
  arr->count = 0;

  // Allocate memory for elements
  //arr->elements = malloc(sizeof(char*) * capacity);
  // pointer automatically initializes to Null
  arr->elements = calloc(capacity, sizeof(char*));

  return arr;

}


/*****
 * Free memory for an array and all of its stored elements
 *****/
void destroy_array(Array *arr) {

  // Free individual elements
  for (int i = 0; i < arr->count; i++) {
    free(arr->elements[i]);
  }
  
  // Free array
  free(arr->elements);
  free(arr);

}

/*****
 * Create a new elements array with double capacity and copy elements
 * from old to new
 *****/
// Passing a pointer allows us to modify the actual object
void resize_array(Array *arr) {

  // Create a new element storage with double capacity
  int new_capacity = arr->capacity * 2

  // Create a new array with double capacity
  //char **new_elements = malloc(new_capacity * sizeof(char *));
  char **new_elements = calloc(new_capacity, sizeof(char *));
  
  // Copy elements into the new storage
  for (int i = 0; i < arr->count; i++) {
    new_elements[i] = arr->elements[i];
  }

  // Free the old elements array (but NOT the strings they point to)
  free(arr->elements);

  // Update the elements and capacity to new values
  arr->elements = new_elements; // update the garbage pointer to a new pointer
  arr->capacity = new_capacity;
}



/************************************
 *
 *   ARRAY FUNCTIONS
 *
 ************************************/

/*****
 * Return the element in the array at the given index.
 *
 * Throw an error if the index is out of range.
 *****/
char *arr_read(Array *arr, int index) {

  // Throw an error if the index is greater or equal to than the current count
  if (index >= arr->count) {
    fprintf(stderr, "Invalid Index: index out of range\n");
    return NULL;
  } 

  // Otherwise, return the element at the given index
  return arr->elements[index];

}


/*****
 * Insert an element to the array at the given index
 *
 * Store the VALUE of the given string, not the REFERENCE
 *****/
void arr_insert(Array *arr, char *element, int index) {

  // Throw an error if the index is greater than the current count
  if (index > arr->count) {
    fprintf(stderr, "Invalid Index: index %d out of range\n", index);
    return;
  } 

  // Resize the array if the number of elements is over capacity
  if (arr->capacity <= arr->count) {
    resize_array(arr);
  }

  // Move every element after the insert index to the right one position
  // Start at the end of the array, continue one step backwards until we hit the index
  for (int i = arr->count;i > index; i--) {
    // Element at each index is set to the previous value 
    arr->elements[i] = arr->elements[i-1];
  }

  // Copy the element (hint: use `strdup()`) and add it to the array
  char *element_copy = strdup(element);
  arr->elements[index] = element_copy;

  // Increment count by 1
  arr->count++;

}

/*****
 * Append an element to the end of the array
 *****/
void arr_append(Array *arr, char *element) {

  // Resize the array if the number of elements is over capacity
  // or throw an error if resize isn't implemented yet.
  if (arr->capacity <= arr->count) {
    
    // Resize
    resize_array(arr);

    // Throw an error
    fprintf(stderr, "Not enough space\n");
    return;
  }

  // Copy the element and add it to the end of the array
  // Make pointer reference a copy
  // Has malloc under the hood - so will need to free this also
  char *element_copy = strdup(element);
  arr->elements[arr->count] = element_copy;
  // OR: arr->elements[arr->count] = strdup(element);

  // Increment count by 1
  arr->count++;

  // This can be done in one line by calling the arr_insert() function
  //arr_insert(arr, element, arr->count);
}

/*****
 * Remove the first occurence of the given element from the array,
 * then shift every element after that occurence to the left one slot.
 *
 * Throw an error if the value is not found.
 *****/
void arr_remove(Array *arr, char *element) {

  // Keeps track of whether or not we found the element ot be removed
  int removed = 0; // Set to False

  // Search for the first occurence of the element and remove it.
  // Don't forget to free its memory!
  for (int i = 0; i < arr->count; i++) {

    // If something was removed
    if (removed) {
      // Set previous element to the current one
      // Shift over every element after the removed element to the left one position
      arr->elements[i-1] = arr->element[i];
    
    // if what is at our current element is equal to what we are looking for
    // strcmp() compares 2 strings - returns 0 if strings are identical
    } else if (strcmp(arr->elements[i], elements) == 0) {
        // Free element
        free(arr->elements[i]);
        removed = 1;
      }
    }

    // If something was removed:
    if (removed) {
      // Decrement count by 1
      arr->count--;
      // Decrement the array element that is at the pointer of the index of the count to NULL
      arr->elements[arr->count] = NULL;
    } else {
      fprintf(stderr, "ValueError: %s not in array", elements);
    }
  }
}


/*****
 * Utility function to print an array.
 *****/
void arr_print(Array *arr) {
  printf("[");
  for (int i = 0 ; i < arr->count ; i++) {
    printf("%s", arr->elements[i]);
    if (i != arr->count - 1) {
      printf(",");
    }
  }
  printf("]\n");
}


#ifndef TESTING
int main(void)
{

  Array *arr = create_array(8);

  arr_insert(arr, "STRING0", 0);
  arr_append(arr, "STRING1");
  arr_append(arr, "STRING2");
  arr_print(arr);
  printf("SECOND ELEMENT IS: %s\n", arr_read(arr, 1));
  arr_insert(arr, "STRING2", 0);
  arr_insert(arr, "STRING3", 1);
  arr_print(arr);
  arr_remove(arr, "STRING3");
  arr_print(arr);

  destroy_array(arr);

  return 0;
}
#endif
