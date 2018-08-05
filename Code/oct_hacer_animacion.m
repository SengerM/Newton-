clc
close all
clear all
more off % Esto es para que muestre los printf(), disp() y resultados a medida que los ejecuta y no todo junto al final
set(0,'defaultlinelinewidth',2); % Setea el 'LineWidth' predeterminado en 2.

DIAMETRO_LUNA = 3.474e6;
MASA_LUNA = 7.34767309e22; % kilograms, from Google.


% ------ CONFIGURACIÓN -------
GUARDAR_GRAFICOS.SVG = 0;
GUARDAR_GRAFICOS.PNG = 0;

COLORES = {[0 0 0], [1 0 0], [0 .7 0], [0 0 1], [1 .5 0], [0 .7 .7], [.7 0 .7]};
% TAMAÑO DE FIGURAS ----------------------------------------
X_mm = 150; % ancho de las imágenes en milímetros
PROPORCION = [1 1];		x_pixels = X_mm*3.779527559*PROPORCION(1); y_pixels = round(PROPORCION(2)*x_pixels);
% ----------------------------------------------------------
% ------- LEVANTAR TODAS LAS MEDICIONES EN FORMA AUTOMATIZADA DE UN DIRECTORIO --------
DIRECTORIO_ARCHIVOS = "resultados"; % Directorio en el cual se encuentran los archivos a leer.
[out, txt] = system(sprintf("ls -1 %s/", DIRECTORIO_ARCHIVOS)); % Le digo al comando «ls» que me liste todos los archivos en «DIRECTORIO».
ARCHIVOS_TEMP = strsplit(txt); % Genero un cell array con cada uno de los archivos listaos por el comando «ls».
for (k = 1:(length(ARCHIVOS_TEMP)-1) ) % Tengo que eliminar el último elemento de «ARCHIVOS_TEMP» porque es una cadena vacía. Siempre lo devuelve así.
	ARCHIVOS{k} = ARCHIVOS_TEMP{k}; % No me dejaba hacer ARCHIVOS = ARCHIVOS{1:end-1}; Andá a saber por qué...
endfor
clear out txt DIRECTORIO ARCHIVOS_TEMP k
% Al terminar esto, en «ARCHIVOS{:}» se encuentran los nombres de todos los archivos ubicados en «DIRECTORIO_ARCHIVOS», en forma de cadenas de texto.
% -------------------------------------------------------------------------------------
ARCHIVOS
fecha_simulacion = ARCHIVOS{input("Numero de simulacion a animar --> ")};


DIRECTORIO_DE_RESULTADOS = sprintf("%s/%s", DIRECTORIO_ARCHIVOS, fecha_simulacion);
DIRECTORIO_PARA_GUARDAR_GRAFICOS = DIRECTORIO_DE_RESULTADOS;


data = dlmread(sprintf("%s/simulation_output.txt", DIRECTORIO_DE_RESULTADOS),'\t',0,0);

N_particulas = floor((length(data(1,:))-1)/3); % Una columna es el tiempo y luego hay tres de posición para cada partícula.
tiempo = data(:,1);

% Muestro datos sobre la simulación ---------
printf("Numero de particulas %i\n", N_particulas);
printf("El tiempo de simulacion va desde %.3g segundos hasta %.3g segundos.\n", min(tiempo), max(tiempo));
printf("Puntos simulados --> %i\n", length(tiempo));






% Todo lo que sigue a continuación es por el tema del error que tira y se cuelga Octave, para no perder todo lo que tenaímos hecho...
[out, txt] = system(sprintf("ls -1 %s/", DIRECTORIO_PARA_GUARDAR_GRAFICOS)); % Le digo al comando «ls» que me liste todos los archivos en «DIRECTORIO».
ARCHIVOS_TEMP = strsplit(txt); % Genero un cell array con cada uno de los archivos listaos por el comando «ls».
for (k = 1:(length(ARCHIVOS_TEMP)-1) ) % Tengo que eliminar el último elemento de «ARCHIVOS_TEMP» porque es una cadena vacía. Siempre lo devuelve así.
	ARCHIVOS_TEMPORALES{k} = ARCHIVOS_TEMP{k}; % No me dejaba hacer ARCHIVOS = ARCHIVOS{1:end-1}; Andá a saber por qué...
end%for
clear out txt DIRECTORIO ARCHIVOS_TEMP k

kk_inicial = 1;
ARCHIVO_TEMPORAL_PARA_EL_ERROR = "hubo_error";
if (sum(ismember(ARCHIVOS_TEMPORALES, ARCHIVO_TEMPORAL_PARA_EL_ERROR)))
	load(sprintf("%s/%s", DIRECTORIO_PARA_GUARDAR_GRAFICOS, ARCHIVO_TEMPORAL_PARA_EL_ERROR));
	kk++;
	printf("Se cargo el archivo porque antes hubo un error. Se continua desde el paso kk == %i\n", kk);
	kk_inicial = kk;
else
	% CONFIGURACIÓN DE LA ANIMACIÓN ----------------------------------------
	N_FRAMES = input("Numero de fotogramas de la animacion (0 usa todos los puntos disponibles): "); % Número de fotogramas de la animación. Si se pone 0 entonces se usan todos los puntos disponibles de la simulación.
	VENTANA_TEMPORAL = [tiempo(1), tiempo(end)]; % [input("Inicio de la ventana temporal a animar: "), input("Fin de la ventana temporal a animar: ")]; % Ventana de tiempo de la simulación a incluir en la película.
	DURACION_DEL_VIDEO = input("Duracion del video en segundos (0 tiempo real): "); % En segundos. Si se pone 0 entonces la duración es "real time" con la simulación.
	CREAR_PELICULA = input("Crear un archivo «.avi» con la animacion? (Esto toma mucho mas tiempo) --> ");
