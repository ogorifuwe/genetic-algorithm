#include <stdio.h>
//#include <conio.h>
#include <curses.h>
#include <stdlib.h>

//#include <my_timer.h>
//#include <aligned_allocator.h>


/* defining OpenMP with header */
#if defined (_OPENMP)
#include <omp.h>
#endif




/* This Genetic Algorithm is based on the function y = -x^2 + 5
   used to find the maximum value of the function */



/* creating the chrome structure of our population */
typedef struct Chrome {
    short int bit[6];
    int fit;
}chrome;


const int POP_SIZE = 1 << 16;
/* defining the functions of our algorithm */
void *population(chrome current_popluation[]);
int x(chrome current_population);
int y(int x);
void *selection(chrome current_population[]);
void *crossover(chrome next_population[]);
void *mutation(chrome next_population[]);



/* creation of the population */
void *population(chrome current_population[]) {     			 // take a pointer of a chrome of 4 elements
  int i, j, value;
  int random;
  for (j=0; j<POP_SIZE; ++j) {        						                 // loop to choose chromes from [0] to [3]
    for (i=0; i<6; ++i) {      						                 // loop of i to choose the gene of the chrome for [0] to [5]
      random = rand();         						                 // creating random value
      random = (random%2);     						                 // making the random value a 0 or 1 only
      current_population[j].bit[i]=random;         			   // initializing the bit[i] of chrome[j] with random
    }     // end of for(i)

    value = x(current_population[j]);              				                // get the value of the chrome as Integer
    current_population[j].fit=y(x(current_population[j]));     			      // calculate the fitness of the chromes[j]
    printf("\n current_population[%d]=%d%d%d%d%d%d    value=%d    fitness==%d", j,
    current_population[j].bit[5], current_population[j].bit[4], current_population[j].bit[3],
    current_population[j].bit[2], current_population[j].bit[1], current_population[j].bit[0],
    value, current_population[j].fit);                                    // print the chrome[j]
  }     // end of for (j)

  return 0;
}     // end of population function



/* function for evaluating the value of a given chrome */
int x(chrome current_population) {
  int z;
  z = (current_population.bit[0] * 1) + (current_population.bit[1] * 2) + (current_population.bit[2] * 4) +
  (current_population.bit[3] * 8) + (current_population.bit[4] * 16);
  if (current_population.bit[5] == 1)
  z = z * (-1);     							 // z=sum of the { bits * their weights } with the sign value
  return(z);        							 // return the value of z
}                    // end x function



/* function used to find the fitness value
  takes x(func. for evaluating the value of a given chrome)
  as a parameter  */
