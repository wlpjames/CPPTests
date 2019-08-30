/* 
	_to use this one must change the path by running these commends

	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

	sudo ldconfig

	and compile with: 
	
	g++ -Wall -o liq_test liquid_test.c -lliquid


*/



#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <complex.h>
#include <liquid/liquid.h>


#ifdef __cplusplus
extern "C"{
#endif

float* wills_fft(float* signal, int sigLen, float* output) {

	int flags=0; 
	// allocated memory arrays
    std::complex<float> * x = (std::complex<float> *) malloc(sigLen * sizeof(std::complex<float>));
    for (int i = 0; i < sigLen; i++) {
    	x[i] = signal[i];
    }

    //the output
    std::complex<float>* y = (std::complex<float> *) malloc(sigLen * sizeof(std::complex<float>));

    // create FFT plan
    fftplan q = fft_create_plan(sigLen, x, y, FFT_FORWARD, flags);

    //execute FFT (repeat as necessary)
    fft_execute(q);

    //copy back to signal
    for (int i = 0; i < sigLen; i++) {
    	output[i] = std::real(y[i]);
    }

    // destroy FFT plan and free memory arrays
    fft_destroy_plan(q);
    free(x);
    free(y);

    return 0;
}

float* wills_ifft(float* signal, int sigLen, float* output) {

	int flags=0; 
	// allocated memory arrays
    std::complex<float> * x = (std::complex<float> *) malloc(sigLen * sizeof(std::complex<float>));
    for (int i = 0; i < sigLen; i++) {
    	x[i] = signal[i];
    }

    //the output
    std::complex<float>* y = (std::complex<float> *) malloc(sigLen * sizeof(std::complex<float>));

    // create FFT plan
    fftplan q = fft_create_plan(sigLen, x, y, FFT_REVERSE, flags);

    //execute FFT (repeat as necessary)
    fft_execute(q);

    //copy back to signal
    for (int i = 0; i < sigLen; i++) {
    	output[i] = std::real(y[i]) / (float) sigLen;
    }

    // destroy FFT plan and free memory arrays
    fft_destroy_plan(q);
    free(x);
    free(y);

    return 0;
}

int main() {

    // options
    int n=3200;  // input data size

    //create a square wave at nyquist freq
    float * square = (float*) malloc(n * sizeof(float));
    for (int i = 0; i < n; i++) {
    	square[i] = (i % 2 == 0) ? 0 : 1;
    }

    //translated to freq domain
    float* fft = (float*) malloc(n * sizeof(float));

    wills_fft(square, n, fft);

    //translated back to time domain
    float* ifft = (float*) malloc(n * sizeof(float));

    wills_ifft(fft, n, ifft);

    //print fft data // the last 20
    for (int i = 0; i < 20; i++) {
    	int ind = (n - 20) + i;
    	printf("%f : %f : %f \n",std::real(square[ind]), std::real(fft[ind]), std::real(ifft[i]));
	}
    printf("done.\n");

    return 0;

}

#ifdef __cplusplus
}
#endif 