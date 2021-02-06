How to run this code
- Type "make" to compile the code.
- run "./mmm" to multiply two matrices in the matrix.txt file and put the result in res.txt.
- run "./mmm SIZE 1" replacing SIZE with a number to test the L1 , L2 cache miss rates on a matrix of that size for the invariants.
- Expected output
  INVARIANT Results
  L1 Data Cache miss : RESULT
  L1 Data Cache accesses : RESULT
  L1 Miss rate : RESULT
  L2 Data Cache miss : RESULT
  L2 Data Cache accesses : RESULT
  L2 Miss rate : RESULT
- run "./mmm SIZE 0" replacing SIZE with a number to test the Total load and store, cycles and floating point instructions for the invariants.

- Expected output
  INVARIANT Results
  Load instructions:  RESULT
  Store instructions:  RESULT
  Total instructions:  RESULT  
  Total cycle:  RESULT
  Total number of load and store:  RESULT
  Floating point instructions:  RESULT
  
- The output for the two previous commands should list the matrix invariants with their corresponding PAPI measurements.
- If you want to measure the time taken running it normally will measure it with papi, but commenting out the calls to PAPI will measure it without PAPI.
- If you want to measure the time taken with CLOCK_REALTIME instead of the THREADTIME just comment out the THREADTIME call to clock_gettime and uncomment the REALTIME calls.
- These values were taken on orcrist-21
