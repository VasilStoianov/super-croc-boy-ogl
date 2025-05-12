
echo 'Bulding and running Super Croc boy'
rm game
 gcc main.c glad.c input.c -o game -Wall -Wextra -lglfw -ldl -lm
 ./game