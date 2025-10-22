# UA-Data-Oct-competition
This C++ program processes and analyses messy player performance data. It automatically cleans, structures, and computes meaningful statistics such as win rates and expected values based on tier differences between players. The tool is designed to make sense of inconsistent data and extract insights into player performance trends.

## Background
My friend and I participated in a UA competition from September to October 2025, which lasted for four weeks.
The game host recorded all match data in a notebook app on his phone.
This program was created to clean, organise, and analyse that raw data using C++, turning unstructured notes into clear, meaningful performance statistics.

## Clean and Organise Data

The file **`openfile.cpp`** is responsible for cleaning and organising raw match data from the input file **`DataUA`**, and storing the processed results in a CSV file for further analysis.

### Raw Data Example
Below are examples of the unprocessed data as recorded by the game host:

YMH:紫坂 T1.5 W +10\
YWH:手指 T1.5 -10

YMH:黃坂 T1.5 L-10\
YWH:早乙女T1.5 W+10

YMH:綠雀頭 T1.5 W+10\
Eric:黃EMT T1.5 L-10

YWH:早乙女 T1.5 -10\
Francis:紫EMT T1.5 +10

YWH:兵長 T2 -10\
Francis:忍野 T1.5 +10

YWH:早乙女 T1.5 -10 （先功 AP抽4次0 零費）\
Eric:黃EMT T1.5 +10

### Cleaned and Organised Format
After processing, the data is structured into a consistent and machine-readable format:
\<Player name\>,\<Card Name\>,\<Tier\>,\<Result (Win/Loss)\>

For example, the following line should organised to the:\
YMH:綠雀頭 T1.5 W+10\
YMH,綠雀頭,1.5,1

Here, **1** represents a win and **0** represents a loss.

Since the original data is often inconsistent or incomplete, the program attempts to interpret and extract valid information automatically. If it cannot determine the correct structure, it outputs the problematic line and prompts the user to review and modify it manually.

### Separator Detection
The program attempts to identify **separators** that divide each piece of information within a raw data line.\
Expected general format:\
\<Player name\>\<Seperator 1\>\<Card Name\>\<Seperator 2\>\<Tier\>\<Seperator 3\>\<Result (Win/Loss)\>

The candidate for the seperator are the following:
| Seperator   | Candidates                     |
|-------------|:------------------------------:|
| Seperator 1 | Whitespace " ", ":"            |
| Seperator 2 | "T" or "t" followed by a number|
| Seperator 3 | "w", "l", "+", "-"             |

If the program cannot detect the required separators, it cannot correctly extract the data fields. In such cases, the line will be considered as **unreadable**, and the program will prompt the user to manually review and modify the entry.

## Check duplication
The file **`UA_dup.cpp`** is responsible for generating a unique list of player names and card types. Since the game host occasionally used inconsistent naming conventions (e.g., `Ray` and `RAY`, `Francis` and `FFC`, `羽川` and `紫物語`), the program helps identify potential duplicates so the user can review and unify them manually.

## Score Calculation
Every players have a initial score of 40. The tier rank from highest (1) to lowest (5) with increamentation of 0.5. When a player win, he/she gain points and their opponent reduce point. Note that the sum of points gained and point deduced are zero. Also, a player cannot has score lower than zero. If a player who score 10 loss a round and need to reduce his/her point by 20. His/her final score will be 0.\
The score calculation is seperated into two part, simple and complex. If a player use a lower tier card against a player who use a higher tier and win, the player gain extra point by following the complex score calculation. The following table show the condition that a complex score calculation is needed.

| Player 1 result (win = 1, loss = 0) |Tier difference (no = 0, yes = 1) | Is player 1 has higher tier (no = 0, yes = 1) | Simple (0) or Complex (1) | pos or neg score (pos = 1, neg = 0) |
|:-:|:-:|:-:|:-:|:-:|
| 0 | 0 | 0 | 0 | 0 |
| 0 | 0 | 1 | 0 | 0 |
| 0 | 1 | 0 | 0 | 0 |
| 0 | 1 | 1 | 1 | 0 |
| 1 | 0 | 0 | 0 | 1 |
| 1 | 0 | 1 | 0 | 1 |
| 1 | 1 | 0 | 1 | 1 |
| 1 | 1 | 1 | 0 | 1 |
