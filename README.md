TSP and TSPN for Uniform Disks
------------------------------

This software implements the following algorithms:
* Wrapper for the TSP [concorde solver](http://www.math.uwaterloo.ca/tsp/concorde.html)
* 2 approximation algorithm for the TSP using the MST
* TSPN for uniform radius disks, as given in [this paper](http://www.ams.sunysb.edu/~jsbm/papers/tspn-soda01.pdf)


USAGE
-----

There are two demo programs written in './src/'; one each for the TSP and TSPN algorithms. However, there are a few options that need to be set before you are able to execute the code. They are both in the file './include/tsp_options.h'. The two options are:

(1) Set which TSP implementation to use. Linkern and Concorde are two TSP implementations in the Concorde library. (Preferred is Concorde.) If none are set, then the two approximation is used.

(2) Set the path to where the concorde implementation is location

Once these options are set, then the code can be compiled by the usual "make" command. "make clean" removes all the build files. "make docs" creates the doxygen documentation of the code.