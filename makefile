parametrosCompilacao= #-Wshadow
nomePrograma=separa

all: $(nomePrograma)

$(nomePrograma): main.cpp
	g++ -g -o $(nomePrograma) main.cpp $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)
