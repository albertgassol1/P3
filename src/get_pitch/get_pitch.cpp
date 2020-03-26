/// @file

#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "wavfile_mono.h"
#include "pitch_analyzer.h"

#include "docopt.h"

#define FRAME_LEN   0.030 /* 30 ms. */
#define FRAME_SHIFT 0.015 /* 15 ms. */

using namespace std;
using namespace upc;

static const char USAGE[] = R"(
get_pitch - Pitch Detector 

Usage:
    get_pitch [options] <input-wav> <output-txt> <th1> <th2> <zeros> <centerClipping> <CLcoef> <filtroMediana> <MFcoefs>
    get_pitch (-h | --help)
    get_pitch --version

Options:
    -h, --help  Show this screen
    --version   Show the version of the project

Arguments:
    input-wav   Wave file with the audio signal
    output-txt  Output file: ASCII file with the result of the detection:
                    - One line per frame with the estimated f0
                    - If considered unvoiced, f0 must be set to f0 = 0
    th1         r[1]/r[0] threshold
    th2         r[lap]/r[0] threshold
    zeros       Cruces por cero de señal sorda
    centerClipping    1 si queremos center clipping
    CLcoef            Coeficiente coef*CL
    filtroMediana     1 si queremos filtro de mediana
    MFcoefs           Numero de coeficientes del filtro de mediana
)";

int main(int argc, const char *argv[]) {
	/// \TODO 
	///  Modify the program syntax and the call to **docopt()** in order to
	///  add options and arguments to the program.
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
        {argv + 1, argv + argc},	// array of arguments, without the program name
        true,    // show help if requested
        "2.0");  // version string

	std::string input_wav = args["<input-wav>"].asString();
	std::string output_txt = args["<output-txt>"].asString();
  float th1 = stof(args["<th1>"].asString());
  float th2 = stof(args["<th2>"].asString());
  int zeros = args["<zeros>"].asLong();
  float coef = stof(args["<CLcoef>"].asString());
  int centerClip = args["<centerClipping>"].asLong();
  int filtroMediana = args["<filtroMediana>"].asLong();
  int MFcoefs = args["<MFcoefs>"].asLong();

  /// \HECHO
  /// Añadimos los thresholds de r[1]/r[0] y r[lap]/r[0], los cruces por zero de una señal
  /// sorda y la potencia de una señal sonora
  // Read input sound file
  unsigned int rate;
  vector<float> x;
  if (readwav_mono(input_wav, rate, x) != 0) {
    cerr << "Error reading input file " << input_wav << " (" << strerror(errno) << ")\n";
    return -2;
  }

  int n_len = rate * FRAME_LEN;
  int n_shift = rate * FRAME_SHIFT;

  // Define analyzer
  PitchAnalyzer analyzer(n_len, rate, th1, th2, zeros, PitchAnalyzer::HAMMING, 50, 500);

  /// \TODO
  /// Preprocess the input signal in order to ease pitch estimation. For instance,
  /// central-clipping or low pass filtering may be used.
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
  
  /// \HECHO
  /// CENTER CLIPPING : Nos fijamos en las muestras de la primera y última trama y encontramos
  /// el valor máximo en valor absoluto de la amplitud de ambas secuencias. El threshold del center clipping
  /// es el mínimo de estos dos máximos multiplicado por coef.

  // Iterate for each frame and save values in f0 vector
  vector<float> f0;
  for (iX = x.begin(); iX + n_len < x.end(); iX = iX + n_shift) {
    float f = analyzer(iX, iX + n_len);
    f0.push_back(f);
  }

  /// \TODO
  /// Postprocess the estimation in order to supress errors. For instance, a median filter
  /// or time-warping may be used.

  if(filtroMediana == 1){
    vector<float> medianWindow(MFcoefs);
    int ini = (MFcoefs - 1)/2;
    void _medianfilter(const element* signal, element* result, int N) {
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
}
  /// \HECHO 
  /// Útilización de filtro de mediana de MFcoefs ceficientes para eliminar errores gruesos en la
  /// detección de pitch
  // Write f0 contour into the output file
  ofstream os(output_txt);
  if (!os.good()) {
    cerr << "Error reading output file " << output_txt << " (" << strerror(errno) << ")\n";
    return -3;
  }

  os << 0 << '\n'; //pitch at t=0
  for (iX = f0.begin(); iX != f0.end(); ++iX) 
    os << *iX << '\n';
  os << 0 << '\n';//pitch at t=Dur

  return 0;
}
