% "t" y "x" son dos vectores (del mismo largo) que representan respectivamente el tiempo y el valor de la 
% muestra asociada a cada tiempo. Se asume un muestreo uniforme. El parametro "n" es opcional y es por si
% se quiere aumentar la resolucion de la DFT.
% El parametro "f_lim" es un vector de dos componentes [f_min f_max] para delimitar un intervalo de 
% frecuencias retorno de la funcion.
% "f y "X" representan el eje de las frecuencias y el valor de la transformada de Fourier.

function [f X] = FT_completa (t, x, f_lim, n)
	if (nargin < 4)
		X = fft(x);
	else
		X = fft(x, n);
	endif
	
	if (size(x)(1) > size(x)(2))
		w = linspace(0, 2*pi, length(X)+1)';
	else
		w = linspace(0, 2*pi, length(X)+1);
	endif
	w = w(1:end-1);
	
	if (size(t)(1) < size(t)(2)) % Convertir a vector fila
		t = t';
	endif
	
	Ts = diff(t)(1);
	w = w./Ts;
	X = fftshift(X.*exp(-j*t(1).*w));
	w = w - w(end)/2;
	
	f = w./(2*pi);
	
	if (nargin > 2)
		n = find(f>=f_lim(1) & f<=f_lim(2));
		f = f(n);
		X = X(n);
	endif
	
endfunction
