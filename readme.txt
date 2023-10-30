Lab 2 Eric Todd (490360) e.todd@wustl.edu

Description:
    This program is a card game simulator which supports Pinochle and Texas HoldEm. It takes in a game type and player names as command line arguments. 
    It then deals cards to the players and prints them. Then it asks if the user wants to end the game. If not, it deals another board and prints it, 
    continueing for as long as the user wants to play. In either game mode it will evaluate the board state and display the results to the user.

Compilation:
    make
    or: g++ -o lab2 -DUNIX   -Wall -W -g -std=c++17 -DTEMPLATE_HEADERS_INCLUDE_SOURCE Suit.cpp PinochleDeck.cpp HoldEmDeck.cpp main.cpp Game.cpp PinochleGame.cpp HoldEmGame.cpp

Usage:
    "Usage: lab2 <Game(Pinochle|HoldEm)> <player1> <player2> ... <player9>"
            "Pinocle requires 4 players, HoldEm requires 2-9 players"

Design:
    The program is designed to be extensible to other card games. The Game class is an abstract class which defines the interface for a card game. 
    It has a pure virtual function deal which deals cards to the players. It also has a pure virtual function print which prints the players' hands and the board.
    The PinochleGame and HoldEmGame classes inherit from Game and implement the deal and print functions. They are also templatized based on a game specific Rank
    and Suit. 

Learning:
    I learned a lot about working with a larger code base with a lot of logic. Adding in the game logic adds a lot of complexity, and I tried very hard
    to keep the code clean and readable. In PinochleGame::suit_independent_evaluation() I wrote a few helper lambdas that allowed me to give a descriptive
    name to some potentially confusing operations. It lead to some very nice expressions such as:

        if(hand_has_both_of_each_suit(PinochleRank::ace)) { // Thousand Aces
            melds.push_back(PinochleMelds::thousandaces);
        } else if(hand_has_one_of_each_suit(PinochleRank::ace)) { // Hundred Aces
            melds.push_back(PinochleMelds::hundredaces);
        }
    
    which I am particularly proud of. The code almost reads like an english sentence that perfectly explains it's function. 

    HoldEmGame::holdem_hand_evaluation() was a much more complicated function to write, and I ended up splitting it into many different functions, one
    for each of the possible hand ranks. I spent the majority of my time and effort in HoldEmGame.cpp working on this problem and trying to keep code
    duplication to a minimum. To this end I ended up adding additional output parameters to the check functions to report more information about the
    result, beyond the return value of true or false. For example, checkPair sets a parameter reference to the pair it finds. This was crucial for the
    operator< code. I also wrote an overloaded version of these check functions without the out parameter that just forwards to the base function so
    that I could use them when I just needed to check something and didn't care about any more information.

    Overall I had to think about, plan for, and rework this code quite a bit to keep it as clean and efficient as I could, and I am very proud of the
    result.

