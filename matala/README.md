Project:      HOME_ASSIGNMENT
Description:  README FILE
Author:       Michael
Date:         13/12/23
Version:      1.0

For understanding the performance you can read AssignmentBenchmark.pdf

Benchmark file create an instance of Data class, default size of 1 mil ints [0;100],
writes it to binary file in ./data folder and it will perform 4 tests on it:

    a. Regular Bit packing test (each int becomes 7 bits)
    b. Bitpacking with Read-Length (34 34 34 34 becomes 127 34 4 - where 127 is a special value)
    c. Bitpacking with Read-Length on same data after rearranging it to make repetitions
    d. Multithreaded Bitpacking

On each test it reads the previously created data to a vector, compresses it, 
decompresses and then checks that the length and the members are the same as original.
if g_toWrite is set (false by default) it also writes the compressed data to a 
corresponding file in ./data folder.

Test results (passed / not passed) will be printed into a log file located in log
folder.

After that the g_Test and g_toEtite are set to false to not interfere with the 
times testing and the benchmark tests are being run - 

    a. Regular bitpacking and unpacking of data (without testing - as it was tested earlier)
    b. Multithreaded bitpacking, using 6 cores (default - can be changed)

********************************************************************************

To run the benchmark, use "./benchmark DATASIZE THREADCOUNT WRITE"
    a. DATASIZE - number of int elements you want to test (1000000 on default)
    b. THREADCOUNT - for how many threads will spread the compression (6 on default)
    c. WRITE - 1 to test the algorithms, 0 to skip the test ( 0 on default )

example ./benchmark 10000 4 1

NOTE :  if the values are entered incorrect - it will run with defaults
        you can enter the values partually (only the size, or size and threadcount)

********************************************************************************

Results -

For each run of benchmark file there will be two results inseted - Parallel Compress 
and Regular Compress times in milliseconds , as follows :

* Wall time:        time elapsed in the real world ( with no multithreading is wequal to CPU time)
* CPU run time:     time elapsed on CPU cores ( on multithreading is the sum of all threads times)
* User mode time:   time elapsed in user mode
* Kernel mode time: time elapsed in kernel mode ( in our cas - paging of the data )

example from log :
* [16:09:32.649] INFO [test/benchmark.cpp:264] Regular Compress: 
* [16:09:32.649] INFO [test/benchmark.cpp:265] Wall time:        683.760819 ms
* [16:09:32.649] INFO [test/benchmark.cpp:266] CPU run time:     682.302000 ms
* [16:09:32.649] INFO [test/benchmark.cpp:267] User mode time:   660.000000 ms
* [16:09:32.649] INFO [test/benchmark.cpp:268] Kernel mode time: 30.000000 ms
