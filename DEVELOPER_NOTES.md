## Introduction
On this note i will talk a little bit about how was my experience doing this library, some challenges and solutions that i found, the good parts and the boring ones too, hope you enjoy!

## Hello!
My name is Murilo, I'm 20, and i'm from Brazil, actually i'm a student of computer science at a university called IFFAR, i always loved technology so much, and having the opportunity to learn deeply how it works is amazing. 

## Motivation
This is my very first library, and my goal here was not to make the most efficient dynamic array in C, I'm sure you can find appropriate libraries to it. I did it to explore my knowledge not just about the language C but with all the process associated with it, i'm happy to say that it works! You will find functions that did not use "conventional" logic, because i tried my best to make all from scratch, just knowing what that function was supposed to do!

## Why specifically dynamic array?
Well, I began studying C# to learn a about OOP, and when i was taking a look at its built-in data structure, i stopped with something called Lists, until that time, i only knew what C has and what i did learn to do on it, somethings like:
- Linked List
- Stack
- Binary tree
- Queue
So, looking at a array that can automatically reallocate it's size to a bigger one when detects that the space already allocated is running out was AMAZING.
This made me think if it was possible to me to just implement it in C, at that time i was not looking to make a library, just to see if i could do it, and, as you can see, it works!

## Challenges
### Introduction
Working with generic types is a big challenge because you mostly need to use void pointer and work with abstract data, what leads you to work with the lowest level memory functions in C, until now, I never used any void pointers or knew anything about making a library.
### Memorable errors
I stepped with lots of errors, and those below at the most memorable that i will never forget!
#### Segmentation Fault
By the course of this path, i saw many many many many `SEGMENTATION FAULT`, gosh, i swear i dreamed with this type of error, but amazes me the fact that C allows you to go so deep on memory manipulation that you can actually get an error like this one! 
#### Off-by-one 
This one was by far the most challenging problem that i found, the fact that index starts with 0 and you quantify the number of elements in a array starting with 1 lead me to many many errors, mostly because i did not know when i was using an index and when i was looking for quantify the elements.

### What is a library?
Until the beginning of this project, i just used libraries, but never looked up how actually those are made, and to be honest, even now i don't know much, but surely i learned a lot doing one. And this brings a boring topic that i honestly did not like doing...
#### Documentation
For god sake, document a code is so so boring, basically half of the lines used at darray.c are documentation lines, and it's hard to find the correct words to describe in just a few of them how that function works and what it does.
#### More than one file?
Well, when i started this project, it was not a project, just a doubt to be cleared, and i never separated my code on two files, i actually did not even know i can have a `.h` extension, so i had to explore a bit how to structure my code, and with it, somethings like what the prefix `static` means and adding my own `.h` as a library to the main implementation file!

### Low level memory manipulation
This is the topic that by far most amazes me, i had to learn exactly how C manipulates the memory, about heap, stack, global variables memory and what declare types actually does. Working with abstract types forces you to understand how pointers works, the importance of casting, learn that the OS can deeply change how everything works, and why making multi-platform applications are not that easy, i went until functions like `brk`and `sbrk`, learning that the power that C have is all gave by the OS, and we live on a illusion even with memory addresses, virtual memory showed me that!! 

#### String.h functions
I found out that this library is not just for strings! It has some really fun features like `memcpy`, `memset`, `memmove` and etc. The way i organized my functions you will see that i suddenly started using it when i found this actually exists, and this topic leads to the most controversial one, using AI.

### Design decisions
After the end of the project, i went to study a bit about how actually devs implements some of the functions that i tried to do by myself, and i stepped on more efficient solutions to some functions like `array_binary_search()` or `array_reverse()`, and some of a pattern that is followed by developers, and for this project, i decided to keep my logic and my chooses how they were, i think on the future, i will look behind and see those implementations and i will be able to see how much i actually improved, as i said, the main goal here was not to make a efficient and standard library, was to test my knowledge!
## The impact of AI
Before start this topic, i want to make clear that all the code as written by me, i honestly don't think that use IA to make code for you is even programming. But YES, i used it a LOT during the course of the project mainly to explain some functions for me and alternatives that i could use, and one of this alternatives was [[#String.h functions]], this was game changer for me, and i could not continue doing any other line of code before learning how those functions works and how i could use it for my benefit.
I also used IA to help me debug parts of my code where i could not figure out what was happening, and some security that I left behind.
### My opinion about AI
I think IA changed the world and it will keep changing for many many years, and the impact that it has on programming was immeasurable, I use it a lot to learn, and Its a phenomenal tool, at this project, the whole `main.c` was done by IA, and i think it's OK, because this file is just the presentation of what I made and implemented by myself.
Other than this file, pretty much everything i did was taken out of my neurons, and I'm sure many of them died on this process, lol.

## Conclusion
Making this project was a big challenge, it had cool moments, boring moments, studying moments, neuron burn moments, but at the and, I can say that this thing works, and it is well documented, at least i hope so. Thank you for reading, see you on the next project!