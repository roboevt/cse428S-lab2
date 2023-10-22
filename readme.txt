Lab 2 Eric Todd (490360) e.todd@wustl.edu

Description:
    This program is a card game simulator which supports Pinochle and Texas HoldEm. It takes in a game type and player names as command line arguments. 
    It then deals cards to the players and prints them. Then it asks if the user wants to end the game. If not, it deals another board and prints it, 
    continueing for as long as the user wants to play.

Compilation:
    make
    or: g++ -o lab1 -DUNIX   -Wall -W -g -std=c++17 -DTEMPLATE_HEADERS_INCLUDE_SOURCE Suit.cpp PinochleDeck.cpp HoldEmDeck.cpp main.cpp Game.cpp PinochleGame.cpp HoldEmGame.cpp

Usage:
    "Usage: lab1 <Game(Pinochle|HoldEm)> <player1> <player2> ... <player9>"

Design:
    The program is designed to be extensible to other card games. The Game class is an abstract class which defines the interface for a card game. 
    It has a pure virtual function deal which deals cards to the players. It also has a pure virtual function print which prints the players' hands and the board.
    The PinochleGame and HoldEmGame classes inherit from Game and implement the deal and print functions. They are also templatized based on a game specific Rank
    and Suit. 

Learning:
    I learned a lot about writing templates in c++. In particular, how to split them between header and source files, something I have struggled with in the past.
    This was also a good experience in a larger project, split between many files, even if each file is relatively simple. It is helpful to think about how to tie
    different pieces of code together and where to split them apart.

Compilation Errors:
    Deck.cpp:9:18: error: no matching function for call to ‘std::mersenne_twister_engine<long unsigned int, 32, 624, 397, 31, 2567483615, 11, 4294967295, 7, 2636928640, 15, 4022730752, 18, 1812433253>::mersenne_twister_engine(std::random_device&)’
    9 |     std::mt19937 gen(rng);
      |                  ^~~
    In file included from /usr/include/c++/11/random:49,
                    from Deck.cpp:3,
                    from Deck.h:16,
                    from PinochleDeck.h:8,
                    from PinochleGame.h:4,
                    from PinochleGame.cpp:1:
    /usr/include/c++/11/bits/random.h:541:9: note: candidate: ‘template<class _Sseq, class> std::mersenne_twister_engine<_UIntType, __w, __n, __m, __r, __a, __u, __d, __s, __b, __t, __c, __l, __f>::mersenne_twister_engine(_Sseq&) [with _Sseq = _Sseq; <template-parameter-2-2> = <template-parameter-1-2>; _UIntType = long unsigned int; long unsigned int __w = 32; long unsigned int __n = 624; long unsigned int __m = 397; long unsigned int __r = 31; _UIntType __a = 2567483615; long unsigned int __u = 11; _UIntType __d = 4294967295; long unsigned int __s = 7; _UIntType __b = 2636928640; long unsigned int __t = 15; _UIntType __c = 4022730752; long unsigned int __l = 18; _UIntType __f = 1812433253]’
    541 |         mersenne_twister_engine(_Sseq& __q)
        |         ^~~~~~~~~~~~~~~~~~~~~~~
    /usr/include/c++/11/bits/random.h:541:9: note:   template argument deduction/substitution failed:
    /usr/include/c++/11/bits/random.h: In substitution of ‘template<class _UIntType, long unsigned int __w, long unsigned int __n, long unsigned int __m, long unsigned int __r, _UIntType __a, long unsigned int __u, _UIntType __d, long unsigned int __s, _UIntType __b, long unsigned int __t, _UIntType __c, long unsigned int __l, _UIntType __f> template<class _Sseq> using _If_seed_seq = typename std::enable_if<std::__detail::__is_seed_seq<_Sseq, std::mersenne_twister_engine<_UIntType, __w, __n, __m, __r, __a, __u, __d, __s, __b, __t, __c, __l, __f>, _UIntType>::value>::type [with _Sseq = std::random_device; _UIntType = long unsigned int; long unsigned int __w = 32; long unsigned int __n = 624; long unsigned int __m = 397; long unsigned int __r = 31; _UIntType __a = 2567483615; long unsigned int __u = 11; _UIntType __d = 4294967295; long unsigned int __s = 7; _UIntType __b = 2636928640; long unsigned int __t = 15; _UIntType __c = 4022730752; long unsigned int __l = 18; _UIntType __f = 1812433253]’:
    /usr/include/c++/11/bits/random.h:539:32:   required from ‘void Deck<R, S>::shuffle() [with R = PinochleRank; S = Suit]’
    PinochleGame.cpp:8:17:   required from here
    /usr/include/c++/11/bits/random.h:502:15: error: ‘class std::random_device’ has no member named ‘generate’     
    502 |         using _If_seed_seq = typename enable_if<__detail::__is_seed_seq<
        |               ^~~~~~~~~~~~

   Fixed: RNG generator must be passed invocation of rng (rng()), not (rng)

    /usr/include/c++/11/ext/new_allocator.h:162:11: error: invalid conversion from ‘char**’ to ‘const char**’ [-fpermissive]
    162 |         { ::new((void *)__p) _Up(std::forward<_Args>(__args)...); }
        |           ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        |           |
        |           char**
    In file included from main.cpp:5:
    HoldEmGame.h:25:45: note:   initializing argument 2 of ‘HoldEmGame::HoldEmGame(int, const char**)’
    25 |     HoldEmGame(int numPlayers, const char** playerNames);
        |                                ~~~~~~~~~~~~~^~~~~~~~~~~
    make: *** [Makefile:61: lab1] Error 1

    Note (https://stackoverflow.com/questions/2463473/why-am-i-getting-an-error-converting-a-float-to-const-float)
    Fixed by making all char** const char** in main.cpp (parameters to main, usage, create, checkArgs)

    /usr/bin/ld: /tmp/ccwIirhv.o: warning: relocation against `_ZTV10HoldEmGame' in read-only section `.text._ZN10HoldEmGameD2Ev[_ZN10HoldEmGameD5Ev]'
    /usr/bin/ld: /tmp/ccwIirhv.o: in function `void __gnu_cxx::new_allocator<PinochleGame>::construct<PinochleGame, int&, char const**&>(PinochleGame*, int&, char const**&)':
    /usr/include/c++/11/ext/new_allocator.h:162: undefined reference to `PinochleGame::PinochleGame(int, char const**)'
    /usr/bin/ld: /tmp/ccwIirhv.o: in function `void __gnu_cxx::new_allocator<HoldEmGame>::construct<HoldEmGame, int&, char const**&>(HoldEmGame*, int&, char const**&)':
    /usr/include/c++/11/ext/new_allocator.h:162: undefined reference to `HoldEmGame::HoldEmGame(int, char const**)'
    /usr/bin/ld: /tmp/ccwIirhv.o: in function `HoldEmGame::~HoldEmGame()':
    /mnt/c/dev/428/lab1/HoldEmGame.h:8: undefined reference to `vtable for HoldEmGame'
    /usr/bin/ld: /tmp/ccwIirhv.o: in function `PinochleGame::~PinochleGame()':
    /mnt/c/dev/428/lab1/PinochleGame.h:6: undefined reference to `vtable for PinochleGame'
    /usr/bin/ld: warning: creating DT_TEXTREL in a PIE
    collect2: error: ld returned 1 exit status
    make: *** [Makefile:61: lab1] Error 1

    Fixed by adding (Game.cpp PinochleGame.cpp HoldEmGame.cpp) to CMPL_SRCS in Makefile

Compilation Warnings:
    Many warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<CardSet<PinochleRank, Suit> >::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]
   10 |         for(int j = 0; j < hands.size(); j++) 
   in for loops

   Fixed by changing int to size_t when comparing to std::vector::size() in a loop.

