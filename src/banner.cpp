// ASCII-art screens. Moved verbatim from the original FunctionDefinitions.h;
// only the function names changed and system("clear") became clearScreen().
#include "banner.h"

#include <cstdio>
#include <iostream>
#include <string>

#include "console.h"

using std::cout;
using std::endl;
using std::string;

void printMainMenu(){
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "      \e[01;36m+-------------------+-------------------+-------------------+-------------------+-------------------+\e[0m" << endl;
    cout << endl;
    cout << "\e[01;35m";
    string MainMenu = R"(
            |   \/   |     /   \     |  | |  \ |  |    |   \/   | |   ____||  \ |  | |  |  |  |
            |  \  /  |    /  ^  \    |  | |   \|  |    |  \  /  | |  |__   |   \|  | |  |  |  |
            |  |\/|  |   /  /_\  \   |  | |  . `  |    |  |\/|  | |   __|  |  . `  | |  |  |  |
            |  |  |  |  /  _____  \  |  | |  |\   |    |  |  |  | |  |____ |  |\   | |  `--'  |
            |__|  |__| /__/     \__\ |__| |__| \__|    |__|  |__| |_______||__| \__|  \______/
                                                                                                                                                                        )";
    cout << MainMenu << "\e[0m"<< endl;
    cout << "      \e[01;36m+-------------------+-------------------+-------------------+-------------------+-------------------+\e[0m" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "      \e[01;36m+-------------------+-------------------+-------------------+-------------------+-------------------+\e[0m" << endl;
    cout << endl;
    cout << "      +-------------------+  << --------------- \e[01;32m+ 1. Set Weights +\e[0m --------------- >> +-------------------+" << endl;
    cout << endl;
    cout << "      +-------------------+  << ------------- \e[01;34m+ 2. Current Results +\e[0m ------------- >> +-------------------+" << endl;
    cout << endl;
    cout << "      +-------------------+  << --------------- \e[01;31m+ 3. Calculate +\e[0m ----------------- >> +-------------------+" << endl;
    cout << endl;
    cout << "      +-------------------+  << --------------- \e[01;33m+   4. Input   +\e[0m ----------------- >> +-------------------+" << endl;
    cout << endl;
    cout << "      +-------------------+  << ------------------ \e[37m+ 5.QUIT +\e[0m -------------------- >> +-------------------+" << endl;
    cout << endl;
    cout << "      \e[01;36m+-------------------+-------------------+-------------------+-------------------+-------------------+\e[0m" << endl;
    
    cout << endl;
    cout << endl;
    cout << endl;
}

void printWelcome(){
    clearScreen();
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    string s = R"(                  __        __   _  ____
                  \ \      / /__| |/ ___|___  _ __ ___   ___
                   \ \ /\ / / _ \ | |   / _ \| '_ ` _ \ / _ \
                    \ V  V /  __/ | |__| (_) | | | | | |  __/
                     \_/\_/ \___|_|\____\___/|_| |_| |_|\___|
                                                                                      )";
    string s2 = R"(
   /\      \     __     _ __\ \ \/'\ /\ \__/  __  __\ \ \L\_\    ___    \_\ \
   \ \ \__\ \  /'__`\  /\`'__\ \ , < \ \ ,__\/\ \/\ \\ \ \L_L   / __`\  /'_` \
    \ \ \_/\ \/\ \L\.\_\ \ \/ \ \ \\`\\ \ \_/\ \ \_\ \\ \ \/, \/\ \L\ \/\ \L\ \
     \ \_\\ \_\ \__/.\_\\ \_\  \ \_\ \_\ \_\  \ \____/ \ \____/\ \____/\ \___,_\
      \/_/ \/_/\/__/\/_/ \/_/   \/_/\/_/\/_/   \/___/   \/___/  \/___/  \/__,_ /)";
    cout << "\e[01;05;31m";
    cout << s << endl;
    cout << "\e[0m";
    cout << "\e[01;34m";
    cout << s2 << endl;
    cout << "\e[0m";
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
}

