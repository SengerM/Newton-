clc
close all
clear all
more off % Esto es para que muestre los printf(), disp() y resultados a medida que los ejecuta y no todo junto al final
set(0,'defaultlinelinewidth',2); % Setea el 'LineWidth' predeterminado en 2.

% ------ CONFIGURACIÓN -------
GUARDAR_GRAFICOS.SVG = 0;
GUARDAR_GRAFICOS.PNG = 0;
GUARDAR_GRAFICOS.PDF = 1;

COLORES = {[1 1 1].*1e-3, [1 1e-3 1e-3], [1e-3 .7 1e-3], [1e-3 1e-3 1], [1 .5 1e-3], [1e-3 .7 .7], [.7 1e-3 .7]};
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
fecha_simulacion = ARCHIVOS{end}; %input("Cuál qurés graficar? --> ")}

DIRECTORIO_DE_RESULTADOS = sprintf("%s/%s", DIRECTORIO_ARCHIVOS, fecha_simulacion)
DIRECTORIO_PARA_GUARDAR_GRAFICOS = DIRECTORIO_DE_RESULTADOS

data = dlmread(sprintf("%s/simulation_output.txt", DIRECTORIO_DE_RESULTADOS),'\t',0,0);
tiempo = data(:,1);

N_particulas = (length(data(1,:))-1)/3; % Una columna es el tiempo y luego hay tres de posición para cada partícula.
columnas_x = 2:3:(3*N_particulas-3);
columnas_y = 3:3:(3*N_particulas-3);
columnas_z = 4:3:(3*N_particulas+1-3);
max_x = max(max(data(:,columnas_x)));
min_x = min(min(data(:,columnas_x)));
max_y = max(max(data(:,columnas_y)));
min_y = min(min(data(:,columnas_y)));
max_z = max(max(data(:,columnas_z)));
min_z = min(min(data(:,columnas_z)));
coordenada_maxima = max([max_x, max_y, max_z]);
coordenada_minima = min([min_x, min_y, min_z]);

k = N_particulas;
proyectil = data(:, (2+3*k-3):(2+3*k-1));

%%%%%%%%%%%%%%%%%%%%%%%      FIGURA      %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 TITULO_GRAFICO = sprintf("%s trayectorias", fecha_simulacion);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure('Position',[0,0,x_pixels,y_pixels]); hold on; clear("leyenda"); clear("graf");
	plotf = @plot3; % Reemplazar por «semilogy», «loglog», etc y automáticamente pone los tics
	
	%~ for (k = 1:length(objetivo)) % Imprimo sólo el cristal en reposo (para que sea más liviano computacionalmente).
		%~ graf(k) = plotf(objetivo{k}(2,1), objetivo{k}(2,2), objetivo{k}(2,3));
		%~ set(graf(k), 
			%~ 'LineWidth', 2,
			%~ 'Marker', '.',
			%~ 'Color', COLORES{1});
	%~ end%for
	
	graf = plotf(data(2,columnas_x), data(2,columnas_y), data(2,columnas_z));
	set(graf, 
		'LineWidth', 2,
		'LineStyle', 'none',
		'Marker', '.',
		'Color', COLORES{1});
	graf_proyectil = plotf(proyectil(:,1), proyectil(:,2), proyectil(:,3));
	set(graf_proyectil, 
		'LineWidth', .5,
		'Marker', 'x',
		'MarkerSize', 4,
		'Color', COLORES{2});
	
	% CONFIGURACION DEL GRAFICO ---------------------------------------------
	xlabel("x");
	ylabel("y");
	% VENTANA DEL GRAFICO -------------------------------------
	X_MIN = min_y + (max_y-min_y)/2.5;
	X_MAX = max_y + (max_y-min_y)/2.5;
	Y_MIN = min_y;
	Y_MAX = max_y;
	Z_MIN = coordenada_minima;
	Z_MAX = coordenada_maxima;
	axis([X_MIN X_MAX Y_MIN Y_MAX Z_MIN Z_MAX]);
	view([0,0,1]);
	grid on;
		
	% IMAGEN DE SALIDA ----------------------------------------
	XY_RATIO = sprintf("-S%i,%i", x_pixels, y_pixels); ARCHIVO_IMAGEN = strrep(TITULO_GRAFICO, " ", "_");
	if (GUARDAR_GRAFICOS.SVG)
		print(sprintf("%s/%s.svg", DIRECTORIO_PARA_GUARDAR_GRAFICOS, ARCHIVO_IMAGEN),'-dsvg', XY_RATIO);
	end%if
	if (GUARDAR_GRAFICOS.PNG)
		print(sprintf("%s/%s.png", DIRECTORIO_PARA_GUARDAR_GRAFICOS, ARCHIVO_IMAGEN),'-dpng', '-r200', XY_RATIO);
	end%if
	if (GUARDAR_GRAFICOS.PDF)
		print(sprintf("%s/%s.pdf", DIRECTORIO_PARA_GUARDAR_GRAFICOS, ARCHIVO_IMAGEN),'-dpdf', XY_RATIO);
	end%if
% -------------------------------------------------------------------------------------