end%if
# Hasta acá es por el error de Octave.









puntos_a_conservar = find(tiempo>=VENTANA_TEMPORAL(1) & tiempo<=VENTANA_TEMPORAL(2));
tiempo = tiempo(puntos_a_conservar);

for (k = 1:N_particulas)
	posicion{k} = data(puntos_a_conservar, (2+3*k-3):(2+3*k-1));
	
	max_x(k) = max(posicion{k}(:,1));
	min_x(k) = min(posicion{k}(:,1));
	max_y(k) = max(posicion{k}(:,2));
	min_y(k) = min(posicion{k}(:,2));
	max_z(k) = max(posicion{k}(:,3));
	min_z(k) = min(posicion{k}(:,3));
end%for

coordenada_maxima = max([max_x, max_y, max_z]);
coordenada_minima = min([min_x, min_y, min_z]);



% Posición del centro de masa.
m_i = MASA_LUNA/N_particulas; % Masa de cada una de las partículas.
R_CM = posicion{1}.*0; 
for (k = 1:N_particulas)
	R_CM += m_i.*posicion{k};
end%for
R_CM /= MASA_LUNA;





if ( (paso = floor(length(tiempo)/N_FRAMES)) < 1 || N_FRAMES == 0)
	paso = 1;
	N_FRAMES = length(tiempo);
end%if
t = 1:paso:length(tiempo);
clear paso;

if (CREAR_PELICULA)
	set(0, 'defaultfigurevisible', 'off');
else
	set(0, 'defaultfigurevisible', 'on');
end%if

figure('Position',[0,0,x_pixels,y_pixels]); hold on; clear("leyenda"); clear("graf");
% CONFIGURACION DEL GRAFICO ---------------------------------------------
% VENTANA DEL GRAFICO -------------------------------------
X_MIN = coordenada_minima;
X_MAX = coordenada_maxima;
Y_MIN = coordenada_minima;
Y_MAX = coordenada_maxima;
Z_MIN = coordenada_minima;
Z_MAX = coordenada_maxima;
if (kk == 1)
axis([X_MIN X_MAX Y_MIN Y_MAX Z_MIN Z_MAX]);
end%if
grid on
view([0,0,1]);
for (k = 1:N_particulas)
	graf(k) = plotf(posicion{k}(:,1)(t(kk)), posicion{k}(:,2)(t(kk)), posicion{k}(:,3)(t(kk)));
	set(graf(k), 
		'LineWidth', 2,
		'Marker', 'o',
		'MarkerSize', 10,
		'Color', COLORES{mod(k, length(COLORES))+1});
end%for
tic();
for (kk = kk_inicial:length(t))
%%%%%%%%%%%%%%%%%%%%%%%      FIGURA      %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 TITULO_GRAFICO = sprintf("%.5i", kk);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	plotf = @plot3; % Reemplazar por «semilogy», «loglog», etc y automáticamente pone los tics
	
	for (k = 1:N_particulas)
		set(graf(k), "xdata", posicion{k}(:,1)(t(kk)), "ydata", posicion{k}(:,2)(t(kk)), "zdata", posicion{k}(:,3)(t(kk)));
	end%for
	drawnow();
	%~ view_param = (kk-kk_inicial)/(length(t)-kk_inicial);
	%~ view([ cos(view_param*pi/2)^2, sin(0.5*view_param*pi)^2, .3*sin(view_param*pi)^2]);
	% IMAGEN DE SALIDA ----------------------------------------
	if (CREAR_PELICULA)
		XY_RATIO = sprintf("-S%i,%i", x_pixels, y_pixels); ARCHIVO_IMAGEN = strrep(TITULO_GRAFICO, " ", "_");
		print(sprintf("%s/%s.png", DIRECTORIO_PARA_GUARDAR_GRAFICOS, ARCHIVO_IMAGEN),'-dpng', '-r200', XY_RATIO);
		save(sprintf("%s/%s", DIRECTORIO_PARA_GUARDAR_GRAFICOS, ARCHIVO_TEMPORAL_PARA_EL_ERROR), "kk", "N_FRAMES", "VENTANA_TEMPORAL", "DURACION_DEL_VIDEO", "CREAR_PELICULA"); % Esto es por si surge el error que crashea Octave para no perder todo.
	else
		if (DURACION_DEL_VIDEO == 0) %% En este caso lo hacemos en tiempo real.
			DURACION_DEL_VIDEO = VENTANA_TEMPORAL(2) - VENTANA_TEMPORAL(1);
		end%if
		frames_per_second = (N_FRAMES+1)/DURACION_DEL_VIDEO;
		pause(1/frames_per_second);
	end%if
	print_processing_times(kk, kk_inicial, length(t));
% -------------------------------------------------------------------------------------
end%for

if (CREAR_PELICULA)
	frames_per_second = (N_FRAMES+1)/DURACION_DEL_VIDEO;
	
	command = sprintf("mencoder mf://%s/*.png -mf w=640:h=480:fps=%i:type=png -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -oac copy -o %s/video.avi", DIRECTORIO_PARA_GUARDAR_GRAFICOS, frames_per_second, DIRECTORIO_DE_RESULTADOS);
	system(command);
	
	system(sprintf("rm %s/*.png", DIRECTORIO_PARA_GUARDAR_GRAFICOS));
	
	system(sprintf("rm %s/%s", DIRECTORIO_PARA_GUARDAR_GRAFICOS, ARCHIVO_TEMPORAL_PARA_EL_ERROR));
end%if












