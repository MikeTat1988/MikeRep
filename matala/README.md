Project:      HOME_ASSIGNMENT
Description:  README FILE
Author:       Michael
Date:         13/12/23
Version:      1.1 = threadpool added

For understanding the performance you can read AssignmentBenchmark.pdf

Benchmark file create an instance of Data class, default size of 1 mil ints [0;100],
writes it to binary file in ./data folder and it will perform 4 tests on it:

    a. Regular Bit packing test (each int becomes 7 bits)
    b. Bitpacking with Read-Length (34 34 34 34 becomes 127 34 4 - where 127 is a special value)
    c. Bitpacking with Read-Length on same data after rearranging it to make repetitions
    d. Multithreaded Bitpacking
    e. Multithreaded Bitpacking using Threadpool

On each test it reads the previously created data to a vector, compresses it, 
decompresses and then checks that the length and the members are the same as original.
if g_toWrite is set (false by default) it also writes the compressed data to a 
corresponding file in ./data folder.

Test results (passed / not passed) will be printed into a log file located in log
folder.

After that the g_Test and g_toEtite are set to false to not interfere with the 
times testing and the benchmark tests are being run - 

    a. Single threaded compress - bitpacking algorithm
    b. Multithreaded compress - bitpacking algorithm
    c. Multithreaded compress with Threadpool 
    d. Single threaded decompress of data
    e. Multithreaded decompress of data
    f. Multithreaded decompress with Threadpool 

********************************************************************************
run make before runing the execurtable

To run the benchmark, use "./benchmark DATASIZE THREADCOUNT WRITE"

* DATASIZE - number of int elements you want to test (1000000 on default)
* THREADCOUNT - for how many threads will spread the compression (6 on default)
* WRITE - 1 to test the algorithms, 0 to skip the test ( 0 on default )

example ./benchmark 10000 4 1

NOTE :  if the values are entered incorrect - it will run with defaults
        you can enter the values partually (only the size, or size and threadcount)

********************************************************************************

Results -

For each run of benchmark file there will be two results inseted - Parallel Compress 
and Regular Compress times in milliseconds , as follows :

* Benchmark time:   time elapsed from start to end of compression / decompression
* CPU run time:     additional info - time elapsed on CPU threads
* User mode time:   additional info -time elapsed in user mode
* Kernel mode time: additional info -  time elapsed in kernel mode 

example from log :
* [18:57:36.403] INFO [test/benchmark.cpp:310] 6 Threads Bitpacking Decompress: 
* [18:57:36.403] INFO [test/benchmark.cpp:311] Benchmark time:   13.221033 ms
* [18:57:36.403] INFO [test/benchmark.cpp:312] CPU run time:     59.417000 ms
* [18:57:36.403] INFO [test/benchmark.cpp:313] User mode time:   60.000000 ms
* [18:57:36.403] INFO [test/benchmark.cpp:314] Kernel mode time: 0.000000 ms

