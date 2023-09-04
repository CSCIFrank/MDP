
CXX=g++

mdp: mdp.cpp
	$(CXX) -o mdp mdp.cpp

clean:
	rm -f *.o mdp