int y(int x) {
   int y;
   y = -(x * x) + 5;       						 // the function is y = -(x^2) + 5
   return(y);
 }    //end of y function



 /* selection of chromes
    takes a pointer to array of chromes */
 void *selection(chrome current_popluation[]) {
   int i, j;
   chrome temp_chrome;          					         // temp chrome to use in sorting
   for (i=0; i<(POP_SIZE-1); ++i)   						               // sorting the given set wrt to fitness
      for (j=0; j<(POP_SIZE-1); ++j) {
        if (current_popluation[j+1].fit>current_popluation[j].fit) {
          temp_chrome = current_popluation[j+1];
          current_popluation[j+1] = current_popluation[j];
          current_popluation[j] = temp_chrome;
        }       // end of if
      }         // end of inner for loop

      for (i=0; i<POP_SIZE; ++i)
    printf("\nSorting:new_population[%d]  fitness=%d", i, current_popluation[i].fit);      // printing the result
    printf("\n");
      fflush(stdout);
    return 0;
 }         // end of selection function



 /* crossover function takes a pointer to array of chromes */
 void  *crossover(chrome next_population[]) {
   int random;
   int i;
   random = rand();                  // random cross over point
   random = ((random%5) + 1);

         // cross point should be between (1 - 5)
   for (i=0; i<random; ++i){       // crossing the bits below the cross point index
     //next_population[2].bit[i] = next_population[0].bit[i];    //child 1 cross over
     //next_population[3].bit[i] = next_population[1].bit[i];    // child 2 cross over
     for (int j = 0; j < POP_SIZE/2; ++j)
       next_population[j+POP_SIZE/2].bit[i] = next_population[j].bit[i];
   }   // end of for


   for (i=random; i<6; ++i) {    // crossing the bit beyond the cross point index
     //next_population[2].bit[i] = next_population[1].bit[i];    // child 1 cross over
     //next_population[3].bit[i] = next_population[0].bit[i];    // child 2 cross over
     for (int j = 0; j < POP_SIZE/2; ++j)
       next_population[j+POP_SIZE/2].bit[i] = next_population[j].bit[i];
   }   // end of for

   for (i=0; i<POP_SIZE; ++i)       // calculating the fitness of the new genes
     next_population[i].fit = y(x(next_population[i]));

     for (i=0; i<POP_SIZE; ++i)
     printf("\nCrossOver next_population[%d]=%d%d%d%d%d%d   value=%d    fitness=%d",i,
     next_population[i].bit[5], next_population[i].bit[4], next_population[i].bit[3],
     next_population[i].bit[2], next_population[i].bit[1], next_population[i].bit[0],
     x(next_population[i]), next_population[i].fit);
     // printing the bits, value and fitness for the chromes of the new set
   return 0;
 }       // end of crossover function



 /* mutation function given a pointer to an array of chromes */
 void *mutation(chrome next_population[]) {
   int random;
   int row, col, value;
   random = rand()%50;     // random value is between (0 - 49)

   if (random==25) {        // supposing probabilty of mutation is 2 %
     col = rand()%6;          // random column (gene) choosing
       row = rand()%POP_SIZE;        // random row (chrome)  choosing

       //invert the bit to 1 if it was 0
       if (next_population[row].bit[col]==0)
       next_population[row].bit[col]=1;

       // invert the bit to 0 if it was 1
       else if (next_population[row].bit[col]==1)
       next_population[row].bit[col]=0;

       // calculating the fitness for the mutated chrome
       next_population[row].fit=y(x(next_population[row]));
       value = x(next_population[row]);

       // print the chrome index, bits, value and fitness of the mutated chrome
       printf("\nMutation occured in the next_population[%d] bit[%d]:=%d%d%d%d%d%d   value=%d fitness=%d", row, col, next_population[row].bit[5], next_population[row].bit[4], next_population[row].bit[3],
       next_population[row].bit[2], next_population[row].bit[1], next_population[row].bit[0], value, next_population[row].fit);
   }       // end of if
   return 0;
 }     // end of mutation





/* main function */
int main(int argc, char* argv[]) {
    int num = 10;
    int i, j;

    if ( argc > 1)
      num = atoi(argv[1]);

    /*...enter the number of iterations to run for,
      ...before finding the optimal value...
    enter: printf("\nPlease enter the number of iterations: ");
    scanf("%d", &num); */

    //chrome current_population[POP_SIZE];			// using that we make 4 chromes of the current population
    //chrome next_population[POP_SIZE];				// then we make 4 chromes of the next population
    chrome *current_population = new chrome[POP_SIZE];			// using that we make 4 chromes of the current population
    chrome *next_population = new chrome[POP_SIZE];				// then we make 4 chromes of the next population

    //if (num < 1) {
      //goto enter;					         // handling entries of negative values
    //}

    population(current_population);       			// initializing current population

    for(i=0; i<num; ++i) {               			 // loop num times
      printf("\n i = %d\n", i);          			 // print the iteration number

      for (j=0; j<POP_SIZE; ++j)
        next_population[j] = current_population[j];          // copy current population to new population for adjustment
        selection(next_population);                          // apply selection to choose fittest chromes
        crossover(next_population);                          // crossig them over to get new children chromes/population
        mutation(next_population);                           // mutating with a low probabilty

        for (j=0; j<POP_SIZE; ++j)
          current_population[j] = next_population[j];        // copying the chromes of new_population to current_population
    }                                                       // loop back until the iterations are completed
    printf("\n Press any key to end ! ");

    fflush(stdout);            				 // flush the input buffer
    //getch();

return 0;                 				 // wait for a character from the keyboard to end
}                           // end main
