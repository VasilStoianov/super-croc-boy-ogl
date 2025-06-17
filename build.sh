
echo 'Bulding and running Super Croc boy'
rm game
clear
 gcc main.c lib/glad.c lib/image_imp.c input.c -o game -Wall -Wextra -lglfw -ldl -lm
 ./game