Compilation Errors:
    PinochleDeck.cpp: In constructor ‘PinochleDeck::PinochleDeck()’:
    PinochleDeck.cpp:3:28: error: use of deleted function ‘Deck<PinochleRank, Suit>::Deck()’
        3 | PinochleDeck::PinochleDeck() {
        |                            ^
    In file included from PinochleDeck.h:10,
                    from PinochleDeck.cpp:1:
    Deck.h:11:7: note: ‘Deck<PinochleRank, Suit>::Deck()’ is implicitly deleted because the default definition would be ill-formed:
    11 | class Deck : public CardSet<R, S> {
        |       ^~~~

    Fixed by adding default constructor to CardSet, required for sorting

    In file included from HoldEmGame.cpp:4:
    HoldEmGame.h: In instantiation of ‘void std::__insertion_sort(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<HoldEmGame::Player*, std::vector<HoldEmGame::Player> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]’:
    /usr/include/c++/11/bits/stl_algo.h:1866:25:   required from ‘void std::__final_insertion_sort(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<HoldEmGame::Player*, std::vector<HoldEmGame::Player> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]’
    /usr/include/c++/11/bits/stl_algo.h:1957:31:   required from ‘void std::__sort(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<HoldEmGame::Player*, std::vector<HoldEmGame::Player> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]’
    /usr/include/c++/11/bits/stl_algo.h:4842:18:   required from ‘void std::sort(_RAIter, _RAIter) [with _RAIter = __gnu_cxx::__normal_iterator<HoldEmGame::Player*, std::vector<HoldEmGame::Player> >]’
    HoldEmGame.cpp:85:14:   required from here
    HoldEmGame.h:66:12: warning: implicitly-declared ‘CardSet<HoldEmRank, Suit>& CardSet<HoldEmRank, Suit>::operator=(const CardSet<HoldEmRank, Suit>&)’ is deprecated [-Wdeprecated-copy]
    66 |     struct Player{
        |            ^~~~~~
    In file included from CardSet.h:42,
                    from Deck.h:8,
                    from HoldEmDeck.h:10,
                    from HoldEmGame.h:8,
                    from HoldEmGame.cpp:4:
    CardSet.cpp:36:1: note: because ‘CardSet<HoldEmRank, Suit>’ has user-provided ‘CardSet<R, S>::CardSet(const CardSet<R, S>&) [with R = HoldEmRank; S = Suit]’
    36 | CardSet<R, S>::CardSet(const CardSet<R, S>& other) : cards(other.cards) {}
        | ^~~~~~~~~~~~~

    Fixed by using default copy constructor for CardSet. This still performs a deep copy as the copy constructor of a vector performs a deep copy.


Testing:
    No args:
        ./lab2
        Usage: lab2 <Game(Pinochle|HoldEm)> <player1> <player2> ... <player9>
        Pinocle requires 4 players, HoldEm requires 2-9 players
        echo $?
        1
    Invalid game:
        ./lab2 TicTacToe
        Usage: lab2 <Game(Pinochle|HoldEm)> <player1> <player2> ... <player9>
        Pinocle requires 4 players, HoldEm requires 2-9 players
        echo $?
        1
    No player names:
        ./lab2 Pinochle
        Usage: lab2 <Game(Pinochle|HoldEm)> <player1> <player2> ... <player9>
        Pinocle requires 4 players, HoldEm requires 2-9 players
        echo $?
        1
    Wrong number of player names:
        ./lab2 Pinochle bob joe
        Usage: lab1 <Game(Pinochle|HoldEm)> <player1> <player2> ... <player9>
        Pinocle requires 4 players, HoldEm requires 2-9 players
        echo $?
        1
    Pinochle works:
        ./lab2 Pinochle bob joe mary john
        bob: 9H QS JS 9C KD TH KD JD 9C AC TS AS 9C 9C 9H JD JS QS KD KD TH TS AC AS
                Melds: Pinochle 40 points,
        joe: AS AH QH AC KH AH TH QH AD 9D KC QS 9D QH QH QS KC KH TH AC AD AH AH AS
                Melds: Hundred Aces 100 points,
        mary: AD 9H 9D JC KH QD QC KS TC JD 9S QC 9D 9H 9S JC JD QC QC QD KH KS TC AD
                Melds:
        john: JH KS JS TC KC JH TS TD TD JC 9S QD 9S JC JH JH JS QD KC KS TC TD TD TS
                Melds:
        Would you like to end the game? (yes/no): yes
        roboevt@DESKTOP-JHQK8MN:/mnt/c/dev/428/lab2$ echo $?
        0
    HoldEm works:
        ./lab2 HoldEm bob joe mary john
        bob: AH 10C
        joe: 6C 4H
        mary: JD 6H
        john: KC JS
        BOARD(flop): 4C 7C QD
        joe:    Cards: 6C 4H QD 7C 4C    Rank: Pair
        bob:    Cards: AH 10C QD 7C 4C   Rank: X High
        john:   Cards: KC JS QD 7C 4C    Rank: X High
        mary:   Cards: JD 6H QD 7C 4C    Rank: X High
        BOARD(turn): 4C 7C QD 9C
        BOARD(river): 4C 7C QD 9C 6D
        Would you like to end the game? (yes/no): yes
        roboevt@DESKTOP-JHQK8MN:/mnt/c/dev/428/lab2$ echo $?
        0
    HoldEm Multiple runs (different cards and correctly identifying hand ranks):
        ./lab2 HoldEm bob joe mary john
        bob: KD 5C
        joe: 4H 8H
        mary: 10H AC
        john: 9H 9S
        BOARD(flop): 8D 9C KS
        john:   Cards: 9H 9S KS 9C 8D    Rank: Three of a Kind
        bob:    Cards: KD 5C KS 9C 8D    Rank: Pair
        joe:    Cards: 4H 8H KS 9C 8D    Rank: Pair
        mary:   Cards: 10H AC KS 9C 8D   Rank: X High
        BOARD(turn): 8D 9C KS 7C
        BOARD(river): 8D 9C KS 7C QC
        Would you like to end the game? (yes/no): no
        bob: 6D 9D
        joe: 3D 7H
        mary: 3C 2C
        john: 3H 5S
        BOARD(flop): 5C 2S QH
        john:   Cards: 3H 5S QH 2S 5C    Rank: Pair
        mary:   Cards: 3C 2C QH 2S 5C    Rank: Pair
        bob:    Cards: 6D 9D QH 2S 5C    Rank: X High
        joe:    Cards: 3D 7H QH 2S 5C    Rank: X High
        BOARD(turn): 5C 2S QH 4C
        BOARD(river): 5C 2S QH 4C 4H
        Would you like to end the game? (yes/no): no
        bob: 5C 10H
        joe: 3C 5S
        mary: 8D JS
        john: 6C QS
        BOARD(flop): 6D 9D 10S
        bob:    Cards: 5C 10H 10S 9D 6D          Rank: Pair
        john:   Cards: 6C QS 10S 9D 6D   Rank: Pair
        mary:   Cards: 8D JS 10S 9D 6D   Rank: X High
        joe:    Cards: 3C 5S 10S 9D 6D   Rank: X High
        BOARD(turn): 6D 9D 10S AD
        BOARD(river): 6D 9D 10S AD 5H
        Would you like to end the game? (yes/no): yes
    Pinochle Multiple runs (different cards and correctly identifying melds):
        ./lab2 Pinochle bob joe mary john
        bob: TS JC KH TS TC JC TD 9S KC 9H AC AD 9H 9S JC JC KC KH TC TD TS TS AC AD
                Melds:
        joe: TC QC QS JD JH JD KD QD QH KD QH AC JD JD JH QC QD QH QH QS KD KD TC AC
                Melds: Sixty Queens 60 points, Pinochle 40 points,
        mary: TD KC JS AH AS 9H JH 9D AH QS 9C 9D 9C 9D 9D 9H JH JS QS KC TD AH AH AS
                Melds:
        john: TH AS 9S AD QC QD 9C JS KS KH KS TH 9C 9S JS QC QD KH KS KS TH TH AD AS
                Melds:
        Would you like to end the game? (yes/no): no
        bob: AD TC 9H 9C AD 9C 9S TS QC AC TD AH 9C 9C 9H 9S QC TC TD TS AC AD AD AH
                Melds:
        joe: KC TS KH TH TC KD AS JC QC KS JS QS JC JS QC QS KC KD KH KS TC TH TS AS
                Melds: Eighty Kings 80 points,
        mary: AC 9H KD 9S 9D 9D AH QD QD KC KS QS 9D 9D 9H 9S QD QD QS KC KD KS AC AH
                Melds:
        john: TH AS JH KH JD JC QH QH JH TD JD JS JC JD JD JH JH JS QH QH KH TD TH AS
                Melds: Forty Jacks 40 points,
        Would you like to end the game? (yes/no): no
        bob: TS QH JH KD 9S JS 9C JC JD TS AS AC 9C 9S JC JD JH JS QH KD TS TS AC AS
                Melds: Forty Jacks 40 points,
        joe: QC JD QD 9D AD KH JH KS JS AH TH KC 9D JD JH JS QC QD KC KH KS TH AD AH
                Melds:
        mary: AC KS 9C JC KC QC 9H AH AD TC QS QS 9C 9H JC QC QS QS KC KS TC AC AD AH
                Melds:
        john: QD AS TH TC KD QH 9S TD 9H 9D TD KH 9D 9H 9S QD QH KD KH TC TD TD TH AS
                Melds:
        Would you like to end the game? (yes/no): yes