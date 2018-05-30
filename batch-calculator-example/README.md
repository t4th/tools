# batch calculator

Example of making program faster by restructuring data access.

## why

After watching Mike Acton video about data oriented programing i tested what he preached and this small program is one of follow up prove of concept result. As poc, this is draft code - no checks, no const correctness, no nothing :). Things that might cause problems are commented.

## how

Build and run without argument for no optimization and with any argument for optimization run.
Code is pretty straight forrward. To increase scale for faster/slower targets adapt buffer count:
```c++
#define BUFFER_COUNT 10000000
```
 or cache line adapted batch size:
 ```c++
#define BATCH_SIZE   32
```
 
## example output

run non-optimized
```
C:\Projects\github\tools\Debug>example1-non-optimized.exe
run non-optimized
time elapsed: 0.0873855
```

Typical non-optimized time is between 0.083 - 0.089 ticks on my pc due to scheduling and other independant stuff.

run optimized
```
C:\Projects\github\tools\Debug>example1-non-optimized.exe 1
run optimized
time elapsed: 0.03666
```

Typical optimized time is between 0.033 - 0.037 ticks.

So thats almost ~2.5x time less with almost no effort, other than restructuring data access.
