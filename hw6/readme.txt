For this assignment, I was to implement multithreading for an implementation of SRT. 
The code works by generating the color of each pixel in a nested loop. Therefore, to split up 
this task among multiple threads, it becomes a simple matter of having each thread simultaneously 
generate a different section of the overall picture, then combine all the sections. In my case, I 
divided the picture into vertical strips.

In order to generate the pixels in multiple threads, I had to put the entire nested loop in a function
that could be called by pthread_create. The first problem I encountered was that many of the variables
declared in main() could no longer be accessed in the loop once it was put in a function. This was solved
by making the variables global.

Next, I ran the code as is but found that the output images were jumbled up. This was caused by 
individual threads simply outputting the pixels in the order that they were generated, rather than 
in the proper location in the image. I solved this problem by creating an array containing space for 
each pixel in the image. Instead of having the threads output the pixels, I had them write the pixels
to the proper location in the array, then I outputted the contents of the array after all threads 
had finished. This resulted in the proper image being generated.

We can see the drastic improvement in real time performance with increasing numbers of threads:

time ./srt 1-test.ppm >1-test.ppm.tmp

real	0m48.143s
user	0m48.148s
sys	0m0.001s

time ./srt 2-test.ppm >2-test.ppm.tmp

real	0m25.195s
user	0m48.780s
sys	0m0.003s

time ./srt 4-test.ppm >4-test.ppm.tmp

real	0m16.509s
user	0m48.541s
sys	0m0.006s

time ./srt 8-test.ppm >8-test.ppm.tmp

real	0m10.595s
user	0m50.227s
sys	0m0.003s