Bugs:
    HoldEm printing one state ahead. Example:
        roboevt@DESKTOP-JHQK8MN:/mnt/c/dev/428/lab1$ ./lab1 HoldEm a b c d
        a: 
        b: 
        c: 
        d: 
        BOARD(flop):
        BOARD(turn): 6C AD 3H
        BOARD(river): 6C AD 3H 10D
        Would you like to end the game? (yes/no):
    Caused by state being incremented in deal, before printing.
    Fixed by printing previous step (print player hands when in flop, flop board when in turn, etc.)

Testing:
    No args:
        ./lab1
        Usage: lab1 <Game(Pinochle|HoldEm)> <player1> <player2> ... <player9>
        Pinocle requires 4 players, HoldEm requires 2-9 players
        echo $?
        1
    Invalid game:
        ./lab1 TicTacToe
        Usage: lab1 <Game(Pinochle|HoldEm)> <player1> <player2> ... <player9>
        Pinocle requires 4 players, HoldEm requires 2-9 players
        echo $?
        1
    No player names:
        ./lab1 Pinochle
        Usage: lab1 <Game(Pinochle|HoldEm)> <player1> <player2> ... <player9>
        Pinocle requires 4 players, HoldEm requires 2-9 players
        echo $?
        1
    Wrong number of player names:
        ./lab1 Pinochle bob joe
        Usage: lab1 <Game(Pinochle|HoldEm)> <player1> <player2> ... <player9>
        Pinocle requires 4 players, HoldEm requires 2-9 players
        echo $?
        1
    Pinochle works:
        ./lab1 Pinochle bob joe mary john
        bob: 9S JH QC
        joe: 9C TS KS
        mary: AH QC JD
        john: JC 9C KH
        Would you like to end the game? (yes/no): yes
        echo $?
        0
    HoldEm works:
        ./lab1 HoldEm bob joe mary john
        bob: JD 8D
        joe: 5H 10C
        mary: 5D 3C
        john: 3S 8C
        BOARD(flop): KH 6C KC
        BOARD(turn): KH 6C KC AH
        BOARD(river): KH 6C KC AH KD
        Would you like to end the game? (yes/no): yes
        echo $?
        0
    Multiple runs (different cards):
        ./lab1 HoldEm bob joe mary john
        bob: 10C 8C
        joe: 2D 10D
        mary: 2C 8S
        john: JH KD
        BOARD(flop): 4D KC 3D
        BOARD(turn): 4D KC 3D QS
        BOARD(river): 4D KC 3D QS 3C
        Would you like to end the game? (yes/no): no
        bob: QC 9S
        joe: 3C 6D
        mary: 5C 10C
        john: KD KH
        BOARD(flop): 7C 3H 7S
        BOARD(turn): 7C 3H 7S 6S
        BOARD(river): 7C 3H 7S 6S AH
        Would you like to end the game? (yes/no): no
        bob: 7H AH
        joe: 9D 10H
        mary: 5C 3C
        john: 5S QC
        BOARD(flop): 3S KH AS
        BOARD(turn): 3S KH AS 2D
        BOARD(river): 3S KH AS 2D 8S
        Would you like to end the game? (yes/no): no
        bob: 6H 4C
        joe: QH QS
        mary: 5S 6D
        john: KS 9C
        BOARD(flop): AC 5C 3D
        BOARD(turn): AC 5C 3D 5D
        BOARD(river): AC 5C 3D 5D 8D
        Would you like to end the game? (yes/no): yes
        echo $?
        0