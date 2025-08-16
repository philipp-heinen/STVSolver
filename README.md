# STVSolver
STVSolver is a software implementing the [Single Transferable Vote](https://en.wikipedia.org/wiki/Single_transferable_vote) (STV) system for elections and other social choices. It supports the Gregory, Meek and Warren version of STV as well as the Hare and the Droop quota.

## Installation and prerequisites
STVSolver is written in C++ and needs nothing else than the C++ standard library. Compiling is as simple as 
```
g++ -std=c++17 -o STVSolver STVSolver.cpp
```
which creates the executable ``STVSolver``.

## Usage
STVSolver requires as input a CSV file where each vote occupies one line. The $N$ candidates must be numbered as $1\dots N$. At the first position in a line, there must be the first preference of that voter, at the second position the second preference and so on. E.g. part of the CSV file could look like
```
1,2,3,4,5
3,4,2,1,5
5,4,1,3,2
...
```
This means the first voter has chosen candidate 1 as their first preference, candidate 2 as their second and so on, while the second voter has candidate 3 as first preference, candidate 4 as second preference and so on. It is possible that voters do not rank all candidates.

The general syntax is 
```
STVSolver [options] input.csv
```
The command line options are 

| option | input value | explanation |
| ------ | ----------- | ----------- |
|-s, --seats | number of seats | Sets the number of seats to fill. Default is 1. |
|-c, --candidates | number of candidates | Sets the number of candidates. Default is to infer the number from the input file. |
|-m, --method | "gregory", "meek" or "warren" | Determines which counting method to use. Default is "meek". |
|-q, --quota | "hare" or "droop" | Determines which quota to use. Default is "hare". |
|-r, --random | – | If this flag is set, in case of exactly equal vote numbers between two candidates, a random generator is used to decide which to exclude. Default is to exclude the one with the lower index instead.| 
|-f, --file | filename base for the output files | If provided, the elected candidates as well as the intermediate candidate votes and weights are written into output files named `<filename base>_votes.csv`, `<filename base>_weights.csv`, `<filename base>_elected_candidates.csv`. Default is not to create any output file but to write these data to the standard output only.|

For example, a command processing the votes stored in `election.csv` could look like
```
STVSolver -s 5 -c 40 -m "warren" -q "hare" -r -f "output" election.csv
```
This will write the intermediate and final results of the processing to the standard output as well as files `output_votes.csv`, `output_weights.csv`, `output_elected_candidates.csv`.
