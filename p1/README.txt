Measurements from 6 variants of matrix multiplication Table
        50x50     100x100   200x200  400x400   800x800   1200x1200 1600x1600 2000x2000
      +---------+---------+---------+---------+---------+---------+---------+---------+
      |         |         |         |         |         |         |         |         |
IJK   |         |         |         |         |         |         |         |         |
      |         |         |         |         |         |         |         |         |
      |         |         |         |         |         |         |         |         |
      +---------+---------+---------+---------+---------+---------+---------+---------+
      |         |         |         |         |         |         |         |         |
JIK   |         |         |         |         |         |         |         |         |
      |         |         |         |         |         |         |         |         |
      |         |         |         |         |         |         |         |         |
      +---------+---------+---------+---------+---------+---------+---------+---------+
      |         |         |         |         |         |         |         |         |
JKI   |         |         |         |         |         |         |         |         |
      |         |         |         |         |         |         |         |         |
      |         |         |         |         |         |         |         |         |
      +---------+---------+---------+---------+---------+---------+---------+---------+
      |         |         |         |         |         |         |         |         |
KJI   |         |         |         |         |         |         |         |         |
      |         |         |         |         |         |         |         |         |
      |         |         |         |         |         |         |         |         |
      +---------+---------+---------+---------+---------+---------+---------+---------+
      |         |         |         |         |         |         |         |         |
IKJ   |         |         |         |         |         |         |         |         |
      |         |         |         |         |         |         |         |         |
      |         |         |         |         |         |         |         |         |
      +---------+---------+---------+---------+---------+---------+---------+---------+
      |         |         |         |         |         |         |         |         |
KIJ   |         |         |         |         |         |         |         |         |
      |         |         |         |         |         |         |         |         |
      |         |         |         |         |         |         |         |         |
      +---------+---------+---------+---------+---------+---------+---------+---------+








Answers to Questions for Project 1: Programming For Performance CS377p
Name: Abhijit Raman
EID : akr2495

Q1
What are data and control dependences? Give simple examples to illustrate these concepts?

Answer:
Data dependences occur when there exist two instructions such that one executes after the other, they both access the same data and one of the accesses is a write.
An example of a data dependence could be the instructions  i1 : movq %r1 %r2  i2 : addq %r2 %r3/. In These instructions both access r2 and write to r2 , if they execute
out of the order that they were written in then their could be a logical error to the code and it won't do what the programmer expects.

Control dependences occur in situations where an instruction(i1) will only execute if a previous instruction(i2) evaluates to value that allows i1 to execute.
So in the following example, the instruction "i = 5" only executes conditionally if i > 40 evaluates to true. So regarding the definition "i=5" is i1 and
"i > 40" is i2.

if(i > 40)
  i = 5;





Q2
Explain out-of-order execution and in-order retirement/commit. Why do high-performance processors execute instructions out of order but retire them in order?
What hardware structure(s) are used to implement in-order retirement?

Out-of-order execution and in-order retirement.
