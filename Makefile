compile:
	c++ rawls_reader.cpp -o rawls_reader
	c++ rawls_h_flip.cpp -o rawls_h_flip

clean: 
	rm rawls_reader && rm rawls_h_flip