clc
close all
clear all
more off % Esto es para que muestre los printf(), disp() y resultados a medida que los ejecuta y no todo junto al final
set(0,'defaultlinelinewidth',2); % Setea el 'LineWidth' predeterminado en 2.

% ------ CONFIGURACIÓN -------
GUARDAR_GRAFICOS.SVG = 0;
GUARDAR_GRAFICOS.PNG = 0;
GUARDAR_GRAFICOS.PDF = 1;

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
fecha_simulacion = ARCHIVOS{end}

DIRECTORIO_DE_RESULTADOS = sprintf("%s/%s", DIRECTORIO_ARCHIVOS, fecha_simulacion)
DIRECTORIO_PARA_GUARDAR_GRAFICOS = DIRECTORIO_DE_RESULTADOS

data = dlmread(sprintf("%s/simulation_output.txt", DIRECTORIO_DE_RESULTADOS),'\t',0,0);

N_particulas = (length(data(1,:))-1)/3; % Una columna es el tiempo y luego hay tres de posición para cada partícula.

tiempo = data(:,1);
for (k = 1:N_particulas)
	posicion{k} = data(:, (2+3*k-3):(2+3*k-1));
	
	max_x(k) = max(posicion{k}(:,1));
	min_x(k) = min(posicion{k}(:,1));
	max_y(k) = max(posicion{k}(:,2));
	min_y(k) = min(posicion{k}(:,2));
	max_z(k) = max(posicion{k}(:,3));
	min_z(k) = min(posicion{k}(:,3));
end%for

coordenada_maxima = max([max_x, max_y, max_z]);
coordenada_minima = min([min_x, min_y, min_z]);

%%%%%%%%%%%%%%%%%%%%%%%      FIGURA      %%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 TITULO_GRAFICO = sprintf("%s trayectorias", fecha_simulacion);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure('Position',[0,0,x_pixels,y_pixels]); hold on; clear("leyenda"); clear("graf");
	plotf = @plot3; % Reemplazar por «semilogy», «loglog», etc y automáticamente pone los tics
	
	for (k = 1:N_particulas)
		graf(k) = plotf(posicion{k}(:,1), posicion{k}(:,2), posicion{k}(:,3));
#			leyenda{k} = "Leyenda1";
		set(graf(k), 
			'LineWidth', 2,
			'Color', COLORES{mod(k, length(COLORES))+1});
	end%for
	
	puntos_marker = 1:floor(length(tiempo)/10):length(tiempo);
	for (k = 1:N_particulas)
		graf_puntos(k) = plotf(posicion{k}(puntos_marker,1), posicion{k}(puntos_marker,2), posicion{k}(puntos_marker,3));
		set(graf_puntos(k), 
			'LineWidth', 1,
			'LineStyle', 'none',
			'Marker', '.',
			'MarkerSize', 10,
			'Color', COLORES{mod(k, length(COLORES))+1});
	end%for
	
	
% CONFIGURACION DEL GRAFICO ---------------------------------------------
#		XLABEL = "Eje x (metros)";
#		YLABEL = "Eje y (metros)";
#		% TAMAÑOS DE FUENTE -------------------------------------
		TITLE_FONT_SIZE = 13;
		AXIS_LABEL_FONT_SIZE = 10;
		AXIS_TICKS_FONT_SIZE = 10;
#		% -------------------------------------------------------
#		title(TITULO_GRAFICO, "FontSize", TITLE_FONT_SIZE);
#		xlabel(XLABEL, 'FontSize', AXIS_LABEL_FONT_SIZE);
#		ylabel(YLABEL, 'FontSize', AXIS_LABEL_FONT_SIZE);
#		set(gca(),'FontSize', AXIS_TICKS_FONT_SIZE);
##		% LEYENDA -------------------------------------------------
#		if (exist('leyenda'))
#			LEGEND_FONT_SIZE = 10;
#			leyendaa = legend(graf(find(graf!=0)), leyenda{find(graf!=0)}, 'Location', 'SouthOutside', 'Orientation', 'Horizontal');
#			set(leyendaa, 'FontSize', LEGEND_FONT_SIZE);
#		end%if
#		% GRID ----------------------------------------------------
		grid on;
#		% TICS ----------------------------------------------------
#		% LOG TICS (desde «1 pico» hasta «1 Giga») ----------------
			puntos_tics_log = 10.^(-9:9);
			etiquetas_tics_log = {"1p"	"10p"	"100p"	"1µ"	"10µ"	"100µ"	"1m"	".01"	".1"	"1"	"10"	"100"	"1k"	"10k"	"100k"	"1M"	"10M"	"100M"	"1G"};
		% -------
		if (plotf == @semilogx || plotf == @loglog)
			tics ('x', puntos_tics_log, etiquetas_tics_log);
		endif
		if (plotf == @semilogy || plotf == @loglog)
			tics ('y', puntos_tics_log, etiquetas_tics_log);
		endif
#		% VENTANA DEL GRAFICO -------------------------------------
			X_MIN = coordenada_minima;
			X_MAX = coordenada_maxima;
			Y_MIN = coordenada_minima;
			Y_MAX = coordenada_maxima;
		axis ([[X_MIN X_MAX] [Y_MIN Y_MAX]]);
		view([1,.7,.5]);
#		axis ('tight');

		pause(1);
		
		
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
