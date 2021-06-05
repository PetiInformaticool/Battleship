	
run: bin/server bin/player bin/player2  | bin pipes/playerX pipes/player0 pipes/server0 pipes/serverX
	 bin/server & bin/player & bin/player2 & wait

pipes:
	mkdir pipes
	
pipes/playerX: | pipes
	mkfifo pipes/playerX
pipes/player0: | pipes
	mkfifo pipes/player0
pipes/serverX: | pipes
	mkfifo pipes/serverX
pipes/server0: | pipes
	mkfifo pipes/server0
	
bin:
	mkdir bin
	
bin/viewer.o: src/viewer.cpp src/viewer.h | bin
	g++ -c src/viewer.cpp -o bin/viewer.o
	
bin/game.o: src/game.cpp src/game.h | bin
	g++ -c src/game.cpp -o bin/game.o

bin/protocol.o: src/protocol.cpp src/protocol.h src/game.h | bin
	g++ -c src/protocol.cpp -o bin/protocol.o

bin/server.o: src/server.cpp src/protocol.h src/game.h | bin
	g++ -c src/server.cpp -o bin/server.o

bin/player.o: src/player.cpp src/protocol.h src/game.h | bin
	g++ -c src/player.cpp -o bin/player.o

bin/player2.o: src/player2.cpp src/protocol.h src/game.h | bin
	g++ -c src/player2.cpp -o bin/player2.o

bin/server: bin/server.o  bin/viewer.o bin/game.o bin/protocol.o | bin
	g++ bin/server.o bin/viewer.o bin/game.o bin/protocol.o  -lglut -lGL -o bin/server

bin/player: bin/player.o bin/game.o bin/protocol.o | bin
	g++ bin/player.o bin/game.o bin/protocol.o -o bin/player

bin/player2: bin/player2.o bin/game.o bin/protocol.o | bin
	g++ bin/player2.o bin/game.o bin/protocol.o -o bin/player2
bin/texture: src/texture.cpp
	g++  src/texture.cpp -lglut -lGL -o bin/texture 

kill: 
	killall server & wait
	killall player & wait
	killall player2
	
clean:
	rm bin -R

