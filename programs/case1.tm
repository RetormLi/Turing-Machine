; This example is for L = {a_i b_j a_i b_j | i,j > 0}.
; Input: a string of a's and b's, e.g. 'aabaab'
; The procedure is:
; cut the front a^ib^j to tape2
; compare tape1 and tape2
; if not the same, tape2 go to head( tape1 all ready at head) and all wipe off and reject.

; the finite set of states
#Q = {0,cpa,cpb,cmpa,cmpb,mh,wipe,accept,accept2,accept3,accept4,halt_accept,reject,reject2,reject3,reject4,reject5,halt_reject}

; the finite set of input symbols
#S = {a,b}

; the complete set of tape symbols
#G = {a,b,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 2

; the transition functions

; State 0: start state
0 a_ a_ ** cpa
0 b_ b_ ** reject
0 __ __ ** reject ; empty input

; State cpa: cut the first a-part string to the 2nd tape 
cpa a_ _a rr cpa
cpa b_ b_ ** cpb
cpa __ __ *l wipe

; State cpb: cut the b-part string to the 2nd tape 
cpb a_ a_ *l mh
cpb b_ _b rr cpb
cpb __ __ *l wipe

; State mh: move 2st head to the left
mh aa aa *l mh
mh ab ab *l mh
mh ba ba *l mh
mh bb bb *l mh
mh b_ b_ *r cmpa
mh a_ a_ *r cmpa

; State cmpa: compare a-part of two strings
cmpa aa __ rr cmpa
cmpa bb __ rr cmpb
cmpa ab ab ** wipe
cmpa ba ba ** wipe
cmpa _a _a ** wipe
cmpa _b _b ** wipe
cmpa a_ a_ ** reject
cmpa b_ b_ ** reject
cmpa __ __ ** reject

; State cmpb: compare b-part of two strings
cmpb aa __ ** wipe
cmpb bb __ rr cmpb
cmpb ab ab ** wipe
cmpb ba ba ** wipe
cmpb _a _a ** wipe
cmpb _b _b ** wipe
cmpb a_ a_ ** reject
cmpb b_ b_ ** reject
cmpb __ __ ** accept

;state wipe: move 2st head to the left to wipe
wipe aa aa *l wipe
wipe ab ab *l wipe
wipe ba ba *l wipe
wipe bb bb *l wipe
wipe _a _a *l wipe
wipe _b _b *l wipe
wipe __ __ *r reject
wipe a_ a_ *r reject
wipe b_ b_ *r reject

; State accept*: write 'true' on 1st tape
accept __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt_accept

; State reject*: wipe all symbols and write 'false' on 1st tape
reject aa __ rr reject
reject ab __ rr reject
reject ba __ rr reject
reject bb __ rr reject
reject a_ __ rr reject
reject b_ __ rr reject
reject _b __ rr reject
reject _a __ rr reject
reject __ f_ r* reject2
reject2 __ a_ r* reject3
reject3 __ l_ r* reject4
reject4 __ s_ r* reject5
reject5 __ e_ ** halt_reject
