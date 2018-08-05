function print_processing_times(n, n_inicial, n_final)
	persistent now = clock();
	if (n == n_inicial)
		return;
	end%if
	elapsed = clock() - now;
	seconds = elapsed(6) + 60*elapsed(5) + 60*60*elapsed(4) + 60*60*24*elapsed(3);
	_ASD_ASD_ = "%.3i %% --- Elapsed time = %.004i s (%.004i m) --- Remaining time = %.004i s (%.004i m)";
	
	printf(_ASD_ASD_, round((n-n_inicial)/(n_final-n_inicial)*100), round(seconds.*[1 1/60]), round(seconds/(n-n_inicial)*(n_final-n).*[1 1/60]));
	for (q = 1:length(_ASD_ASD_))
		printf("\b\b");
	end%for
	
end%function
