// PROGRAMA.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí. 
// 
#include <iostream> 
#include <stdlib.h> //
#include <fstream> //Incluye las funciones new-delete[]
#include <sstream>
using namespace std;
int analisisDatos(string); 
void Operaciones(int);
void Imprimir_Matriz(int, int, double**); 
void Imprimir_Matriz_lineal(int, double*);
void Imprimir_resultado(int, double*);
double** matriz, *vector_respuesta, *resultado;

int main()
{
    string nombre_archivo;
    int num = 0, val = 0;
    do
    {
        cout << "Resolucion de matrices\n";
        cout << "Resolvamos una matriz\n";
        cout << "Ingresa el nombre de tu archivo: "; cin >> nombre_archivo;
        num = analisisDatos(nombre_archivo);

        //imprimos las matrices
        cout << "Matriz: \n";
        Imprimir_Matriz(num, num, matriz);
        cout << "Vector_respuesta: \n";
        Imprimir_Matriz_lineal(num, vector_respuesta);

        //Se realizan las operaciones
        Operaciones(num);

        //Se muestran resultados
        cout << "El resultado de cada variable es el siguiente: \n";
        Imprimir_resultado(num, resultado);

        //Se finalizan los arreglos
        for (int i = 0; i < num; i++)
        {
            delete[] *(matriz + i);
        }
       delete[] matriz; //Se borra la matriz
                
        delete[] vector_respuesta;  //Se borra el vector respuesta

        delete[] resultado; //Se borra resultado
        
        cout << "Realiza otra operacion: si->1 | no->0\n"; cin >> val;
        cout << "\n";
    } while (val == 1);

    return 0;
}

int analisisDatos(string nombre) {
    ifstream archivo;
    string texto, subtexto;
    size_t posicion = 0;
    int contador = 0, j = 0, k = 0, m = 0;
    archivo.open(nombre); //Abrimos archivo
    if (archivo.is_open())
    {
        cout << "Se logro abrir archivo" << endl;
      //Obtenemos la primera linea del archvio, y contamos las ' , '
        getline(archivo, texto);
       for (int i = 0; i < texto.length() - 1; i++)
       {
            if (texto[i] == ',')
            {
                contador++;
            }
        }
       contador++; 
       archivo.close();

       //Se abre archivo nuevamente
       archivo.open(nombre); 
        //Declaracion de la matriz
        matriz = new double* [contador]; //Se crean las filas
        for (int j = 0; j < contador; j++)
        {
            matriz[j] = new double[contador]; //Se crean las columnas
        }
        //Declaracion del vector respuesta
        vector_respuesta = new double[contador];
        //Obtencion de datos
        while (getline(archivo, texto))
        {
            posicion = texto.find("=");
            texto.erase(posicion, texto.length() - 1);
            stringstream x(texto);
            k = 0;
            while (getline(x, subtexto, ','))   //obtenemos cada cifra entre comas
            {
                char* cadena = new char[subtexto.length()];
                char* puntero = &subtexto[0];
                for (int l= 0; l < subtexto.length(); l++)
                {
                    cadena[l] = *(puntero + l); 
                } 
                *(*(matriz + j) + k) = stod(cadena); //Convertimos el string a double
                delete[] cadena;
                k++;
             }
            j++;
        }
        archivo.close();

        //Se abre archivo nuevamente
        archivo.open(nombre);
        while (getline(archivo,texto))
        {
            posicion = texto.find("=") + 1;
            texto.erase(0, posicion);
            char* cadena = new char[texto.length()];
            char* puntero = &texto[0];
            for (int l = 0; l < texto.length(); l++)
            {
                cadena[l] = *(puntero + l);
            }
            *(vector_respuesta + m) = stod(cadena); //Convertimos el string a float 
            delete[] cadena;
            m++;
        }
        archivo.close();
    }
    else {
        cout << "El archivo no pudo abrirse\n";
    }
    //Retornamos el valor del contador;
    return contador;  
} 

void Operaciones(int contador) {
    //Resolución de un sistema de ecuaciones por método de 'MONTANTE'
    double pivote, pivote_anterior, elemento_filaPiv, elemento_columPiv, elemento;
    
    //Declaramos una nueva matriz para hacer las operaciones
    double** matriz_op;
    matriz_op = new double* [contador]; //Se crean las filas 
    for (int j = 0; j < contador; j++)
    {
        matriz_op[j] = new double[contador + 1]; //Se crean las columnas 
    }

    //Colocamos los valores de esta nueva matriz
    for (int i = 0; i < contador; i++)
    {
        for (int j = 0; j < contador + 1 ; j++)
        {
            if (j == contador) {
                *(*(matriz_op + i) + j) = *(vector_respuesta + i);
            }
            else {
                *(*(matriz_op + i) + j) = *(*(matriz + i) + j);
            }
        }
    }
    //Imprimimos la matriz nueva
    cout << endl;
    cout << "Matriz ampliada: \n";
    Imprimir_Matriz(contador, contador + 1, matriz_op);
    cout << endl;

    //Comienzan las operaciones con esta nueva matriz
    for (int i = 0; i < contador; i++)
    {
        //Definicion del pivote
        if (i == 0){
            pivote_anterior = 1; 
            pivote = *(*(matriz_op + i) + i); 
        }
        else {
            pivote = *(*(matriz_op + i) + i); 
            pivote_anterior = *(*(matriz_op + (i - 1)) + (i - 1)); 
        }
        //Cambio de valor a cada elemento por determinantes 2x2
        for (int j = 0; j < contador; j++)
        {
            for (int k = contador; k >= 0; k--) 
            {               
                if (j != i)
                {
                    elemento = *(*(matriz_op + j) + k);
                    elemento_filaPiv = *(*(matriz_op + i) + k);
                    elemento_columPiv = *(*(matriz_op + j) + i);
                    *(*(matriz_op + j) + k) = (pivote * elemento - elemento_filaPiv * elemento_columPiv) / pivote_anterior;
                } 
            }
        }
        //Mostramos la matriz para observar su cambio
        cout << "Operacion " << i  + 1 << "\n";
        Imprimir_Matriz(contador, contador + 1, matriz_op);
        cout << "\n\n";
    }
    
    //Una vez operado con la 'matriz op' guardamos el resultado final en la matriz 'resultado'
    //Declaramos la matriz
    resultado = new double[contador];
    //Hacemos las operaciones para guardar el valor de cada variable
    for (int i = 0; i < contador; i++)
    {  
        *(resultado + i) = *(*(matriz_op + i) + contador) / *(*(matriz_op + i) + i);
    }

    //Borramos la nueva matriz 'matriz_op'
    for (int i = 0; i < contador; i++)
    {
        delete[] matriz_op[i]; 
    }
    delete[] matriz_op;
}

void Imprimir_Matriz(int contador_filas, int contador_columnas, double**matriz) {
    for (int i = 0; i < contador_filas; i++)
    {
        for (int j = 0; j < contador_columnas; j++)
        {
            cout << *(*(matriz + i) + j) << "\t";
        }
        cout << endl;
    }
}

void Imprimir_Matriz_lineal(int contador, double* matriz) {
    for (int i = 0; i < contador; i++) 
    {
        cout << *(vector_respuesta + i) << "\t";
    }
    cout << "\n\n";
}

void Imprimir_resultado(int contador, double* matriz) {
    for (int i = 0; i < contador; i++)
    {
        cout << "x_" << i + 1 << ": " << matriz[i] << "\n";
    }
}



// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
