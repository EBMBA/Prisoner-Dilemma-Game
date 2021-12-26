# Prisoner Dilemma

<b>Project realised by KVEP in 3ICS</b>
<br>

***

## Project

<br>

The ICS (institute of Cognitive Sciences) is an interdisciplinary laboratory which integrates the expertise of researchers in the Life Sciences (cognitive psychology, neurosciences) and medicine (child psychiatry, neuro-pediatrics) with that of researchers in the Human and Social Sciences (computational and theoretical linguistics and philosophy) to study the nature and specificity of the human mind.

The doctoral student, who is not a developer, needs to accumulate experimental data. He needs volunteers to play against each other an indefinite number of times, never knowing who their opponents are.  

We will define a game as being a certain number of rounds. A round is defined as a betrayal-collaboration confrontation between the two volunteers.

***

<br>

## Rules and functioning  

<br>

A game is composed of several rounds. Each round of the game, a certain sum is won (fixed sum throughout the game)

Player gain a percentage of sum depending on the players choice : <br>
- If both players decide to cooperate, each of them gets 50% of the winnings
- If a player decides to betray the other player who cooperates with him, the traitor gets 100% of the winnings.
- If both players decide to betray, neither of them gets any money.

<br>

| Player 1 | Player 2 | Gain                                         |
| --------------- | --------------- | ------------------------------------------------------ |
| Cooperate | Cooperate |  50% / 50%                                |
| Betray     | Cooperate | 100% / 0% |
| Cooperate | Betray     | 0% / 100%|
| Betray    | Betray     | 0% / 0%                |

<br>

Between each round, the players know what they won on the previous round allowing them to adapt their future choices.  

All parameters like number of rounds in a game, sum put into play each round, name of the game, etc... can be modified in the server config file.

At the end of a game, a file containing the choices of each player in each round, the decision time and the amounts won.

***

<br>

## Command

<br>

`make all` : compile the project <br>
`make server` : compile the server <br>
`make client` : compile the client <br>

`make run` : start the project (1 server and 2 client) <br>
`make runServer` : start the Server only<br>
`make runClient` : start the Client only<br>

`make documentation` : create documentation in `doc` folder, and open it <br>
`make clean` : clean up directories containing source code and documentation

<br>

***

###
