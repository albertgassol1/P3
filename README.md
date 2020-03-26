PAV - P3: detección de pitch
============================

## Albert Gassol - Andrea Iturralde

<img src="img/unnamed.jpg" width = "200" align="center">

Esta práctica se distribuye a través del repositorio GitHub [Práctica 3](https://github.com/albino-pav/P3).
Siga las instrucciones de la [Práctica 2](https://github.com/albino-pav/P2) para realizar un `fork` de la
misma y distribuir copias locales (*clones*) del mismo a los distintos integrantes del grupo de prácticas.

Recuerde realizar el *pull request* al repositorio original una vez completada la práctica.

Ejercicios básicos
------------------

- Complete el código de los ficheros necesarios para realizar la detección de pitch usando el programa
  `get_pitch`.

   * Complete el cálculo de la autocorrelación e inserte a continuación el código correspondiente.
   
   ```cpp
   
   for (unsigned int l = 0; l < r.size(); ++l) {
      for(unsigned int n=0; n<x.size()-1-l; ++n){ 
        r[l] += x[n]*x[n+l];   
      } 
    }
    
    ```

   * Inserte una gŕafica donde, en un *subplot*, se vea con claridad la señal temporal de un sonido sonoro
     y su periodo de pitch; y, en otro *subplot*, se vea con claridad la autocorrelación de la señal y la
	 posición del primer máximo secundario.

	 NOTA: es más que probable que tenga que usar Python, Octave/MATLAB u otro programa semejante para
	 hacerlo. Se valorará la utilización de la librería matplotlib de Python.

<p align="center">
  <img width="1100" src="img/autocorrelation.png">
</p>

Se ha utilizado el script de Python `autocorrelation.py` para obtener esta gráfica. Este script se encuentra en la carpeta `scripts` de este repositorio.
 
   * Determine el mejor candidato para el periodo de pitch localizando el primer máximo secundario de la autocorrelación. Inserte a continuación el código correspondiente.
     
    ```cpp
    while(*iR > 0 && iR != r.end()){
      ++iR;
    }

    if(iR == r.end()){
      
      iRMax = r.begin() + npitch_max;
      for(iR = r.begin() + npitch_min; iR != r.end(); ++iR){

        if(*iR > *iRMax){
          iRMax = iR;
        }
      }

    }else{

      if(iR < r.begin() + npitch_min){

        iR += npitch_min;
      }
      iRMax = iR;

      while(iR != r.end()){

        if(*iR > *iRMax){

          iRMax = iR;
        }

        ++iR;
      }
    }
    
     ```

   * Implemente la regla de decisión sonoro o sordo e inserte el código correspondiente.

  ```cpp
bool PitchAnalyzer::unvoiced(float pot, float r1norm, float rmaxnorm, float zeros) const {

    return (r1norm <= th1 || rmaxnorm <= th2) || zeros > zero;

  }

int PitchAnalyzer::compute_zcr(std::vector<float> & x, unsigned int N, float fm) const{

    int i = 0;
    int ZCR = 0;
    for(i = 1; i < N; i++){
        
        if(x[i - 1] * x[i] < 0){

            ZCR++;
        }
    }

    return ZCR * (fm/(2*(N-1)));
  }

  ```

Utilizamos únicamente los coeficientes de la autocorrelación. Establecemos unos thresholds con los que determinamos si el sonido es sordo o sonoro.
- Una vez completados los puntos anteriores, dispondrá de una primera versión del detector de pitch. El 
  resto del trabajo consiste, básicamente, en obtener las mejores prestaciones posibles con él.

  * Utilice el programa `wavesurfer` para analizar las condiciones apropiadas para determinar si un
    segmento es sonoro o sordo. 
	
	  - Inserte una gráfica con la detección de pitch incorporada a `wavesurfer` y, junto a ella, los 
	    principales candidatos para determinar la sonoridad de la voz: el nivel de potencia de la señal
		(r[0]), la autocorrelación normalizada de uno (r1norm = r[1] / r[0]) y el valor de la
		autocorrelación en su máximo secundario (rmaxnorm = r[lag] / r[0]).

		Puede considerar, también, la conveniencia de usar la tasa de cruces por cero.

	    Recuerde configurar los paneles de datos para que el desplazamiento de ventana sea el adecuado, que
		en esta práctica es de 15 ms.

<p align="center">
  <img width="1100" src="img/pitchgraph.png">
</p>

En la imagen podemos observar 5 gráficas. La primera es rmaxnorm (r[lag] / r[0]), la segunda r1norm (r[1] / r[0]), la tercera los cruces por cero, la cuarta el pitch y la quinta la grabación de audio.

Podemos distinguir los sonidos sordos de los sonoros ya que, en los sonoros, tanto r1norm como rmaxnorm toman valores alrededor de 1, mientras que en los sordos toman valores más cercanos a 0.

Esto es debido a que la correlación entre las muestras de un sonido sonoro es muy alta, mientras que la de los sordos, como semejan ruido, no lo es. También se debe a que los sonidos sonoros son periódicos, lo que nos asegura que los máximos se repetiran eventualmente, lo cual aproxima a 1 la relación entre el segundo y primer máximo, que son de valores similares.

```cpp

int zeros = compute_zcr(x, x.size(), samplingFreq);

    float pot = 10 * log10(r[0]);

    ofstream os("r1norm.txt", std::ofstream::app);
    if (!os.good()) {
      cerr << "Error reading output file " << "datos.txt" << " (" << strerror(errno) << ")\n";
      return -3;
    }

  
    os <<r[1]/r[0]<<'\n';

    ofstream os1("rmaxnorm.txt", std::ofstream::app);
    if (!os1.good()) {
      cerr << "Error reading output file " << "datos.txt" << " (" << strerror(errno) << ")\n";
      return -3;
    }

  
    os1 <<r[lag]/r[0]<<'\n';
    
    ofstream os2("zeros.txt", std::ofstream::app);
    if (!os2.good()) {
      cerr << "Error reading output file " << "datos.txt" << " (" << strerror(errno) << ")\n";
      return -3;
    }

  
    os2 <<zeros<<'\n';
```

  * Use el detector de pitch implementado en el programa `wavesurfer` en una señal de prueba y compare su r
    resultado con el obtenido por la mejor versión de su propio sistema.
    Inserte una gráfica ilustrativa del resultado de ambos detectores.
    
<p align="center">
  <img width="1100" src="img/pitchcompare.png">
</p>

  * Optimice los parámetros de su sistema de detección de pitch e inserte una tabla con las tasas de error
    y el *score* TOTAL proporcionados por `pitch_evaluate` en la evaluación de la base de datos 
	`pitch_db/train`..

<p align="center">
  <img width="200" src="img/pitchevaluate1.png">
</p>


   * Inserte una gráfica en la que se vea con claridad el resultado de su detector de pitch junto al del
     detector de Wavesurfer. Aunque puede usarse Wavesurfer para obtener la representación, se valorará
	 el uso de alternativas de mayor calidad (particularmente Python).

<p align="center">
  <img width="1100" src="img/pitchgraphs.png">
</p>

El código utilizado para obtener los gráficos se encuentra en la carpeta `scripts` de este repositorio y se llama `pitchcompare.py`. En la primera gráfica, vemos el pitch calculado por nuestro programa. En la segunda, se puede ver el pitch calculado por `wavesurfer`.

Ejercicios de ampliación
------------------------

- Usando la librería `docopt_cpp`, modifique el fichero `get_pitch.cpp` para incorporar los parámetros del
  detector a los argumentos de la línea de comandos.
  
  Esta técnica le resultará especialmente útil para optimizar los parámetros del detector. Recuerde que
  una parte importante de la evaluación recaerá en el resultado obtenido en la detección de pitch en la
  base de datos.

  * Inserte un *pantallazo* en el que se vea el mensaje de ayuda del programa y un ejemplo de utilización
    con los argumentos añadidos.

<p align="center">
  <img width="900" src="img/docopt.png">
</p>

- Implemente las técnicas que considere oportunas para optimizar las prestaciones del sistema de detección
  de pitch.

  Entre las posibles mejoras, puede escoger una o más de las siguientes:

  * Técnicas de preprocesado: filtrado paso bajo, *center clipping*, etc.
  * Técnicas de postprocesado: filtro de mediana, *dynamic time warping*, etc.
  * Métodos alternativos a la autocorrelación: procesado cepstral, *average magnitude difference function*
    (AMDF), etc.
  * Optimización **demostrable** de los parámetros que gobiernan el detector, en concreto, de los que
    gobiernan la decisión sonoro/sordo.
  * Cualquier otra técnica que se le pueda ocurrir o encuentre en la literatura.
  
De todas las posibles mejoras, hemos decidido implementar tres de ellas: el preprocesado, el postprocesado y la optimización de parámetros del detector.
  
En primer lugar, hemos aplicado el método *center clipping* para el preprocesado. Hemos detectado los valores máximos de la primera y última trama de la señal, y los hemos utilizado para establecer un umbral. Este umbral es el mínimo de los máximos y se aplica de la siguiente manera a las muestras de la señal:
  	* Si el valor de la señal es mayor que el umbral, se le resta dicho umbral a la muestra.
	* Si el valor de la señal es menor que el umbral cambiado de signo, se le suma dicho umbral a la muestra.
	* En caso contrario, el valor de la muestra pasa a ser 0.
	
El códgo implementado es el siguiente:
  
  ```cpp
  
    vector<float>::iterator iX;

  if(centerClip == 1){
    float clip, max1, max2;
    max1 = max2 = 0;

    for (iX = x.begin(); iX < x.begin() + n_len; ++iX) {
      if(fabs(*iX) > max1){
        max1 = fabs(*iX);
      }
    }

    for (iX = x.end() - n_len; iX < x.end(); ++iX) {
      if(fabs(*iX) > max2){
        max2 = fabs(*iX);
      }
    }

    clip = coef * min(max1,max2);

    for (iX = x.begin(); iX < x.end(); ++iX) {
      if (*iX >= clip){
        *iX = *iX - clip;
      }
      else if (*iX <= -clip){
        *iX = *iX + clip;
      }
      else{
        *iX = 0;
      }
    }
  }
  
  ```
  Aplicamos este preprocesado para intentar periodificar al máximo posible la señal. Podemos ver que el resultado es mejor ya que ahora la *score* es de XXXXX
  
  FOTO D LA SCORE :D
  
  En segundo lugar, hemos aplicado el filtro de mediana para el postprocesado. Para ello, hemos implementado un código que permite al usuario introducir por pantalla el tamaño de la ventana con la que haremos el filtrado, que reordena los valores de menor a mayor y que escoge el valor central como nuevo valor de la muestra. 
  
  Lo hemos implementado de manera que la ventana sea un vector y no empiece centrada en la muestra 0 de la señal, sinó en la posición central de la ventana. De esta manera, los primeros y últimos *(MFcoefs - 1)/2* se mantienen a su valor inicial.  
  
El códgo implementado es el siguiente:
  
  ```cpp
  
  if(filtroMediana == 1){
    
    vector<float> medianWindow(MFcoefs);
    int ini = (MFcoefs - 1)/2;

    for (int i = ini; i < f0.size() - ini; ++i) {

      for (int j = 0; j < MFcoefs; ++j) {

        medianWindow[j] = f0[i - ini + j];
      }
      for (int j = 0; j < ini + 1; ++j) {

        int min = j;
        for (int k = j + 1; k < MFcoefs; ++k) {
          
            if (medianWindow[k] < medianWindow[min]) {
              min = k;
            }
        }
        float aux = medianWindow[j];
        medianWindow[j] = medianWindow[min];
        medianWindow[min] = aux;
      }
      f0[i] = medianWindow[ini];
    }
  }   
  ```
 
 Tal y como pasó al aplicar el preprocesado, con el postprocesado también detectamos una mejora en la *score*, que ahora es de XXXX
 
 FOTO D LA SCORE :D
 
 Finalmente, para optimizar los parámetros del detector, hemos generado un script que itera de cuatro maneras diferentes los siguientes valores: los thresholds de la correlación, el ZCR, el coeficiente del clipping y el número de coeficientes del filtro de mediana.
 
 En la primera iteramos los parámetros de la práctica sin la ampliación, en la segunda añadimos únicamente el clipping, en la tercera añadimos únicamente el filtro de mediana y en la cuarta añadimos ambos métodos de procesado.
 
 FOTO D LAS 4 MEJORES FSCORE
 
 Podemos confirmar de nuevo, que el programa detecta mejor el pitch con los métodos de pre y postprocesado, y que los valores óptimos de los parámetros son:
 	* th1  = 
	* th2 = 
	* ZCR = 
	* coefClipping = 
	* nCoefMedian = 

  Encontrará más información acerca de estas técnicas en las [Transparencias del Curso](https://atenea.upc.edu/pluginfile.php/2908770/mod_resource/content/3/2b_PS Techniques.pdf)
  y en [Spoken Language Processing](https://discovery.upc.edu/iii/encore/record/C__Rb1233593?lang=cat).
  También encontrará más información en los anexos del enunciado de esta práctica.

  Incluya, a continuación, una explicación de las técnicas incorporadas al detector. Se valorará la
  inclusión de gráficas, tablas, código o cualquier otra cosa que ayude a comprender el trabajo realizado.

  También se valorará la realización de un estudio de los parámetros involucrados. Por ejemplo, si se opta
  por implementar el filtro de mediana, se valorará el análisis de los resultados obtenidos en función de
  la longitud del filtro.
   

Evaluación *ciega* del detector
-------------------------------

Antes de realizar el *pull request* debe asegurarse de que su repositorio contiene los ficheros necesarios
para compilar los programas correctamente ejecutando `make release`.

Con los ejecutables construidos de esta manera, los profesores de la asignatura procederán a evaluar el
detector con la parte de test de la base de datos (desconocida para los alumnos). Una parte importante de
la nota de la práctica recaerá en el resultado de esta evaluación.
