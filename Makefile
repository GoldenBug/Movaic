default:
	g++ main.cpp -o movaic `pkg-config --cflags --libs opencv`
