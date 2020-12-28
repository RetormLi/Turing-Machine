cc = g++
prom = turing
source = turing_project/main.cpp turing_project/TuringMachine.cpp

$(prom): $(source)
	$(cc) -o $(prom) $(source)

clean:
	rm -rf turing