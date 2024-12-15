##
# Project Title
# RAYS
#
# @file :)
# @version 0.5
build:
	gcc -Wall -std=c99 ./src/*.c -lraylib -lm -o game

run:
	./game

clean:
	rm game

# end
