#ifndef FunctionDefinitions_h
#define FunctionDefinitions_h
#include "FunctionsAnnounce.h"
using namespace std;

student stu[10];

standard standardWeights;

result Result;

int passJudge(int pass) {
    if(pass == 0) return 0; // << -- 未通过
    else if(pass == 1) return 1; // << -- 通过
    return 114514;
}

int timeJudge(int time)
{
    if(time <= 500) return 2;
    else if(time > 500 && time <= 1000) return 3;
    else if(time > 1000 && time <= 3000) return 4;
    else if(time > 3000) return 5;
    return 114514;
}

int duplicateJudge(int duplicate) {
    if(duplicate <= 20) return 6;
    else if(duplicate > 20 && duplicate <= 35) return 7;
    else if(duplicate > 35 && duplicate <= 50) return 8;
    else if(duplicate > 50) return 9; // << -- 相当于未通过
    return 114514;
}

int wrongtimeJudge(int wrongtime){
    if(wrongtime == 0) return 10;
    else if(wrongtime > 0 && wrongtime <= 3) return 11;
    else if(wrongtime > 3) return 12;
    return 114514;
}

int mixtureJudge(int x, int y) {
    switch (y) {
        case 10:
            if(x == 6) return 13;  // << -- duplicate <= 20 && wrong == 0
            else if(x == 7) return 14; // << -- 20 < dup <= 35 && wrong == 0
            else if(x == 8) return 15; // << -- 30 < dup <= 50 && wrong == 0
            //else if(x == 9) return 16; // << -- 50 < dup && wrong == 0
            break;
        case 11:
            if(x == 6) return 17;  // << -- duplicate <= 20 && 0 < wrong <=3
            else if(x == 7) return 18; // << -- 20 < dup <= 35 && 0 < wrong <=3
            else if(x == 8) return 19; // << -- 30 < dup <= 50 && 0 < wrong <=3
            //else if(x == 9) return 20; // << -- 50 < dup && 0 < wrong <=3
            break;
        case 12:
            if(x == 6) return 21;  // << -- duplicate <= 20 && 3 < wrong
            else if(x == 7) return 22; // << -- 20 < dup <= 35 && 3 < wrong
            else if(x == 8) return 23; // << -- 30 < dup <= 50 && 3 < wrong
            //else if(x == 9) return 24; // << -- 50 < dup && 3 < wrong
            break;
        default:
            break;
    }
    return 114514;
}
void InitialWeights(){
    standardWeights.buttomTime = 25; standardWeights.mediumTime = 20;
    standardWeights.higherTime = 10;
    standardWeights.topTime = 0;
    standardWeights.buttomWrongTime = 25; standardWeights.mediumWrongTime = 20;
    standardWeights.topWrongTime = 10;
    standardWeights.buttomDuplicateRate = 25; standardWeights.mediumDuplicateRate = 20; standardWeights.topDuplicateRate = 10;
    standardWeights.pass0 = 0; standardWeights.pass1 = 1;
}


studentNode* Locate(studentList L, long long studentNumber){
    studentNode *p, *s[10];
    for(int k = 0; k < 10; k ++) s[k] = NULL;
    p = L -> next;
    int i = 0;
    while(p != NULL) {
        if(p -> studentNumber != studentNumber) {
            p = p -> next;
        }
        else if (p -> studentNumber == studentNumber && p -> visited != 1){
            p -> visited = 1;
            s[i ++] = p;
            p = p -> next;
        }
        else if(p -> studentNumber == studentNumber && p -> visited == 1){
            return p;
        }
    }
    return s[0];
}

