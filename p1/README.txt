How to run this code
- Type "make" to compile the code.
- run "./mmm" to multiply two matrices in the matrix.txt file and put the result in res.txt.
- run "./mmm SIZE 1" replacing SIZE with a number to test the L1 , L2 cache miss rates on a matrix of that size for the invariants.
- run "./mmm SIZE 0" replacing SIZE with a number to test the Total load and store, cycles and floating point instructions for the invariants.
- The output for the two previous commands should list the matrix invariants with their corresponding PAPI measurements.
- If you want to measure the time taken running it normally will measure it with papi, but commenting out the calls to PAPI will measure it without PAPI.
- If you want to measure the time taken with CLOCK_REALTIME instead of the THREADTIME just comment out the THREADTIME call to clock_gettime and uncomment the REALTIME calls.
