function retval = modulo_vector(vectores)
	for (k = 1:size(vectores)(1))
		retval(k,1) = sqrt(dot(vectores(k,:),vectores(k,:)));
	end%for
end%function
