clc
close all
clear all
more off % Esto es para que muestre los printf(), disp() y resultados a medida que los ejecuta y no todo junto al final
set(0,'defaultlinelinewidth',2); % Setea el 'LineWidth' predeterminado en 2.

% ------ CONFIGURACIÓN -------
GUARDAR_GRAFICOS.SVG = 0;
GUARDAR_GRAFICOS.PNG = 0;

COLORES = {[1e-3 1e-3 1e-3], [1 1e-3 1e-3], [1e-3 .7 1e-3], [1e-3 1e-3 1], [1 .5 1e-3], [1e-3 .7 .7], [.7 1e-3 .7]};
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
end%for
clear out txt DIRECTORIO ARCHIVOS_TEMP k
% Al terminar esto, en «ARCHIVOS{:}» se encuentran los nombres de todos los archivos ubicados en «DIRECTORIO_ARCHIVOS», en forma de cadenas de texto.
% -------------------------------------------------------------------------------------
ARCHIVOS
fecha_simulacion = ARCHIVOS{end};%input("Numero de simulacion a animar --> ")};

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
	N_FRAMES = 0;%input("Numero de fotogramas de la animacion (0 usa todos los puntos disponibles): "); % Número de fotogramas de la animación. Si se pone 0 entonces se usan todos los puntos disponibles de la simulación.
	VENTANA_TEMPORAL = [tiempo(1), tiempo(end)]; % [input("Inicio de la ventana temporal a animar: "), input("Fin de la ventana temporal a animar: ")]; % Ventana de tiempo de la simulación a incluir en la película.
	DURACION_DEL_VIDEO = 20;%input("Duracion del video en segundos (0 tiempo real): "); % En segundos. Si se pone 0 entonces la duración es "real time" con la simulación.
	CREAR_PELICULA = 1;%input("Crear un archivo «.avi» con la animacion? (Esto toma mucho mas tiempo) --> ");
end%if
# Hasta acá es por el bug de Octave -----------------------------------------------------------------------

puntos_a_conservar = find(tiempo>=VENTANA_TEMPORAL(1) & tiempo<=VENTANA_TEMPORAL(2));
tiempo = tiempo(puntos_a_conservar);

for (k = 1:N_particulas-1) % La última es el proyectil.
	objetivo{k} = data(puntos_a_conservar, (2+3*k-3):(2+3*k-1));
	max_x(k) = max(objetivo{k}(:,1));
	min_x(k) = min(objetivo{k}(:,1));
	max_y(k) = max(objetivo{k}(:,2));
	min_y(k) = min(objetivo{k}(:,2));
	max_z(k) = max(objetivo{k}(:,3));
	min_z(k) = min(objetivo{k}(:,3));
end%for
coordenada_maxima = max([max_x, max_y, max_z]);
coordenada_minima = min([min_x, min_y, min_z]);

k = N_particulas;
proyectil = data(:, (2+3*k-3):(2+3*k-1));

if ( (paso = floor(length(tiempo)/N_FRAMES)) < 1 || N_FRAMES == 0)
	paso = 1;
	N_FRAMES = length(tiempo);
end%if
t = 1:paso:length(tiempo);
clear paso;

if (CREAR_PELICULA)
	set(0, 'defaultfigurevisible', 'off'); % Lo desactivo para que valla más rápido.
else
	set(0, 'defaultfigurevisible', 'on');
end%if



figure('Position',[0,0,x_pixels,y_pixels]); hold on; clear("leyenda"); clear("graf");
kk = kk_inicial;
trayectoria = plot3(proyectil(:,1)(1:t(kk)), proyectil(:,2)(1:t(kk)), proyectil(:,3)(1:t(kk)));
set(trayectoria, 
		'LineWidth', .5,
		'Marker', 'x',
		'MarkerSize', 4,
		'Color', COLORES{2}.^0.05);

for (k = 1:N_particulas-1)
	graf(k) = plot3(objetivo{k}(:,1)(t(kk)), objetivo{k}(:,2)(t(kk)), objetivo{k}(:,3)(t(kk)));
	set(graf(k), 
		'LineWidth', 2,
		'Marker', '.',
		'Color', COLORES{1});
