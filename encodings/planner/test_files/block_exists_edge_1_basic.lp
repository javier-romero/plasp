#program block_base.
disable(A1,A2) :- action(A1), action(A2), A1 != A2,
                  postcondition(A1,_,X,V1), precondition(A2,X,V2), V1 != V2.

disable :- #sum{1,A1 : disable(A1,A2); -1,A2 : disable(A1,A2)} > 0.

#program block_step(t).

#program block_model(m,length).
time(T,m) :- _test(_error(A,T),m).

#edge((A1,T),(A2,T)) : disable(A1,A2), occurs(A1,T), not disable, time(T,m).
#edge((A1,T),(A2,T)) : disable(A1,A2), occurs(A2,T),     disable, time(T,m).

