# The Game of Din

The game of Din is a single-player game in which the player starts off with three boxes of marbles (or other small items like stones or playing cards). The only valid move in the game is to transfer marbles from one box to another in such a way that the number of marbles in the target box is doubled. The objective of the game is to end up with an empty box.

Let's call the boxes A, B and C and look at an example game:

|A|B|C|Move|
|-|-|-|-|
|1|2|3||
||||C&rarr;B
|1|4|1||
||||C&rarr;A
|2|4|0||

Box C is empty and the game is over.

There are other sequences of moves which complete this configuration. For example a first move of C&rarr;A will leave two marbles in each box and any other move will achieve the objective.

Write a program which plays the game of Din. On the standard input the program shall receive three integers separated by space **A**, **B** and **C** (**0 < A < B < C < 100 000**) &ndash; the number of marbles in each box, respectively. On the standard output the program shall print the sequence of moves, each on a separate line, in the format `<source box>-><target box>`. If the objective cannot be completed, the program shall print `NOT POSSIBLE` on the standard output.

## Example
**Input:**
```
1 2 3
```

**Output:**
```
C->B
C->A
```

*We remind you that this is not the only valid output for the example input*

## Ranking

The maximum allowed time for the program per test is 5 seconds. All solutions which exceed this time or produce a wrong answer (a sequence of moves which doesn't lead to an empty box, or an ouput of `NOT POSSIBLE` when there is a possible solution) receive 0 points.

After this there are two modes of ranking:

### 1. Shortest Move Sequence

For each test the correct solutions are sorted by the number of moves. The solutions with the shortest sequence receive 10 points and the rest receive 1 point. The shortest sequence solutions are additionally sorted by execution time and the top three of them receive 3, 2, and 1 additional points.

### 2. Fastest Correct Solution

For each tests the solutions are sorted by execution time regardless of the number of moves. The top 9 of them receive from 10 to 2 points respectively and the rest receive 1 point.