end%for
graf_proyectil = plot3(proyectil(:,1)(t(kk)), proyectil(:,2)(t(kk)), proyectil(:,3)(t(kk)));
set(graf_proyectil, 
	'LineWidth', 2,
	'Marker', '.',
	'MarkerSize', 20,
	'Color', COLORES{2});
% CONFIGURACION DEL GRAFICO ---------------------------------------------
% VENTANA DEL GRAFICO -------------------------------------
X_MIN = coordenada_minima + (coordenada_maxima-coordenada_minima)/4;
X_MAX = coordenada_maxima + (coordenada_maxima-coordenada_minima)/4;
Y_MIN = min(min_y);
Y_MAX = max(max_y);
Z_MIN = coordenada_minima;
Z_MAX = coordenada_maxima;
axis([X_MIN X_MAX Y_MIN Y_MAX Z_MIN Z_MAX]);
view([0,0,1]);
grid on;
tic();
for (kk = kk_inicial:length(t))
	%%%%%%%%%%%%%%%%%%%%%%%      FIGURA      %%%%%%%%%%%%%%%%%%%%%%%
	TITULO_GRAFICO = sprintf("%.5i", kk);
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	set(trayectoria, "xdata", proyectil(:,1)(1:t(kk)), "ydata", proyectil(:,2)(1:t(kk)), "zdata", proyectil(:,3)(1:t(kk)));
	for (k = 1:N_particulas-1)
		set(graf(k), "xdata", objetivo{k}(:,1)(t(kk)), "ydata", objetivo{k}(:,2)(t(kk)), "zdata", objetivo{k}(:,3)(t(kk)));
	end%for
	set(graf_proyectil, "xdata", proyectil(:,1)(t(kk)), "ydata", proyectil(:,2)(t(kk)), "zdata", proyectil(:,3)(t(kk)));
	
	drawnow(); % Usar esto hace que valla MUCHO más rápido.
	
	
	% IMAGEN DE SALIDA ----------------------------------------
	if (CREAR_PELICULA)
		XY_RATIO = sprintf("-S%i,%i", x_pixels, y_pixels); ARCHIVO_IMAGEN = strrep(TITULO_GRAFICO, " ", "_");
		print(sprintf("%s/%s.png", DIRECTORIO_PARA_GUARDAR_GRAFICOS, ARCHIVO_IMAGEN),'-dpng', '-r250', XY_RATIO);
		save(sprintf("%s/%s", DIRECTORIO_PARA_GUARDAR_GRAFICOS, ARCHIVO_TEMPORAL_PARA_EL_ERROR), "kk", "N_FRAMES", "VENTANA_TEMPORAL", "DURACION_DEL_VIDEO", "CREAR_PELICULA"); % Esto es por si surge el error que crashea Octave para no perder todo.
	else
		if (DURACION_DEL_VIDEO == 0) %% En este caso lo hacemos en tiempo real.
			DURACION_DEL_VIDEO = VENTANA_TEMPORAL(2) - VENTANA_TEMPORAL(1);
		end%if
		frames_per_second = (N_FRAMES+1)/DURACION_DEL_VIDEO;
		pause(1/frames_per_second);
	end%if
	% -------------------------------------------------------------------------------------
	print_processing_times(kk, kk_inicial, length(t));
end%for

if (CREAR_PELICULA)
	frames_per_second = (N_FRAMES+1)/DURACION_DEL_VIDEO;
	command = sprintf("mencoder mf://%s/*.png -mf w=640:h=480:fps=%i:type=png -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -oac copy -o %s/video.avi", DIRECTORIO_PARA_GUARDAR_GRAFICOS, frames_per_second, DIRECTORIO_DE_RESULTADOS);
	system(command);
	system(sprintf("rm %s/*.png", DIRECTORIO_PARA_GUARDAR_GRAFICOS));
	system(sprintf("rm %s/%s", DIRECTORIO_PARA_GUARDAR_GRAFICOS, ARCHIVO_TEMPORAL_PARA_EL_ERROR));
end%if