const string& zzuArt() {
    static const string zzu = R"((",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,""^^^^";i_}\rcUQmqpbkhaahkbpqm0Jcx\{_iI,^^^^"",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,:
{                                    .:<{fXQqbhakdqZQJXcvuxrrxnvcXULOwdkaakq0Xj{~;'                                   .
{                               .;-tUqa*apQc/}+!"''.                  `:i~}tcQqa*opJf?I'                              .
{                           'l{cma#hZv)>"'  _rvvn]. [X\ ,ucI   ;cu: (X}  <~^  ',<{xQk#owz)!'                          .
{                        ^_nm*#dU\~^   !]^ tWOI;L&x n8C ;aM~   >MoI U8v >aMm"}O/ ;-I">(Yp#*qu?"                       .
{                     ^?cbMoQ\i^  '{f:`C&L"[hp}}ma) 1oq?\oq:   ,wo/]w#('0MchCp#++b#1':  '!|LaMkX["                    .
{                  .~udMaC):.ifQ] 'xWwLC0MZ;:?))?: '"?xXYu{+_+~>?txj(> :cv.{oMv(#k- ]ov ^!` ,[YhMkc_'                 .
{                ^1O##O(:  }CUZWf ",/Mhi \u+"!?|xY0mqwmZOOZZZmmmmmmmZQJv\[<:!<")v! .U#(/pm}`]/!:{Q#MZ|,               .
{              ,/pWaz~  l>;Il cWvx0\ ?)~_\zZdpZUn\}+!;"^`''..''`^";!~[\nYOpdmYt+"  "doqC[:}OMZJY]`>nhWdf:             .
{            "/dWhnl .\ZXYxf?'xkY}""?rQbdQx}!^                            ^l]rCpbmu}_?I ,CWmnLf?)'^;:fkWbf,           .
{          '{qWauI   >#bi:uaou^",}UkkL/<^                                      ^>|JbbU1; ,)JY{I'+cqpx,:rhWp('         .
{         >LM#U! }Qx-;[LqLJY~,?YhkY}:                      ;l                      "]zbaJ]' <I\pddO}da~ lz#M0+        .
{       'taWq[ ,<}nw#hL(:I^"(q*01,                        :LZ!                        "}L*dt" l];(#wntI^  -mMor^      .
{      ;CM#z, ^Ya#hLnft> ^/b*Y~                          :CbbO!                          >cohj" "cc{'i0ZL( "u#M0l     .
{     >qMo\ ,f!'I}n0O<  )dMY>                           :CbppbOi                           !z#k|' 'qU?MC/W?  1hMd+    .
{    >d#a[ ImO-L]:x~; iQWZ_                            ,JbppppbZ>                            ~QWO~ [ZChL`<;l. -k#k_   .
{   lp#a[ .uhwka\pJ^ )a*f`                            ,JbpppppqqQ!^""",,:::;:`                '\oo/ `<_-\Yqw+  _h#b<  .
{  "O#*( .;"">{xOc'.xMb-                      .^:!~?}(Qabkhkkkkbbdppdbbbkkkkkq0]        ,?"     ~pMc':CpCt+"'`  }o#q; .
{  c##v  !ZmCv\?l  nMd>                 "i]\uJOqkao###*oooooooooo**oooooooo*M#m_      ijp#px>    lwWz':"    \d?  j##U..
{ ?**w" l~iJ#vnQ[ /Mk<             '!{nQda####**oooooooooooooooooooooooo##oqv-'   '!\La#*o*#aC}"  !dMx  >1xJw*h~ `0##{
{'O#*} .rmwoax}` _o#{           '_uma##*oooooo**####*abppdkaooooooo*##*dJ\>.  'l}nZa##ooooooo#W0^  ?o*[ \Ln)~iCj. _o*w"
[_**Z'   .:>}jf'`ZMY.          _Z*#*ooooo*##*apQXr(]+]/XZbhaooooookmz|<".^l]fUwh##*ooooooo*##dv!    vMq"       "  .L#*|
}v##/  ;]!   i: }#a>          ,b#ooooo*#amX/-l^   ^{Ydo**ooooooooOf|)/uJmba###*ooooo*###ob0n?,      lkM|  .`>/0J.  1#*O
|Z*a~  tMhC-!oj UMQ.          ^m#oooo*Q(!'      >vb##ooooooooooooao*####**o**####*abmJn)+:.          UMQ^YOmoMJ?'  lhoa
xd*b,  zh+xddW1'qMr            >w#ook{.       :z*M#*oooooooooo***######*oakpmLzr)?i,'                \Mb;i<i_\zZ+  ^w*o
uk*p^  >_  ,)fl,kM|             :uhWt         lJYJ0mpbkhaahkbpwOQJznf\1[-+;^'                        }#a!      '`  'Z*o
uk*p^          ,kM\               lxf'       "YdZJcxjjffjrf\|()))(\fxuzULZf.                         {#h!          'Z*o
jp*k;          `mMu                 ^'      ,Ubpddbbbbddpppbbbbbbbbbbbbbddbj.                        fMd,          "qoo
(0*o_           cMZ'                       :Jbppppppppppppb0?Lbppppppppppppbj.                      .LMJ           iaoh
[x##x           _**-                      ;CbppppppppppppbZi !Zdppppppppppppbj.                     <a#}           \#*C
[>o*q"          .JM0`                    ;Lbppppppppppppdm>   <wdppppppppppppbj.                   .JM0`          `Z*o}
{ J##\           !bMf                   IQbppppppppppppdw~     _qdppppppppppppbj.                  |#a<           {*#Q`
{ >a*bl           [o*{                 l0bppppppppppppdq+       [pdppppppppppppbj.                ?a#)           :p*o_
{  t##C'           (#o}               !Obppppppppppppdq-         )ddppppppppppppbj.              -hMt           .X##x .
{  'Y##n            1a*\.            iObppppppppppppdp]           \bpppppppppppppbf.            )o*\            f##L` .
{   "Q##f            +wWUl          >Zbppppppppppppdp}            .jbpppppppppppppbf.         :zMp?            (*#m:  .
{    :0M*f.           "n*h/^       ~mbppppppppppppdd{              'ubpppppppppppppbf.      `)b#c:            \oMml   .
{     ,JM#c"            <CMq1`    +wbdddddddddddddd)                `zbdddddddddddddbj.   '[m#L+            `n#MQ;    .
{      'roMZ~             -L#q(" iQmZZZZZZZZZZZZZm(                  "YmZZZZZZZZZZZZZm( ^1Z*Zf<            iQM*v^     .
{        ?wMaj"            '_Xhbv?:``^"""""""""""^                    '"""""""""""^^`,?nboLv{1r!~`       ^thWp}       .
{         "xaWw{'       .""]x(cnZawx+`                                            '<jmawfl>q-?J_q<'l`  .]ZMov:        .
{           >U#M0?.     l\0d],l1<itOhdY(>^                                    ^i)XphZj>'  '<[k1>j0mml _CMMC+          .
{             _C#MQ]' >)rLk{.xuJf.  ;}vZbpQn{>:'                        ',>}xLpbmc{l.       `_"`tuQ0nJ#M0?.           .
{              ._J*Mm(+-f0);c/zi.      .:_/XZppwQYx\{?~i!lI;;Il!i~?}|jXQmppZYt-;.                ~v0##L?.             .
{                 >vkWkc_,~Y[~!             .^l+}tuXLOZmwqqqqqqwwmZQCLf1_!".                  `_vbMhc+.               .
{                   ,1CoMpm\^                   ; "] :Yl^",::,"^^'. .\u                    ^_npM*Q(:                  .
{                      I(Lh#kU)!`              lk<1a:-ol             'wf]l             'l1XbMoQ\l                     .
{                         ;}zp#*pY|~,.       }){WrCa:j0.            ;?0q);         "<(Xq*#dX{I                        .
{                            `i)zwa#hZc|_l^. ?0U?\m1"0|             ~[Q0fYXfi^I+(vOk#oqX(>^                           .
{                                ';_/Ywh*okmCnJc+[C:,0)..      ..'`^"[Y({nOwqbo*hwUt-I'                               .
}                                      ^l-(xY0mphkbbpdbZO0QLLQ0OZmqdbkbkbwQXx(?!^                                     .
j~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~++++++~~<<>>><+-[)\jnzUCQ0ZmqqmZ0QLJXuj/)[-+<>>><<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
##############################M#oooooooooooooooaaahhhhhhhhhhhhhhhhhhhhaaaooooo*########################################)";
    return zzu;
}

void printZZU(){
    clearScreen();
    const string& zzu = zzuArt();
    cout <<"\e[01;31m";
    cout << zzu;
    cout <<"\033[0m";
    cout << endl;
    
    cout << endl;
}

void printPTA() {
    string PTA = R"(
                                                                      
                    PPPPPPPPPPPPPPPPP   TTTTTTTTTTTTTTTTTTTTTTT         AAA
                    P::::::::::::::::P  T:::::::::::::::::::::T        A:::A
                    P::::::PPPPPP:::::P T:::::::::::::::::::::T       A:::::A
                    PP:::::P     P:::::PT:::::TT:::::::TT:::::T      A:::::::A
                      P::::P     P:::::PTTTTTT  T:::::T  TTTTTT     A:::::::::A
                      P::::P     P:::::P        T:::::T            A:::::A:::::A
                      P::::PPPPPP:::::P         T:::::T           A:::::A A:::::A
                      P:::::::::::::PP          T:::::T          A:::::A   A:::::A
                      P::::PPPPPPPPP            T:::::T         A:::::A     A:::::A
                      P::::P                    T:::::T        A:::::AAAAAAAAA:::::A
                      P::::P                    T:::::T       A:::::::::::::::::::::A
                      P::::P                    T:::::T      A:::::AAAAAAAAAAAAA:::::A
                    PP::::::PP                TT:::::::TT   A:::::A             A:::::A
                    P::::::::P                T:::::::::T  A:::::A               A:::::A
                    P::::::::P                T:::::::::T A:::::A                 A:::::A
                    PPPPPPPPPP                TTTTTTTTTTTAAAAAAA                   AAAAAAA
  
                                                                                                                                            )";

    string Model = R"(
                                                                    dddddddd
        MMMMMMMM               MMMMMMMM                             d::::::d                   lllllll
        M:::::::M             M:::::::M                             d::::::d                   l:::::l
        M::::::::M           M::::::::M                             d::::::d                   l:::::l
        M:::::::::M         M:::::::::M                             d:::::d                    l:::::l
        M::::::::::M       M::::::::::M   ooooooooooo       ddddddddd:::::d     eeeeeeeeeeee    l::::l
        M:::::::::::M     M:::::::::::M oo:::::::::::oo   dd::::::::::::::d   ee::::::::::::ee  l::::l
        M:::::::M::::M   M::::M:::::::Mo:::::::::::::::o d::::::::::::::::d  e::::::eeeee:::::eel::::l
        M::::::M M::::M M::::M M::::::Mo:::::ooooo:::::od:::::::ddddd:::::d e::::::e     e:::::el::::l
        M::::::M  M::::M::::M  M::::::Mo::::o     o::::od::::::d    d:::::d e:::::::eeeee::::::el::::l
        M::::::M   M:::::::M   M::::::Mo::::o     o::::od:::::d     d:::::d e:::::::::::::::::e l::::l
        M::::::M    M:::::M    M::::::Mo::::o     o::::od:::::d     d:::::d e::::::eeeeeeeeeee  l::::l
        M::::::M     MMMMM     M::::::Mo::::o     o::::od:::::d     d:::::d e:::::::e           l::::l
        M::::::M               M::::::Mo:::::ooooo:::::od::::::ddddd::::::dde::::::::e         l::::::l
        M::::::M               M::::::Mo:::::::::::::::o d:::::::::::::::::d e::::::::eeeeeeee l::::::l
        M::::::M               M::::::M oo:::::::::::oo   d:::::::::ddd::::d  ee:::::::::::::e l::::::l
        MMMMMMMM               MMMMMMMM   ooooooooooo      ddddddddd   ddddd    eeeeeeeeeeeeee
                                                                                                                                  
                                                                                                                                  )";
    cout << "\e[01;31m"<< PTA << "\e[0m" << endl <<"\e[01;35m"<< Model << "\e[0m"<< endl;
}

void printGoodBye(){
    string str = R"(
_/_/_/_/_/  _/                      _/_/_/_/                        _/    _/            _/
   _/      _/_/_/    _/    _/      _/        _/_/    _/  _/_/      _/    _/    _/_/_/      _/_/_/      _/_/_/
  _/      _/    _/    _/_/        _/_/_/  _/    _/  _/_/          _/    _/  _/_/      _/  _/    _/  _/    _/
 _/      _/    _/  _/    _/      _/      _/    _/  _/            _/    _/      _/_/  _/  _/    _/  _/    _/
_/      _/    _/  _/    _/      _/        _/_/    _/              _/_/    _/_/_/    _/  _/    _/    _/_/_/
                                                                                                       _/
                                                                                                  _/_/      )";
    cout << "\e[01;36m\e[01;36m " << str << "\e[0m" << endl;
}
