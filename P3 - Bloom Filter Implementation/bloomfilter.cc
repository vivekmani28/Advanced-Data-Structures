#include <stdio.h>
#include <stdlib.h>

#define ARRAY_COUNT 8
#define LENGTH 250000
#define MAXP 100000014259	

typedef unsigned long long mytype;

unsigned char maskArray[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

mytype aArray[ARRAY_COUNT] = {38932338087, 43216338087, 78963338005, 65432156842, 78946543216, 68875432135, 26532157896, 68546132169};
mytype bArray[ARRAY_COUNT] = {78956432123, 56548785489, 36585566888, 27943219683, 86532165487, 85465432197, 55631265491, 98976543216};

typedef struct bf_t{
	char values[ARRAY_COUNT][LENGTH];
	mytype a[ARRAY_COUNT];
	mytype b[ARRAY_COUNT];
	
}bf_t;

mytype universalhashfunction(char *str, int index, int length, bf_t *bloom){
		mytype hash = 5381;
		mytype value;
        int c;
        while (c = *str++)
            hash = ((hash << 5) + hash) + c; 
        value = ((((bloom->a[index] * hash) + bloom->b[index]) % MAXP ) % (LENGTH*8));
        return value;
}
bf_t * create_bf(){
	bf_t *bloomArray = (bf_t *) malloc( sizeof(bf_t));
	int i,j;
		
	for(i=0;i<ARRAY_COUNT;i++){
		bloomArray->a[i] = aArray[i];
		bloomArray->b[i] = bArray[i];
		for(j=0;j<LENGTH;j++)
			bloomArray->values[i][j] = 0x00;
	}	 
	return bloomArray;	
}

void insert_bf(bf_t *bloomArray, char *s){
	int length = 0;
	mytype index;
	int i;
	for(length = 0; s[length] != '\0'; ++length);
	for(i=0;i<ARRAY_COUNT;i++){
		index = universalhashfunction(s,i, length,bloomArray);
		bloomArray->values[i][(index/8)] = bloomArray->values[i][(index/8)] | maskArray[index%8];
	}
}

int is_element(bf_t *bloomArray, char *s){
	int length = 0;
	mytype index;
	int i;
	for(length = 0; s[length] != '\0'; ++length);
	for(i=0;i<ARRAY_COUNT;i++){
			index = universalhashfunction(s,i, length,bloomArray);
			if((bloomArray->values[i][index/8] & maskArray[index%8]) == 0x00){
			return 0;
		}
	}
	return 1;
}