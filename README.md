# Authors
(2014)
- Fabrice Servais
- Laurent Vanosmael

# Program
## Goal
The goal of the program is to provide the best keyboard layout regarding to an input text. This project has been made in the context of a student project for the course of *Discrete Optimization* at *ULg*. 

## Compile
Use `make` command to compile.
### Note
2 Makefile are provided depending on the platform, the "default" Makefile is for MacOS. Furthermore, the Makefile may not work depending on the version of Gurobi (v5.6.3 is used))

## Run
`./main <count of symbols file> <count of bigrams file>`
Example: `./main Datafile/sched-1.txt Datafile/sched-2.txt`
