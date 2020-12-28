; This example program checks if the input formula is correct.
; The language is {1^ix1^j=1^{ixj} | i, j > 0}. Such as '111x11=111111' for 2x3=6.
; Input: a formula string, e.g. '111x11=111111'
; The procedure is:
; 1. check if the input fit in requirement
; 2. put the multiplicand in tape2, multiplier in tape3, result in tape1
; 3. do multiply: read tape3 for tape2 times.

; the finite set of states
#Q = {0,check1,check2,check3,check4,ready,cp1,cp2,mh,mh2,mh3,mh4,multi,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {x,1,=}

; the complete set of tape symbols
#G = {x,1,=,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 3

; the transition functions

; State 0: start state
0 1__ 1__ *** check1
0 x__ x__ *** reject
0 =__ =__ *** reject
0 ___ ___ *** reject

; State check: check whether is correct form
check1 1__ 1__ r** check1
check1 x__ x__ r** check2
check1 =__ =__ *** mh2
check1 ___ ___ l** mh2

check2 1__ 1__ r** check2
check2 x__ x__ *** mh2
check2 =__ =__ r** check3
check2 ___ ___ l** mh2

check3 1__ 1__ r** check4
check3 x__ x__ *** mh2
check3 =__ =__ *** mh2
check3 ___ ___ l** mh2

check4 1__ 1__ r** check4
check4 x__ x__ *** mh2
check4 =__ =__ *** mh2
check4 ___ ___ l** ready

; State ready: ready to go
ready ___ ___ r** cp1
ready 1__ 1__ l** ready
ready x__ x__ l** ready
ready =__ =__ l** ready

; State cp1: copy the multiplicand to the 2nd tape 
cp1 1__ _1_ rr* cp1
cp1 x__ ___ rl* cp2
cp1 =__ ___ rl* mh2
cp1 ___ ___ *l* mh2

; State cp2: copy the multiplier to the 3nd tape 
cp2 11_ _11 r*r cp2
cp2 =1_ _1_ r*l mh
cp2 =11 _11 r** mh
cp2 =__ =__ *** reject
cp2 ___ ___ *** reject
cp2 _1_ _1_ *** mh2

; State mh: move all heads to the left
mh _11 _11 *ll mh
mh __1 __1 **l mh
mh _1_ _1_ *l* mh
mh ___ ___ rrr multi
mh 111 111 lll mh
mh 1_1 1_1 l*l mh
mh 11_ 11_ ll* mh
mh 1__ 1__ l** mh

mh =__ =__ l** mh
mh x__ x__ l** mh

; State mh2: move heads of all tape to the left and ready to reject
mh2 _11 _11 *ll mh2
mh2 __1 __1 **l mh2
mh2 _1_ _1_ *l* mh2
mh2 ___ ___ rrr reject
mh2 111 111 lll mh2
mh2 1_1 1_1 l*l mh2
mh2 11_ 11_ ll* mh2
mh2 1__ 1__ l** mh2

mh2 =__ =__ l** mh2
mh2 x__ x__ l** mh2

; State mh3: move heads of tape3 to left and ready to do multi
mh3 _11 _11 **l mh3
mh3 __1 __1 **l mh3
mh3 _1_ _1_ **r multi
mh3 ___ ___ **r multi
mh3 111 111 **l mh3
mh3 1__ 1__ **r multi
mh3 1_1 1_1 **l mh3
mh3 11_ 11_ **r multi

; State mh4: move heads of tape3 to left and ready to accept
mh4 _11 _11 **l mh4
mh4 __1 __1 **l mh4
mh4 _1_ _1_ **r accept
mh4 ___ ___ **r accept
mh4 111 111 **l mh4
mh4 1__ 1__ **r accept
mh4 1_1 1_1 **l mh4
mh4 11_ 11_ **r accept

; State multi: multiply
multi _11 _11 *** mh2
multi _1_ ___ *rl mh3
multi __1 __1 *** mh4
multi ___ ___ *** accept

multi 111 _11 r*r multi
multi 1_1 1_1 *** mh2
multi 11_ 1__ *rl mh3
multi 1__ 1__ *ll mh2

; State accept*: write 'true' on 1st tape
accept __1 ___ **r accept
accept ___ t__ r** accept2
accept2 ___ r__ r** accept3
accept3 ___ u__ r** accept4
accept4 ___ e__ *** halt_accept

; State reject*: write 'false' on 1st tape
reject x11 ___ rrr reject
reject x_1 ___ r*r reject
reject x1_ ___ rr* rejectshu
reject x__ ___ r** reject

reject =11 ___ rrr reject
reject =_1 ___ r*r reject
reject =1_ ___ rr* reject
reject =__ ___ r** reject

reject _11 ___ *rr reject
reject _1_ ___ *r* reject
reject __1 ___ **r reject
reject 111 ___ rrr reject
reject 1_1 ___ r*r reject
reject 11_ ___ rr* reject
reject 1__ ___ r** reject

reject ___ f__ r** reject2
reject2 ___ a__ r** reject3
reject3 ___ l__ r** reject4
reject4 ___ s__ r** reject5
reject5 ___ e__ *** halt_reject
