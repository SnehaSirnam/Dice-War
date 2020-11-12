# Dice-War

Modular Programming is implemented to achieve this goal. The requirement was to achieve thread synchronization which is achieved by thread semaphore and mutex variables 
which locks and unlocks the resources when needed in main program 5 threads are created 4 for players and one for dealer to atomize the random number generator for dice throw.
A single code file is compiled to run the code the following command will compile it in linux environment  
Please go to the folder and execute below commands in order
                                            gcc -std=c99 main.c -lpthread
                                           ./a .out 30
