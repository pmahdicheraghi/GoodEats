CC = g++
STD = -std=c++11 -pedantic
CF = $(STD)
BD = build
TEMPLATE_DIR = .template

all: $(BD) goodeats.out

$(BD):
	mkdir -p $(BD)

$(BD)/template_parser.o: utils/template_parser.cpp utils/template_parser.hpp utils/request.cpp utils/request.hpp utils/utilities.hpp utils/utilities.cpp
	$(CC) $(CF) -c utils/template_parser.cpp -o $(BD)/template_parser.o

$(BD)/response.o: utils/response.cpp utils/response.hpp utils/include.hpp
	$(CC) $(CF) -c utils/response.cpp -o $(BD)/response.o

$(BD)/request.o: utils/request.cpp utils/request.hpp utils/include.hpp utils/utilities.hpp
	$(CC) $(CF) -c utils/request.cpp -o $(BD)/request.o

$(BD)/utilities.o: utils/utilities.cpp utils/utilities.hpp
	$(CC) $(CF) -c utils/utilities.cpp -o $(BD)/utilities.o

$(BD)/server.o: server/server.cpp server/server.hpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp utils/template_parser.hpp utils/template_parser.cpp
	$(CC) $(CF) -c server/server.cpp -o $(BD)/server.o

$(BD)/route.o: server/route.cpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c server/route.cpp -o $(BD)/route.o

$(BD)/handlers.o: handlers.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c handlers.cpp -o $(BD)/handlers.o


goodeats.out: $(BD)/main.o $(BD)/handlers.o $(BD)/response.o $(BD)/request.o $(BD)/utilities.o $(BD)/server.o $(BD)/route.o $(BD)/template_parser.o $(BD)/goodeats.o $(BD)/user.o $(BD)/recipe.o $(BD)/shelf.o $(BD)/filter.o $(BD)/offer.o $(BD)/order.o
	$(CC) $(CF) $(BD)/main.o $(BD)/handlers.o $(BD)/response.o $(BD)/request.o $(BD)/utilities.o $(BD)/server.o $(BD)/route.o $(BD)/template_parser.o $(BD)/goodeats.o $(BD)/user.o $(BD)/recipe.o $(BD)/shelf.o $(BD)/filter.o $(BD)/offer.o $(BD)/order.o -o goodeats.out

clean:
	rm -rf $(BD) $(TEMPLATE_DIR) *.o *.out &> /dev/null

$(BD)/main.o: main.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp classes/goodeats.hpp
	$(CC) $(CF) -c main.cpp -o $(BD)/main.o

$(BD)/goodeats.o: classes/goodeats.cpp classes/goodeats.hpp classes/error.hpp classes/user.hpp classes/shelf.hpp classes/filter.hpp classes/offer.hpp classes/order.hpp
	$(CC) $(CF) -c classes/goodeats.cpp -o $(BD)/goodeats.o

$(BD)/order.o: classes/order.cpp classes/order.hpp classes/offer.hpp classes/user.hpp classes/recipe.hpp
	$(CC) $(CF) -c classes/order.cpp -o $(BD)/order.o

$(BD)/filter.o: classes/filter.cpp classes/filter.hpp classes/recipe.hpp
	$(CC) $(CF) -c classes/filter.cpp -o $(BD)/filter.o

$(BD)/shelf.o: classes/shelf.cpp classes/shelf.hpp classes/recipe.hpp
	$(CC) $(CF) -c classes/shelf.cpp -o $(BD)/shelf.o

$(BD)/user.o: classes/user.cpp classes/user.hpp
	$(CC) $(CF) -c classes/user.cpp -o $(BD)/user.o

$(BD)/recipe.o: classes/recipe.cpp classes/recipe.hpp
	$(CC) $(CF) -c classes/recipe.cpp -o $(BD)/recipe.o

$(BD)/offer.o: classes/offer.cpp classes/offer.hpp
	$(CC) $(CF) -c classes/offer.cpp -o $(BD)/offer.o