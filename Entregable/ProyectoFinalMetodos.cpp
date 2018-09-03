
// ifstream constructor.
#include <iostream>     // std::cout
#include <fstream> 
#include <string>      // std::ifstream
using namespace std;

int filas, columnas, restricciones,basica, no_basica;
float Info[10][10];
float beneficio[10];
float restr[10][10];
float simplex[100][100];
int basicos[100];

void basicosInit(){
	basicos[0]=-1;
	for (int i = 1; i < basica; ++i)
	{
		basicos[i]=filas+i;
	}
}

void simplexInit(){
	simplex[0][0]=1; // Z = 1
	basica = 1+filas+restricciones;
	no_basica = 2 + filas*2 + restricciones;
	for (int i = filas; i < no_basica; ++i)
	{
		if(i-filas+1<basica)
			simplex[i-filas+1][0]=0;
		simplex[0][i]=0;
	}
	for (int i = 1; i < columnas; ++i)
	{
		simplex[0][i]=beneficio[i-1]*-1;
	}
	for (int i = 1; i < basica; ++i)
	{
		for (int j = 0; j < columnas; ++j)
		{
			if(i-1<filas){
				
				if(j==columnas-1)
					simplex[i][no_basica-1]=Info[i-1][j];
				else
					simplex[i][j+1]= Info[i-1][j];

			}
			else{
				if(j==columnas-1){
					simplex[i][no_basica-1]=restr[i-filas-1][j];
				}
				else
					simplex[i][j+1]=restr[i-filas-1][j];
			}

		}
	}
	for (int i = 1; i < filas+restricciones+1; ++i)
	{
		for (int j = columnas; j < columnas+filas+restricciones; ++j)
		{
			if(j-columnas==i-1){
				simplex[i][j]=1;
			}else{
				simplex[i][j]=0;
			}
		}
	}
	basicosInit();
}

int optimality(){
	int pos=-1;
	float aux=0;
	for (int i = 0; i < columnas; ++i)
	{
		if (simplex[0][i+1]<aux){
			aux = simplex[0][i+1];
			pos=i+1;
		}
	}
	printf("Posicion Optimalidad: %i\n", pos);
	return pos;
}

int varSaliente(int pos_columna){
	int pos=-1;
	float aux=99999999,res =0;
	for (int i = 1; i < basica; ++i)
	{
		res = simplex[i][no_basica-1]/simplex[i][pos_columna];
		if (res<aux && res >0){
			aux = res;
			pos=i;
		}
	}
	return pos;
}

void pivoteo(int columna_pivote, int fila_pivote){
	float divisor=simplex[fila_pivote][columna_pivote];
	for (int i = 0; i < no_basica; ++i)
	{
		simplex[fila_pivote][i]=simplex[fila_pivote][i]/divisor;
	}
	basicos[fila_pivote]=columna_pivote;

	for (int i = 0; i < basica; i++){
		if(i!=fila_pivote){
			divisor = simplex[i][columna_pivote]*-1;
			for (int j = 0; j < no_basica; j++){
				simplex[i][j]=simplex[i][j]+simplex[fila_pivote][j]*divisor;
			}
		}
	}

}

void interpretar(){
	cout <<"\nResultados : \n";
	int aux;
	for (int i = 0; i < basica; ++i)
	{
		aux = basicos[i];
		if(aux==-1){
			cout << "La Maximizacion da como resultado :  " << simplex[i][no_basica-1] << "\n";
		}
		else if(aux>0 && aux<=filas){
			cout << "Variable " << aux << " -> Valor óptimo:  " << simplex[i][no_basica-1] << "\n";
		}else if(aux>filas){
			cout << "Restriccion " << aux-filas << " -> Holgura de:  " << simplex[i][no_basica-1] << "\n";
		}
	}
	cout << "Las restricciones faltantes terminaron en una holgura de:  0 \n";
}

int main () {

/*Lectura desde Archivos*/
cout << "Bienvenidos al programa de calculo de maximizaciones por metodo simplex, del equipo MMM \n";
ifstream f("matrix2.txt");

f >> filas >> columnas;

for (int i = 0; i < filas; i++)
for (int j = 0; j < columnas; j++)
  f >> Info[i][j];



for (int i = 0; i < columnas-1; ++i)
{
	f>> beneficio[i];	
}

f>> restricciones;

for (int i = 0; i < restricciones; ++i)
{
	for (int j = 0; j < columnas; ++j){
		f >> restr[i][j];
	}
}


f.close();
cout << "Matriz Proporcionada: \n";

for (int i = 0; i < filas; i++){
	for (int j = 0; j < columnas; j++){
		cout << Info[i][j] << " ";
	}
	cout << "\n";
}

cout << "Beneficio obtenido: \n";


for (int i = 0; i < columnas-1; i++){
	cout << beneficio[i] << " ";
}
cout << "\n";

cout << "Restricciones: \n";

for (int i = 0; i < restricciones; i++){
	for (int j = 0; j < columnas; j++){
		cout << restr[i][j] << " ";
	}
	cout << "\n";
}

simplexInit();

cout << "Tabla simplex incial\n";

for (int i = 0; i < basica; i++){
	for (int j = 0; j < no_basica; j++){
		cout << simplex[i][j] << " ";
	}
	cout << "\n";
}

cout << "En filas: (Indices respectivos a las variables/restricciones de la matriz) \n";


for (int i = 0; i < basica; i++){
	cout << basicos[i] << " ";
}


/*cout << "Basicos Inicializacion\n";


for (int i = 0; i < basica; i++){
	cout << basicos[i] << " ";
}*/
cout << "\n";
cout << "\n";
cout << "\n";

int columna_pivote,fila_pivote;
while (true){

	columna_pivote = optimality();

	if(columna_pivote==-1)
		break;

	fila_pivote = varSaliente(columna_pivote);


	cout << "\nLa variable saliente fue la localizada en la fila " << fila_pivote << " de la matriz (Recuerda que es respecto a la matriz de arriba) \n\n";

	pivoteo(columna_pivote,fila_pivote);

	cout << "Nueva tabla simplex:\n";
	for (int i = 0; i < basica; i++){
		for (int j = 0; j < no_basica; j++){
			cout << simplex[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "En filas: (Indices respectivos a las variables/restricciones de la matriz) \n";


	for (int i = 0; i < basica; i++){
		cout << basicos[i] << " ";
	}
	cout << "\n";
}
interpretar();
  return 0;
}
