/////////////////////////////////////////////////////////
///////// Lectura de grafos con un bit por celda ////////
/////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#define FILENAME "brock200_1.clq"
#define SIZE_OF_LINE 255

typedef unsigned char BYTE;

int find_cell(BYTE** matrix,int e, int w, int b){
	int cell=0;
	cell = matrix[e][w];
	cell <<= (7 - b);
	cell >>= (7 - b);
	return cell;
}

void free_matrix(int n, BYTE ** matrix) {
	int i;
	if (matrix != NULL) {
		for (i = 0; i<n; i++) {
			free(matrix[i]);
		}
		free(matrix);
		matrix = NULL;
	}
}

BYTE** allocate_matrix(int* e, int* w) {

	BYTE** matrix = new BYTE*[*e];
	for (int i = 0; i < *e; ++i)
		matrix[i] = new BYTE[*w];

	//malloc de un puntero de *e filas y *w columnas
	//BYTE** matrix = (BYTE**)malloc(sizeof(BYTE)* *e);
	//for (int i = 0; i<*e; i++) {
	//	matrix[i] = (BYTE*)malloc(sizeof(BYTE)* *w);
	//}
	//Inicializar a 0 todos los words
	for (int i = 0; i<*e; i++) {
		for (int j = 0; j<*w; j++) {
			matrix[i][j] = 0x00;
		}
	}
	return matrix;
}

BYTE** read_matrix(int *e, int* w, int* b) {
	int row = 0, column = 0, counter = 0;
	char line[SIZE_OF_LINE], str[10], token = 0;
	BYTE** matrix = NULL;

	//open file
	FILE * f;
	f = fopen(FILENAME, "r");

	while (!feof(f)) {
		//protocol (extracts headers)

		fgets(line, SIZE_OF_LINE, f);
		sscanf(line, "%c", &token);

		/*if (fscanf(f, "%c", &token) != 1) {
			puts("unrecognized protocol");
			return 0;
		}*/

		switch (token) {
		case 'c':	//comment
					//***
			break;
		case 'p':  //reads vertices and edges

			sscanf(line, "%c %s %d",&token, str, e);

			*w = *e / 8; //Every bit of each word correspond to a single column
			matrix = allocate_matrix(e, w);

			break;
		case 'e': //reads edge			

			sscanf(line, "%c %d %d", &token, row, column);

			*b = column % 8; //Decide which bit of the corresponding word represent that column
			*w = column / 8; //Decide in which word should go that column
			matrix[row - 1][*w] += 1 << *b; //Set cell to 1

											//Suposed symetric matrix
			*b = row % 8;
			*w = row / 8;
			matrix[column - 1][*w] += 1 << *b; //Set cell to 1

			counter++;
			if (counter == *e) {
				//all edges are read
				fclose(f);
				return matrix;
			}

			break;
		default:
			puts("incorrect DIMACS protocol");
			if (matrix) {
				free_matrix(*e, matrix);
				matrix = NULL;
				return matrix;
			}
		}
	}
}

int main(){
	
	int edges=0, word=0,bit=0,data=0,column=0;
	
	//Read file and create matrix
	BYTE** matrix=read_matrix(&edges,&word,&bit);
	//printf("v: %d e: %d expected_e: %d\n", n, num_edges(matrix, n), edges);

	//Read individual cells
	scanf("%d %d",&edges,&column);
	bit = column % 8;
	word = column / 8;
	data = find_cell(matrix,edges,word,bit);
	printf("El valor de la fila y columna %d %d es %d \n", edges, column, data);
	
	free_matrix(edges, matrix);
	return 0;
}