void PrintMainMenu(){
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
void PrintStart(){
    
    printf("==========================================================\n");
    printf("+=========  ==========  ----/\\----  +=======--  ==========\n");
    printf("|               ||      ---/  \\---  |       \\-      ||    \n");
    printf("|               ||      --/----\\--  |        |      ||    \n");
    printf("+========+      ||      -/      \\-  +=======/-      ||    \n");
    printf("         |      ||      /        \\  |      \\--      ||    \n");
    printf("         |      ||      ----------  |       \\-      ||    \n");
    printf("=========+      ||      ----------  |        \\      ||    \n");
    printf("==========================================================\n");
}
void PrintWelcome(){
    system("clear");
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
void PrintZZU(){
    system("clear");
    string zzu = R"((",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,""^^^^";i_}\rcUQmqpbkhaahkbpqm0Jcx\{_iI,^^^^"",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,:
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
    cout <<"\e[01;31m";
    cout << zzu;
    cout <<"\033[0m";
    cout << endl;
    
    cout << endl;
}
void PrintPTA() {
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
void modifyValue(){
}
void InitList(studentList &L){
    L = (studentNode *)malloc(sizeof(studentNode));
    L -> next = NULL;
}
void CreateFromTail(studentList &L){
    studentNode *r, *s, *p;
    long long studentnumber;
    char problemNumber;
    int time;
    int pass;
    int duplicateRate;
    int wrongTimes;
    p = L;
    while(p -> next != NULL) {
        p = p -> next;
    }
    r = p;
    //int flag = 1;
    char judge;
    system("clear");
    while(1) {
        cout << "\e[01;33;46mPlease Input the Student Number:\e[0m " << endl;
        cin >> studentnumber;
        if(studentnumber < 100000000000 || studentnumber > 1000000000000) {
            cout << "\e[01;33;41m Wrong Fromat, Please Type in the Right Format \e[0m" << endl;
            continue;
        }
        cout << "\e[01;33;46mPlease Input the Problem Number:\e[0m " << endl;
        cin >> problemNumber;
        cout << "\e[01;33;46mPlease Input the time used:\e[0m " << endl;
        cin >> time;
        cout << "\e[01;33;46mPlease tell the system whether you passed:\e[0m [\e[01;33;41m1/0\e[0m]:" << endl;
        cin >> pass;
        cout << "\e[01;33;46mPlease Input Wrong Times:\e[0m " << endl;
        cin >> wrongTimes;
        cout << "\e[01;33;46mPlease Input Duplicate Rate:\e[0m " << endl;
        cin >> duplicateRate;
        s = (studentNode*)malloc(sizeof(studentNode));
        s -> studentNumber = studentnumber;
        s -> problemNumber = problemNumber;
        s -> time = time;
        s -> pass = pass;
        s -> wrongTimes = wrongTimes;
        s -> duplicateRate = duplicateRate;
        s -> visited = 0;
        s -> results = 0.00;
        r -> next = s;
        r = s;
        cout << "\e[01;33;46mPlease Specify whether you want to continue\e[0m [\e[01;33;41my/n\e[0m]?";
        
        while (1) {
            cin >> judge;
            if(judge == 'n') {
                r -> next = NULL;
                system("clear");
                return;
            }
            else if (judge == 'y')
                break;
            else {
                cout << "\e[01;33;41mCannot identify your input, please retry(y/n)\e[0m" << endl;
                continue;
            }
        }
        system("clear");
        continue;
    }
    
}
void showListElement(studentList L) {
    studentNode* s = L -> next;
    printf("\e[36m+----------------+------+------+------+------+------+------+\e[0m\n");
    printf("| \e[37mStudent Number\e[0m | \e[37mPnum\e[0m | \e[37mTime\e[0m | \e[37mPass\e[0m |  \e[37mNo\e[0m  | \e[37mRate\e[0m |\e[37mResult\e[0m|\n");
    printf("\e[36m+----------------+------+------+------+------+------+------+\e[0m\n");
    while (s != NULL) {
        printf("|  \e[01;33m%lld\e[0m  |\e[01;33m%-6c\e[0m|\e[01;33m%-6d\e[0m|\e[01;33m%-6d\e[0m|\e[01;33m%-6d\e[0m|\e[01;33m%-6d\e[0m|\e[01;33m%0.2lf\e[0m |\n", s -> studentNumber,s -> problemNumber, s -> time, s -> pass, s -> wrongTimes, s -> duplicateRate, s ->results);
        printf("\e[36m+----------------+------+------+------+------+------+------+\e[0m\n");
        s = s -> next;
    }
    //cout << endl;
}
void showSpecifiedListElement(studentNode* L, long long studentNumber) {
    studentNode* s = L;
    printf("\e[36m+----------------+------+------+------+------+------+------+\e[0m\n");
    printf("| \e[37mStudent Number\e[0m | \e[37mPnum\e[0m | \e[37mTime\e[0m | \e[37mPass\e[0m |  \e[37mNo\e[0m  | \e[37mRate\e[0m |\e[37mResult\e[0m|\n");
    printf("\e[36m+----------------+------+------+------+------+------+------+\e[0m\n");
    while (s != NULL) {
        if (s -> visited == 1 && s -> studentNumber == studentNumber) { // << -- 注意这里需要先locate过才能输出元素 假如没有被访问过就不会输出
            printf("|  \e[01;33m%lld\e[0m  |\e[01;33m%-6c\e[0m|\e[01;33m%-6d\e[0m|\e[01;33m%-6d\e[0m|\e[01;33m%-6d\e[0m|\e[01;33m%-6d\e[0m|\e[01;33m%0.2lf\e[0m |\n", s -> studentNumber,s -> problemNumber, s -> time, s -> pass, s -> wrongTimes, s -> duplicateRate, s ->results);
            printf("\e[36m+----------------+------+------+------+------+------+------+\e[0m\n");
        }
        s = s -> next;
    }
}

void PrintGoodBye(){
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

void PrintChoice_1()
{
    cout << "Please Type in \e[01;32;44mButtom, Medium, Higher or Top\e[0m to continue: " << endl;
}

void PrintChoice_2()
{
    cout << "Please Type in \e[01;32;44mButtom, Medium or Top\e[0m to continue: " << endl;
}

void EditValue(string choice) {
    cout << "Please Type in the "<< choice << " value that you want to edit : " ;
}

int Strains(int value) {
    if (value > 25) {
        cout << "\e[01;33;41mThe Number is too Big, Please type in smaller value!:\e[0m" << endl;
        return 0;
    }
    else if(value <= 25)
    return 1;
    return 114514;
}
void SetWeights()
{
    system("clear");
    while(1) {
        printf("\e[36m+--------------+----------+----------+-----------+----------+\e[0m\n");
        printf("|  \e[37mRange\\Name\e[0m  |   \e[37mTime\e[0m   |   \e[37mBase\e[0m   | \e[37mDuplicate\e[0m |\e[37mWrong Time\e[0m|\n");
        printf("\e[36m+--------------+----------+----------+-----------+----------+\e[0m\n");
        printf("|    \e[37mButtom\e[0m    |\e[01;33m%-10d\e[0m|    \e[35m25\e[0m    |\e[01;33m%-11d\e[0m|\e[01;33m%-10d\e[0m|\n", standardWeights.buttomTime,standardWeights.buttomDuplicateRate,standardWeights.buttomWrongTime);
        printf("\e[36m+--------------+----------+----------+-----------+----------+\e[0m\n");
        printf("|    \e[37mMedium\e[0m    |\e[01;33m%-10d\e[0m|    \e[35m25\e[0m    |\e[01;33m%-11d\e[0m|\e[01;33m%-10d\e[0m|\n", standardWeights.mediumTime, standardWeights.mediumDuplicateRate,standardWeights.mediumWrongTime);
        printf("\e[36m+--------------+----------+----------+-----------+----------+\e[0m\n");
        printf("|    \e[37mHigher\e[0m    |\e[01;33m%-10d\e[0m|    \e[35m25\e[0m    |\e[35mNULL\e[0m       |\e[35mNULL\e[0m      |\n", standardWeights.higherTime);
        printf("\e[36m+--------------+----------+----------+-----------+----------+\e[0m\n");
        printf("|   \e[37m- Top -\e[0m    |\e[01;33m%-10d\e[0m|    \e[35m25\e[0m    |\e[01;33m%-11d\e[0m|\e[01;33m%-10d\e[0m|\n", standardWeights.topTime,standardWeights.topDuplicateRate, standardWeights.topWrongTime);
        printf("\e[36m+--------------+----------+----------+-----------+----------+\e[0m\n");
        cout << "\e[01;32mThese are current Weights, What would you like to set? \e[0m(\e[01;31mPress N to Refresh\e[0m): " << endl;
        //cout << "Press 'Show' to read cueen: " << endl;
        cout << "You can Type in \e[01;32;44mTime, Duplicate or WrongTime\e[0m to continue (\e[01;31mPress Q to Quit\e[0m): "<< endl;
        string choice;
        cin >> choice;
        string choice2;
        if (choice == "Time") {
            PrintChoice_1();
            cin >> choice2;
            if(choice2 == "Buttom") {
                while(1) {
                EditValue(choice2);
                cin >> standardWeights.buttomTime;
                if(Strains(standardWeights.buttomTime) == 1) break;
                else if(Strains(standardWeights.buttomTime) == 0) continue;
                }
            }
            else if(choice2 == "Medium") {
                while(1) {
                EditValue(choice2);
                cin >> standardWeights.mediumTime;
                if(Strains(standardWeights.mediumTime) == 1) break;
                else if(Strains(standardWeights.mediumTime) == 0) continue;
                }
            }
            else if(choice2 == "Higher") {
                while(1) {
                EditValue(choice2);
                cin >> standardWeights.higherTime;
                if(Strains(standardWeights.higherTime) == 1) break;
                else if(Strains(standardWeights.higherTime) == 0) continue;
                }
            }
            else if(choice2 == "Top") {
                while(1) {
                EditValue(choice2);
                cin >> standardWeights.topTime;
                if(Strains(standardWeights.topTime) == 1) break;
                else if(Strains(standardWeights.topTime) == 0) continue;
                }
            }
        }
        else if (choice == "Duplicate") {
            PrintChoice_2();
            cin >> choice2;
            if(choice2 == "Buttom") {
                while(1) {
                EditValue(choice2);
                cin >> standardWeights.buttomDuplicateRate;
                if(Strains(standardWeights.buttomDuplicateRate) == 1) break;
                else if(Strains(standardWeights.buttomDuplicateRate) == 0) continue;
                }
            }
            else if(choice2 == "Medium") {
                while(1) {
                EditValue(choice2);
                cin >> standardWeights.mediumDuplicateRate;
                if(Strains(standardWeights.mediumDuplicateRate) == 1) break;
                else if(Strains(standardWeights.mediumDuplicateRate) == 0) continue;
                }
            }
            else if(choice2 == "Top") {
                while(1) {
                EditValue(choice2);
                cin >> standardWeights.topDuplicateRate;
                if(Strains(standardWeights.topDuplicateRate) == 1) break;
                else if(Strains(standardWeights.topDuplicateRate) == 0) continue;
                }
            }
        }
        else if (choice == "WrongTime") {
            PrintChoice_2();
            cin >> choice2;
            if(choice2 == "Buttom") {
                while(1) {
                EditValue(choice2);
                cin >> standardWeights.buttomWrongTime;
                if(Strains(standardWeights.buttomWrongTime) == 1) break;
                else if(Strains(standardWeights.buttomWrongTime) == 0) continue;
                }
            }
            else if(choice2 == "Medium") {
                while(1) {
                EditValue(choice2);
                cin >> standardWeights.mediumWrongTime;
                if(Strains(standardWeights.mediumWrongTime) == 1) break;
                else if(Strains(standardWeights.mediumWrongTime) == 0) continue;
                }
            }
            else if(choice2 == "Top") {
                while(1) {
                EditValue(choice2);
                cin >> standardWeights.topWrongTime;
                if(Strains(standardWeights.topWrongTime) == 1) break;
                else if(Strains(standardWeights.topWrongTime) == 0) continue;
                }
            }
        }
        
        else if(choice == "Q") {
            system("clear");
            break;
        }
        else if(choice == "N") {
            system("clear");
            continue;
        }
        else {
            system("clear");
            cout << "\e[01;33;41m Wrong Fromat, Please Type in the Right Format \e[0m" << endl;
            continue;
        }
    }
    
}
#endif /* FunctionDefinitions_h */
