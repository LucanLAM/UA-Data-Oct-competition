# UA-Data-Oct-competition
This C++ program processes and analyses messy player performance data. It automatically cleans, structures, and computes meaningful statistics such as win rates and expected values based on tier differences between players. The tool is designed to make sense of inconsistent data and extract insights into player performance trends.

## Features
- Cleans and structures raw, inconsistent player data
- Detects and resolves name duplicates
- Computes win rates and score adjustments based on tier differences
- Exports results to CSV for further analysis

## Table of Contents
- [Background](#background)
- [Data Interpretation](#data-interpretation)
- [Cleaning and Organising Data](#clean-and-organise-data)
- [Duplicate Checking](#check-duplication)
- [Score Calculation](#score-calculation)
- [Conclusion](#Conclusion)

## Background
My friend and I participated in a UA competition from September to October 2025, which lasted for four weeks.
The game host recorded all match data in a notebook app on his phone.
This program was created to clean, organise, and analyse that raw data using C++, turning unstructured notes into clear, meaningful performance statistics.

## Data Interpretation
In each round of the game, the game host records two entries. They are:\
\<Player 1 name\>,\<Player 1 Card Name\>,\<Tier\>,\<Result (Win/Loss)\>\
\<Player 2 name\>,\<Player 2 Card Name\>,\<Tier\>,\<Result (Win/Loss)\>\
However, the game host's records are inconsistent. Missing tier, same player or card has a different name. Therefore, it needs some manual modification.
### Raw Data Example
Below are examples of the unprocessed data as recorded by the game host:
```
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
```

## Clean and Organise Data

The file **`openfile.cpp`** is responsible for cleaning and organising raw match data from the input file **`DataUA`**, and storing the processed results in a CSV file for further analysis.

### Cleaned and Organised Format
After processing, the data is structured into a consistent and machine-readable format:
\<Player name\>,\<Card Name\>,\<Tier\>,\<Result (Win/Loss)\>

For example, the following line should be organised to the:\
YMH:綠雀頭 T1.5 W+10\
YMH,綠雀頭,1.5,1

Here, **1** represents a win and **0** represents a loss.

Since the original data is often inconsistent or incomplete, the program attempts to interpret and extract valid information automatically. If it cannot determine the correct structure, it outputs the problematic line and prompts the user to review and modify it manually.

### Separator Detection
The program attempts to identify **separators** that divide each piece of information within a raw data line.\
Expected general format:\
\<Player name\>\<separator 1\>\<Card Name\>\<separator 2\>\<Tier\>\<separator 3\>\<Result (Win/Loss)\>

The candidates for the separator are the following:
| separator   | Candidates                     |
|-------------|:------------------------------:|
| separator 1 | Whitespace " ", ":"            |
| separator 2 | "T" or "t" followed by a number|
| separator 3 | "w", "l", "+", "-"             |

If the program cannot detect the required separators, it cannot correctly extract the data fields. In such cases, the line will be considered as **unreadable**, and the program will prompt the user to manually review and modify the entry.

## Check duplication
The file **`UA_dup.cpp`** is responsible for generating a unique list of player names and card types. Since the game host occasionally used inconsistent naming conventions (e.g., `Ray` and `RAY`, `Francis` and `FFC`, `羽川` and `紫物語`), the program helps identify potential duplicates so the user can review and unify them manually.

## Score Calculation
The file **`UA_score`** handles score calculation.  
Each player starts with an initial score of **40**. The card tier ranking ranges from **1 (highest)** to **5 (lowest)**, increasing in increments of **0.5**. When a player wins, they gain points while their opponent loses the same amount. The total change in score for both players in a round always sums to **zero**. However, a player’s score cannot drop below **0** — for example, if a player with 10 points loses a round and should lose 20 points, their final score will be set to **0**.

The score calculation is divided into two parts:

#### Simple Calculation
Used when both players are of the same tier, or when a lower-tier player loses to a higher-tier player.
  - Winner: +10
  - Losser: -10
#### Complex Calculation
Used when players have different tiers and the winning player has a lower-tier card.   
  - If a lower-tier player defeats a higher-tier opponent, they gain **extra points** as a reward.
  - Conversely, if a higher-tier player loses to a lower-tier opponent, they lose **additional points**.
  - Winner: 10 + |Tier Difference| * 10
  - Losser: -10 - |Tier Difference| * 10

Player 1’s score is calculated first, and Player 2’s score is always the negative of Player 1’s score.

The table below shows the conditions under which the **complex score calculation** is applied for Player 1.

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

Hence, Player 1’s result determines the score’s sign (positive or negative).  
The following condition determines whether **complex calculation** should be applied:
```
tier_difference > 0 && ((round.record1.result == 0 && (round.record1.tier<round.record2.tier))||(round.record1.result == 1) && (round.record1.tier>round.record2.tier
```

## Conclusion
The **`stas_analysis.cpp`** handles the following analysis.  
The following table shows the performance when two players use different tier cards:

| Tier Difference | Rounds | Win Rate | EV |
|:-:|:-:|:-:|:-:|
| 0 | 50 | n/a | n/a |
| 0.5 | 26 | 46.15% | 1.53846 |
| 1 | 19 | 31.58% | -0.526316 |
| 1.5 | 3 | 33.33% | 1.66667 |
