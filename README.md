# Parallel_CPP_JAVA_experiment
Trying to compare parallel processing times of c++ and java on same data set.


Repo includes a small text file **accounts -Small.txt** you must grow to excessive line numbers (1.5gb) to test *(the java program can also generate)*
and rename according to whatever is marked const/final in the programs. Each Customer entry consists of 2 lines, a str for accnt name, and a double for balance.
-This file needs to be moved into the root DIR of each version of the code you are running.

For whatever reason the Java code seems to run faster overall on smaller data in particular especially when:
-Parsing a file 

Java is significantly faster from the IDE than C++ in the IDE.

However, when getting to large data sets, and building the C++ program to RELEASE, it then becomes sigificantly faster than Java.

In the C++ version the Vector class implementation beats the Array implementation which is bizarre.
-the array version requires reading the file twice, to allocate the space for the array, which is expected to be longer,
however, even with this overhead, the actual ms of time spent calculating is still sometime significantly faster with the vector,
than passing a pointer to the array .. which makes no sense. My only guess could be something with passing by ref/pointer is going wrong,
although I have verified the memory address's are the same, so its not copying the data

-To add insult to injury, the C++ array will error out when run in RELEASE mode, but runs fine in DEBUG.

- I am not 100% sure I believe the millisecond prints statements in C++, it seems similar in time between the vector and array at lower timing,
just wild that accessing an array would take so long?

- using C++ MUTEX is absolute garbage for performance, and locks up your threads for excessive time.
	- best solution was to pass an array of summations, and give each thread an index for their local counts.
	  This was kind of gross... but works.

- using something called file.tellg(); when reading the file to advance lines made reading the file take excessive amounts of time,
	still not sure whats going on there. The C++ documentation hurts my soul, as well as 20 year old posts of ancient code snippets.

**This could all be because of my novice ability with C++ and I am simply doing something wrong.. but idk. I cant figure it out.**


__JAVA__ Output *(IDE)*:
Reading the file : / accounts.txt
parsing file run took in miliseconds: 185067
file has : 140000000 entries
Number of accounts with less than 1000$ is: 6998196
Parallel run [isOptimized = true] took in miliseconds: 198

__CPP__ Output Vector *(RELEASE)* :
Start Program accounts.txt
Done Reading File
[CreateThreadsVector] parsing file run took in miliseconds:  30739.1 ms
accounts.txt had 140000000 lines resulting in 70000000 customers

parallel c++ VECTOR time: 0.021 milisec?

The final VECTOR counter is 6998196


__CPP__ Output Vector *(DEBUG/IDE)* :
Start Program accounts.txt
Done Reading File
[CreateThreadsVector] parsing file run took in miliseconds:  460576 ms
accounts.txt had 140000000 lines resulting in 70000000 customers

parallel c++ VECTOR time: 0.02 milisec?

The final VECTOR counter is 6998196


__CPP__ Output Array *(DEBUG/IDE)*:
Start Program accounts.txt
Done Reading File
[CreateThreadsArray] parsing file run took in miliseconds:  683711 ms
accounts.txt had 140000000 lines resulting in 70000000 customers

parallel ARRAY c++ time: 179.387 milisec?

The final counter ARRAY is 6998196




